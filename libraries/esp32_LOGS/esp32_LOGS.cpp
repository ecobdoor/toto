/** @file */
#include <esp32_LOGS.h>
#include "esp32_SPRINT.h"
#include "esp32_SPRINT.h"
#include "rov-CfgH_000_WS3.h"
c_myLOGS myLOGS;
int8_t c_myLOGS_dbglvl=0;
c_myLOGS::c_myLOGS() :
	Core("LOG", &c_myLOGS_dbglvl){
}
//---------------------------------------------------------------------
bool c_myLOGS::_UDP_(char *BUFFER){
	static uint32_t _calls_ = 0;
	_calls_++;
	if (!myUDP.isOpen()) {
		freeBuf(BUFFER); // No UDP => freeBuffer !!!
		return false;
	}
	try {
		myUDP.push(_calls_, BUFFER, strlen(BUFFER));
	} catch (EXCEPT const &e) {
		freeBuf(BUFFER); // Push error => freeBuffer !!!
		FIX_ERROR();
		return false;
	}
	return true;
}
//---------------------------------------------------------------------
/**
 * @fn c_myLOGS::_LOG_(const int8_t LVL, const int8_t THIS_DBGMAX, const char *FORMAT, ...)
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
void c_myLOGS::_LOG_(const int8_t LVL, const int8_t THIS_DBGMAX, const char *FORMAT, ...){
//	_SERIAL_0("\n** this_DebugLVL %i LVL'%i'",this_DebugLVL,LVL);
	static uint32_t _calls_ = 0;
	int8_t debTruc = 7; // when #ifdef DEF LOG_TRUNC skipped
	if (myUDP.isOpen())
		debTruc = THIS_DBGMAX;
#ifdef DEF LOG_TRUNC
	else if (CTX.SOFTctx.ALL_DBGMAX_TRUNC < THIS_DBGMAX)
		debTruc = CTX.SOFTctx.ALL_DBGMAX_TRUNC;
#endif
	if (LVL <= debTruc) {
		if (myUDP.isOpen())
			digitalWrite(bro1PIN_udp, HIGH);
		else
			digitalWrite(red2PIN_serial, HIGH);
		uint64_t microts_deb = micro_TS();
		char *buffer;
		try {
			int16_t cnt ;
			buffer = allocBuf(DEF_DBGMSG_BUFSIZE, &cnt);
			va_list args;
			va_start(args, FORMAT);
//_SERIAL_0("\nsizeof(*buffer)=[%i]\n",sizeof(*buffer)); // It's 1 for one char !!! use DEF_DBGMSG_BUFSIZE
			//VSPRINTF_NLVL(LVL, FORMAT, &args, DEF_DBGMSG_BUFSIZE, cnt, buffer);
			VSPrintF( DEF_DBGMSG_BUFSIZE, cnt, buffer, FORMAT, &args);
			va_end(args);
			if (!myUDP.isOpen()) {
				USE_SERIAL.printf("%s", buffer);
				USE_SERIAL.printf("~"); //			_SERIAL_0(" serial %i", _calls_);
				freeBuf(buffer); // => freeBuffer !!!
			} else {
				try {
					myUDP.push(_calls_, buffer, cnt);
				} catch (EXCEPT const &e) {
					freeBuf(buffer); // Push error => freeBuffer !!!
					FIX_ERROR();
				}
			}
			USE_SERIAL.flush();
			_calls_++;
			if (myUDP.isOpen())
				digitalWrite(bro1PIN_udp, LOW);
			else
				digitalWrite(red2PIN_serial, LOW);
		}
		catch (EXCEPT const &e) {
			THROW_UP();
		}
	}
}
///////////////////////////////////////////////////////////////////////
