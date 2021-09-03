#include <000_SIGNAL_PROCESS.h>
#include <esp32_LOGS.h>
#include <random>
//int8_t this_DebugLVL=0;
///////////////////////////////////////////////////////////////////////
sigDsp::sigDsp(){
//	defEngine(time(0));
	defEngine();
}
bool sigDsp::Begin(const String SKMD){
	try {
		if (!splitString(_args, SKMD, ":"))
			THROWERR(DSP_ERR_SPLIT, SKMD.c_str());
		_CntSamples = Int16(_args[1].c_str());
		_MtsSample = Int64(_args[2].c_str());
		_MtsStart = Int64(_args[3].c_str());
		if (_args[4].length() > 0)
			_Amplitude = Float(_args[4].c_str());
		else
			_Amplitude = 0.0;
		_NoiseAmplitude = Float(_args[5].c_str());
		_Offset = Float(_args[6].c_str());
//	assert((_CntSamples %2)==0);
		switch(toupper(_args[0][0])){
			case 'A':
				compute = [=](const int32_t SAMPLE){
					return this->calc_Amplitude(SAMPLE);
				};
				break;
			case 'D':
				compute = [=](const int32_t SAMPLE){
					return this->calc_Dirac(SAMPLE);
				};
				break;
			case 'O':
				compute = [=](const int32_t SAMPLE){
					return this->calc_Offset(SAMPLE);
				};
				break;
			case 'S':
				compute = [=](const int32_t SAMPLE){
					return this->calc_halfCosSquare(SAMPLE);
				};
				break;
			case 'Z':
				compute = [=](const int32_t SAMPLE){
					return this->calc_Zero(SAMPLE);
				};
				break;
			default:
				THROWERR(DSP_ERR_SPLIT, _args[0].c_str());
				break;
		}
		_sampleIdx = -1;
		_curMicroTS = _MtsStart;
		return true;
	} catch (EXCEPT const &e) {
		FIX_ERROR();
	}
}
//---------------------------------------------------------------------
s_Dsp sigDsp::next(){
	_curMicroTS += _MtsSample; // _MtsStart could be any
	_sampleIdx = (_sampleIdx + 1) % (int32_t)_CntSamples;
	_curValue = _Offset + compute(_sampleIdx) + _NoiseAmplitude * floatDistro(defEngine);
	s_Dsp nxt = {
		.MicroTS = _curMicroTS,
		.Value = _curValue,
	}
	;
	return nxt;
}
//---------------------------------------------------------------------
/*
 * _curValue store the POSITION of pitch, yaw or roll function of time
 */
/*
 float sigDsp::calc_GYR(const int32_t  SAMPLE){
 _curValue = _Amplitude*_curMicroTS;
 return _Amplitude/_MtsSample;
 }
 */
//---------------------------------------------------------------------
float sigDsp::calc_Amplitude(const int32_t SAMPLE){
	return _Amplitude;
}
//---------------------------------------------------------------------
float sigDsp::calc_Offset(const int32_t SAMPLE){
	return _Offset;
}
//---------------------------------------------------------------------
float sigDsp::calc_Dirac(const int32_t SAMPLE){
	if (SAMPLE == 0)
		return _Amplitude;
	else
		return 0.0;
}
//---------------------------------------------------------------------
float sigDsp::calc_Zero(const int32_t SAMPLE){
	return 0.0;
}
float sigDsp::calc_halfCosSquare(const int32_t SAMPLE){
	static int8_t un = -1;
	if (SAMPLE == 0)
		un *= -1;
	if (SAMPLE < _CntSamples / 2)
		return +un * _Amplitude;
	else
		return -un * _Amplitude;
}
///////////////////////////////////////////////////////////////////////
imuDsp::imuDsp(){
}
imuDsp::~imuDsp(){
	for (size_t i = 0; i < _signal.size(); i++)
		delete _signal[i];
	_signal.clear();
}
sigDsp* imuDsp::new_sigDsp(const char *FORMAT, const float AMP){
	sigDsp *sd = new sigDsp();
	int16_t cnt = 0;
	char buffer[100];
// _Amplitude _CntSamples _MtsSample _MtsStart _NoiseAmplitude Offset
	SPrintF(sizeof(buffer), cnt, buffer, FORMAT, _CntSamples, _MtsSample, _MtsStart, AMP);
	if (!sd->Begin(String(buffer)))
		_SERIAL_0("\n >>>>>>>>>>>ERROR \"%s\"", buffer);
	return sd;
}
//---------------------------------------------------------------------
/*
 Example

 1)cntSamples 2)mtsSample 3)mtsStart 4)amplACC 5)amplGYR 6)amplMAG 7)amplUSD
 */
bool imuDsp::Begin(const String SKMD, String &CHN){
	try {
		if (!splitString(_args, SKMD, ":"))
			THROWERR(DSP_ERR_SPLIT, SKMD.c_str());
		_CntSamples = Int16(_args[1].c_str());
		_MtsSample = Int64(_args[2].c_str());
		_MtsStart = Int64(_args[3].c_str());
		_AA = Float(_args[4].c_str()); // acceleration amplitude
		_GA = Float(_args[5].c_str()); // gyroscop & magnetic amplitude
		_MA = Float(_args[6].c_str()); // magnetic amplitude
		_RA = Float(_args[7].c_str()); // radar amplitude
//	assert((_CntSamples %2)==0);
		switch(toupper(_args[0][0])){
			case 'I': {
				if (_args[0][1] == '1') {
					//				CntSamples : MtsSample : MtsStart : Amplitude : NoiseAmplitude : Offset
					_signal.push_back(new_sigDsp("S:%i:%llu:%llu:%.8f:0.00:0.0", _AA));
					_signal.push_back(new_sigDsp("O:%i:%llu:%llu:%.8f:0.00:3.14159", _GA)); // unused just to test_signal vector usage
					_signal.push_back(new_sigDsp("A:%i:%llu:%llu:%.8f:0.00:0.0", _MA));
					imuCalc = [=](s_imuDsp &IMU, const int32_t SAMPLE){
						this->imu_model1(IMU, SAMPLE);
					};
					CHN = "Ax Gy Mzo";
				}
				else {
					CHN = "Only `imu_model1` is implemented";
					return false;
				}
				break;
			}
			default:
				CHN = "Only 'I' command is implemented";
				return false;
		}
		Reset();
		return true;
	} catch (EXCEPT const &e) {
		FIX_ERROR();
	}
}
//---------------------------------------------------------------------
void imuDsp::Reset(){
	_sampleIdx = -1;
	_curMicroTS = _MtsStart;
}
//---------------------------------------------------------------------
void imuDsp::next(s_imuDsp &IMU){
	_curMicroTS += _MtsSample; // _MtsStart could be any
	_sampleIdx = (_sampleIdx + 1) % (int32_t)_CntSamples;
	imuCalc(IMU, _sampleIdx);
}
//---------------------------------------------------------------------
// !! Same order that psuh_back
typedef enum {
	ACCX,
	GX_OFFSET,
} e_simOP;
void imuDsp::imu_model1(s_imuDsp &IMU, const int32_t SAMPLE){
//	_SERIAL_0("\n >>>>>>>>>>> %i", SAMPLE);
	float alpha = _GA * _curMicroTS / 1000000.0; // µs !!!
	if (alpha > 2 * pi)
		alpha -= 4 * pi; // alpha €[-2pi,+2pi] to keep float precision !!!!
	float alphaOffset = _signal[GX_OFFSET]->compute(SAMPLE);
	float mo = _MA * sin(30.0f * pi / 180.0f);
	float my = -mo * sin(20.0f * pi / 180.0f); //*sin(alpha+alphaOffset);
	float mz = -mo * cos(20.0f * pi / 180.0f); //cos(alpha+alphaOffset);
	float mx = _MA * cos(30.0f * pi / 180.0f);
IMU= {
	.MTS=_curMicroTS,
	.AX=_signal[ACCX]->compute(SAMPLE),
	.AY=-my, //0.5f*_signal[ACCX]->compute(SAMPLE),//0,//-1.0f*my/_MA,//earth gravity
	.AZ=-mz,//-1.0f*mz/_MA,// g unity
	.GX=0,
	.GY=0.0,
	.GZ=0.0,
	.MX=mx,
	.MY=my,
	.MZ=mz,
	.USD=555.0,
	.SAMPLE=SAMPLE
};
}
///////////////////////////////////////////////////////////////////////
