// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//     Config ROVER  Version:'000_WS3' du 29 juil. 2021 à 15:59:35
//     File /media/DEV/ROB_000/setup/SCRIPTS/../SET_000_WS3/rov-CfgHPP(000_WS3).hpp
//
//     This file is generated automatically. Don't modify it !!!
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#pragma once
//	======================================================
//	 URL & WS CONFIGURATION DATA
//	------------------------------------------------------
s_ALL allCFG ={
	.roverURL	="192.168.4.1",		//	ROVER server url
	.roverPORT	=80,		//	ROVER server port
	.basisURL	="localhost",		//	BASIS server url
	.basisPORT	=1337,		//	BASIS server port
	.depthFIFO	=16,		//	depth of all fifos
	.BamountTMS	=500,		//	(ms) buffered amount timeout
};
//	======================================================
//	======================================================
//	******************************************************
//	 "MOTORS" MODULE CONFIGURATION DATA
//	------------------------------------------------------
s_MOTORS MOTORScfg ={		//	LALALA
	.MOD	={		//	Module standard parent configuration
		.loop_TMS	=DEF_SEND_TMS,		//	(ms) for scheduler
		.Name	="MOTORS",		//	module name
		.Mid	="MOT",		//	module identifier
		.Qtype	=DEF_QUERY_ROVER_TO_PILOT,		//	query string protocol
	},
	.pidCnam	="pid",		//	pid Core name
	.pwmCnam	="pwm",		//	pwm Core name
	.rotPID	={
		._kp	=0.5,		//	gain proportionnel sur la moyenne
		._dp	=0,		//	profondeur de moyenne (<=DEF_MOTORS_RQSIZE !!!)
		._ki	=3.0,		//	gain intégral
		._di	=1,		//	profondeur d'integration (<=DEF_MOTORS_RQSIZE !!!)
		._kd	=0.0,		//	gain dérivé
		._dd	=0,		//	profondeur de derivation (<=DEF_MOTORS_RQSIZE !!!)
		._Min	=-1.0,		//	seuil min de la consigne
		._Max	=+1.0,		//	seuil max de la consigne
		._RQ	=new ringQueue<pidQueueData>(DEF_MOTORS_RQSIZE,0),		//	ringQueue
	},
	.linPID	={
		._kp	=0.5,		//	gain proportionnel sur la moyenne
		._dp	=0,		//	profondeur de moyenne (<=DEF_MOTORS_RQSIZE !!!)
		._ki	=3.0,		//	gain intégral
		._di	=1,		//	profondeur d'integration (<=DEF_MOTORS_RQSIZE !!!)
		._kd	=0.0,		//	gain dérivé
		._dd	=0,		//	profondeur de derivation (<=DEF_MOTORS_RQSIZE !!!)
		._Min	=-1.0,		//	seuil min de la consigne
		._Max	=+1.0,		//	seuil max de la consigne
		._RQ	=new ringQueue<pidQueueData>(DEF_MOTORS_RQSIZE,0),		//	ringQueue
	},
	.leftPWM	={
#ifdef DEF_WITHMOTORSPINS
		._pinSENS	=vio7PIN_Lsns,		//	Sens de rotation
		._pinPWM	=blu6PIN_Lpwm,		//	Vitesse de rotation
#endif
		._channel	=2,		//	Canal du timer utilisé pour le PWM
		._resolution	=DEF_PWM_RESOLUTION,		//	Nombre de bits de résolution
		._frequence	=1000,		//	Fréquence
		._valueMax	=DEF_PWM_VALUEMAX,		//	Valeur max
		._dutyMax	=DEF_PWM_DUTYMAX,		//	{((1<<DEF_PWM_RESOLUTION)-1)/DEF_PWM_VALUEMAX},//8191 from 2 ^ 13 - 1
		._valueCur	=0,		//	last consigne
	},
	.rightPWM	={
#ifdef DEF_WITHMOTORSPINS
		._pinSENS	=gre5PIN_Rsns,		//	Sens de rotation
		._pinPWM	=yel4PIN_Rpwm,		//	Vitesse de rotation
#endif
		._channel	=0,		//	Canal du timer utilisé pour le PWM
		._resolution	=DEF_PWM_RESOLUTION,		//	Nombre de bits de résolution
		._frequence	=1000,		//	Fréquence
		._valueMax	=DEF_PWM_VALUEMAX,		//	Valeur max
		._dutyMax	=DEF_PWM_DUTYMAX,		//	{((1<<DEF_PWM_RESOLUTION)-1)/DEF_PWM_VALUEMAX},//8191 from 2 ^ 13 - 1
		._valueCur	=0,		//	last consigne
	},
};
//	======================================================
//	 DRIVER MODULE CONFIGURATION DATA
//	------------------------------------------------------
s_DRIVER DRIVERcfg ={
	.MOD	={		//	Module standard parent configuration
		.loop_TMS	=100,		//	(ms) !!!
		.Name	="DRIVER",		//	module name
		.Mid	="DRV",		//	module identifier
		.Qtype	=DEF_QUERY_ROVER_TO_PILOT,		//	query string protocol
	},
	.imuCnam	="imu",		//	IMU Core name
	.usdCnam	="usd",		//	USD Core name
	.IMU	={
		.LMD	=0.0,		//	local magnetic declination in radian(s)
		.SAMPLERATE	=0x09,		//	IMU refresh frequency (0x00:1 kHz, 0x04:200 Hz, 0x09:100 Hz)
	},
};
//	======================================================
//	 WATCHDOG MODULE CONFIGURATION DATA
//	------------------------------------------------------
s_WATCHDOG WATCHDOGcfg ={
	.MOD	={		//	Module standard parent configuration
		.loop_TMS	=1000,		//	(ms) Emergency stop !!!
		.Name	="WATCHDOG",		//	module name
		.Mid	="WAT",		//	module identifier
		.Qtype	=DEF_QUERY_ROVER_TO_PILOT,		//	query string protocol
	},
};
//	======================================================
//	 JOBGOAL MODULE CONFIGURATION DATA
//	------------------------------------------------------
s_JOBGOAL JOBGOALcfg ={
	.MOD	={		//	Module standard parent configuration
		.loop_TMS	=2000,		//	(ms) NO scheduling !!!
		.Name	="JOBGOAL",		//	module name
		.Mid	="JOB",		//	module identifier
		.Qtype	=DEF_QUERY_ROVER_TO_PILOT,		//	query string protocol
	},
};
//	======================================================
//	 TIMEMS MODULE CONFIGURATION DATA
//	------------------------------------------------------
s_TIMEMS TIMEMScfg ={
	.MOD	={		//	Module standard parent configuration
		.loop_TMS	=10*DEF_SEND_TMS,		//	(ms) for scheduler
		.Name	="TIMEms",		//	module name
		.Mid	="TMS",		//	module identifier
		.Qtype	=DEF_QUERY_ROVER_TO_PILOT,		//	query string protocol
	},
};
//	======================================================
//	 BASIS CONFIGURATION DATA
//	------------------------------------------------------
s_BASIS basisCFG ={
	.loop_TMS	=DEF_SEND_TMS,		//	(ms) for scheduler
	.watch_TMS	=DEF_WATCH_TMS,		//	(ms) for client connect retry
	.cnx_ROVER	=true,		//	webSocket server on|off
	.PAT2NOD	="/media/DEV/ROB_000/node.js/node_modules/",		//	path to node
};
//	======================================================
//	 PILOT CONFIGURATION DATA
//	------------------------------------------------------
s_PILOT pilotCFG ={
	.loop_TMS	=DEF_SEND_TMS,		//	(ms) for scheduler
	.watch_TMS	=DEF_WATCH_TMS,		//	(ms) for clients connect retry
	.cnx_ROVER	=true,		//	connect to ROVER server on|off
	.cnx_BASIS	=true,		//	connect to BASIS server on|off
};
//	//////////////////////////////////////////////////////
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
