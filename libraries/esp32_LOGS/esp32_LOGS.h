/** @file */
#pragma once
#include "esp32_DEBUG.h"
#include "esp32_UDP_log.h"
#include <esp32_TASKS.h>
using namespace std;
typedef s_logITM *pudpx; /** < @todo !!! pour aller avec classe public pdatagram avec aussi pwsx vers les fifos de régulation de 000_DATAG.h vs flow (datagramme) */
///////////////////////////////////////////////////////////////////////
class c_myLOGS: public Core {
public:
	c_myLOGS();
	void _LOG_(const int8_t LVL, const int8_t THIS_DBGMAX, const char *FORMAT, ...);
	bool _UDP_(char *BUFFER);
};
//bool _UDP_(char *BUFFER);
//bool sendUDP(const uint32_t CALLS, const char * FORMAT, va_list * ARGS,	const char * PREFIX = nullptr);
///////////////////////////////////////////////////////////////////////
#define DEBUG_FATAL_ERR true
//=====================================================================
/**
 *
 *	To output these debug levels must be less than debug level MAX 
 *	(vs this_DebugLVL) from where their macro are called.
 *
 *
 */
#define LVL_TSK_WSX_CHANGE					4 // task RCV & SND
#define _MAX_DBG_APP_BOOT					-1
#define _MAX_DBG_APP_LOOP									8
#define _MAX_DBG_APP_KMDS					4
#define _MAX_DBG_DRV_XIMU						-1
#define _MAX_DBG_TSK_ROOT					4
#define _MAX_DBG_MOD_WHAT			2 // All modules
#define _MAX_DBG_MOD_FLOW 										9
#define _MAX_DBG_DRV_KINE 										9
#define _MAX_DBG_DRV_PUSH					4//					6 //
#define _MAX_DBG_DRV_FIFO 										9 // IMU fifo in out
#define _MAX_DBG_MOT_PIDX					4					
//==============================================================
#ifdef LVL_TSK_WSX_CHANGE
#define _SERIAL_WSXCHANGE(TITLE,DIR,PWSX,CALL,DATA,LEN) \
	do { \
		serial_WSXCHANGE(TITLE, DIR,  PWSX, CALL,DATA,LEN); \
	}while(0)
#else
#define _SERIAL_WSXCHANGE(...)
#endif
//==============================================================
#ifdef LVL_TSK_WSX_DATA
#define _DBG_WSXDATA(format,...) \
	do { \
	myLOGS._LOG_(LVL_TSK_WSX_DATA,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_WSXDATA(...)
#endif
//==============================================================
//	TSK 	===========================================================
//	_DBG_TSK_EXEC-----------------------------------------------------------
#ifdef _MAX_DBG_TSK_ROOT
#define _DBG_TSK_ROOT(format,...) \
	do { \
		myLOGS._LOG_(_MAX_DBG_TSK_ROOT,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_TSK_ROOT(...)
#endif
//	APP 	===========================================================
//	APP BOOT-----------------------------------------------------------
#ifdef _MAX_DBG_APP_BOOT
#define _DBG_APP_BOOT(format,...) \
	do { \
		myLOGS._LOG_(_MAX_DBG_APP_BOOT,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_APP_BOOT(...)
#endif
//	APP LOOP-----------------------------------------------------------
#ifdef _MAX_DBG_APP_LOOP
#define _DBG_APP_LOOP(format,...) \
	do { \
		myLOGS._LOG_(_MAX_DBG_APP_LOOP,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_APP_LOOP(...)
#endif 
//	APP KMDS-----------------------------------------------------------
#ifdef _MAX_DBG_APP_KMDS
#define _DBG_APP_KMDS(format,...) \
	do { \
		myLOGS._LOG_(_MAX_DBG_APP_KMDS,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_APP_KMDS(...)
#endif 
//	MOD 	===========================================================
//	MOD WHAT-----------------------------------------------------------
#ifdef _MAX_DBG_MOD_WHAT
#define _DBG_MOD_WHAT(format,...) \
	do { \
		myLOGS._LOG_(_MAX_DBG_MOD_WHAT,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_MOD_WHAT(...)
#endif
//	MOD FLOW-----------------------------------------------------------
#ifdef _MAX_DBG_MOD_FLOW
#define _DBG_MOD_FLOW(format,...) \
	do { \
		myLOGS._LOG_(_MAX_DBG_MOD_FLOW,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_MOD_FLOW(...)
#endif 
//	DRV 	===========================================================
//	DRV XIMU-----------------------------------------------------------
#ifdef _MAX_DBG_DRV_XIMU
#define _DBG_DRV_XIMU(format,...) \
	do { \
		myLOGS._LOG_(CTX.SOFTctx.MAX_DBG_DRV_XIMU,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_DRV_XIMU(...)
#endif
//	DRV KINE-----------------------------------------------------------
#ifdef _MAX_DBG_DRV_KINE
#define _DBG_DRV_KINE_(format,...) \
	do { \
		myLOGS._LOG_(_MAX_DBG_DRV_KINE,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_DRV_KINE_(...)
#endif 
//	DRV DATA-----------------------------------------------------------
#ifdef _MAX_DBG_DRV_PUSH
#define _DBG_DRV_PUSH(format,...) \
	do { \
		myLOGS._LOG_(_MAX_DBG_DRV_PUSH,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_DRV_PUSH(...)
#endif
//	DRV FIFO-----------------------------------------------------------
#ifdef _MAX_DBG_DRV_FIFO
#define _DBG_DRV_FIFO(format,...) \
	do { \
		myLOGS._LOG_(_MAX_DBG_DRV_FIFO,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_DRV_FIFO(...)
#endif 
//	MOT 	===========================================================
//	MOT PIDX-----------------------------------------------------------
#ifdef _MAX_DBG_MOT_PIDX
#define _DBG_MOT_PIDX(format,...) \
	do { \
		myLOGS._LOG_(_MAX_DBG_MOT_PIDX,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_MOT_PIDX(...)
#endif 
///////////////////////////////////////////////////////////////////////
extern c_myLOGS myLOGS;
///
//#endif //__esp32_LOGS__
