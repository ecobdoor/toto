/** @file */
#include <esp32_SPRINT.h>
#include "esp32_GOODIES.h"
/**
 * @fn VSPRINTF_NLVL(const int8_t LVL, const char *FORMAT, va_list *ARGS, int16_t SIZE, int16_t &CNT, char *BUFFER)
 * @brief Fills a char BUFFER with an enhanced printf output of va_list ARGS
 * replace each '\n' by "\nprefix" depending of LVL
 * @param LVL debug or log proposed level to output if accepted
 * @param FORMAT as for printf
 * @param ARGS arguments lists of values to output following FORMAT
 * @param SIZE or BUFFER
 * @param CNT position of output in buffer
 * @param BUFFER char[SIZE]
 */
void VSPRINTF_NLVL(const int8_t LVL, const char *FORMAT,
	va_list *ARGS, int16_t SIZE, int16_t &CNT, char *BUFFER){
	VSPrintF(SIZE, CNT, BUFFER, FORMAT, ARGS);
	if (true) {
		char buff[48];
		if (LVL >= 0)
			sprintf(buff, "\n%i:", LVL);
		else
			sprintf(buff, "\n");
//		sprintf(buff, "\n%-*i:",4*LVL, LVL); // with tabulation
		std::string tab(buff);
		size_t lenNewocc = tab.length();
		std::string CHN(BUFFER);
		size_t start = 0;
		size_t pos = CHN.find("\n", start);
		while (pos != string::npos) {
			CHN.replace(pos, 1, tab);
			start = pos + lenNewocc;
			pos = CHN.find("\n", start);
		}
		strcpy(BUFFER, CHN.c_str());
		CNT = CHN.length();
	}
}
//---------------------------------------------------------------------
void SPrintF(const uint16_t SIZE, int16_t &CNT, char *BUFFER, const char *FORMAT, ...){
	va_list args;
	va_start(args, FORMAT);
	VSPrintF(SIZE, CNT, BUFFER, FORMAT, &args);
	va_end(args);
}
//---------------------------------------------------------------------
/**
 * @fn void SPrintF_tilte(const uint16_t SIZE, int16_t &CNT, char *BUFFER, const char *FORMAT, ...)
 * @brief Outputs as printf & complete the buffer with first character of FORMAT
 * @param SIZE of buffer
 * @param CNT pointer in the buffer
 * @param BUFFER to be completed
 * @param FORMAT as printf for variadic parameters (must not finish by \n...)
 */
void SPrintF_tilte(const uint16_t SIZE, int16_t &CNT, char *BUFFER, const char *FORMAT, ...){
	ASSERT(FORMAT[0] != '\n');
	ASSERT(FORMAT[strlen(FORMAT) - 1] != '\n');
	SPrintF(SIZE, CNT, BUFFER, "\n");
	va_list args;
	va_start(args, FORMAT);
	VSPrintF(SIZE, CNT, BUFFER, FORMAT, &args);
	va_end(args);
	ASSERT(CNT < 80);
	String chn = " ";
	char c = FORMAT[0]; //FORMAT[0];
	for (size_t i = 0; i < 80 - CNT; i++) {
		chn += c;
	}
	chn += '\n';
	SPrintF(SIZE, CNT, BUFFER, chn.c_str());
}
//---------------------------------------------------------------------
void VSPrintF(const uint16_t SIZE, int16_t &CNT, char *BUFFER, const char *FORMAT, va_list *ARGS){
	int16_t cnt;
	try {
		cnt = vsnprintf(BUFFER + CNT, SIZE - CNT, FORMAT, *ARGS);
		if (cnt < 0)
			THROWERR(BUF_UNDERSIZED, SIZE, cnt);
	} catch (EXCEPT const &e) {
		THROW_UP();
	}
	/**
	 ASSERT(cnt >= 0);
	 ASSERT((cnt - 1) < (SIZE - CNT));
	 */
	//_SERIAL_0("{%i}",CNT );Serial.flush();
	CNT += cnt;
}
///////////////////////////////////////////////////////////////////////
