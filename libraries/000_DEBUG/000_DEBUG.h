#ifndef __000_DEBUG__
#define __000_DEBUG__
/** @file */
#include "Arduino.h"
//#include "000_UTL_SYS_BASIC.h"
#define DEF_SERIAL_BUFSIZE 256
void _DEBUG_(const size_t BUFSIZE,const int8_t LVL, const int8_t THIS_DBGMAX, const char *FORMAT, ...);
void VSPRINTF_BUFFER(const int8_t LVL, const char *FORMAT,
	va_list *ARGS, int16_t SIZE, int16_t &CNT, char *BUFFER);
//=====================================================================
#define LVL_DBG_0 // for ERRORS...
#define LVL_DBG_1
#define LVL_DBG_2
#define LVL_DBG_3
#define LVL_DBG_4
#define LVL_DBG_5
#define LVL_DBG_6
#define LVL_DBG_7
#define LVL_DBG_8
#define LVL_DBG_9
//==============================================================
/**
 * Shortest form
 */
#ifdef LVL_DBG_0
#define _SERIAL_0(format,...) \
	do { \
		_DEBUG_(DEF_SERIAL_BUFSIZE,0,0,format,## __VA_ARGS__); \
	}while(0)
#else
#define _SERIAL_0(...)
#endif //==============================================================
#ifdef LVL_DBG_1
#define _SERIAL_1(format,...) \
	do { \
		_DEBUG_(DEF_SERIAL_BUFSIZE,1,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _SERIAL_1(...)
#endif //==============================================================
#ifdef LVL_DBG_2
#define _SERIAL_2(format,...) \
	do { \
		_DEBUG_(DEF_SERIAL_BUFSIZE,2,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _SERIAL_2(...)
#endif //==============================================================
#ifdef LVL_DBG_3
#define _SERIAL_3(format,...) \
	do { \
		_DEBUG_(DEF_SERIAL_BUFSIZE,3,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _SERIAL_3(...)
#endif //==============================================================
#ifdef LVL_DBG_4
#define _SERIAL_4(format,...) \
	do { \
		_DEBUG_(DEF_SERIAL_BUFSIZE,4,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _SERIAL_4(...)
#endif //==============================================================
#ifdef LVL_DBG_5
#define _SERIAL_5(format,...) \
	do { \
		_DEBUG_(DEF_SERIAL_BUFSIZE,5,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _SERIAL_5(...)
#endif //==============================================================
#ifdef LVL_DBG_6
#define _SERIAL_6(format,...) \
	do { \
		_DEBUG_(DEF_SERIAL_BUFSIZE,6,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _SERIAL_6(...)
#endif //==============================================================
#ifdef LVL_DBG_7
#define _SERIAL_7(format,...) \
	do { \
		_DEBUG_(DEF_SERIAL_BUFSIZE,7,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _SERIAL_7(...)
#endif //==============================================================
#ifdef LVL_DBG_8
#define _SERIAL_8(format,...) \
	do { \
		_DEBUG_(DEF_SERIAL_BUFSIZE,8,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _SERIAL_8(...)
#endif //==============================================================
#ifdef LVL_DBG_9
#define _SERIAL_9(format,...) \
	do { \
	_DEBUG_(DEF_SERIAL_BUFSIZE,9,this_DebugLVL,format,## __VA_ARGS__); \
	}while(0)
#else
#define _SERIAL_9(...)
#endif //==============================================================
///////////////////////////////////////////////////////////////////////
#endif //__000_DEBUG__
