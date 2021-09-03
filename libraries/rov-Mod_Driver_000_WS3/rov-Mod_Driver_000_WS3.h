#ifndef _MOD_DRIVER_000_WS3_
#define _MOD_DRIVER_000_WS3_
#include "rov-Mod_000_WS3.h"
#include "rov-Hard_IMU_000_WS3.h"
#include "rov-Kine_000_WS3.h"
///////////////////////////////////////////////////////////////////////
/**
 * Folowing https://en.wikipedia.org/wiki/Axes_conventions
 * https://fr.wikipedia.org/wiki/Cin%C3%A9matique#Navigation_maritime_et_a%C3%A9rienne
 * parameters rd/? m/s² & g
 */
///////////////////////////////////////////////////////////////////////
class Driver;
typedef bool (Driver::*OPdrv)(JsonObject &JOBJ);
typedef std::map<const char*, OPdrv, strCmp> t_map_OPdriver;
typedef struct { //	s_kineFifo definition
	uint64_t TMS; // milli seconds time;
	t_vct3F XYZmin = { { -10.0, -10.0, -10.0 } }; //	Min threshold
	t_vct3F XYZmax = { { +10.0, +10.0, +10.0 } }; //	Max threshold
	t_vct3F ABCmin = { { -10.0, -10.0, -10.0 } }; //	Min threshold
	t_vct3F ABCmax = { { +10.0, +10.0, +10.0 } }; //	Max threshold
	t_vct3F XYZ = { { 0.0, 0.0, 0.0 } }; //Linear coordinates (integrated from speeds)
	t_vct3F ABC = { { 0.0, 0.0, 0.0 } }; // Current Roll Pitch Yaw attitude (Euler)
} s_Trajectory;
/**
 * \class Watchdog :public Module
 */
class Driver: public Module {
private:
	Kine_1 &_Kine;
	H_AccGyrMag &_Ximu;
	H_ultraSounds &_usdDrv;
	s_DRVctx &_Ctx;
	imuDsp *_Simimu = nullptr;
	uint64_t _prevMTS = micro_TS();
	s_Trajectory _Trajectory;
	uint32_t _imuTIK = 0;
	enum class e_drvMod {
		explore, jobgoal, driveMap, driveMot, learning
	};
	t_map_OPdriver map_OPdriver; // Mapping of available module's operations
	uint64_t _prev_TMS;
	e_drvMod drvMode = e_drvMod::explore;
	bool _colliding;
	int16_t _speed;
	int16_t _dir;
	s_fusensors _last_rtFusensors;
	bool _needsUSD = true;
	void json_Cinematic();
	uint8_t pull_RTdata();
	void clear_Cinematique_Rover();
	//void updateTrajectory(kineModelData & K);
public:
	void push_RTdata();// public not in acq_... to see Bitscope pin after acquisition time
	void acq_simuIMU();
	void acq_trueIMU(const uint64_t ISR_MICRO_TS);
	/*
	 bool srcIMUchanged(){
	 if (_Simimu != nullptr)
	 if (_Ctx.simul_IMU) {
	 return
	 }
	 */
	//friend class Motors;
	/**
	 * \fn Driver(Flow * FLOW,const s_DRIVER MOD_CFG)
	 * \param Flow * FLOW for queries/replies management
	 * \param const s_DRIVER MOD_CFG driver module configuration
	 *
	 * Driver is responsible for IMU, USD... sensors acquisition & coherence.
	 * It drives the rover for the given trajectory setpoints
	 * USD is acquired every 100ms>= 65ms SFR02 specification
	 * IMU is acquired every 20ms
	 */
	Driver(Flow *FLOW, const s_MODULE MOD_CFG, s_DRVctx *CONTEXT, Kine_1 *KINE, H_AccGyrMag *IMU,
		H_ultraSounds *USD, int8_t *DBGMAX);
	bool auto_MSG(const uint64_t DTMS);
	bool auto_REPLY();
	/**
	 * \fn bool Driver::complete_MSG(JsonObject & KMD)
	 * \brief Completes the \ref tmsout_MSG & process specific routines attached to the Driver module
	 *	t_vct4F AXYZ; //Cartesian position relative to RefPoint (at T0 & Offset)
	 *
	 * \param JsonObject & JOBJ default tmsout_MSG
	 */
	bool complete_MSG(JsonObject &KMD);
	bool reply2pilotQ_OP(JsonObject &JOBJ);
	int8_t getKINEdbgLvl();
	void setKINEdbgLvl(const int8_t DBGMAX);
};
///////////////////////////////////////////////////////////////////////
inline int8_t Driver::getKINEdbgLvl(){
	return _Kine.get_dbgLVL();
}
inline void Driver::setKINEdbgLvl(const int8_t DBGMAX){
	_Kine.set_dbgLVL(DBGMAX);
}
#endif
///////////////////////////////////////////////////////////////////////
