#ifndef __000_SIGNAL_PROCESS__
#define __000_SIGNAL_PROCESS__
//#include <esp32_LOGS.h>
#include <Arduino.h>
#include <random>
#include <ctime>
#include <vector>
#include <functional>
using namespace std;
typedef struct{
	int64_t MicroTS;
	float Value;
} s_Dsp;
typedef std::function<float(const int32_t)> f_compute;
class sigDsp {
private:
	int16_t _CntSamples;
	int64_t _MtsSample;
	int64_t _MtsStart;
	float _Amplitude;
	float _NoiseAmplitude;
	float _Offset;
	vector<String> _args;
	int32_t _sampleIdx;
	int64_t _curMicroTS;
	float _curValue;
	default_random_engine defEngine;
	uniform_real_distribution<float> floatDistro;
public:
	f_compute compute;
int8_t this_DebugLVL=0;
	sigDsp();
	bool Begin(const String SKMD);
	s_Dsp next();	//using compute=f_compute;
	float calc_Amplitude(const int32_t  SAMPLE);
	float calc_Dirac(const int32_t  SAMPLE);
	float calc_Zero(const int32_t  SAMPLE);
	float calc_Offset(const int32_t  SAMPLE);
	float calc_halfCosSquare(const int32_t  SAMPLE);
};
///////////////////////////////////////////////////////////////////////
typedef struct{
	int64_t MTS;
	float AX;
	float AY;
	float AZ;
	float GX;
	float GY;
	float GZ;
	float MX;
	float MY;
	float MZ;
	float USD;
	uint32_t SAMPLE;
} s_imuDsp;
typedef std::function<void(s_imuDsp & ,const int32_t)> f_imuCalc;
class imuDsp {
private:
	int16_t _CntSamples;
	int64_t _MtsSample;
	int64_t _MtsStart;
	float _AA;
	float _GA;
	float _MA;
	float _RA;
	vector<String> _args;
	int32_t _sampleIdx;
	int64_t _curMicroTS;

	vector<sigDsp *> _signal;

	f_imuCalc imuCalc;
public:
int8_t this_DebugLVL=0;
	imuDsp();
	~imuDsp();
	bool Begin(const String SKMD,String & CHN);
	void Reset();
	sigDsp * new_sigDsp(const char * FORMAT,const float AMP);
	void next(s_imuDsp & IMU);
	void imu_model1(s_imuDsp & IMU,const int32_t  SAMPLE);
};
#endif //__000_SIGNAL_PROCESS__
