/** @file */
#include "esp32_DEBUG.h"
#include "esp32_SYS_basic.h"
#include "esp32_SPRINT.h"
#include "rov-CfgH_000_WS3.h"
#include <string.h>// to substitute \n by adhoc prefix
/**
 * @fn _SERIAL_(const int8_t LVL, const int8_t THIS_DBGMAX, const char *X, const char *FORMAT, ...)
 * @brief if LVL<=THIS_DBGMAX, serial prints variadic values ... with an enhanced printf
 * @param LVL debug level proposed to output if accepted
 * @param THIS_DBGMAX debug level required level below which the output will be made
 * @param FORMAT as for printf
 * @param list of variadic values to output following FORMAT
 */
/////////////////////////
/// VSPRINTF_NLVL CONSOMME TROP DE HEAP
void _SERIAL_(const size_t BUFSIZE,const int8_t LVL, const int8_t THIS_DBGMAX, const char *FORMAT, ...){
	if (LVL <= THIS_DBGMAX) {
		digitalWrite(red2PIN_serial, HIGH);
		int16_t cnt = 0;
		char buffer[BUFSIZE];
		va_list args;
		va_start(args, FORMAT);
//		VSPRINTF_NLVL(LVL, FORMAT, &args, sizeof(buffer), cnt, buffer);
		VSPrintF( sizeof(buffer), cnt, buffer, FORMAT, &args);
		va_end(args);
		USE_SERIAL.printf("%s", (char*) buffer);
		USE_SERIAL.flush();
		digitalWrite(red2PIN_serial,LOW);
	}
}
