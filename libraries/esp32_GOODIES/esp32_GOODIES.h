/** @file */
#ifndef __000_ESPINFO__
#define __000_ESPINFO__
///////////////////////////////////////////////////////////////////////
#include <Arduino.h>
/**
 * @fn #define timeit(label, code)
 * @brief Executes code and prints label with duration in milliseconds
 */
#define DEF_DBG
#ifdef DEF_DBG
	#define timeit(label, code) \
	do { \
		uint64_t start=micro_TS(); \
		code; \
		Serial.printf("\nDuration=%10.3f ms for %s.",(micro_TS()-start)/1000.0, label); \
	}while(0)
#else
	#define timeit(label, code) \
	do { \
		code; \
	}while(0)
#endif
///////////////////////////////////////////////////////////////////////
bool SETUP_ESPINFO();
/**
 * @fn void halt(const char *REASON, ...);
 * @brief printf(REASON, args) & goes to deep sleep state
 *
 * This is used for unrecoverable errors:
 * * setup errors. For example camera or ftp errors (could be retried later)
 * * left division operators of pixel templates vs 2/RGB888...
 * * forbidden pixel cross constructors (be ware, don't use casting)
 * @param REASON
 * @param ... params of REASON format printf
 * @return void... But ESP in deep sleep state
 */
void halt(const char *REASON, ...);
/**
 * @fn void reboot(const char *REASON, ...);
 * @brief printf(REASON, args) & goes to deep sleep state
 *
 * This is used for unrecoverable errors:
 * * setup errors. For example camera or ftp errors (could be retried later)
 * * left division operators of pixel templates vs 2/RGB888...
 * * forbidden pixel cross constructors (be ware, don't use casting)
 * @param REASON
 * @param ... params of REASON format printf
 * @return void... But ESP reboots
 */
void reboot(const char *REASON, ...);
/**
 * @fn uint64_t micro_TS()
 * @brief Measures microtime
 *
 * Because micro_TS() is called from ISR the IRAM_ATTR attribute forces to be load in ram
 * Cf. https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/spi_flash.html#iram-safe-interrupt-handlers
 * @return esp_timer_get_time() in microsecond since boot
 */
//uint64_t IRAM_ATTR micro_TS();
void printSysState();
void printStackLeft(const char *COMMENT = nullptr);
void sprint(const char *FORMAT, ...);
/**
 * @fn String datim(uint64_t TMS, const int SHIFTHOURS = 1)
 * @param TMS timestamp in micro seconds
 * @param SHIFTHOURS from GMT to country meridian
 * @return human readable date & time
 */
String datim(uint64_t TMS, const int SHIFTHOURS = 1);
#define _DEBUG_Motion_(LEVEL,FORMAT,...) \
	do { \
		if(LEVEL <= motionCtx.dbgLevel()) \
			sprint(FORMAT,## __VA_ARGS__); \
	}while(0)
//
#define _DEBUG_EEPROM
#ifdef _DEBUG_EEPROM
#define _DEBUG_Others_(LEVEL,FORMAT,...) \
	do { \
		if(LEVEL <= 3) \
			sprint(FORMAT,## __VA_ARGS__); \
	}while(0)
#else
#define _DEBUG_Others_(LEVEL,FORMAT,...)
#endif
///////////////////////////////////////////////////////////////////////
#endif
