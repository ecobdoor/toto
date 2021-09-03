/** @file */
#ifndef __000_LOGS__
#define __000_LOGS__
#include "000_DEBUG.h" 
#include <000_FSTR.h>
#include <000_WSCHK.h>
#include "esp32_UDPL_log.h"
#include <000_TASKS.h>
using namespace std;
/*
 typedef struct {
 TaskHandle_t tsk = nullptr;
 //s_tskInfo tsk Info; //={nullptr,0,0,0} ;
 uint32_t msgID = 99999999;
 s_wsChecker *pwsx = nullptr;
 uint16_t wsWait = 0;
 char outMSG[DEF_WSMSG_BUFSIZE] = "";
 uint16_t SCBlen = 0;
 char jsonExplain[DEF_JEXPL_BUFSIZE] = "";
 Scratch *rtnScratch = nullptr;
 int msWaitQueue = 0;
 int msWaitNotify = 0;
 } s_OUTfifoItem;
 */
typedef s_logITM *pudpx; /** < @todo !!! pour aller avec classe public pdatagram avec aussi pwsx vers les fifos de régulation de 000_DATAG.h vs flow (datagramme) */
///////////////////////////////////////////////////////////////////////
int16_t insertSenderCtrlBlock(const uint32_t NUMMSG, const uint64_t OLDTMS,
	const uint64_t NEWTMS, s_wsChecker *PWSX, char *BUFFER,
	const uint16_t SIZE/*,const char * SRC*/);
//void notify_SENDER(s_OUTfifoItem &ITM);
//bool SND_DOIT(s_OUTfifoItem &ITM);
bool _CSV_(const char *FORMAT, ...);
void _LOG_(const int8_t LVL, const int8_t THIS_DBGMAX, const char *FORMAT, ...);
//bool sendUDP(const uint32_t CALLS, const char * FORMAT, va_list * ARGS,	const char * PREFIX = nullptr);
///////////////////////////////////////////////////////////////////////
#define DEBUG_FATAL_ERR true
#define LVL_DBG__ERR
//=====================================================================
/**
 *
 *	To output these debug levels must be less than debug level MAX 
 *	(vs this_DebugLVL) from where their macro are called.
 *
 *
 */
#define LVL_TSK_WSX_CHANGE					4 // task RCV & SND
#define LVL_TSK_WSX_DATA						5
#define _MAX_DBG_APP_BOOT					-1
#define _MAX_DBG_APP_LOOP									8
#define _MAX_DBG_APP_KMDS					4
#define _MAX_DBG_TSK_ROOT					4
#define _MAX_DBG_MOD_WHAT			2 // All modules
#define _MAX_DBG_MOD_FLOW 										9
#define _MAX_DBG_DRV_XIMU 										9
#define _MAX_DBG_DRV_KINE 										9
#define _MAX_DBG_DRV_PUSH					4//					6 //
#define _MAX_DBG_DRV_FIFO 										9
#define _MAX_DBG_MOT_PIDX					4					
//#####################################################################
#ifdef LVL_TSK_WSX_DATA
#define _SERIAL_WSXDATA(DIRSTR,DATA) \
	do { \
		extern int8_t & this_DebugLVL; \
		serial_WSXDATA(DIRSTR,DATA,this_DebugLVL); \
	}while(0)
#else
#define _SERIAL_WSXDATA(...)
#endif
//==============================================================
#ifdef LVL_TSK_WSX_CHANGE
#define _SERIAL_WSXCHANGE(DIR,PWSX,CALL) \
	do { \
		extern int8_t & this_DebugLVL; \
		serial_WSXCHANGE( DIR,  PWSX, CALL,this_DebugLVL); \
	}while(0)
#else
#define _SERIAL_WSXCHANGE(...)
#endif
//==============================================================
#ifdef LVL_TSK_WSX_DATA
#define _DBG_WSXDATA(format,...) \
	do { \
	_LOG_(LVL_TSK_WSX_DATA,this_DebugLVL,format,## __VA_ARGS__); \
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
	_LOG_(_MAX_DBG_TSK_ROOT,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_TSK_ROOT(...)
#endif
//	APP 	===========================================================
//	APP BOOT-----------------------------------------------------------
#ifdef _MAX_DBG_APP_BOOT
#define _DBG_APP_BOOT(format,...) \
	do { \
	_LOG_(_MAX_DBG_APP_BOOT,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_APP_BOOT(...)
#endif
//	APP LOOP-----------------------------------------------------------
#ifdef _MAX_DBG_APP_LOOP
#define _DBG_APP_LOOP(format,...) \
	do { \
		_LOG_(_MAX_DBG_APP_LOOP,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_APP_LOOP(...)
#endif 
//	APP KMDS-----------------------------------------------------------
#ifdef _MAX_DBG_APP_KMDS
#define _DBG_APP_KMDS(format,...) \
	do { \
		_LOG_(_MAX_DBG_APP_KMDS,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_APP_KMDS(...)
#endif 
//	MOD 	===========================================================
//	MOD WHAT-----------------------------------------------------------
#ifdef _MAX_DBG_MOD_WHAT
#define _DBG_MOD_WHAT(format,...) \
	do { \
	_LOG_(_MAX_DBG_MOD_WHAT,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_MOD_WHAT(...)
#endif
//	MOD FLOW-----------------------------------------------------------
#ifdef _MAX_DBG_MOD_FLOW
#define _DBG_MOD_FLOW(format,...) \
	do { \
		_LOG_(_MAX_DBG_MOD_FLOW,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_MOD_FLOW(...)
#endif 
//	DRV 	===========================================================
//	DRV XIMU-----------------------------------------------------------
#ifdef _MAX_DBG_DRV_XIMU
#define _DBG_DRV_XIMU(format,...) \
	do { \
		_LOG_(_MAX_DBG_DRV_XIMU,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_DRV_XIMU(...)
#endif
//	DRV KINE-----------------------------------------------------------
#ifdef _MAX_DBG_DRV_KINE
#define _DBG_DRV_KINE_(format,...) \
	do { \
		_LOG_(_MAX_DBG_DRV_KINE,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_DRV_KINE_(...)
#endif 
//	DRV DATA-----------------------------------------------------------
#ifdef _MAX_DBG_DRV_PUSH
#define _DBG_DRV_PUSH(format,...) \
	do { \
	_LOG_(_MAX_DBG_DRV_PUSH,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_DRV_PUSH(...)
#endif
//	DRV FIFO-----------------------------------------------------------
#ifdef _MAX_DBG_DRV_FIFO
#define _DBG_DRV_FIFO(format,...) \
	do { \
		_LOG_(_MAX_DBG_DRV_FIFO,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_DRV_FIFO(...)
#endif 
//	MOT 	===========================================================
//	MOT PIDX-----------------------------------------------------------
#ifdef _MAX_DBG_MOT_PIDX
#define _DBG_MOT_PIDX(format,...) \
	do { \
		_LOG_(_MAX_DBG_MOT_PIDX,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _DBG_MOT_PIDX(...)
#endif 
//=====================================================================
//
//#####################################################################
//
#ifdef LVL_DBG__ERR
#define _DEBUG_ERR(format,...) \
	do { \
		Serial.printf(format,## __VA_ARGS__); \
		if (DEBUG_FATAL_ERR){ \
			c_linkISR::detach_ISR(); \
			Serial.printf("\n%s",dump_TASKS().c_str()); \
			Serial.printf("\n##############################################"); \
			char x=inKey("\nFATAL ERROR. Wait for hit key (Space=continue, Enter=restart)..."); \
			Serial.printf("\n############################################## '%c' %i",x,x); \
			if(x=='\n') \
				ESP.restart(); \
			/*vTaskSuspendAll();*/ \
			while (true) { NOP(); } \
		} \
	}while(0)
#else
#define _DEBUG_ERR(...)
#endif //==============================================================
#endif //__000_LOG__
