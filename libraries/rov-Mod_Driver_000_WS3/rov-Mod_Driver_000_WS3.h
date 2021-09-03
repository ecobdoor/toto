#ifndef _MOD_DRIVER_000_WS3_
#define _MOD_DRIVER_000_WS3_
#include "rov-Mod_000_WS3.h"
//#include "rov-Hard_IMU_000_WS3.h"
#include "rov-Kine_000_WS3.h"
#include "rov-Mod_Motors_000_WS3.h"
#include "rov-Mod_Jobgoal_000_WS3.h"
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
//	------------------------------------------------------
typedef struct {
	float Speed = 0.0;
	float Dir = 0.0;
	float Left = 0.0;
	float Right = 0.0;
	uint64_t microTS = 0;
} s_ctrlMotion;
/**
 * \class Watchdog :public Module
 */
class Driver: public Module {
private:
	QueueHandle_t _IMUqueue;
	Kine_1 &_imu_Kine;
	Kine_1 &_drv_Kine;
	s_ctrlMotion Motion;
	H_AccGyrMag &_Ximu;
	H_ultraSounds &_usdDrv;
	s_DRVctx &_Ctx;
	bool _mustClearMAP = false;
	bool _mustClearDRV = false;
	bool _mustClearIMU = false;
	bool _mustClearOGL = false;
	imuDsp *_Simimu = nullptr;
	s_Trajectory _Trajectory;
	uint32_t _imuTIK = 0;
	t_map_OPdriver map_OPdriver; // Mapping of available module's operations
	bool _colliding;
	bool _pushDone = true;
	int16_t _speed;
	int16_t _dir;
	bool _needsUSD = true;
	e_hasMsg jsonDRVinfos();
	uint8_t pull_RTdata();
	void clear_CineRover();
	//void updateTrajectory(kineModelData & K);
	void dashBoard(Kine_1 &KINE, e_telemetry &TELEMETRY, const bool TELEM, bool &CLROPNGL,
		const bool OPNGL, const char *NL0 = "\n", const char *NLN = "\n");
	void telemetry_MOTION(const size_t SIZ, int16_t &CNT, char *BUF);
	//	static Module *mod = findModule("MOT");
	Motors &_motors;
	//	static Module *mod = findModule("MOT");
	Jobgoal &_jobgoal;
	bool mOP_Param(JsonObject &JOBJ);
	bool mOP_calMagnet(JsonObject &JOBJ);
	bool mOP_calAccGyr(JsonObject &JOBJ);
	public:
	void push_RTdata(); // public not in acq_... to see Bitscope pin after acquisition time
	void acq_simuIMU();
	void acq_trueIMU(const uint64_t ISR_MICRO_TS);
	Kine_1& imu_Kine();
	Kine_1& drv_Kine();
	/*
	 bool srcIMUchanged(){
	 if (_Simimu != nullptr)
	 if (_Ctx.simul_IMU) {
	 return
	 }
	 */
	//friend class Motors;
	/**
	 * \fn Driver(c_myFlow * FLOW,const s_DRIVER MOD_CFG)
	 * \param c_myFlow * FLOW for queries/replies management
	 * \param const s_DRIVER MOD_CFG driver module configuration
	 *
	 * Driver is responsible for IMU, USD... sensors acquisition & coherence.
	 * It drives the rover for the given trajectory setpoints
	 * USD is acquired every 100ms>= 65ms SFR02 specification
	 * IMU is acquired every 20ms
	 */
	Driver(c_myFlow *FLOW, const s_MODULE MOD_CFG, s_DRVctx *CONTEXT, Kine_1 *IMUKINE,
		Kine_1 *DRVKINE, H_AccGyrMag *IMU, H_ultraSounds *USD, int8_t *DBGMAX);
	/**
	 * @fn e_hasMsg auto_MSG(const uint64_t DTMS,JsonObject &KMD)
	 * @brief Sends messages during cal MAG
	 * @param DTMS deadline since the last time out vs. @ref _milliLastTS=@ref milli_TS();
	 * @return @ref e_hasMsg to send or not a message info Ack | request Full
	 */
	e_hasMsg auto_MSG(const uint64_t DTMS,JsonObject &KMD);
	/**
	 * \fn bool Driver::complete_MSG(JsonObject & KMD)
	 * \brief Completes the \ref tmsout_MSG & process specific routines attached to the Driver module
	 *	t_vct4F AXYZ; //Cartesian position relative to RefPoint (at T0 & Offset)
	 *
	 * \param JsonObject & JOBJ default tmsout_MSG
	 */
	e_hasMsg complete_MSG(JsonObject &KMD);
	bool reply2pilotQ_OP(JsonObject &JOBJ);
	bool updateATreply(JsonObject &JOBJ);
	//	int8_t getKINEdbgLvl();
//	void setKINEdbgLvl(const int8_t DBGMAX);
	void calib_AccGyr(const int16_t samples=0);
	void calib_Compas(const int16_t samples=0);
	bool pushDone();
};
inline bool Driver::pushDone(){
	return _pushDone;
}
inline Kine_1& Driver::imu_Kine(){
	return _imu_Kine;
}
inline Kine_1& Driver::drv_Kine(){
	return _drv_Kine;
}
///////////////////////////////////////////////////////////////////////
/*
 inline int8_t Driver::getKINEdbgLvl(){
 return _imu_Kine.get_dbgLVL();
 }
 inline void Driver::setKINEdbgLvl(const int8_t DBGMAX){
 _imu_Kine.set_dbgLVL(DBGMAX);
 _drv_Kine.set_dbgLVL(DBGMAX);
 }
 */
template<typename T>
T asIs(JsonVariant data){
	return (T)(data.as<T>());
}

#endif
///////////////////////////////////////////////////////////////////////
