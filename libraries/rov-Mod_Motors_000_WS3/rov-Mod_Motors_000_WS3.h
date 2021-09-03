#ifndef _MOD_MOTORS_000_WS3_
#define _MOD_MOTORS_000_WS3_
/** @file */
#include "rov-Mod_000_WS3.h"
#include "rov-Hard_MOT_000_WS3.h"
#include "SPIFFS.h" // File
///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
class Motors;
typedef bool (Motors::*OPmot)(JsonObject &JOBJ);
typedef std::map<const char*, OPmot, strCmp> t_map_OPmotors;
/**
 * \class Motors :public Module
 */
class Motors: public Module {
private:
	int16_t SET_SD[DEF_MOTORS_NBAXIS] = { 0, 0 }; ///< Velocity & Direction set
	array<int32_t, DEF_MOTORS_NBAXIS> motCMD { 0, 0 }; ///< Left & Right motors PWM commands
	t_AfloatAxes SENSOR; ///< simulate sensors (L,R) with previous command (L,R)	t_AfloatAxes setPTS; ///< format setpoint (S,D) in [-1.0,+1.0]
	t_AfloatAxes ERROR; ///< (Velocity,Direction) error between order & sensor (simulated)
	t_AfloatAxes pidOUT; ///< Velocity,Direction output from PID
	t_AfloatAxes motPID;
	t_AfloatAxes setPTS;
	t_map_OPmotors map_OPmotors; // Mapping of available module's operations
	H_pwmMotors &_pwmMot;
	H_pidMotors &_pidMot;
	H_speedSensorMotors &_speedMot;
	H_setpointSensorMotors &_joyMot;
	String _learnFileName;
	s_MOTctx &_Ctx;
	File _learnFile ;
	bool mOPSIMIMUx_Mot(JsonObject &JOBJ);
	/**
	 * @fn bool Motors::mOP_Mot(JsonObject &JOBJ)
	 * @brief computes the PID regulation (speed,direction) & commands the PWM motors (left,right)
	 *
	 * As the rover has two-wheel drive, speed & direction must be converted into setpoints for the
	 * left and right wheels function of the wheelbase E.
	 *
	 * In user units speed is in meters/second and rotation in radians/second
	 * To truncate easily values PID regulator computes in [-1.0,+1.0]
	 *
	 * JOBJ fields implied
	 * - #param JOBJ["KMD"][DEF_KF_SPD].as<int16_t>() speed ctrl driver or joystick asked position € [-255, +255]
	 * - #param JOBJ["KMD"][DEF_KF_DIR].as<int16_t>() direction ctrl driver or joystick asked position € [-255, +255]
	 * - #return JOBJ["KMD"][DEF_KF_LFT] value of order for left motor speed € [-1023, +1023] more accurate because of PID ring queue filter
	 * - #return JOBJ["KMD"][DEF_KF_RGT]  value of order for right motor speed € [-1023, +1023] more accurate because of PID ring queue filter
	 * - #return JOBJ["KMD"][DEF_KF_EXE] = status true if all ok else false (hard policy is reset what else);
	 *
	 * @param JOBJ JSON motors command speed:["KMD"][DEF_KF_SPD] & direction:["KMD"][DEF_KF_DIR]
	 * @return true
	 */
	bool mOP_Mot(JsonObject &JOBJ);
	bool mOP_Pid(JsonObject &JOBJ);
	bool mOP_close_Learning(JsonObject &JOBJ);
	bool mOP_open_Learning(JsonObject &JOBJ);

public:
	friend class Watchdog;///< surveillor class
	/**
	 * @fn Motors(c_myFlow *FLOW, const s_MODULE MOD_CFG, s_MOTctx *CONTEXT, H_pwmMotors *PWM,
		H_pidMotors *PID, H_speedSensorMotors *SPEED,
		H_setpointSensorMotors *JOYSTICK, int8_t *DBGMAX)
	 * @param FLOW query/reply map controller. Every query needs a reply!!!
	 * @param MOD_CFG module configuration
	 * @param CONTEXT module context
	 * @param PWM pulse width modulation data
	 * @param PID proportional integral derived data
	 * @param SPEED speed sensor data (emultated & not implemented yet)
	 * @param JOYSTICK pilot command
	 * @param DBGMAX debug level
	 */
	Motors(c_myFlow *FLOW, const s_MODULE MOD_CFG, s_MOTctx *CONTEXT, H_pwmMotors *PWM,
		H_pidMotors *PID, H_speedSensorMotors *SPEED,
		H_setpointSensorMotors *JOYSTICK, int8_t *DBGMAX);
	/**
	 * @fn bool isLearning()
	 * @brief check if a learn file if opened
	 * @return true if opened, false otherwise
	 */
	bool isLearning();
	bool areStopped();
	void UDP_dashboard();
	/**
	 * @fn e_hasMsg Motors::auto_MSG(const uint64_t DTMS,JsonObject &KMD)
	 * @brief If needs generates a specific info or query to browser
	 * @param DTMS time since last call in µsec
	 * @return e_hasMsg::No (not used)
	 */
	e_hasMsg auto_MSG(const uint64_t DTMS,JsonObject &KMD);
	/**
	 * @fn e_hasMsg Motors::complete_MSG(JsonObject &KMD)
	 * @brief completes standard reply to browser query
	 * @param KMD JsonObject to be completed
	 * @return e_hasMsg::Ack (single acknowledge to browser query)
	 */
	e_hasMsg complete_MSG(JsonObject &KMD);
	/**
	 * \fn bool reply2pilotQ_OP(JsonObject & JOBJ);
	 * \brief See the pure virtual parent method for behavior
	 */
	bool reply2pilotQ_OP(JsonObject &JOBJ);
	bool updateATreply(JsonObject &JOBJ){
		return false;
	}
	int32_t getMotLeft();
	int32_t getMotRight();
	int32_t getMotSpeed();
	int32_t getMotDir();
};
inline bool Motors::isLearning(){
	return _learnFile != 0;
}
inline int32_t Motors::getMotLeft(){
	return motCMD[0];
}
inline int32_t Motors::getMotRight(){
	return motCMD[1];
}
inline int32_t Motors::getMotSpeed(){
	return SET_SD[0];
}
inline int32_t Motors::getMotDir(){
	return SET_SD[1];
}
///////////////////////////////////////////////////////////////////////
#endif
