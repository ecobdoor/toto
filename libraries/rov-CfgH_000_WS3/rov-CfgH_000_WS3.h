// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//     Config ROVER  Version:'000_WS3' du 29 juil. 2021 à 15:59:35
//     File /media/DEV/ROB_000/setup/SCRIPTS/../SET_000_WS3/rov-CfgH(000_WS3).h
//
//     This file is generated automatically. Don't modify it !!!
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef	 _CFG_ROVER_000_WS3_
#define	 _CFG_ROVER_000_WS3_	
#include "../rov-CfgH_000_WS3/rov_INC.inc"		//	reference telemetry (kamm... udp log & csv) !!!
//	======================================================
//	 ESPDUINO 32 PINOUT & ADDRESSES
//	------------------------------------------------------
#define	 DEF_BLUE_LED	2
#define	 DEF_PIN_SENSLEFT	13		//	Sens de rotation
#define	 DEF_PIN_PWMLEFT	12		//	Vitesse de rotation
#define	 DEF_PIN_SENSRIGHT	14		//	Sens de rotation !! UP@upload
#define	 DEF_PIN_PWMRIGHT	27		//	Vitesse de rotation
#define	 DEF_PIN_I2CSCL	0		//	I2C clock
#define	 DEF_PIN_I2CSDA	0		//	I2C data
#define	 DEF_AGM_ADDRS	0		//	AccGyrMag address
#define	 DEF_USD_ADDRS	0		//	ultrasonic address
//	======================================================
//	 GLOBAL WS & JSON PROTOCOLS
//	------------------------------------------------------
//	---- dialogs ROVER BASIS -----------------------------
#define	 DEF_QUERY_ROVER_TO_BASIS	"rQb"
#define	 DEF_REPLY_BASIS_TO_ROVER	"bRr"
#define	 DEF_QUERY_BASIS_TO_ROVER	"bQr"
#define	 DEF_REPLY_ROVER_TO_BASIS	"rRb"
//	---- dialogs PILOT ROVER -----------------------------
#define	 DEF_QUERY_PILOT_TO_ROVER	"pQr"
#define	 DEF_REPLY_ROVER_TO_PILOT	"rRp"
#define	 DEF_QUERY_ROVER_TO_PILOT	"rQp"
#define	 DEF_REPLY_PILOT_TO_ROVER	"pRr"
//	---- dialogs BASIS PILOT -----------------------------
#define	 DEF_QUERY_BASIS_TO_PILOT	"bQp"
#define	 DEF_REPLY_PILOT_TO_BASIS	"pRb"
#define	 DEF_QUERY_PILOT_TO_BASIS	"pQb"
#define	 DEF_REPLY_BASIS_TO_PILOT	"bRp"
//	======================================================
//	 GLOBAL COMMANDS DICTIONNARY
//	------------------------------------------------------
#define	 DEF_KF_DIR	"D"		//	KMD field
#define	 DEF_KF_SPD	"S"		//	KMD field
#define	 DEF_KF_LFT	"L"		//	KMD field
#define	 DEF_KF_RGT	"R"		//	KMD field
#define	 DEF_KF_FIL	"F"		//	KMD field
#define	 DEF_KF_GET	"@"		//	KMD field
#define	 DEF_KF_EXE	"X"		//	KMD field
#define	 DEF_KF_MTS	"MTS"		//	KMD field (µs)
#define	 DEF_KF_PMTS	"MTS"		//	KMD field
#define	 DEF_KF_IMUT	"DTK"		//	KMD field
#define	 DEF_KF_XC0	"XC0"		//	KMD field
#define	 DEF_KF_YC0	"YC0"		//	KMD field
#define	 DEF_KF_ZC0	"ZC0"		//	KMD field
#define	 DEF_KF_ROLL	"DNR"		//	KMD field
#define	 DEF_KF_PITCH	"DEP"		//	KMD field
#define	 DEF_KF_YAW	"DDY"		//	KMD field
#define	 DEF_KF_GRX	"DGX"		//	KMD field
#define	 DEF_KF_GRY	"DGY"		//	KMD field
#define	 DEF_KF_GRZ	"DGZ"		//	KMD field
#define	 DEF_KF_DIST	"DST"		//	KMD field
#define	 DEF_KF_LAX	"DAX"		//	KMD field
#define	 DEF_KF_LAY	"DAY"		//	KMD field
#define	 DEF_KF_LAZ	"DAZ"		//	KMD field
#define	 DEF_KF_SPX	"SPX"		//	KMD field
#define	 DEF_KF_SPY	"SPY"		//	KMD field
#define	 DEF_KF_SPZ	"SPZ"		//	KMD field
#define	 DEF_KF_Iax	"Iax"		//	KMD field
#define	 DEF_KF_Iay	"Iay"		//	KMD field
#define	 DEF_KF_Iaz	"Iaz"		//	KMD field
#define	 DEF_KF_Igx	"Igx"		//	KMD field
#define	 DEF_KF_Igy	"Igy"		//	KMD field
#define	 DEF_KF_Igz	"Igz"		//	KMD field
#define	 DEF_KF_Imx	"Imx"		//	KMD field
#define	 DEF_KF_Imy	"Imy"		//	KMD field
#define	 DEF_KF_Imz	"Imz"		//	KMD field
//	======================================================
//	 GLOBAL OPERATIONS DICTIONNARY
//	------------------------------------------------------
#define	 DEF_OP_INFO	"Inf"		//	KMD operation
#define	 DEF_OP_MOT	"Mot"		//	KMD operation
#define	 DEF_OP_PID	"Pid"		//	KMD operation
#define	 DEF_OP_STOP	"Stop"		//	KMD operation
#define	 DEF_OP_OPEN	"Open"		//	KMD operation
#define	 DEF_OP_CLOSE	"Close"		//	KMD operation
#define	 DEF_OP_SYNC	"Syn"		//	KMD operation
#define	 DEF_OP_LEARN	"Learn"		//	KMD operation
#define	 DEF_OP_JOBGOAL	"Jobgoal"		//	KMD operation
#define	 DEF_OP_PILOT	"Pilot"		//	KMD operation
#define	 DEF_OP_TSTSQ	"TstSQ"		//	KMD operation
#define	 DEF_OP_SETDLVL	"set_DLVL"		//	KMD operation
#define	 DEF_OP_SETTOUT	"setTmsOut"		//	KMD operation
#define	 DEF_OP_GETTOUT	"getTmsOut"		//	KMD operation
#define	 DEF_OP_OK	"Ok?"		//	KMD operation
#define	 DEF_OP_CHK	"Chk"		//	KMD operation
//	======================================================
//	 If browser client is in javascript debug (devtools...)
//	 Add a delay to period timing
#define	 DEF_DBGBROWSER	3
//	------------------------------------------------------
//	 If rover is in debug (print delays the application bandwidth)
//	 Add delay to period timing
#define	 DEF_DBGLVLMAX	3
//	======================================================
//	 Default values for timing
//	 Add delay to period timing
//	------------------------------------------------------
#define	 DEF_SEND_TMS	0
#define	 DEF_SEND_TMS_JOBGOAL	333
#define	 DEF_WATCH_TMS	150+(50*DEF_DBGLVLMAX)		//	for WS reconnection
#include	 "WString.h"		//	reference for String on Esp32 !!!
//	======================================================
//	 URL & WS CONFIGURATION TYPE
//	------------------------------------------------------
typedef struct {
	String roverURL; //	ROVER server url
	uint16_t roverPORT; //	ROVER server port
	String basisURL; //	BASIS server url
	uint16_t basisPORT; //	BASIS server port
	uint8_t depthFIFO; //	depth of all fifos
	uint16_t BamountTMS; //	(ms) buffered amount timeout
} s_ALL;
//	======================================================
//	 MODULE CONFIGURATION Analogical operator
//	------------------------------------------------------
#include	 <array>
#define	 DEF_MOTORS_NBAXIS	2
#define	 DEF_MOTORS_RQSIZE	5
#define	 DEF_KINE_RQSIZE	20
typedef std::array<float, DEF_MOTORS_NBAXIS> t_AfloatAxes;
typedef std::array<int32_t, DEF_MOTORS_NBAXIS> t_Aint32Axes;
//	======================================================
//	 MODULE CONFIGURATION TYPE
//	------------------------------------------------------
typedef struct {
	uint32_t loop_TMS; //	(ms) for scheduler max 1 mn (65535)
	String Name; //	module name
	String Mid; //	module identifier
	String Qtype; //	query string protocol
} s_MODULE;
#define	 MPU9250_ADDRESS	0x68
#define	 MAG_ADDRESS	0x0C
#define	 GYRO_FULL_SCALE_250_DPS	0x00
#define	 GYRO_FULL_SCALE_500_DPS	0x08
#define	 GYRO_FULL_SCALE_1000_DPS	0x10
#define	 GYRO_FULL_SCALE_2000_DPS	0x18
#define	 ACC_FULL_SCALE_2_G	0x00
#define	 ACC_FULL_SCALE_4_G	0x08
#define	 ACC_FULL_SCALE_8_G	0x10
#define	 ACC_FULL_SCALE_16_G	0x18
//	======================================================
//	 IMU DEFINITION
//	------------------------------------------------------
typedef struct {
	String Cnam; //	Core name
	std::array<float, 3> acc;
	std::array<float, 3> gyr;
	std::array<float, 3> mag;
} asmIMU;
//	======================================================
//	 USD DEFINITION
//	------------------------------------------------------
typedef struct {
	String Cnam; //	Core name
	float dist;
} asmUSD;
//	******************************************************
//	 RINGQUEUE ITEM DEFINITION
//	------------------------------------------------------
typedef struct { //	pidQueueData definition
	uint64_t micTSrq;
	float DER;
	float INT;
	float inERR;
	float outCMD;
	char MSG[232 - 36];
} pidQueueData;
//	******************************************************
//	 PID DEFINITION
//	------------------------------------------------------
#include "000_QUEUE_RING.h"
typedef struct { //	s_pid definition
	float _kp; //	gain proportionnel sur la moyenne
	int _dp; //		profondeur de moyenne
	float _ki; //	gain intégral
	int _di; //		profondeur d'integration
	float _kd; //	gain dérivé
	int _dd; //		profondeur de derivation
	float _Min; //	seuil min de la consigne
	float _Max; //	seuil max de la consigne
	ringQueue<pidQueueData> *_RQ; //	ringQueue pointer
} s_pid;
//	------------------------------------------------------
typedef struct { //	asmPIDs definition
	s_pid _pid[DEF_MOTORS_NBAXIS];
} asmPIDs;
#define	 DEF_PWM_RESOLUTION	12
#define	 DEF_PWM_VALUEMAX	1023
#define	 DEF_PWM_DUTYMAX	((1<<DEF_PWM_RESOLUTION)-1)/DEF_PWM_VALUEMAX
//	 PWM DEFINITION
//	------------------------------------------------------
//	WARNING: non-trivial designated initializers not supported
//	the order of initialization needs to be in the exact order of declaration.
typedef struct { //	PWM definition
	uint8_t _pinSENS; //	Sens de rotation
	uint8_t _pinPWM; //	Vitesse de rotation
	uint8_t _channel; //	Canal du timer utilisé pour le PWM
	uint8_t _resolution; //	Nombre de bits de résolution
	double _frequence; //	Fréquence
	int32_t _valueMax; //	Valeur max
	uint32_t _dutyMax; //	{((1<<_resolution)-1)/_valueMax},//8191 from 2 ^ 13 - 1
	int32_t _valueCur; //	last consigne
} s_pwm;
//	------------------------------------------------------
typedef struct { //	PWM definition
	s_pwm _pwm[DEF_MOTORS_NBAXIS];
} asmPWMs;
;
//	======================================================
//	 MOTORS DEFINITION
//	------------------------------------------------------
typedef struct {
	s_MODULE MOD; //	Module standard parent configuration
	String pidCnam; //	Core name
	String pwmCnam; //	Core name
	s_pid rotPID;
	s_pid linPID;
	s_pwm leftPWM;
	s_pwm rightPWM;
} s_MOTORS;
//	 "MOTORS" MODULE CONFIGURATION DATA
//	======================================================
//	 IMU DEFINITION
//	------------------------------------------------------
typedef struct {
	float LMD; //	local magnetic declination in radian(s)
	uint8_t SAMPLERATE; //	IMU refresh frequency (0x00:1 kHz, 0x04:200 Hz, 0x09:100 Hz)
} s_IMU;
//	======================================================
//	 DRIVER MODULE CONFIGURATION TYPE
//	------------------------------------------------------
typedef struct {
	s_MODULE MOD; //	Module standard parent configuration
	String imuCnam; //	Core name
	String usdCnam; //	Core name
	s_IMU IMU; //	IMU definition
} s_DRIVER;
//	======================================================
//	 WATCHDOG MODULE CONFIGURATION TYPE
//	------------------------------------------------------
typedef struct {
	s_MODULE MOD; //	Module standard parent configuration
} s_WATCHDOG;
//	======================================================
//	 JOBGOAL MODULE CONFIGURATION TYPE
//	------------------------------------------------------
typedef struct {
	s_MODULE MOD; //	Module standard parent configuration
} s_JOBGOAL;
//	======================================================
//	 TIMEMS MODULE CONFIGURATION TYPE
//	------------------------------------------------------
typedef struct {
	s_MODULE MOD; //	Module standard parent configuration
} s_TIMEMS;
//	======================================================
//	 BASIS CONFIGURATION TYPE
//	------------------------------------------------------
typedef struct {
	uint32_t loop_TMS; //	(ms) for scheduler
	uint16_t watch_TMS; //	(ms) for client connect retry
	bool cnx_ROVER; //	connect to ROVER server on|off
	String PAT2NOD; //	path to node
} s_BASIS;
//	======================================================
//	 PILOT CONFIGURATION TYPE
//	------------------------------------------------------
typedef struct {
	uint32_t loop_TMS; //	(ms) for scheduler
	uint16_t watch_TMS; //	(ms) for client connect retry
	bool cnx_ROVER; //	connect to ROVER server on|off
	bool cnx_BASIS; //	connect to BASIS server on|off
} s_PILOT;
//	//////////////////////////////////////////////////////
#endif
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
