/** @file */
#pragma once
///////////////////////////////////////////////////////////////////////
#include "esp32_EEPROM.h"
/*
 #define DEF_TSK_APP__DBGMAX 				4
 #define DEF_TSK_IMU__DBGMAX 			3
 #define DEF_TSK_MOD__DBGMAX 			3
 #define DEF_TSK_RCV__DBGMAX 					5 // 4 WSXEVENT 5 WSXDATA
 #define DEF_TSK_SND__DBGMAX 					5 // 4 WSXEVENT 5 WSXDATA
 #define DEF_MOD_DRV__DBGMAX 							7
 #define DEF_MOD_JOB__DBGMAX 0
 #define DEF_MOD_MOT__DBGMAX 0
 #define DEF_MOD_TMS__DBGMAX 0
 #define DEF_MOD_WAT__DBGMAX 			3	//
 #define DEF_FLOW_____DBGMAX 0
 #define DEF_IMU______DBGMAX 0
 #define DEF_JOYSTICK_DBGMAX 0
 #define DEF_KINE_____DBGMAX 0
 #define DEF_PID______DBGMAX 0
 #define DEF_PWM______DBGMAX 0
 #define DEF_SPEED____DBGMAX 0
 #define DEF_USD______DBGMAX 0
 */
typedef char t_char8[8];

///////////////////////////////////////////////////////////////////////
/**
 * @brief global rover states @ref roverMode
 *
 * Changes when a query comes from browser pilot interface
 */
enum class e_roverMode {
	idle, ///< ready
	roaming, ///< autonomous exploration of the environment with construction of the terrain map
	working, ///< executing a autonomous mission file
	driving, ///< manual piloting from human browser interface
	learning, ///< learning from manual piloting from human browser interface
	error
};
/// @brief Current rover state
typedef struct {
	uint64_t TMS = 0;
	t_char8 Ename = "BOOT ! ";
	int HARD = 0;
	int SOFT = 0;
	e_roverMode roverMode = e_roverMode::idle;

} s_BOOTctx;
///////////////////////////////////////////////////////////////////////
typedef struct {
	bool on = false;
} s_WATctx;
//---------------------------------------------------------------------
/**
 * @brief State of telemetry
 */
enum class e_telemetry {
	no, ///< no telemetry
	drv, ///< drv control telemetry with motion
	imu ///< imu telemetry whithout motion
};
/**
 * @brief typedef struct s_DRVctx driver context
 *
 * 
 */
typedef struct {
	/*@{*/
	/**
	 * @name General purpose driver context
	 */
	bool jsonDRVinfos = true;
	bool show_drvInfos = false;
	/**
	 * @brief If true reset driver cinematic data\ref clear_CineRover()
	 */
	bool clear_CineRover = false;///< flag to reset the rover position
	bool simul_IMU = false;
	e_telemetry telemetry = e_telemetry::no; ///< UDP output no:none, drv:pull_driver 10Hz, imu:push_isr 100Hz
	uint8_t openGL = 0; ///< Serial output 0:none, 1:driver 10Hz, 2:isr 100Hz
	uint8_t udpLvl_Kine = 1; ///< UDP output 0:none, 1:driver 10Hz, 2:isr 100Hz, 3:twice together
	/**
	 * @brief Index of IMU simulation command string to run
	 */
	int8_t kmdIdx = 1;
	/*@}*/
	/**
	 * @name IMU simulation driver commands strings syntax (8 parameters)
	 * @n 0)\ref imuDsp nature name:
	 * @li char 0: Nature of test (I for IMU)
	 * @li char 1: Type of simulation (1 for linear acceleration sinusoids)
	 * @li char 3... Label of test
	 *
	 * @n 1) Int16 cntSamples number of samples per simulation cycle
	 * @n 2) Int64 mtsSample time step between samples in milliseconds
	 * @n 3) Int64 mtsStart start time in milliseconds
	 * @n 4) Float amplACC acceleration amplitude
	 * @n 5) Float amplGYR gyroscopic amplitude
	 * @n 6) Float amplMAG magnetic amplitude
	 * @n 7) Float amplUSD ultrasonic amplitude
	 */
	/*@{*/
#define drvKmdCount 5
	String drvKmd[drvKmdCount] = {
		"Z0", // 0
		"I1_simpleAX:100:10000:0:1.0:3.14159:95.00:0.0", // 1
		"I2_simpleAX: 50:10000:0:1.0:0.0:0.0:0.0", // 2
		"", // 3
		"", // 4
		};
	/*@}*/
} s_DRVctx;
typedef struct {
	/**
	 * @brief Max of debug level authorized (Motors updated runs 10 Hz!!!)
	 *
	 * @n 0) No dump
	 * @n 1) Motors & pid speeds
	 * @n 2) Pid ringqueue input/output
	 */
	int8_t dmpLvl_Pidmot = 2;
} s_MOTctx;

typedef struct {
	uint64_t TMS = 0;
	t_char8 Ename = "SOFT * ";///< Label of s_SOFTctx context *€[0...2]
	uint32_t delayUDPStart = 5000;
	int32_t delayPINStest = 5000; // <0: until keyboard input, 0:no test, >0: test during milliseconds
	bool benchMark = false;
	bool showWSX_event = true;
	bool showWSX_data = true;
	bool showWSX_fifo = true;
	bool trace_UDP = false;
	s_WATctx WATctx;
	s_DRVctx DRVctx;
	s_MOTctx MOTctx;
	/**
	 *
	 */
	int8_t MAX_DBG_DRV_XIMU= 2;

	int8_t ALL_DBGMAX_TRUNC = 4;
	int8_t FCT_DBGMAX_SERV = 4;
	int8_t FCT_DBGMAX_MAIN = 4;
	int8_t FCT_DBGMAX_BOOT = 4;
	int8_t FCT_DBGMAX_LOOP = 8;
	int8_t FCT_DBGMAX_KMDS = 5;
	int8_t TSK_DBGMAX_SEND = 5;
	int8_t TSK_DBGMAX_UDPL = 5;
	int8_t TSK_DBGMAX_RECV = 5;
	int8_t TSK_DBGMAX_LOGS = 4;
	int8_t TSK_DBGMAX_XIMU = 3;
	int8_t TSK_DBGMAX_CORE = 3;
	int8_t MOD_DBGMAX_JOB = 0;
	int8_t MOD_DBGMAX_MOT = 9; // 5 short 6 detail
	int8_t MOD_DBGMAX_TMS = 0;
	int8_t MOD_DBGMAX_WAT = 3;
	/**
	 * @brief Max of debug level authorized (IMU runs 100 Hz!!!)
	 *
	 *
	 * Authorizes children modules (Kine, imu...) to dump if levels coherent
	 *
	 * @n 0) No dump
	 * @n 1) Short setup info
	 * @n 2) Detailed setup info
	 * @n 3) 
	 * @n 4) 
	 * @n 5) Shortest acquisition info (timing & count)
	 * @n 6) More quaternion & radar info
	 * @n 7) More acceleration, gyroscope & compass info
	 * @n 8) 
	 * @n 9) 
	 */
	int8_t MOD_DBGMAX_DRV = 6;
	/**
	 * @brief Max of debug level authorized (IMU runs 100 Hz!!!)
	 *
	 * @n 0) No dump
	 * @n 1) Short setup info
	 * @n 2) Detailed setup info
	 * @n 3) 
	 * @n 4) 
	 * @n 5) 
	 * @n 6) NED attitude info
	 * @n 7) More XYZ position, speed, acceleration & gravity info
	 * @n 8) 
	 * @n 9) 
	 */
	int8_t KINE___DBGMAX_ = 5;
	/**
	 * @brief Max of debug level authorized (IMU runs 100 Hz!!!)
	 *
	 * @n 0) No dump
	 * @n 1) Short setup info
	 * @n 2) Detailed setup info
	 * @n 3) Short calibration dialog
	 * @n 4) Detailed calibration dialog
	 * @n 5) SERIAL shortest acquisition info (timing & count)
	 * @n 6) More quaternion & radar info
	 * @n 7) More acceleration, gyroscope & compass info
	 * @n 8) 
	 * @n 9) 
	 */
	int8_t IMU____DBGMAX_ = 6;
	int8_t JSTICK_DBGMAX_ = 0;
	int8_t PID____DBGMAX_ = 4;
	int8_t PWM____DBGMAX_ = 4;
	int8_t SPEED__DBGMAX_ = 0;
	int8_t USD____DBGMAX_ = 0;
	int8_t FLOW___DBGMAX_ = 0;
} s_SOFTctx;

///////////////////////////////////////////////////////////////////////
// These can be measured once and entered here or can be calculated each time the device is powered on
typedef struct {
	//bool ok = true;
	/*
	 float gyroBi	float qz= FUSE.q[3];
	 as[3] = { 1.65, 0.06, 0.51 };
	 float accBias[3] = { -0.02197, 0.04803, 0.0725 };
	 float magBias[3] = { -23.15, 359.62, -463.05 };
	 float magScale[3] = { 1.00, 1.02, 0.98 };

	 float gyrBias[3] = { -2.83969, 1.91603, -1.21374 };
	 float accBias[3] = { 0.00336, 0.03033, 0.00232 };
	 float magBias[3] = { -16059.96094, 118.45177, 25.48962 };
	 float magScale[3] = { 0.33634, 45.73750, 203.27777 };

	 calibrate_ACCGYR calibrating...

	 float gyrBias[3] ={   -3.51145,    1.67176,   -0.38931};
	 float accBias[3]={   -0.09729,   -0.02875,   -0.00574};
	 float magBias[3]  ={   71.04000,  122.43000,  -36.90000};
	 float magScale[3] ={    1.01000,    1.03000,    0.96000};

	 float gyrBias[3] = { -2.83969, 1.91603, -1.21374 };
	 float accBias[3] = { 0.00336, 0.03033, 0.00232 };

	 */
	bool adjust_Bias = false;
	float gyrBias[3] = { -3.51145, 1.67176, -0.38931 };
	float accBias[3] = { -0.09729, -0.02875, -0.00574 };

	// default  ???
	float magBias[3] = { 71.04, 122.43, -36.90 };
	float magScale[3] = { 1.01, 1.03, 0.96 };

	/*

	 // ----- 2021 10 12 Calibré sur table en verre
	 float magBias[3] = { 248.75224, 316.97446, -101.20879 };
	 float magScale[3] = { 0.73380, 0.87328, 2.03205 };

	 // ----- 2021 10 11 Calibré sur table en verre
	 float magBias[3] = { 147.19788, 132.69597, -73.37637 };
	 float magScale[3] = { 1.17749, 0.63850, 1.71069 };


	 */
	float beta = 0.0037;
	uint8_t cntInt=10;// count of filter integrations
	/**
	 * @brief Max of debug level authorized (IMU runs 100 Hz!!!)
	 *
	 * @n 0) No dump
	 * @n 1) Quaternion & radar info
	 * @n 2) More acceleration, gyroscope & compass info
	 */
} s_ximuCTX;
typedef struct {
	/**
	 * @brief Max of debug level authorized (Rinq Queue runs 10 Hz!!!)
	 *
	 * @n 0) No dump
	 * @n 1) Half first ring queue lines
	 * @n 2) More regulation coefficients
	 * @n 3) More full ring queue lines
	 */
	int8_t udpLvl_PIDrq = 3; ///< 0:none, 1:half rq, 2:full rq, 3:with coefs & depths
} s_xpidCTX;
typedef struct {
//	float linAcc_threshold=0.1;// mm/s²
	float thr_linAcc = 0.1; //0.005; //m/s²
	int8_t udpLvl_Fusensor = 2; // 0: Nothing, 1: Quaternion, 2: Acc+Gyr+Mag
	int8_t udpLvl_Attitude = 2; // 0: Nothing, 1: RoolPitchYaw, 2: XYZ+Speed+Acc+Gravity
	int8_t udpLvl_keyFrame = 1; // 0: Nothing, 1: Dxyz+Quality, 2: KF_Matrix
} s_kineCTX;
typedef struct {
	uint64_t TMS = 0;
	t_char8 Ename = "HARD * ";///< Label of s_HARDctx context *€[0...2]
	s_ximuCTX ximuCTX;
	s_xpidCTX xpidCTX;
	s_kineCTX kineCTX;
} s_HARDctx;

//#include "esp_task.h""TSK_DBGMAX_RECV
/*
 http://www.pinon-hebert.fr/Knowledge/index.php/ESP32#RTC_memory
 #include <rom/rtc.h>
 */
/**
 * https://github.com/espressif/arduino-esp32/blob/master/libraries/EEPROM/examples/eeprom_class/eeprom_class.ino
 */
///////////////////////////////////////////////////////////////////////
/**
 * @class Context
 * @brief Stores contexts for configuraton data 0:compiled default, 1:eeprom 1, 2: eeprom 2
 */
class Context {
private:
	s_BOOTctx BOOTctx;

public:
	EEPROMClass &E_BOOT0;
	EEPROMClass &E_HARD1;
	EEPROMClass &E_HARD2;
	EEPROMClass &E_SOFT1;
	EEPROMClass &E_SOFT2;

	s_HARDctx HARDctx;
	s_SOFTctx SOFTctx;
	/**
	 * @fn Context(EEPROMClass *E_B0,EEPROMClass *E_H1,EEPROMClass *E_H2,EEPROMClass *E_S1,EEPROMClass *E_S2)
	 * @brief
	 * @param E_B0
	 * @param E_H1
	 * @param E_H2
	 * @param E_S1
	 * @param E_S2
	 */
	Context(
		EEPROMClass *E_B0,///< Boot context 0
		EEPROMClass *E_H1,///< Hard context 1
		EEPROMClass *E_H2,///< Hard context 2
		EEPROMClass *E_S1,///< Soft context 1
		EEPROMClass *E_S2///< Soft context 2
		);
	/**
	 * @fn void LOAD()
	 * @brief Loads SOFT & HARD contexts following BOOT directives respectively from eeprom 1 or 2
	 */
	void LOAD();
	/**
	 * @fn void SAVE()
	 * @brief Saves SOFT & HARD contexts following BOOT directives respectively from eeprom 1 or 2
	 */
	void SAVE();
	String dumpHARDctx();
	String dumpSOFTctx();
	String dumpBOOTctx();
	/**
	 * @fn void getRoverMode()
	 * @brief Gets the global state of rover
	 * @return current rover mode
	 */
	e_roverMode getRoverMode();
	/**
	 * @fn void setRoverMode(const e_roverMode RM)
	 * @brief Sets the global state of rover
	 * @param Targeted rover mode
	 */
	void setRoverMode(const e_roverMode RM);
	/**
	 * @fn char* strRoverMode()
	 * @brief Gets the global state of rover in text format
	 * @return text rover mode (don't change these labels because they are used by javascript pilot)
	 */
	char* strRoverMode();
	void setBootSOFT(const int IDX);
	void setBootHARD(const int IDX);
	String jsonCONTEXT();
};
//---------------------------------------------------------------------
inline e_roverMode Context::getRoverMode(){
	return BOOTctx.roverMode;
}
//---------------------------------------------------------------------
inline void Context::setRoverMode(const e_roverMode RM){
	BOOTctx.roverMode = RM;
}
//---------------------------------------------------------------------
inline void Context::setBootSOFT(const int IDX){
	BOOTctx.SOFT = IDX;
}
//---------------------------------------------------------------------
inline void Context::setBootHARD(const int IDX){
	BOOTctx.HARD = IDX;
}
///////////////////////////////////////////////////////////////////////
extern Context CTX;
///////////////////////////////////////////////////////////////////////
