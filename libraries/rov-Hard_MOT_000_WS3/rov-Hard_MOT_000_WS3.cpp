#include "esp32_LOGS.h"
#include "esp32_SPRINT.h"
#include "esp32_FSTR.h"
#include "rov-Hard_MOT_000_WS3.h"
//#include "00_MODhardware.h"
#include <algorithm> // transform for_each
#include <functional> // minus
using namespace std;
///////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
H_setpointSensorMotors::H_setpointSensorMotors(const String NAME, int8_t *DBGMAX,
	const bool ON) :
	H_Module(NAME.c_str(), DBGMAX, ON){
}
//---------------------------------------------------------------------
/**
 * @fn t_AfloatAxes H_setpointSensorMotors::Get(int16_t MEASURE[DEF_MOTORS_NBAXIS])
 * @brief return a truncated pid setpoints €[-1.0,+1.0]
 * @param MEASURE
 * @return
 */
t_AfloatAxes H_setpointSensorMotors::Get(int16_t MEASURE[DEF_MOTORS_NBAXIS]){
	t_AfloatAxes measure;
	for (size_t ax = 0; ax < DEF_MOTORS_NBAXIS; ax++) {
		measure[ax] = MEASURE[ax];
		if (measure[ax] < -255)
			measure[ax] = -1;
		else if (255 < measure[ax])
			measure[ax] = +1;
		else
			measure[ax] = measure[ax] / 255;
	}
	return measure;
}
///////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
H_speedSensorMotors::H_speedSensorMotors(const String NAME, int8_t *DBGMAX,
	const bool ON) :
	H_Module(NAME.c_str(), DBGMAX, ON){
}
//---------------------------------------------------------------------
t_AfloatAxes H_speedSensorMotors::Get(H_pidMotors &PIDS){
	t_AfloatAxes measure;
	if (isON()) {
		measure = PIDS.getLastCMD();
	} else {
		for (size_t ax = 0; ax < DEF_MOTORS_NBAXIS; ax++)
			measure[ax] = 0;
	}
// Don't Threshold measures !!!!!!!!!
	for (size_t ax = 0; ax < DEF_MOTORS_NBAXIS; ax++) {
		if (measure[ax] < -1)
			measure[ax] = -1;
		else if (+1 < measure[ax])
			measure[ax] = +1;
	}
	return measure;
}
///////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
H_pwmMotors::H_pwmMotors(asmPWMs *PWM, const s_pwm leftCFG, const s_pwm rightCFG,
	const String NAME, int8_t *DBGMAX, const bool ON) :
	H_Module(NAME.c_str(), DBGMAX, ON), //constructeur classe mère
	data(*PWM)
{
	data._pwm[0] = leftCFG;
//	data._pwm[0]._dutyMax=((1<<DEF_PWM_RESOLUTION)-1)/DEF_PWM_VALUEMAX;
	data._pwm[1] = rightCFG;
//	data._pwm[1]._dutyMax=((1<<DEF_PWM_RESOLUTION)-1)/DEF_PWM_VALUEMAX;
// Setup timer and attach timer to a PWM output pin
	attachPWMpins();
}
//---------------------------------------------------------------------
/**
 * @fn void H_pwmMotors::attachPWMpins()
 * @brief Attaches PWM pins to associated timers
 *
 * https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html
 */
void H_pwmMotors::attachPWMpins(){
// Setups & attaches timers to PWM output pins
#ifdef DEF_WITHMOTORSPINS
	for (int i = 0; i < DEF_MOTORS_NBAXIS; i++) {
		s_pwm & pwm = data._pwm[i];
		ledcSetup(pwm._channel, pwm._frequence, pwm._resolution);
		ledcAttachPin(pwm._pinPWM, pwm._channel);
		pinMode(pwm._pinSENS, OUTPUT);
	}
#endif
}
//---------------------------------------------------------------------
/**
 * @fn void H_pwmMotors::detachPWMpins()
 * @brief Detaches PWM pins from associated timers
 *
 * https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html
 */
void H_pwmMotors::detachPWMpins(){
// Resets & detaches timers of PWM output pins
#ifdef DEF_WITHMOTORSPINS
	for (int i = 0; i < DEF_MOTORS_NBAXIS; i++) {
		s_pwm & pwm = data._pwm[i];
		ledcDetachPin(pwm._pinPWM);
		//ledcSetup(pwm._channel, pwm._frequence, pwm._resolution);
		digitalWrite(pwm._pinPWM, LOW);
	}
#endif
}
//---------------------------------------------------------------------
/**
 *	Truncate _valueCur updated y PID & generates pwm pulses
 */
void H_pwmMotors::analogWrite(const t_Aint32Axes VALUES){
	for (int i = 0; i < DEF_MOTORS_NBAXIS; i++) {
		s_pwm &pwm = data._pwm[i];
		pwm._valueCur = VALUES[i];
#ifdef DEF_WITHMOTORSPINS
// calculate duty, 8191 from 2 ^ 13 - 1
		uint32_t duty = pwm._dutyMax * std::min(std::abs(pwm._valueCur), pwm._valueMax);
// write duty to LEDC
		if (0 < pwm._valueCur) {
			digitalWrite(pwm._pinSENS, LOW);
		} else {
			digitalWrite(pwm._pinSENS, HIGH);
		}
		ledcWrite(pwm._channel, duty);
#endif
	}
}
//---------------------------------------------------------------------
t_Aint32Axes H_pwmMotors::analogRead() {
	array < int32_t, DEF_MOTORS_NBAXIS > values;
	for (int i = 0; i < DEF_MOTORS_NBAXIS; i++) {
		s_pwm & pwm = data._pwm[i];
#ifdef DEF_WITHMOTORSPINS
		values[i] = ledcRead(pwm._channel) / pwm._dutyMax;
		if (0 < digitalRead(pwm._pinSENS))
			values[i] = -values[i];
#else
		values[i] = pwm._valueCur;
#endif
	}
	return values;
}
//---------------------------------------------------------------------
/**
 * @fn
 * @param COMMAND
 * @return
 */
t_Aint32Axes H_pwmMotors::Set(const t_AfloatAxes &COMMAND){
	t_Aint32Axes values;
	for (size_t ax = 0; ax < DEF_MOTORS_NBAXIS; ax++) {
		s_pwm &pwm = data._pwm[ax];
		if (COMMAND[ax] < -1)
			values[ax] = -pwm._valueMax;
		else if (+1 < COMMAND[ax])
			values[ax] = +pwm._valueMax;
		else
			values[ax] = (int32_t)(COMMAND[ax] * pwm._valueMax);
		pwm._valueCur = values[ax];
	}
	analogWrite(values);
	return values;
}
//---------------------------------------------------------------------
t_Aint32Axes H_pwmMotors::Get_valueCur(){
	array<int32_t, DEF_MOTORS_NBAXIS> values;
	for (size_t ax = 0; ax < DEF_MOTORS_NBAXIS; ax++) {
		s_pwm &pwm = data._pwm[ax];
		values[ax] = pwm._valueCur;
	}
	return values;
}
//---------------------------------------------------------------------
void H_pwmMotors::Reset(){
	t_Aint32Axes values = { 0, 0 };
	analogWrite(values);
//	return values;
}
///////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
H_pidMotors::H_pidMotors(asmPIDs *PIDS, const s_pid linCFG, const s_pid rotCFG, s_xpidCTX *CONTEXT,
	const String NAME, int8_t *DBGMAX, const bool ON) :
	H_Module(NAME.c_str(), DBGMAX, ON), _xpidCTX(*CONTEXT), data(*PIDS){
	data._pid[0] = linCFG;
	data._pid[1] = rotCFG;
}
//---------------------------------------------------------------------
// T simulate speed sensor...
t_AfloatAxes H_pidMotors::getLastCMD(){
	t_AfloatAxes measure;
	for (size_t ax = 0; ax < DEF_MOTORS_NBAXIS; ax++) {
		s_pid pid = data._pid[ax];
		ringQueue<pidQueueData> &RQ = *pid._RQ;
		if (RQ.cnt() != 0) {
			ringQueue<pidQueueData>::myIterator it = RQ.begin();
			measure[ax] = (*it).outCMD;
		} else {
			measure[ax] = 0;
		}
	}
	return measure;
}
//---------------------------------------------------------------------
/**
 * @fn void H_pidMotors::Tune(const uint8_t AX, const float KP, const int DP, const float KI,
 const int DI, const float KD, const int DD)
 * @brief Adjust the PID parameters gains & depths
 * @param AX motor axis number
 * @param KP proportional gain
 * @param DP proportional depth
 * @param KI integral gain
 * @param DI integral depth
 * @param KD derivative gain
 * @param DD derivative depth
 */
void H_pidMotors::Tune(const uint8_t AX, const float KP, const int DP, const float KI,
	const int DI, const float KD, const int DD){
	s_pid &pid = data._pid[AX];
	pid._kp = KP;
	pid._dp = DP;
	pid._ki = KI;
	pid._di = DI;
	pid._kd = KD;
	pid._dd = DD;
}
//---------------------------------------------------------------------
void H_pidMotors::Reset(){
	for (size_t ax = 0; ax < DEF_MOTORS_NBAXIS; ax++) {
		s_pid pid = data._pid[ax];
		pid._RQ->empty();
	}
}
//---------------------------------------------------------------------
/**
 * @fn t_AfloatAxes H_pidMotors::Run(const t_AfloatAxes &ERROR,const uint64_t MICROTS)
 * @param ERROR for PID regulation errors
 * @return PID regulation  setpoints
 */
t_AfloatAxes H_pidMotors::Run(const t_AfloatAxes &ERROR, const uint64_t MICROTS){
	t_AfloatAxes propo;
	t_AfloatAxes integ;
	t_AfloatAxes deriv;
	t_AfloatAxes outKD;
	for (size_t ax = 0; ax < DEF_MOTORS_NBAXIS; ax++) {
		s_pid pid = data._pid[ax];
		pid._RQ->clrAllMSG();
		pidQueueData item;
		item.inERR = ERROR[ax];
		item.micTSrq = MICROTS;
		item.MSG[0] = '\0';
		addMSG(item, "@");
		if (isON()) {
			propo[ax] = prorata(item, *pid._RQ, pid._dp);
			integrated(item, *pid._RQ, pid._di);
			derived(item, *pid._RQ, pid._dd);
		} else {
			propo[ax] = item.inERR;
			item.INT = 0;
			item.DER = 0;
		}
		integ[ax] = item.INT;
		deriv[ax] = item.DER;
		item.outCMD = pid._kp * propo[ax] + pid._ki * integ[ax] + pid._kd * deriv[ax];
		outKD[ax] = item.outCMD;
		pid._RQ->enqueue(item);
	}
	if (myUDP.isOpen()) {
		_DBG_MOT_PIDX("%s", dump_RQ().c_str());
		/*
		 _DBG_MOT_PIDX("\n%s",
		 S_flow(milli_TS(), 0, '*', propo[0], propo[1], integ[0], integ[1], deriv[0],
		 deriv[1], outKD[0], outKD[1], "Run").c_str());
		 */
	}
	return outKD;
}
//---------------------------------------------------------------------
/**
 * PID works without thresholding !!!!!!!!!!!!!!!!!!
 * CONSIGNE & SENSORS € [-1,+1]
 * COMMAND can overflow, PWM has to scale & trunc values
 */
//---------------------------------------------------------------------
//  MOYENNE
/*
 TMS en milliseconde
 Retourne le proportionnel de l'erreur x + si _dp>0 la moyenne de _dp échantillons
 */
float H_pidMotors::prorata(pidQueueData &x, ringQueue<pidQueueData> &RQ,
	const uint8_t DEPTH){
	class Summator {
	private:
		int _count = 1;
		float _sum;
		public:
		Summator(pidQueueData &x){ //constructeur
			_sum = x.inERR;
		}
		void operator()(pidQueueData &x){
			_sum += x.inERR;
			_count++;
			addMSG(x, "A");
			/*
			 int len = strlen(x.MSG);
			 x.MSG[len] = '+';
			 x.MSG[len + 1] = '|';
			 x.MSG[len + 2] = '\0';
			 */
		}
		int count() const{
			return _count;
		}
		float sum() const{
			return _sum;
		}
	};
	Summator somme(x); //instanciation
	somme = for_each(RQ.begin(), RQ.stop(DEPTH), somme);
//dumpRQ(RQ);
//_DBG_MOT_PIDX( "\n prorata count=%i sum=%f *******", somme.count(), somme.sum());
	return somme.sum() / somme.count();
}

//---------------------------------------------------------------------
//  INTEGRATION
/*
 TMS en milliseconde
 Retourne l'intégrale par seconde


 Warning... computation is done going back in time
 */
void H_pidMotors::integrated(pidQueueData &x, ringQueue<pidQueueData> &RQ,
	const uint8_t DEPTH){
	class Integrator {
	private:
		uint64_t _savTms;
		float _savErr;
		float _area = 0;
		int _count = 0;
		public:
		Integrator(pidQueueData &x){ //constructeur
			_savTms = x.micTSrq;
			_savErr = x.inERR;
		}
		void operator()(pidQueueData &x){
			_count++;
			_area += (_savTms - x.micTSrq) * (x.inERR + _savErr) / 2000000.0; // µicro->seconds !!
			_savTms = x.micTSrq;
			_savErr = x.inERR;
			addMSG(x, "I");
		}
		float area() const{
			return _area;
		}
		int count() const{
			return _count;
		}
	};
	Integrator integrale(x); //instanciation
	integrale = for_each(RQ.begin(), RQ.stop(DEPTH), integrale);
	if (integrale.count() > 0) {
		ringQueue<pidQueueData>::myIterator it = RQ.begin();
		x.INT = (*it).INT + integrale.area() / integrale.count();
		addMSG(x,
			String((*it).INT, 3) + "+" + String(integrale.area(), 3) + "/"
				+ String(integrale.count(), 3));
	} else {
		x.INT = 0;
	}
}
//---------------------------------------------------------------------
//  DERIVATION
/*
 TMS en milliseconde
 Retourne la dérivée moyenne en dERR/dTMS
 */
void H_pidMotors::derived(pidQueueData &x, ringQueue<pidQueueData> &RQ,
	const uint8_t DEPTH){
	class Derivator {
	private:
		int _count = 0;
		uint64_t _savTms;
		float _savErr;
		float _slope = 0.0;
		public:
		Derivator(pidQueueData &x){ //constructeur
			_savTms = x.micTSrq;
			_savErr = x.inERR;
		}
		void operator()(pidQueueData &x){
			_slope += (_savErr - x.inERR) / (_savTms - x.micTSrq) * 1000000.0; // µicro->seconds !!
			_savTms = x.micTSrq;
			_savErr = x.inERR;
			_count++;
			addMSG(x, " D:" + String(_savErr, 3));
//			addMSG(x, " D:" + String((1.0*_savTms -1.0* x.TMS) / 1000.0,3));
		}
		float slope() const{
			return _slope;
		}
		int count() const{
			return _count;
		}
	};
	/// PROBLEME SI GAIN NUL ET COUNT=1 !!!!!!!!!??????????????????????????????
	Derivator derivee(x); //Instantiation
	derivee = for_each(RQ.begin(), RQ.stop(DEPTH), derivee);
	x.DER = derivee.count() > 0 ? (derivee.slope() / derivee.count()) : 0.0;
}
/*
 float H_pidMotors::integrated(s_pid & PID, pidQueueData & x) {
 ringQueue<pidQueueData> & RQ = *PID._RQ;
 class Integrator {
 private:
 uint64_t _savTms;
 float _savErr;
 float _area = 0;
 public:
 Integrator(pidQueueData & x) { //constructeur
 _savTms = x.TMS;
 _savErr = x.inERR;
 }
 void operator()(pidQueueData & x) {
 _area += (_savTms - x.TMS) * (x.inERR + _savErr) / 2000.0; // seconds !!
 _savTms = x.TMS;
 _savErr = x.inERR;
 }
 float area() const {
 return _area;
 }
 };
 Integrator integrale(x); //instanciation
 integrale = for_each(RQ.begin(), RQ.stop(PID._di), integrale);
 return integrale.area();
 }

 */
//-------------------------------------------------------
//  DUMP
//
/*

 PAS DE DONNEE ******************************************************
 3 !!! RQ.cnt()     =0
 3 !!! RQ.begin()   =/Begin:0/End:0
 3 !!! RRQ.end()    =/Begin:0/End:0 Entrez un caractère (+Enter)?. I received:

 1 DONNEE ******************************************************

 3 !!! RQ.cnt()     =1
 3 !!! RQ.begin()   =+0/Begin:0
 3 !!! RRQ.end()    =/End:1 Entrez un caractère (+Enter)?. I received:

 0:1056458510(+0/Begin:0)|
 0:1056458510()|

 3 !!! RQ.cnt()     =1
 3 !!! RQ.begin()   =+0/Begin:0
 3 !!! RRQ.end()    =/End:1 Entrez un caractère (+Enter)?. I received:

 0:1056458510(+0/Begin:0)|
 0:1056458510()|

 2 DONNEES ******************************************************

 3 !!! RQ.cnt()     =2
 3 !!! RQ.begin()   =/Begin:0/Begin:0/Begin:0/Begin:0/Begin:0
 3 !!! RRQ.end()    =/End:2 Entrez un caractère (+Enter)?. I received:
 1056467422(+0/Start:1)|
 0:1056458510(/Begin:0/Begin:0/Begin:0/Begin:0/Begin:0)|1:1056467422(+0/Start:1)|
 0:1056458510()|1:1056467422()|

 3 !!! RQ.cnt()     =2
 3 !!! RQ.begin()   =/Begin:0/Begin:0/Begin:0/Begin:0/Begin:0
 3 !!! RRQ.end()    =/End:2 Entrez un caractère (+Enter)?. I received:
 1056467422(+0/Start:1)|
 0:1056458510(/Begin:0/Begin:0/Begin:0/Begin:0/Begin:0)|1:1056467422(+0/Start:1)|
 0:1056458510()|1:1056467422()|

 3 DONNEES ******************************************************

 3 !!! RQ.cnt()     =3
 3 !!! RQ.begin()   =/Begin:0/Begin:0/Begin:0/Begin:0/Begin:0
 3 !!! RRQ.end()    =/End:3 Entrez un caractère (+Enter)?
 */
//---------------------------------------------------------------------
void H_pidMotors::dumpRQ(ringQueue<pidQueueData> &RQ){
	int debut = RQ.debut;
	int fin = RQ.fin;
	int cnt = RQ.cnt();
	_DBG_MOT_PIDX("\n *** RQ(cnt,debut,fin) = RQ(%i,%i,%i)  *******", cnt, debut, fin);
	for (int i = 0; i <= DEF_MOTORS_RQSIZE; i++) {
		_DBG_MOT_PIDX("\n%2i %c TMS:%17llu, inERR:%+9.3f, outCMD:%+9.3f, MSG:%s", i,
			((i == debut) && (i == fin)) ?
											'X' :
											((i == debut) ? '>' : ((i == fin) ? '<' : ' ')),
			RQ.getX(i).micTSrq, RQ.getX(i).inERR, RQ.getX(i).outCMD, RQ.getX(i).MSG);
	}
	_DBG_MOT_PIDX(
		"\n *******************************************************************");
}
//---------------------------------------------------------------------
String H_pidMotors::dump_RQ(){
	char buffer[1524];
	buffer[0] = 0;
	int16_t cnt = 0;
	s_pid pid_L = data._pid[0];
	s_pid pid_R = data._pid[1];
	if (1 < _xpidCTX.udpLvl_PIDrq) {
		SPrintF(sizeof(buffer), cnt, buffer,
			"\n              Coeff| %+9.3f %+9.3f | %+9.3f %+9.3f | %+9.3f %+9.3f |",
			data._pid[0]._kp, data._pid[1]._kp,
			data._pid[0]._ki, data._pid[1]._ki,
			data._pid[0]._kd, data._pid[1]._kd);
		SPrintF(sizeof(buffer), cnt, buffer,
			"\n              Depth|     %i         %i     |     %i         %i     |     %i         %i     |",
			data._pid[0]._dp, data._pid[1]._dp,
			data._pid[0]._di, data._pid[1]._di,
			data._pid[0]._dd, data._pid[1]._dd);
	}
	ringQueue<pidQueueData> &RQ_L = *pid_L._RQ;
	ringQueue<pidQueueData> &RQ_R = *pid_R._RQ;
	int debut_L = RQ_L.debut;
	int debut_R = RQ_R.debut;
	int fin_L = RQ_L.fin;
	int fin_R = RQ_R.fin;
	int cnt_L = RQ_L.cnt();
	int cnt_R = RQ_R.cnt();
	if (5 < _xpidCTX.udpLvl_PIDrq)
		SPrintF(sizeof(buffer), cnt, buffer,
			"\n *** RQ(cnt,debut,fin) = RQ_L(%i,%i,%i) | RQ_R(%i,%i,%i) *******",
			cnt_L, debut_L, fin_L, cnt_R, debut_R, fin_R);
	if (0 < _xpidCTX.udpLvl_PIDrq) {
		SPrintF(sizeof(buffer), cnt, buffer, Lang.CST(PID_HEAD));
		int i = debut_L;
		int zend;
		if (1 < _xpidCTX.udpLvl_PIDrq)
			zend = DEF_MOTORS_RQSIZE;
		else
			zend = DEF_MOTORS_RQSIZE / 2;
		for (int zz = 0; zz <= zend; zz++) {
//		RQ_L.clrX(i); //effacer le message
//		RQ_R.clrX(i); //effacer le message
			SPrintF(sizeof(buffer), cnt, buffer, "\n%s",
				S_flow(RQ_L.getX(i).micTSrq, i,
					((i == debut_L) && (i == fin_L)) ?
						'X' : ((i == debut_L) ? '>' : ((i == fin_L) ? '<' : ' ')),
					RQ_L.getX(i).inERR, RQ_R.getX(i).inERR, RQ_L.getX(i).INT,
					RQ_R.getX(i).INT, RQ_L.getX(i).DER, RQ_R.getX(i).DER,
					RQ_L.getX(i).outCMD, RQ_R.getX(i).outCMD, RQ_R.getX(i).MSG).c_str());
			i = (i + 1) % (DEF_MOTORS_RQSIZE + 1);
		}
		//SPrintF(sizeof(buffer), cnt, buffer, Lang.CST(PID_FOOT));
	}
	return String(buffer);
}
//---------------------------------------------------------------------
void H_pidMotors::dump2(){
	/*
	 Serial.println("******************************************************");
	 for (uint8_t ax = 0; ax < DEF_MOTORS_NBAXIS; ax++) {
	 s_pid pid = data._pid[ax];
	 ringQueue<pidQueueData> & RQ = *pid._RQ;

	 int cnt = RQ.cnt();
	 ringQueue<pidQueueData>::myIterator itB = RQ.begin();
	 ringQueue<pidQueueData>::myIterator itE = RQ.end();
	 _DBG_MOT_PIDX( "\n!!! RQ.cnt()     =%i", cnt);
	 _DBG_MOT_PIDX( "\n!!! RQ.begin()   =%s", (*itB).MSG);
	 _DBG_MOT_PIDX( "\n!!! RQ.end()     =%s", (*itE).MSG);
	 int k;
	 _DBG_MOT_PIDX( "\n dump par itérateur **************************");
	 k = RQ.debut;
	 for (ringQueue<pidQueueData>::myIterator it = RQ.begin(); it != RQ.end(); ++it) {
	 _DBG_MOT_PIDX( "\n%2i TMS:%llu, inERR:%+9.3f, outCMD:%+9.3f, MSG:%s", k,
	 (*itE).TMS, (*itE).inERR, (*itE).outCMD, (*it).MSG);
	 k = (k + 1) % (RQ.taille + 1);
	 }
	 _DBG_MOT_PIDX(
	 "\n dump par index avant messages effacés **************************");
	 k = RQ.debut;
	 for (int i = 0; i < cnt; i++) {
	 _DBG_MOT_PIDX( "\n%2i TMS:%llu, inERR:%+9.3f, outCMD:%+9.3f, MSG:%s", k,
	 RQ.getX(k).TMS, RQ.getX(k).inERR, RQ.getX(k).outCMD, RQ.getX(k).MSG);
	 RQ.clrX(k); //effacer le message
	 k = (k + 1) % (RQ.taille + 1);
	 }
	 _DBG_MOT_PIDX(
	 "\n dump par index avec messages effacés **************************");
	 k = RQ.debut;
	 for (int i = 0; i < cnt; i++) {
	 _DBG_MOT_PIDX( "\n%2i TMS:%llu, inERR:%+9.3f, outCMD:%+9.3f, MSG:%s", k,
	 RQ.getX(k).TMS, RQ.getX(k).inERR, RQ.getX(k).outCMD, RQ.getX(k).MSG);
	 k = (k + 1) % (RQ.taille + 1);
	 }
	 _DBG_MOT_PIDX(
	 "\n dump par itérateur avec messages générés **************************");
	 k = RQ.debut;
	 for (ringQueue<pidQueueData>::myIterator it = RQ.begin(); it != RQ.end(); ++it) {
	 _DBG_MOT_PIDX( "\n%2i TMS:%llu, inERR:%+9.3f, outCMD:%+9.3f, MSG:%s", k,
	 (*itE).TMS, (*itE).inERR, (*itE).outCMD, (*it).MSG);
	 k = (k + 1) % (RQ.taille + 1);
	 }
	 _DBG_MOT_PIDX(
	 "\n dump complet sans messages générés **************************");
	 for (int i = 0; i < DEF_MOTORS_RQSIZE; i++) {
	 _DBG_MOT_PIDX( "\n%2i TMS:%llu, inERR:%+9.3f, outCMD:%+9.3f, MSG:%s", i,
	 RQ.getX(i).TMS, RQ.getX(i).inERR, RQ.getX(i).outCMD, RQ.getX(i).MSG);
	 }
	 _DBG_MOT_PIDX(
	 "\n *******************************************************************");
	 }
	 */
}
///////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
String S_flow(const uint64_t TMS, const uint8_t IDX, const char CAR, const float L_ERR,
	const float R_ERR, const float L_INT, const float R_INT, const float L_DER,
	const float R_DER, const float L_OUT, const float R_OUT, const char *MSG){
	char buffer[512];
	int16_t cnt = 0;
	SPrintF(sizeof(buffer), cnt, buffer,
		"%14.6f %i %c | %+9.3f,%+9.3f | %+9.3f,%+9.3f | %+9.3f,%+9.3f | %+9.3f,%+9.3f | '%s'",
		TMS / 1000000.0, IDX, CAR, L_ERR, R_ERR, L_INT, R_INT, L_DER, R_DER, L_OUT, R_OUT, MSG);
	return String(buffer);
}
//---------------------------------------------------------------------
//  addMSG
//
void addMSG(pidQueueData &item, const String MSG){
//_DBG_MOT_PIDX( "\n '%s' '%i' '%s'  '%i'", item.MSG,strlen(item.MSG),MSG.c_str(),MSG.length());
	strncpy(&(item.MSG[strlen(item.MSG)]), MSG.c_str(), 1 + MSG.length());
}
/*
 * https://www.rs-online.com/designspark/noise-sensor-fusion-and-lost-drones-fr
 */
