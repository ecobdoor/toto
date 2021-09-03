#include "rov-Mod_Motors_000_WS3.h"
#include "esp32_LOGS.h"
using namespace std;
///////////////////////////////////////////////////////////////////////
/**
 * \class Motors
 */
Motors::Motors(Flow *FLOW, const s_MODULE MOD_CFG, H_pwmMotors *PWM, H_pidMotors *PID,
	H_speedSensorMotors *SPEED, H_setpointSensorMotors *JOYSTICK, int8_t *DBGMAX)
:
	Module(FLOW, MOD_CFG, DBGMAX), _pwmMot(*PWM), _pidMot(*PID), _speedMot(*SPEED), _joyMot(
		*JOYSTICK){
	map_OPmotors = {
		{ DEF_OP_PID, &Motors::mOP_Pid },
		{ DEF_OP_MOT, &Motors::mOP_Mot },
		{ DEF_OP_OPEN, &Motors::mOP_open_Learning },
		{ DEF_OP_CLOSE, &Motors::mOP_close_Learning }
	};
}
//---------------------------------------------------------------------
/** < @todo Make if learning also from & record to udp file */
/**
 *
 * @param JOBJ
 * @return
 */
bool Motors::mOP_open_Learning(JsonObject &JOBJ){
	_qryFileName = "/" + JOBJ["KMD"][DEF_KF_FIL].as<String>() + ".mis";
	_SERIAL_3("\n-Opening WRITE'%s'", _qryFileName.c_str());
	_qryFile = SPIFFS.open(_qryFileName, "w");
	_isRunning = true;
	JOBJ["KMD"][DEF_KF_EXE] = _isRunning;
	return true;
}
//---------------------------------------------------------------------
bool Motors::mOP_close_Learning(JsonObject &JOBJ){
	_qryFile.close();
	JOBJ["KMD"][DEF_KF_EXE] = true;
	_isRunning = false;
	return true;
}
//---------------------------------------------------------------------
/**
 *
 * @param DTMS
 *
 * Works at core task varying speed loop. Use only for emergency situation
 * @return
 */
bool Motors::auto_MSG(const uint64_t DTMS){
	return false;
}
//---------------------------------------------------------------------
bool Motors::complete_MSG(JsonObject &KMD){
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
bool Motors::reply2pilotQ_OP(JsonObject &JOBJ){
	const char *x = JOBJ["KMD"]["OP"].as<string>().c_str();
	//findOP<OPmot>(map_OPmotors,x);
	(this->*(findOP<OPmot>(map_OPmotors, x)))(JOBJ);
	return true;
}
//---------------------------------------------------------------------
bool Motors::mOPx_Mot(JsonObject &JOBJ){
	int16_t SET_UP[DEF_MOTORS_NBAXIS] = { JOBJ["KMD"][DEF_KF_SPD].as<int16_t>(),
		JOBJ["KMD"][DEF_KF_DIR].as<int16_t>() };
	t_AfloatAxes SETPOINT = _joyMot.Get(SET_UP);
	t_AfloatAxes SENSORS = _speedMot.Get(_pidMot);
	_AsaDriver asa;
	t_AfloatAxes ERROR = asa.DIF(SETPOINT, SENSORS);
	t_AfloatAxes PID_OUT = _pidMot.Run(ERROR, JOBJ["KMD"][DEF_KF_PMTS].as<int64_t>());
	array < int32_t, DEF_MOTORS_NBAXIS > MOTORS = _pwmMot.Set(PID_OUT);
	//t_Aint32Axes verif=_pwmMot.Get_valueCur();// for debug
	_DBG_MOT_PIDX("\n SET_UP(L,R)  =(%-+9i,%-+9i)", SET_UP[0], SET_UP[1]);
	_DBG_MOT_PIDX("\n SETPOINT(L,R)=(%+09.3f,%+09.3f)", SETPOINT[0], SETPOINT[1]);
	_DBG_MOT_PIDX("\n SENSORS(L,R) =(%+09.3f,%+09.3f)", SENSORS[0], SENSORS[1]);
	_DBG_MOT_PIDX("\n ERROR(L,R)   =(%+09.3f,%+09.3f)", ERROR[0], ERROR[1]);
	_DBG_MOT_PIDX("\n PID_OUT(L,R) =(%+09.3f,%0+9.3f)", PID_OUT[0], PID_OUT[1]);
	_DBG_MOT_PIDX("\n MOTORS(L,R)  =(%-+9i,%-+9i)", MOTORS[0], MOTORS[1]); // for debug
	//JOBJ["KMD"].remove(DEF_KF_LFT); //Linear
	//JOBJ["KMD"].remove(DEF_KF_RGT); //Rotation
	JOBJ["KMD"][DEF_KF_EXE] = true;
	JOBJ["KMD"][DEF_KF_LFT] = MOTORS[0]; //Left
	JOBJ["KMD"][DEF_KF_RGT] = MOTORS[1]; //Right
	return true;
}
//---------------------------------------------------------------------
/**
 * @fn bool Motors::mOP_Mot(JsonObject &JOBJ)
 * @brief computes the PID regulation for the requested command (speed,direction)
 *
 * As the rover has two-wheel drive, speed & direction must be converted into setpoints for the
 * left and right wheels function of the wheelbase E.
 *
 * In user units speed is in meters/second and rotation in radians/second
 * To truncate easily values PID regulator computes in [-1.0,+1.0]
 * @param JOBJ JSON motors command speed:["KMD"][DEF_KF_SPD] & direction:["KMD"][DEF_KF_DIR]
 * @return true
 */
bool Motors::mOP_Mot(JsonObject &JOBJ){
	static t_AfloatAxes oldCommand = { 0.0, 0.0 };
	if (_isRunning) { //learning
		String CHN;
		serializeJson(JOBJ, CHN);
		CHN = SUI64(milli_TS()) + CHN;
		_SERIAL_3("\n*************************\tlearn [%s]", CHN.c_str());
		_qryFile.println(CHN);
	}
	float E = 0.06 * 20; // half wheelbase (meter)
	int16_t SET_UP[DEF_MOTORS_NBAXIS] = { JOBJ["KMD"][DEF_KF_SPD].as<int16_t>(),
		JOBJ["KMD"][DEF_KF_DIR].as<int16_t>() };
	//static t_AfloatAxes oldSETPOINT ={0.0,0.0};
	t_AfloatAxes setPTS = _joyMot.Get(SET_UP); // format setpoint (S,D) in [-1.0,+1.0]
	_AsaDriver asa;
	t_AfloatAxes SENSOR = oldCommand; // simulate sensors (L,R) with previous command (L,R)
	//SENSOR = asa.MUL(SENSOR,{1.0,1.0});
	t_AfloatAxes ERROR; // compute the (S,D) error
	ERROR[0] = setPTS[0] - (+SENSOR[0] + SENSOR[1]) / 2; // err(S)=setpoint(S)-(old(L)+old(R))/2
	ERROR[1] = setPTS[1] - (-SENSOR[0] + SENSOR[1]) / 2 / E; // err(D)=setpoint(D)- (-old(L)+old(R))/2/E
	t_AfloatAxes pidOUT = _pidMot.Run(ERROR, JOBJ["KMD"][DEF_KF_PMTS].as<int64_t>());
	pidOUT[0] *= 1.0;
	pidOUT[1] *= E;
	t_AfloatAxes motPID;
	motPID[0] = pidOUT[0] - pidOUT[1];
	motPID[1] = pidOUT[0] + pidOUT[1];
	oldCommand = motPID;
	array < int32_t, DEF_MOTORS_NBAXIS > motCMD = _pwmMot.Set(motPID);
	//t_Aint32Axes verif=_pwmMot.Get_valueCur();// for debug
	char buff[512];
	int16_t cnt = 0;
	if (myUDP.isOpen()) {
		if (5 <= this_DebugLVL) {
			SPrintF(sizeof(buff), cnt, buff, "\n SET_UP(S,D) =(%-+9i,%-+9i)", SET_UP[0], SET_UP[1]);
			SPrintF(sizeof(buff), cnt, buff, "\n SETPTS(S,D) =(%+09.3f,%+09.3f)", setPTS[0],
				setPTS[1]);
			SPrintF(sizeof(buff), cnt, buff, "\n SENSORS(L,R)=(%+09.3f,%+09.3f)", SENSOR[0],
				SENSOR[1]);
			SPrintF(sizeof(buff), cnt, buff, "\n ERROR(S,D)  =(%+09.3f,%+09.3f)", ERROR[0],
				ERROR[1]);
			SPrintF(sizeof(buff), cnt, buff, "\n PID_OUT(S,D)=(%+09.3f,%0+9.3f)", pidOUT[0],
				pidOUT[1]);
		}
		if (6 <= this_DebugLVL)
			SPrintF(sizeof(buff), cnt, buff, "\n motPID(L,R) =(%+09.3f,%0+9.3f)", motPID[0],
				motPID[1]);
		if (4 <= this_DebugLVL)
			SPrintF(sizeof(buff), cnt, buff, "\n motCMD(L,R) =(%-+9i,%-+9i)", motCMD[0], motCMD[1]);
		if (0 < cnt)
			_DBG_MOT_PIDX("%s", buff);
	}
	//JOBJ["KMD"].remove(DEF_KF_LFT); //Linear
	//JOBJ["KMD"].remove(DEF_KF_RGT); //Rotation
	JOBJ["KMD"][DEF_KF_LFT] = motCMD[0]; //Left
	JOBJ["KMD"][DEF_KF_RGT] = motCMD[1]; //Right
	JOBJ["KMD"][DEF_KF_EXE] = true;
	return true;
}
///////////////////////////////////////////////////////////////////////
