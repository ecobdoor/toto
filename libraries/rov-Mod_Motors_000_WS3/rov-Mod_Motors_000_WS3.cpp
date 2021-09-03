#include "rov-Mod_Motors_000_WS3.h"
#include "esp32_LOGS.h"
#include "esp32_SPRINT.h"
using namespace std;
///////////////////////////////////////////////////////////////////////
/**
 * \class Motors
 */
Motors::Motors(c_myFlow *FLOW, const s_MODULE MOD_CFG, s_MOTctx *CONTEXT, H_pwmMotors *PWM,
	H_pidMotors *PID, H_speedSensorMotors *SPEED, H_setpointSensorMotors *JOYSTICK, int8_t *DBGMAX)
:
	Module(FLOW, MOD_CFG, DBGMAX),
		_pwmMot(*PWM),
		_pidMot(*PID),
		_speedMot(*SPEED),
		_joyMot(*JOYSTICK),
		_Ctx(*CONTEXT)
{
	map_OPmotors = {
		{ DEF_OP_PID, &Motors::mOP_Pid },
		{ DEF_OP_MOT, &Motors::mOP_Mot },
		{ DEF_OP_OPEN, &Motors::mOP_open_Learning },
		{ DEF_OP_CLOSE, &Motors::mOP_close_Learning }
	};
}
//---------------------------------------------------------------------
bool Motors::reply2pilotQ_OP(JsonObject &JOBJ){
	const char *x = JOBJ["KMD"]["OP"].as<string>().c_str();
	(this->*(findOP<OPmot>(map_OPmotors, x)))(JOBJ);
	return true;
}
//---------------------------------------------------------------------
bool Motors::mOP_close_Learning(JsonObject &JOBJ){
	if (_learnFile) {
		_learnFile.close();
	}
	_SERIAL_3("\n-Closing WRITE'%s'", _learnFileName.c_str());
	JOBJ["KMD"][DEF_KF_EXE] = true;
	_isRunning = false;
	return true;
}
//---------------------------------------------------------------------
/** < @todo Make if learning also from & record to udp file */
/**
 *
 * @param JOBJ
 * @return
 */
bool Motors::mOP_open_Learning(JsonObject &JOBJ){
	_learnFileName = "/" + JOBJ["KMD"][DEF_KF_FIL].as<String>() + ".work";
	_SERIAL_3("\n-Opening WRITE'%s'", _learnFileName.c_str());
	Motors::_learnFile = SPIFFS.open(_learnFileName, "w");
	_isRunning = true;
	JOBJ["KMD"][DEF_KF_EXE] = _isRunning;
	return true;
}
//---------------------------------------------------------------------
bool Motors::mOP_Mot(JsonObject &JOBJ){
	//roverMode = e_roverMode::driving;
	static t_AfloatAxes oldCommand = { 0.0, 0.0 };
	if (_isRunning) { //learning
		String CHN;
		serializeJson(JOBJ, CHN);
		CHN = SUI64(milli_TS()) + CHN;
		_SERIAL_3("\n*************************\tlearn [%s]", CHN.c_str());
		Motors::_learnFile.println(CHN);
	}
	float E = 0.06 * 20; // 0.06 * 20 =1,2 ?????half wheelbase (meter)
	// float E = 180/pic'est bien une consigne angulaire;
	SET_SD[0] = JOBJ["KMD"][DEF_KF_SPD].as<int16_t>();
	SET_SD[1] = JOBJ["KMD"][DEF_KF_DIR].as<int16_t>();
	//static t_AfloatAxes oldSETPOINT ={0.0,0.0};
	setPTS = _joyMot.Get(SET_SD); // format setpoint (S,D) in [-1.0,+1.0]
	//AsaDriver asa;
	SENSOR = oldCommand; // simulate sensors (L,R) with previous command (L,R)
	//SENSOR = asa.MUL(SENSOR,{1.0,1.0});
	ERROR[0] = setPTS[0] - (+SENSOR[0] + SENSOR[1]) / 2; // err(S)=setpoint(S)-(old(L)+old(R))/2
	ERROR[1] = setPTS[1] - (-SENSOR[0] + SENSOR[1]) / 2 / E; // err(D)=setpoint(D)- (-old(L)+old(R))/2/E
	/*
	 ERROR[0] = 0; // err(S)=setpoint(S)-(old(L)+old(R))/2
	 ERROR[1] = 0; // err(D)=setpoint(D)- (-old(L)+old(R))/2/E
	 */
	pidOUT = _pidMot.Run(ERROR, micro_TS());
	pidOUT[0] *= 1.0;
	pidOUT[1] *= E; ///< @todo should be in rad/sec to do solve the problem of half wheelbase (meter)
	//float zzz=pidOUT[1] * E;
	motPID[0] = pidOUT[0] - pidOUT[1];
	motPID[1] = pidOUT[0] + pidOUT[1];
	oldCommand = motPID;
	motCMD = _pwmMot.Set(motPID);
	//t_Aint32Axes verif=_pwmMot.Get_valueCur();// for debug
	UDP_dashboard();
	//JOBJ["KMD"].remove(DEF_KF_LFT); //Linear
	//JOBJ["KMD"].remove(DEF_KF_RGT); //Rotation
	JOBJ["KMD"][DEF_KF_LFT] = motCMD[0]; //Left
	JOBJ["KMD"][DEF_KF_RGT] = motCMD[1]; //Right
	JOBJ["KMD"][DEF_KF_EXE] = CTX.strRoverMode();
	return true;
}
//---------------------------------------------------------------------
bool Motors::mOP_Pid(JsonObject &JOBJ){
	_pidMot.Tune(JOBJ["KMD"]["ax"].as<uint8_t>(), JOBJ["KMD"]["KP"].as<float>(),
		JOBJ["KMD"]["DP"].as<int>(), JOBJ["KMD"]["KI"].as<float>(),
		JOBJ["KMD"]["DI"].as<int>(), JOBJ["KMD"]["KD"].as<float>(),
		JOBJ["KMD"]["DD"].as<int>());
	return true;
}
//---------------------------------------------------------------------
bool Motors::areStopped(){
	return (!motCMD[0]) && (!motCMD[1]);
}
//---------------------------------------------------------------------
e_hasMsg Motors::auto_MSG(const uint64_t DTMS,JsonObject &KMD){
	return e_hasMsg::No;
}
//---------------------------------------------------------------------
e_hasMsg Motors::complete_MSG(JsonObject &KMD){
	return e_hasMsg::Ack;
}
//---------------------------------------------------------------------
void Motors::UDP_dashboard(){
	if (!myUDP.isOpen())
		return;
	char buff[512];
	int16_t cnt = 0;

	if (1 < _Ctx.dmpLvl_Pidmot) {
		SPrintF(sizeof(buff), cnt, buff, "\n SET_SD(S,D) =(%-+9i,%-+9i)", SET_SD[0], SET_SD[1]);
		SPrintF(sizeof(buff), cnt, buff, "\n SETPTS(S,D) =(%+09.3f,%+09.3f)", setPTS[0],
			setPTS[1]);
		SPrintF(sizeof(buff), cnt, buff, "\n SENSORS(L,R)=(%+09.3f,%+09.3f)", SENSOR[0],
			SENSOR[1]);
		SPrintF(sizeof(buff), cnt, buff, "\n ERROR(S,D)  =(%+09.3f,%+09.3f)", ERROR[0],
			ERROR[1]);
		SPrintF(sizeof(buff), cnt, buff, "\n PID_OUT(S,D)=(%+09.3f,%0+9.3f)", pidOUT[0],
			pidOUT[1]);
	}
	if (0 < _Ctx.dmpLvl_Pidmot) {
		SPrintF(sizeof(buff), cnt, buff, "\n motPID(L,R) =(%+09.3f,%0+9.3f)", motPID[0],
			motPID[1]);
		SPrintF(sizeof(buff), cnt, buff, "\n motCMD(L,R) =(%-+9i,%-+9i)", motCMD[0], motCMD[1]);
	}
	if (0 < cnt)
		_DBG_MOT_PIDX("%s", buff);
}
///////////////////////////////////////////////////////////////////////
