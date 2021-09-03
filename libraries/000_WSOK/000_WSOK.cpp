/** @file */
#include <000_WSOK.h>
#include <rov.CONTEXT.h>
#include <esp32_SYS_json.h>
///////////////////////////////////////////////////////////////////////
c_myWSOK myWSOK;
//---------------------------------------------------------------------
bool c_myWSOK::Begin(){
	_outWSOKqueue = xQueueCreate(5, sizeof(s_OUTfifoItem));
	if (!_outWSOKqueue)
		return false;
	_SERIAL_0("\n********************\x24\x60***********************");
	const char bufRight[10] = "↣"; // \xe2\x86\xa3
	Serial.printf("\n[%s]= ", (char*)&bufRight);
	for (size_t i = 0; i < strlen(bufRight); i++)
		Serial.printf(" %2i:x%02X", i, bufRight[i]);
	const char bufLeft[10] = "↢"; // \xe2\x86\xa2
	Serial.printf("\n[%s]= ", (char*)&bufLeft);
	for (size_t i = 0; i < strlen(bufLeft); i++)
		Serial.printf(" %2i:x%02X", i, bufLeft[i]);
	Serial.printf("\nBLACK:");
	for (size_t i = 0; i < 10; i++)
		Serial.printf("%s", (char*)&blackNum[i]);
	Serial.printf("\nWHITE:");
	for (size_t i = 0; i < 10; i++)
		Serial.printf("%s", (char*)&whiteNum[i]);
	Serial.printf("\n*********************\n");
	return true;
//	_ackWSOKqueue = xQueueCreate(5, sizeof(bool));
}
//---------------------------------------------------------------------
uint8_t c_myWSOK::pull(s_OUTfifoItem * ITEM){
	if(pdPASS==xQueueReceive(_outWSOKqueue,ITEM,0)){
		uint8_t siz=1+uxQueueMessagesWaiting(_outWSOKqueue);
		_SERIAL_0("<w%s",(char*)&blackNum[siz]);
		return siz;
	}
	else
		return 0;
}
//---------------------------------------------------------------------
uint8_t c_myWSOK::push(const uint32_t CALLS, char *BUFFER, const int16_t CNT, s_wsChecker *pwsx){
	s_OUTfifoItem item;
	item.msgID = CALLS;
	item.outMSG = BUFFER;
	item.msgLEN = CNT;
	item.pwsx = pwsx;
	try {
		if (pdPASS != xQueueSend(_outWSOKqueue, &item, 0))
			THROWERR(FIFO_OVERFLOW, CALLS, micro_TS());
		uint8_t siz = uxQueueMessagesWaiting(_outWSOKqueue);
		_SERIAL_0(">w%s", (char* )&blackNum[siz]);
		return siz;
	} catch (EXCEPT const &e) {
		THROW_UP();
	}
}
//---------------------------------------------------------------------
void c_myWSOK::sendJSON(s_wsChecker *pwsx, const String MID){
	static uint32_t _calls_ = 0;
	_calls_++;
	//_SERIAL_3( "\n%s  [%s]", WWW(pwsx).c_str(), pwsx->dump().c_str());
	char *buffer;
	try {
		buffer = charAlloc(DEF_WSMSG_BUFSIZE);
		int16_t cnt = 0;
		SPrintF(DEF_WSMSG_BUFSIZE, cnt, buffer, "%s",
			Header(MID + " ≈> ", Remote(pwsx->protocol[0])).c_str());
		int lenJson = serializeJson(*_jsonDoc, buffer + cnt, DEF_WSMSG_BUFSIZE - cnt);
		try {
			myWSOK.push(_calls_, buffer, lenJson + cnt, pwsx);
		} catch (EXCEPT const &e) {
			charFree(buffer);
			THROW_UP();
		}
	} catch (EXCEPT const &e) {
		FIX_ERROR();//THROW_UP();
	}

	//cnt += sprintf(&itm.outMSG[cnt], "%s", WWW(pwsx).c_str());
	//		_SERIAL_3( "\n%s %s ", WWW(pwsx).c_str(), header.c_str());
	/*
	 //_SERIAL_3( "\n%s  [%s]", WWW(pwsx).c_str(), pwsx->dump().c_str());
	 if ((pwsx->client != nullptr) && (pwsx->status == WS_CONNECTED)) {
	 try {
	 if (!pwsx->client->canSend())
	 THROWERR(WS_ERR_CANTSEND, WWW(pwsx).c_str());
	 s_OUTfifoItem itm;
	 itm.msgID = _calls_;
	 itm.pwsx = pwsx;
	 int16_t cnt = 0;
	 SPrintF(DEF_JEXPL_BUFSIZE, cnt, itm.jsonExplain, "%s",
	 Header(MID + " ≈> ", Remote(pwsx->protocol[0])).c_str());
	 if (CTX.SOFTctx.withCtrlBlock)
	 itm.SCBlen = insertSenderCtrlBlock(_calls_, _oldTMS_, _newTMS_, pwsx,
	 itm.outMSG, DEF_WSMSG_BUFSIZE);
	 //JsonObject root = (*_jsonDoc).as<JsonObject>();
	 int lenJson = serializeJson(*_jsonDoc, &itm.outMSG[0] + itm.SCBlen,
	 DEF_WSMSG_BUFSIZE - itm.SCBlen - 1);
	 notify_SENDER(itm);
	 _DBG_WSXDATA("\nSENT %s%s", itm.jsonExplain,
	 (char*) itm.outMSG);

	 //cnt += sprintf(&itm.outMSG[cnt], "%s", WWW(pwsx).c_str());
	 //		_SERIAL_3( "\n%s %s ", WWW(pwsx).c_str(), header.c_str());
	 _oldTMS_ = _newTMS_;
	 return true;
	 } catch (EXCEPT const &e) {
	 THROW_UP();
	 }
	 }
	 return false;
	 */
}
///////////////////////////////////////////////////////////////////////
String Header(const String HEADER, const String REMOTE){
	JsonObject ROOT = (*_jsonDoc).as<JsonObject>();
	String header = HEADER;
	/*
	 char buffer[32];
	 int16_t cnt = 0;
	 SPrintF(sizeof(buffer), cnt, buffer, "%10llu ", ROOT["CTL"]["TIK"].as<uint64_t>());
	 */
	header += ROOT["KMD"]["MID"].as<String>();
	header += "\\" + REMOTE + " ";
	char X = ROOT["CTL"]["TYP"].as<String>()[1];
	String nat;
	if (X == 'R') {
		if (ROOT["CTL"]["TYP"].as<String>()[2] == 'r')
			nat = "REPLY";
		else
			nat = "Infos";
	} else if (X == 'Q') {
		if (ROOT["CTL"]["TYP"].as<String>()[0] == 'r')
			nat = "QUERY";
		else
			nat = "Asked";
	} else
		nat = "?????";
	header += nat;
	//header += String(buffer);
	return header;
}

