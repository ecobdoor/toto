// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//     Config PILOT  Version:'000_WS3' du 29 juil. 2021 à 15:59:35
//     File /media/DEV/ROB_000/setup/SCRIPTS/../SET_000_WS3/pil-CfgJS(000_WS3).js
//
//     This file is generated automatically. Don't modify it !!!
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//	======================================================
//	 GLOBAL WS & JSON PROTOCOLS
//	------------------------------------------------------
//	---- dialogs ROVER BASIS -----------------------------
const DEF_QUERY_ROVER_TO_BASIS=	"rQb";
const DEF_REPLY_BASIS_TO_ROVER=	"bRr";
const DEF_QUERY_BASIS_TO_ROVER=	"bQr";
const DEF_REPLY_ROVER_TO_BASIS=	"rRb";
//	---- dialogs PILOT ROVER -----------------------------
const DEF_QUERY_PILOT_TO_ROVER=	"pQr";
const DEF_REPLY_ROVER_TO_PILOT=	"rRp";
const DEF_QUERY_ROVER_TO_PILOT=	"rQp";
const DEF_REPLY_PILOT_TO_ROVER=	"pRr";
//	---- dialogs BASIS PILOT -----------------------------
const DEF_QUERY_BASIS_TO_PILOT=	"bQp";
const DEF_REPLY_PILOT_TO_BASIS=	"pRb";
const DEF_QUERY_PILOT_TO_BASIS=	"pQb";
const DEF_REPLY_BASIS_TO_PILOT=	"bRp";
//	======================================================
//	 GLOBAL COMMANDS DICTIONNARY
//	------------------------------------------------------
const DEF_KF_DIR=	"D";		//	KMD field
const DEF_KF_SPD=	"S";		//	KMD field
const DEF_KF_LFT=	"L";		//	KMD field
const DEF_KF_RGT=	"R";		//	KMD field
const DEF_KF_FIL=	"F";		//	KMD field
const DEF_KF_GET=	"@";		//	KMD field
const DEF_KF_EXE=	"X";		//	KMD field
const DEF_KF_MTS=	"MTS";		//	KMD field (µs)
const DEF_KF_PMTS=	"MTS";		//	KMD field
const DEF_KF_IMUT=	"DTK";		//	KMD field
const DEF_KF_XC0=	"XC0";		//	KMD field
const DEF_KF_YC0=	"YC0";		//	KMD field
const DEF_KF_ZC0=	"ZC0";		//	KMD field
const DEF_KF_ROLL=	"DNR";		//	KMD field
const DEF_KF_PITCH=	"DEP";		//	KMD field
const DEF_KF_YAW=	"DDY";		//	KMD field
const DEF_KF_GRX=	"DGX";		//	KMD field
const DEF_KF_GRY=	"DGY";		//	KMD field
const DEF_KF_GRZ=	"DGZ";		//	KMD field
const DEF_KF_DIST=	"DST";		//	KMD field
const DEF_KF_LAX=	"DAX";		//	KMD field
const DEF_KF_LAY=	"DAY";		//	KMD field
const DEF_KF_LAZ=	"DAZ";		//	KMD field
const DEF_KF_SPX=	"SPX";		//	KMD field
const DEF_KF_SPY=	"SPY";		//	KMD field
const DEF_KF_SPZ=	"SPZ";		//	KMD field
const DEF_KF_Iax=	"Iax";		//	KMD field
const DEF_KF_Iay=	"Iay";		//	KMD field
const DEF_KF_Iaz=	"Iaz";		//	KMD field
const DEF_KF_Igx=	"Igx";		//	KMD field
const DEF_KF_Igy=	"Igy";		//	KMD field
const DEF_KF_Igz=	"Igz";		//	KMD field
const DEF_KF_Imx=	"Imx";		//	KMD field
const DEF_KF_Imy=	"Imy";		//	KMD field
const DEF_KF_Imz=	"Imz";		//	KMD field
//	======================================================
//	 GLOBAL OPERATIONS DICTIONNARY
//	------------------------------------------------------
const DEF_OP_INFO=	"Inf";		//	KMD operation
const DEF_OP_MOT=	"Mot";		//	KMD operation
const DEF_OP_PID=	"Pid";		//	KMD operation
const DEF_OP_STOP=	"Stop";		//	KMD operation
const DEF_OP_OPEN=	"Open";		//	KMD operation
const DEF_OP_CLOSE=	"Close";		//	KMD operation
const DEF_OP_SYNC=	"Syn";		//	KMD operation
const DEF_OP_LEARN=	"Learn";		//	KMD operation
const DEF_OP_JOBGOAL=	"Jobgoal";		//	KMD operation
const DEF_OP_PILOT=	"Pilot";		//	KMD operation
const DEF_OP_TSTSQ=	"TstSQ";		//	KMD operation
const DEF_OP_SETDLVL=	"set_DLVL";		//	KMD operation
const DEF_OP_SETTOUT=	"setTmsOut";		//	KMD operation
const DEF_OP_GETTOUT=	"getTmsOut";		//	KMD operation
const DEF_OP_OK=	"Ok?";		//	KMD operation
const DEF_OP_CHK=	"Chk";		//	KMD operation
//	======================================================
//	 If browser client is in javascript debug (devtools...)
//	 Add a delay to period timing
const DEF_DBGBROWSER=	3;
//	------------------------------------------------------
//	 If rover is in debug (print delays the application bandwidth)
//	 Add delay to period timing
const DEF_DBGLVLMAX=	3;
//	======================================================
//	 Default values for timing
//	 Add delay to period timing
//	------------------------------------------------------
const DEF_SEND_TMS=	0;
const DEF_SEND_TMS_JOBGOAL=	333;
const DEF_WATCH_TMS=	150+(50*DEF_DBGLVLMAX);		//	for WS reconnection
//	======================================================
//	 URL & WS CONFIGURATION DATA
//	------------------------------------------------------
var allCFG={
	roverURL:	"192.168.4.1",//"10.42.0.191",//		//	ROVER server url
	roverPORT:	80,		//	ROVER server port
	basisURL:	"localhost",		//	BASIS server url
	basisPORT:	1337,		//	BASIS server port
	depthFIFO:	16,		//	depth of all fifos
	BamountTMS:	500,		//	(ms) buffered amount timeout
};
//	======================================================
const DEF_MOTORS_NBAXIS=	2;
//	======================================================
//	******************************************************
//	 "MOTORS" MODULE CONFIGURATION DATA
//	------------------------------------------------------
var MOTORScfg={		//	LALALA
	MOD:{		//	Module standard parent configuration
		loop_TMS:	DEF_SEND_TMS,		//	(ms) for scheduler
		Name:	"MOTORS",		//	module name
		Mid:	"MOT",		//	module identifier
		Qtype:	DEF_QUERY_ROVER_TO_PILOT,		//	query string protocol
	},
};
//	======================================================
//	 DRIVER MODULE CONFIGURATION DATA
//	------------------------------------------------------
var DRIVERcfg={
	MOD:{		//	Module standard parent configuration
		loop_TMS:	100,		//	(ms) !!!
		Name:	"DRIVER",		//	module name
		Mid:	"DRV",		//	module identifier
		Qtype:	DEF_QUERY_ROVER_TO_PILOT,		//	query string protocol
	},
};
//	======================================================
//	 WATCHDOG MODULE CONFIGURATION DATA
//	------------------------------------------------------
var WATCHDOGcfg={
	MOD:{		//	Module standard parent configuration
		loop_TMS:	1000,		//	(ms) Emergency stop !!!
		Name:	"WATCHDOG",		//	module name
		Mid:	"WAT",		//	module identifier
		Qtype:	DEF_QUERY_ROVER_TO_PILOT,		//	query string protocol
	},
};
//	======================================================
//	 JOBGOAL MODULE CONFIGURATION DATA
//	------------------------------------------------------
var JOBGOALcfg={
	MOD:{		//	Module standard parent configuration
		loop_TMS:	2000,		//	(ms) NO scheduling !!!
		Name:	"JOBGOAL",		//	module name
		Mid:	"JOB",		//	module identifier
		Qtype:	DEF_QUERY_ROVER_TO_PILOT,		//	query string protocol
	},
};
//	======================================================
//	 TIMEMS MODULE CONFIGURATION DATA
//	------------------------------------------------------
var TIMEMScfg={
	MOD:{		//	Module standard parent configuration
		loop_TMS:	10*DEF_SEND_TMS,		//	(ms) for scheduler
		Name:	"TIMEms",		//	module name
		Mid:	"TMS",		//	module identifier
		Qtype:	DEF_QUERY_ROVER_TO_PILOT,		//	query string protocol
	},
};
//	======================================================
//	 BASIS CONFIGURATION DATA
//	------------------------------------------------------
var basisCFG={
	loop_TMS:	DEF_SEND_TMS,		//	(ms) for scheduler
	watch_TMS:	DEF_WATCH_TMS,		//	(ms) for client connect retry
	cnx_ROVER:	true,		//	webSocket server on|off
	PAT2NOD:	"/media/DEV/ROB_000/node.js/node_modules/",		//	path to node
};
//	======================================================
//	 PILOT CONFIGURATION DATA
//	------------------------------------------------------
var pilotCFG={
	loop_TMS:	DEF_SEND_TMS,		//	(ms) for scheduler
	watch_TMS:	DEF_WATCH_TMS,		//	(ms) for clients connect retry
	cnx_ROVER:	true,		//	connect to ROVER server on|off
	cnx_BASIS:	true,		//	connect to BASIS server on|off
};
//	//////////////////////////////////////////////////////
