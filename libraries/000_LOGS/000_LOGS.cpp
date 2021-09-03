/** @file */
#include <000_LOGS.h>
#include <000_CONTEXT.h>
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
	int8_t debTruc;
	if (myUDP.isOpen())
		debTruc = THIS_DBGMAX;
	else if (CTX.SOFTctx.ALL_DBGMAX_TRUNC < THIS_DBGMAX)
		debTruc = CTX.SOFTctx.ALL_DBGMAX_TRUNC;
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
/**
 * @fn void notify_SENDER(const uint32_t MSG_ID, s_wsChecker * PWSX, s_OUTfifoItem & ITM)
 * @brief Sends a message included in \ref s_OUTfifoItem ITEM via webSsocket odr udp if _pwsx = nullptr
 *
 * This function notifies the sender task \ref wsnd_HDL and waits the task to be done
 * @n returning a status error status to manage exceptions
 * @param ITM message to send (text log or serialized json)
 */
/*
 void notify_SENDER(s_OUTfifoItem &ITM){
 static int this_DebugLVL = -1;
 extern TaskHandle_t wsnd_HDL;
 //extern QueueHandle_t outMSGqueue;
 //ITM.msgID = MSG_ID;
 //ITM.pwsx = PWSX;
 ITM.tsk = xTaskGetCurrentTaskHandle();
 //auto it = map _TSKHDL.find(ITM.tsk);
 // Wake up sender
 while ((ITM.msWaitNotify < 10) && (pdPASS
 != xTaskNotify(wsnd_HDL, (uint32_t)std::addressof(ITM), eSetValueWithoutOverwrite))) {
 _SERIAL_0("?");
 vTaskDelay(1 / portTICK_PERIOD_MS);
 ITM.msWaitNotify++;
 }
 if (ITM.msWaitNotify >= 10) {
 ITM.rtnScratch = rtnThrow(ERR_SND_NOTIFY, __FUNCTION__, __LINE__, __FILE__,	ITM.msWaitNotify);
 return;
 }
 // Wait for send_MSG ended
 uint32_t ulNotifiedValue;
 xTaskNotifyWait(0x00, // Don't clear any notification bits on entry.
 ULONG_MAX, // Reset the notification value to 0 on exit.
 &ulNotifiedValue, // Notified value pass out in	 ulNotifiedValue.
 portMAX_DELAY); // Block indefinitely.
 assert(wsnd_HDL == (TaskHandle_t)ulNotifiedValue);
 }
 */
///////////////////////////////////////////////////////////////////////
/**
 * @fn void SND_DOIT(s_OUTfifoItem & ITM)
 * @brief function of task @ref send_MSG no except but returns a scratch pointer in item if error occurs
 * @param ITM
 * @ return true if sent else false
 */
/*
 bool SND_DOIT(s_OUTfifoItem &ITM){
 extern int &this_DebugLVL;
 //		_SERIAL_0("ITM(%p)", std::addressof(ITM));
 //_SERIAL_0(" %i",ESP.getMaxAllocHeap());
 //_SERIAL_0(" %p",(s_OUTfifoItem *) &ITM);
 // Wait for pwsx avalaible
 while ((ITM.msWaitQueue < 10) && (ITM.pwsx->clientID > 0) && (ITM.pwsx->status == WS_CONNECTED)
 && (ITM.pwsx->client->queueIsFull())) { //queueIsFull yes, canSend no!
 _SERIAL_0("§");
 vTaskDelay(1 / portTICK_PERIOD_MS);
 ITM.msWaitQueue++;
 }
 if (ITM.msWaitQueue >= 10)
 {
 ITM.rtnScratch = rtnThrow(ERR_SND_TIMOUT, __FUNCTION__, __LINE__, __FILE__,	ITM.msWaitQueue);
 return false;
 }

 if (withCtrlBlock()) {
 int16_t cnt = 1; //	Overload outMSG buffer cnt=1 !!!
 SPrintF(25, cnt, ITM.outMSG, "%7i %4ims", ITM.msgID, ITM.msWaitQueue);
 memset(ITM.outMSG + cnt, '|', 1);
 }
 if ((ITM.pwsx->clientID > 0) && (ITM.pwsx->status == WS_CONNECTED)) {
 ITM.pwsx->client->text((char*)ITM.outMSG); //by server better No...)
 //webSokSrv(ITM.pwsx->clientID,(char *) &ITM.outMSG[0] + ITM.SCBlen);
 } else {
 ITM.rtnScratch = rtnThrow(ERR_SND_NOTCONNECT, __FUNCTION__, __LINE__,__FILE__, WWW(ITM.pwsx).c_str());
 return false;
 }
 _SERIAL_WSXDATA("≈>", (char* ) ITM.outMSG + ITM.SCBlen);
 if (withCtrlBlock())
 _SERIAL_0("%.*s", ITM.SCBlen, ITM.outMSG);
 return true;
 }
 */
///////////////////////////////////////////////////////////////////////
/*
 int16_t insertSenderCtrlBlock(const uint32_t NUMMSG, const uint64_t OLDTMS,
 const uint64_t NEWTMS, s_wsChecker *PWSX, char *BUFFER, const uint16_t SIZE){
 int16_t cnt = 0;
 //	SPrintF(SIZE,cnt,BUFFER,"                        ║%s",SRC);
 SPrintF(SIZE, cnt, BUFFER, "<                        ║");
 SPrintF(SIZE, cnt, BUFFER,
 "%8i %15.3f:%3llu ms %s (%6.0f cps) >", NUMMSG, NEWTMS / 1000.0,
 NEWTMS - OLDTMS, WWW(PWSX).c_str(),
 (1.0 * 1000) / ((NEWTMS - OLDTMS) / 1000.0));
 return cnt;
 }
 */
