/** @file */
#ifndef __rov_CONTEXT__
#define __rov_CONTEXT__
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
typedef struct {
	uint64_t TMS = 0;
	t_char8 Ename = "BOOT ! ";
	int HARD = 0;
	int SOFT = 0;
} s_BOOTctx;
///////////////////////////////////////////////////////////////////////
typedef struct {
	bool on = false;
} s_WATctx;
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
	/**
	 * @brief If true reset driver cinematic data\ref clear_Cinematique_Rover()
	 */
	bool clear_CineRover = false;
	bool simul_IMU = false;
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
	String drvKmd[10] = {
		"Z0",
		"I1_simpleAX:100:10000:0:1.0:3.14159:95.00:0.0",
		"I2_simpleAX: 50:10000:0:1.0:0.0:0.0:0.0",
	};
	/*@}*/
} s_DRVctx;

typedef struct {
	uint64_t TMS = 0;
	t_char8 Ename = "SOFT * ";
	bool csvActive = false;
	bool txtActive = false;
	bool showWSX_event = true;
	bool showWSX_data = false;
	bool withCtrlBlock = false;
	bool logWebsocket = true;
	bool exportCSV = false;
	s_WATctx WATctx;
	s_DRVctx DRVctx;
	/**
	 *
	 */
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
	int8_t MOD_DBGMAX_MOT = 6;
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
	int8_t IMU____DBGMAX_ = 5;
	int8_t JSTICK_DBGMAX_ = 0;
	int8_t PID____DBGMAX_ = 4;
	int8_t PWM____DBGMAX_ = 4;
	int8_t SPEED__DBGMAX_ = 0;
	int8_t USD____DBGMAX_ = 0;
	int8_t FLOW___DBGMAX_ = 0;
} s_SOFTctx;

///////////////////////////////////////////////////////////////////////
// These can be measured once and entered here or can be calculated each time the device is powered on
typedef struct  {
	//bool ok = true;
	/*
	 float gyroBi	float qz= FUSE.q[3];
	 as[3] = { 1.65, 0.06, 0.51 };
	 float accelBias[3] = { -0.02197, 0.04803, 0.0725 };
	 float magBias[3] = { -23.15, 359.62, -463.05 };
	 float magScale[3] = { 1.00, 1.02, 0.98 };

	 float gyroBias[3] = { -2.83969, 1.91603, -1.21374 };
	 float accelBias[3] = { 0.00336, 0.03033, 0.00232 };
	 float magBias[3] = { -16059.96094, 118.45177, 25.48962 };
	 float magScale[3] = { 0.33634, 45.73750, 203.27777 };

	 calACCGYR calibrating...

	 float gyroBias[3] ={   -3.51145,    1.67176,   -0.38931};
	 float accelBias[3]={   -0.09729,   -0.02875,   -0.00574};
	 float magBias[3]  ={   71.04000,  122.43000,  -36.90000};
	 float magScale[3] ={    1.01000,    1.03000,    0.96000};

	 float gyroBias[3] = { -2.83969, 1.91603, -1.21374 };
	 float accelBias[3] = { 0.00336, 0.03033, 0.00232 };

	 */
	float gyroBias[3] = { -3.51145, 1.67176, -0.38931 };
	float accelBias[3] = { -0.09729, -0.02875, -0.00574 };
	float magBias[3] = { 71.04, 122.43, -36.90 };
	float magScale[3] = { 1.01, 1.03, 0.96 };
	float beta = 0.0588;
	int8_t dmpLvl_Fusensor = 1;// 0: Nothing, 1: Quaternion, 2: Acc+Gyr+Mag
	bool telemetry = false;
} s_ximuCTX;
typedef struct {
	int8_t dmpLvl_Ringqueue= 1;
	bool telemetry = false;
} s_xpidCTX;
typedef struct {
	int8_t dmpLvl_Attitude = 1;
	int8_t dmpLvl_Keyframe = 1;
	bool telemetry = false;
} s_kineCTX;
typedef struct {
	uint64_t TMS = 0;
	t_char8 Ename = "HARD * ";
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
class Context {
public:
	EEPROMClass &E_BOOT0;
	EEPROMClass &E_HARD1;
	EEPROMClass &E_HARD2;
	EEPROMClass &E_SOFT1;
	EEPROMClass &E_SOFT2;

	s_BOOTctx BOOTctx;
	s_HARDctx HARDctx;
	s_SOFTctx SOFTctx;
	Context(
		EEPROMClass *E_B0,
		EEPROMClass *E_H1,
		EEPROMClass *E_H2,
		EEPROMClass *E_S1,
		EEPROMClass *E_S2
		);
	void LOAD();
	void SAVE();
	String dumpHARDctx();
	String dumpSOFTctx();
	String dumpBOOTctx();
};
///////////////////////////////////////////////////////////////////////
extern Context CTX;
#endif //__000_CONTEXT__
