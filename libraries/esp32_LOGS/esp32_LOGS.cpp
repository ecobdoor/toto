/** @file */
#include <esp32_LOGS.h>
bool _CSV_(const char *FORMAT, ...){
	static uint32_t _calls_ = 0;
	_calls_++;
	va_list args;
	va_start(args, FORMAT);
//		bool ok = sendUDP(_calls_, FORMAT, &args);
	va_end(args);
	//return ok;
	return false;
}
//---------------------------------------------------------------------
/**
 * @fn _LOG_(const int8_t LVL, const int8_t THIS_DBGMAX, const char *FORMAT, ...)
 * @brief if LVL<=THIS_DBGMAX, outputs variadic values with an enhanced printf
 *
 * The output may be on serial (limited to 10 kBytes/sec) or via udp (upto 200 kBytes/sec)
 * if @ref myUDP is not open, THIS_DBGMAX is truncated to LVL_DBGMAX_TRUNCATE
 * so as not to saturate the serial line.
 * @nWARNING: char * buffer is created on the heap and has to be deleted
 * in LOG or in task @ref sudp_LOG when udp is used
 *
 * @param LVL
 * @param THIS_DBGMAX
 * @param FORMAT
 * @param ... list of variadic values to output following FORMAT
 */
void _LOG_(const int8_t LVL, const int8_t THIS_DBGMAX, const char *FORMAT, ...){
	extern int vio7PIN_Serial;
	extern int blu6PIN_Logs;
//	_SERIAL_0("\n** this_DebugLVL %i LVL'%i'",this_DebugLVL,LVL);
	static uint32_t _calls_ = 0;
	int8_t debTruc=10;// when #ifdef DEF LOG_TRUNC skipped
	if (myUDP.isOpen())
		debTruc = THIS_DBGMAX;
#ifdef DEF LOG_TRUNC
	else if (CTX.SOFTctx.ALL_DBGMAX_TRUNC < THIS_DBGMAX)
		debTruc = CTX.SOFTctx.ALL_DBGMAX_TRUNC;
#endif
	if (LVL <= debTruc) {
		if (myUDP.isOpen())
			digitalWrite(blu6PIN_Logs, HIGH);
		else
			digitalWrite(vio7PIN_Serial, HIGH);
		uint64_t microts_deb = micro_TS();
		char *buffer;
		try {
			buffer = charAlloc(DEF_DBGMSG_BUFSIZE);
			int16_t cnt = 0;
			va_list args;
			va_start(args, FORMAT);
//_SERIAL_0("\nsizeof(*buffer)=[%i]\n",sizeof(*buffer)); // It's 1 for one char !!! use DEF_DBGMSG_BUFSIZE
			VSPRINTF_BUFFER(LVL, FORMAT, &args, DEF_DBGMSG_BUFSIZE, cnt, buffer);
			va_end(args);
			if (!myUDP.isOpen()) {
				USE_SERIAL.printf("%s", buffer);
				USE_SERIAL.printf("~"); //			_SERIAL_0(" serial %i", _calls_);
				charFree(buffer);
			} else {
				try {
					myUDP.push(_calls_, buffer, cnt);
				} catch (EXCEPT const &e) {
					FIX_ERROR();
				}
			}
			USE_SERIAL.flush();
			_calls_++;
			if (myUDP.isOpen())
				digitalWrite(blu6PIN_Logs, LOW);
			else
				digitalWrite(vio7PIN_Serial, LOW);
		}
		catch (EXCEPT const &e) {
			//FIX_ERROR();//
			THROW_UP();
		}
	}
}
///////////////////////////////////////////////////////////////////////
