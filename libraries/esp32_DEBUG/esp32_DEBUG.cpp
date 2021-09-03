/** @file */
#include "esp32_DEBUG.h"
#include "esp32_SYS_basic.h"
#define DBGPRFX "\t~"
#define DBGTCAR "~~~~"
#include <string.h>// to substitute \n by adhoc prefix
/**
 * @fn _DEBUG_(const int8_t LVL, const int8_t THIS_DBGMAX, const char *X, const char *FORMAT, ...)
 * @brief if LVL<=THIS_DBGMAX, serial prints variadic values with an enhanced printf
 * @param LVL debug level proposed to output if accepted
 * @param THIS_DBGMAX debug level required level below which the output will be made
 * @param FORMAT as for printf
 * @param list of variadic values to output following FORMAT
 */
void _DEBUG_(const size_t BUFSIZE,const int8_t LVL, const int8_t THIS_DBGMAX, const char *FORMAT, ...){
	extern int vio7PIN_Serial;
	if (LVL <= THIS_DBGMAX) {
		digitalWrite(vio7PIN_Serial, HIGH);
		int16_t cnt = 0;
		char buffer[BUFSIZE];
		va_list args;
		va_start(args, FORMAT);
		VSPRINTF_BUFFER(LVL, FORMAT, &args, sizeof(buffer), cnt, buffer);
		va_end(args);
		USE_SERIAL.printf("%s", (char*) buffer);
		USE_SERIAL.flush();
		digitalWrite(vio7PIN_Serial,LOW);
	}
}
