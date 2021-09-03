/** @file */
#ifndef __esp32_SPRINT__
#define __esp32_SPRINT__
#include <esp32_EXCEPT.h>
using namespace std;
///////////////////////////////////////////////////////////////////////
void VSPRINTF_NLVL(const int8_t LVL, const char *FORMAT,
	va_list *ARGS, int16_t SIZE, int16_t &CNT, char *BUFFER);
/**
 * @fn void SPrintF(const uint16_t SIZE, int16_t &CNT, char *BUFFER, const char *FORMAT, ...)
 * @brief as printf outputs variadic variables on BUFFER at CNT position
 * @param SIZE of buffer
 * @param CNT position to begin output & returns final position
 * @param BUFFER
 * @param FORMAT printf formatting string
 */
void SPrintF(const uint16_t SIZE, int16_t &CNT, char *BUFFER, const char *FORMAT, ...);
/// @private
void VSPrintF(const uint16_t SIZE, int16_t &CNT, char *BUFFER, const char *FORMAT, va_list *ARGS);
void SPrintF_tilte(const uint16_t SIZE, int16_t &CNT, char *BUFFER, const char *FORMAT, ...);
///////////////////////////////////////////////////////////////////////
#endif //__esp32_SPRINT__
