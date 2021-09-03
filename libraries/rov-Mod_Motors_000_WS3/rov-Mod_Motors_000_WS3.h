#ifndef _MOD_MOTORS_000_WS3_
#define _MOD_MOTORS_000_WS3_
/** @file */
#include "rov-Mod_000_WS3.h"
#include "rov-Hard_MOT_000_WS3.h"
#include "SPIFFS.h" // File
///////////////////////////////////////////////////////////////////////
class Motors;
typedef bool (Motors::*OPmot)(JsonObject &JOBJ);
typedef std::map<const char*, OPmot, strCmp> t_map_OPmotors;
/**
 * \class Motors :public Module
 */
class Motors: public Module {
private:
	t_map_OPmotors map_OPmotors; // Mapping of available module's operations
	H_pwmMotors &_pwmMot;
	H_pidMotors &_pidMot;
	H_speedSensorMotors &_speedMot;
	H_setpointSensorMotors &_joyMot;
	String _qryFileName;
	File _qryFile;
	bool mOPSIMIMUx_Mot(JsonObject &JOBJ);
	bool mOPx_Mot(JsonObject &JOBJ);
	bool mOP_Mot(JsonObject &JOBJ);
	bool mOP_Pid(JsonObject &JOBJ);
	bool mOP_open_Learning(JsonObject &JOBJ);
	bool mOP_close_Learning(JsonObject &JOBJ);
	public:
	friend class Watchdog;
	Motors(Flow *FLOW, const s_MODULE MOD_CFG, H_pwmMotors *PWM, H_pidMotors *PID,
		H_speedSensorMotors *SPEED, H_setpointSensorMotors *JOYSTICK, int8_t *DBGMAX);
	bool auto_MSG(const uint64_t DTMS);
	bool complete_MSG(JsonObject &KMD);
	/**
	 * \fn bool reply2pilotQ_OP(JsonObject & JOBJ);
	 * \brief See the pure virtual parent method for behavior
	 */
	bool reply2pilotQ_OP(JsonObject &JOBJ);
};
///////////////////////////////////////////////////////////////////////
#endif
