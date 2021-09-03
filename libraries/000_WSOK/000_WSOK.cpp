/** @file */
#include <000_WSOK.h>
#include <rov.CONTEXT.h>
#include <esp32_LOGS.h>
#include "esp32_SPRINT.h"
#include <esp32_SYS_json.h>
#include "rov-CfgH_000_WS3.h"
///////////////////////////////////////////////////////////////////////
c_myWS myWS;
String BDOOR_protocol; // PATCH sur la bib asynchrone !!!
///////////////////////////////////////////////////////////////////////
int8_t c_myWS_dbglvl = 0;
c_myWS::c_myWS() :
	Core("WSmsg", &c_myWS_dbglvl){
	WS_recvMSG_queue = xQueueCreate(inMSGqueueSIZE, sizeof(s_WS_recvMSG));
	WS_sendMSG_queue = xQueueCreate(10, sizeof(s_WS_sendMSG));
}
//---------------------------------------------------------------------
void pulsePINwsx(){
	digitalWrite(whi0PIN_wsx, !digitalRead(whi0PIN_wsx));
	delay(1);
	digitalWrite(whi0PIN_wsx, !digitalRead(whi0PIN_wsx));
}
void c_myWS::wsnd_websock_MSG(){
	s_WS_sendMSG item;
	uint8_t nbr = myWS.peek_WS(&item);
	if (nbr) {
		digitalWrite(whi0PIN_wsx, HIGH);
		c_WS *pwsx = WSA.lookForWSX((char*)item.protout);
		WSA.lock_Wait();
		//if (item.showWSdump)
		//_SERIAL_WSXCHANGE("-> SEND", c_WS::SND, pwsx, item.msgID, item.outMSG, item.msgLEN);
		_SERIAL_0("\n");
		_SERIAL_0(" %2i ", nbr);
		try {
			//_SERIAL_0("\nitem.showWSdump %i", item.showWSdump);
			if (!pwsx) {
				_SERIAL_0("⸮"); //THROWERR(WSX_PROTO_UNREACHABLE, (char* )item.protout, outset(item.outMSG));
				myWS.pull_WS(&item); // suppress from the queue!!!!
				freeBuf(item.outMSG); // Make an ack fifo to every time delete in the same task than new !!!!
			}
			else {
				// if the pwsx has been destroyed since the push in fifo
				if ((pwsx->clientID <= 0) || (pwsx->status != WS_CONNECTED))
					THROWERR(WSX_SND_NOTCONNECT, pwsx->dumpShort(c_WS::SND).c_str());
				if (!pwsx->client->canSend()) {
					_SERIAL_0("? %i",pwsx->client->queueLen());
					//THROWERR(WS_ERR_CANTSEND, pwsx->dumpShort(c_WS::SND).c_str());
				} else {
					_SERIAL_0("> %i",pwsx->client->queueLen());
					pwsx->client->text((char*)item.outMSG); //by server better No...)
					pwsx->event = 99; // Specific for send
					myWS.pull_WS(&item); // suppress from the queue!!!!
					freeBuf(item.outMSG); // Make an ack fifo to every time delete in the same task than new !!!!
				}
			}
			WSA.lock_Clear();
			digitalWrite(whi0PIN_wsx, LOW);
			//vTaskDelay(10); //think about webSocket server to send... (10ms => 100 Hz)
		} catch (EXCEPT const &e) {
			freeBuf(item.outMSG);
			FIX_ERROR();
		} catch (std::exception const &e) {
			Serial.printf("\n@@@@@@@@@@@@@@@@@@@@@ %s @@@@@@@@@@", e.what());
		}
	}
}
//---------------------------------------------------------------------
/**
 * @fn void c_myWS::inAppend(uint8_t *data, const size_t len)
 * @brief Adds the packet data buffer to @ref s_WS_recvMSG rcvItm that will be pushed into fifo
 *
 * @param data part of web socket received packet
 * @param len length of data
 */
void c_myWS::inAppend(uint8_t *data, const size_t len){
	strncpy((char*)&rcvItm.inMSG[rcvItm.msgLEN], (char*)data, len); // & !!!
	rcvItm.msgLEN += len;
}
//---------------------------------------------------------------------
void c_myWS::ws_recvMSG(AsyncWebSocket *server, AsyncWebSocketClient *client,
	AwsEventType type, void *arg, uint8_t *data, size_t len){
	pulsePINwsx();
	char csav;
	if (len) {
		csav = data[len - 1];
		data[len - 1] = 0;
	}
	/// _SERIAL_0("\n!!!!!!!!!!!! %s len(%i)[%s]", SEVT(type).c_str(), len, (len) ? (char* )data : "");
	if (len)
		data[len - 1] = csav;
	//typedef enum { WS_CONTINUATION, WS_TEXT, WS_BINARY, WS_DISCONNECT = 0x08, WS_PING, WS_PONG } AwsFrameType;
	static const char *class_Type = "RCV";
	static int8_t &this_DebugLVL = *registerFCT(e_tasks::WRCV, "Wrcv",
		&CTX.SOFTctx.TSK_DBGMAX_RECV, DEF_PRIO_VRCV);
	static uint32_t _call_ = 0;
	_call_++;
	uint64_t start_event = milli_TS();
	//
	//-------- CONNECT (creates a new PWX or close if MAX connections---------
	//
	if (type == WS_EVT_CONNECT) {
		//_SERIAL_0("\n@@@@ FOUND");
		c_WS *pwsx = openCNX(type, client, BDOOR_protocol);
		_SERIAL_WSXCHANGE(" ! OPEN", c_WS::RCV, pwsx, _call_, (char* )data, len);
		try {
			if (pwsx) {
				String chn = "{\"ROVER_client_connection\":" + String(client->id())
					+ ",\"Sub_protocol\":\"" + BDOOR_protocol + "\"}";
				client->text(chn);
				pwsx->event = 99; // Specific for send
				_SERIAL_WSXCHANGE("-> TEXT", c_WS::SND, pwsx, _call_, chn.c_str(), chn.length());
				chn = "Hello!!";
				client->ping((uint8_t*)chn.c_str(), 1 + chn.length());
				pwsx->event = 98; // Specific for ping
				_SERIAL_WSXCHANGE("-> PING", c_WS::SND, pwsx, _call_, chn.c_str(), chn.length());
				return pulsePINwsx();
			} else {
				THROWERR(WSX_NOT_FOUND, SEVT(type).c_str(), *((uint16_t* )arg), len,
					((len) ? (char* )data : ""));
			}
		} catch (EXCEPT const &e) {
			FIX_ERROR();
		}
		return pulsePINwsx();
	}
	//
	//-------- DISCONNECT ----------------------------------------------------
	//
	else if (type == WS_EVT_DISCONNECT) {
		c_WS *pwsx = WSA.getWSXclient(client);
		WSA.lock_Set();
		//if (pwsx)			pwsx->clientID *= -1; // Urgentissime
		//USE_SERIAL.printf("\nId[%u] disconnect", client->id());
		_SERIAL_WSXCHANGE(" ! CLOS", c_WS::RCV, pwsx, _call_, (char* )data, len);
		if (pwsx)
			pwsx->erase(type, client->status());
//		server->cleanupClients();
//		client->close();
		WSA.lock_Clear();
		return pulsePINwsx();
	}
	//
	//-------- ERROR  --------------------------------------------------------
	//
	else if (type == WS_EVT_ERROR) {
		c_WS *pwsx = WSA.getWSXclient(client);
		//if (pwsx)			pwsx->clientID *= -1; // Urgentissime
		USE_SERIAL.printf("\nId(%u) error(%u):", client->id(), *((uint16_t*)arg));
//			USE_SERIAL.printf("\nId(%u) error(%u): %s", client->id(), *((uint16_t*) arg),(char*) data);
		_SERIAL_WSXCHANGE(" ! ERR!", c_WS::RCV, pwsx, _call_, (char* )data, len);
		//if (pwsx)			pwsx->erase(type, client->status());		client->close();
		//server->cleanupClients();
		return pulsePINwsx();
	}
	//
	//-------- FIND PWX  ---------------------------------------------
	//
	bool msgToqueue = false;
	c_WS *pwsx = WSA.getWSXclient(client);
	try {
		if (!pwsx) {
			//THROWERR(WSX_NOT_FOUND,SEVT(type).c_str(),*((uint16_t*)arg),((len) ? (char*)data : "no data"));
			if (len)
				data[len - 1] = 0;
			_SERIAL_0("\nno PWSX websocket pointer not found : evt(%s) arg(%i) len(%i) data(%s)",
				SEVT(type).c_str(), 999, len, ((len) ? (char* )data : ""));
			THROWERR(WSX_NOT_FOUND, SEVT(type).c_str(), 998/* *((uint16_t* )arg)*/,len,
				((len) ? (char* )data : ""));
			return pulsePINwsx();
		}
	} catch (EXCEPT const &e) {
		FIX_ERROR();
	}

	pwsx->status = client->status();
	pwsx->event = type;
	//
	//-------- PONG  ---------------------------------------------------------
	//
	if (type == WS_EVT_PONG) {
		_SERIAL_WSXCHANGE("<- PONG", c_WS::RCV, pwsx, _call_, (char* )data, len);
	}
	//
	//-------- DATA ----------------------------------------------------------
	//
	else if (type == WS_EVT_DATA) {
		AwsFrameInfo *info = (AwsFrameInfo*)arg;
		/*
		 Serial.printf("\n%i num %i op %i fin %i (idx,len)=(%4llu,%4llu) len=%i:[", sizeof(size_t),
		 info->num, info->opcode, info->final, info->index, info->len, len);
		 for (size_t i = 0; i < info->len; i++) {
		 Serial.printf("%c", (char)data[i]);
		 }
		 Serial.printf("]");
		 */
		//---- The whole message is in a single frame and we got all of it's data (info->opcode text or binary)
		if (info->final && info->index == 0 && info->len == len) {
			rcvItm.msgLEN = 0;
			inAppend(data, len);
			try {
				if (rcvItm.inMSG == "FIFO_FULL") {
					THROWERR(FIFO_OVERFLOW, 999, micro_TS());
//				webSokSrv.close(client->id(),4001,"blabla");
					/*
					 pwsx->erase(type,client->status());
					 client->close(4001,"blabla");//4001,"blabla"
					 */
					return pulsePINwsx();
				} else {
					msgToqueue = true;
				}
			} catch (EXCEPT const &e) {
				FIX_ERROR();
			}
		} // end of single

		//---- message is comprised of multiple kfs or the frame is split into multiple packets
		else {
			if (info->index == 0) {
				if (info->num == 0) {
					rcvItm.msgLEN = 0;
				}
			}
			inAppend(data, len);
			if ((info->index + len) == info->len) {
				if (info->final) {
					if (info->message_opcode != WS_TEXT)
						client->binary("I got your binary message");
					msgToqueue = true;
				}
			}
		} // end of multiple

		//
		//---- PUSH on queue ------------------------------------------
		//
		if (msgToqueue) {
			_SERIAL_WSXCHANGE("<- RECV", c_WS::RCV, pwsx, _call_, (char* )data, len);
			rcvItm.inMSG[rcvItm.msgLEN] = 0;
			//		Serial.printf("\nlen=%4i:[%s]", rcvItm.msgLEN, rcvItm.inMSG);
			msgToqueue = false;

			try {
				if (pdPASS != xQueueSend(WS_recvMSG_queue, &rcvItm, 0))
					THROWERR(FIFO_OVERFLOW, rand() % 2 + 5, micro_TS());
				rcvItm.inMSG[0] = 0; //raz
				rcvItm.msgLEN = 0; //raz
			} catch (EXCEPT const &e) {
				FIX_ERROR();
			}
		} // end of PUSH on queue
	} // end if WS_EVT_DATA
	else {
		_SERIAL_1("*");
	}
	pulsePINwsx();

}
//---------------------------------------------------------------------
void onWSMessage(AsyncWebSocket *server, AsyncWebSocketClient *client,
	AwsEventType type, void *arg, uint8_t *data, size_t len){
	myWS.ws_recvMSG(server, client, type, arg, data, len);
}
//---------------------------------------------------------------------
bool c_myWS::Begin(){
	if (!WS_sendMSG_queue)
		return false;
	return true;
//	_ackWSOKqueue = xQueueCreate(5, sizeof(bool));
}
//---------------------------------------------------------------------
uint8_t c_myWS::pull_WS(s_WS_sendMSG *ITEM){
	if (pdPASS == xQueueReceive(WS_sendMSG_queue, ITEM, 0)) {
		uint8_t siz = 1 + uxQueueMessagesWaiting(WS_sendMSG_queue);
		//if (CTX.SOFTctx.showWSX_fifo) _SERIAL_0("<w%s", (char* )&blackNum[siz]);
		return siz;
	}
	return 0;
}
//---------------------------------------------------------------------
uint8_t c_myWS::peek_WS(s_WS_sendMSG *ITEM){
	if (pdPASS == xQueuePeek(WS_sendMSG_queue, ITEM, 0)) {
		uint8_t siz = uxQueueMessagesWaiting(WS_sendMSG_queue);
		//_SERIAL_0("^w%s", (char* )&blackNum[siz]);
		return siz;
	}
	return 0;
}
//---------------------------------------------------------------------
uint8_t c_myWS::push_WS(const char *PROTOCOL, const uint32_t CALLS, char *BUFFER,
	const int16_t CNT, const bool SHOW_DUMP){
	s_WS_sendMSG item;
	item.msgID = CALLS;
	item.outMSG = BUFFER;
	item.msgLEN = CNT;
	item.showWSdump = SHOW_DUMP;
	strcpy((char*)&item.protout, PROTOCOL);
//_SERIAL_0("]%s[", (char* )&item.protout);
	try {
		if (pdPASS != xQueueSend(WS_sendMSG_queue, &item, 0))
			THROWERR(FIFO_OVERFLOW, CALLS, micro_TS());
		uint8_t siz = uxQueueMessagesWaiting(WS_sendMSG_queue);
		//if (CTX.SOFTctx.showWSX_fifo) _SERIAL_0(">w%s", (char* )&blackNum[siz]);
		return siz;
	} catch (EXCEPT const &e) {
		THROW_UP();
	}
}
//---------------------------------------------------------------------
void c_myWS::sendJSON(const String MID, const char *PROTOCOL, const bool FORCEDUMP){
	static uint32_t _calls_ = 0;
	_calls_++;
	char *buffer;
	int16_t cnt = 0;
	try {
		buffer = allocBuf(DEF_WSMSG_BUFSIZE, &cnt);
		/*
		 SPrintF(DEF_WSMSG_BUFSIZE, cnt, buffer, "%s",
		 Header(String(PROTOCOL) + "~" + MID + "≈>", Remote(PROTOCOL[2])).c_str());
		 */
//		SPrintF(DEF_WSMSG_BUFSIZE, cnt, buffer, "≈>");
		int lenJson = serializeJson(*_jsonDoc, buffer + cnt, DEF_WSMSG_BUFSIZE - cnt);
		try {
			if (MID == "DRV")
				myWS.push_WS(PROTOCOL, _calls_, buffer, lenJson + cnt,
					FORCEDUMP | CTX.SOFTctx.DRVctx.show_drvInfos);
			else
				myWS.push_WS(PROTOCOL, _calls_, buffer, lenJson + cnt, true);
		} catch (EXCEPT const &e) {
			freeBuf(buffer);
			THROW_UP();
		}
	} catch (EXCEPT const &e) {
		FIX_ERROR(); //THROW_UP();
	}
}
///////////////////////////////////////////////////////////////////////
/**
 * @fn String natMSG(const String &TYP, const String &TYK)
 * @brief Human readable nature of a web socket message
 * @param TYP of pseudo sub-protocol (rRp, pRr, rQp, pQr...)
 * @param TYK flow packet control number (0 for single information)
 * @param MID module targeted ID
 * @return Infos | REPLY | QUERY | -kmds (for no json MSG treated as low level command)
 */
String natureMSG(const String &TYP, const String &TIK){
	String nat = "-kmds";
	char X = TYP[1];
	if (X == 'R') {
		if (TIK == "0")
			nat = "Infos";
		else
			nat = "REPLY";
	} else if (X == 'Q')
		nat = "QUERY";
	return nat;
}
//---------------------------------------------------------------------
/**
 * @fn int strFindAfter(String &RTN, const uint8_t LEN, const String &CHN, const String &OCC)
 * @brief extract a substring following an occurrence
 *
 * @param RTN substring to extract after OCC (if OCC not found, RTN="???")
 * @param LEN number of char to peek (if LEN==0, peeks all consecutive digits after OCC)
 * @param CHN string to scan
 * @param OCC searched occurrence
 * @return position of searched OCC in CHN, -1 otherwise
 */
int strFindAfter(String &RTN, const uint8_t LEN, const String &CHN, const String &OCC){
	int pos = CHN.indexOf(OCC);
	if (0 <= pos) {
		if (LEN)
			RTN = CHN.substring(pos + OCC.length(), pos + OCC.length() + LEN);
		else {
			int p = pos + OCC.length();
			RTN = "";
			while (isDigit(CHN[p])) {
				RTN += CHN[p];
				p++;
			}
		}
	} else
		RTN = "???";
//	_SERIAL_0("\nstrFindAfter [%i] [%s] [%s]", pos, RTN.c_str(), OCC.c_str());
	return pos;
}
/**
 * @fn
 * @brief
 *
 */
bool headerString(const String &MSG, String &PRO, String &TIK, String &MID){
	if ((0 <= strFindAfter(PRO, 3, MSG, "\"TYP\":\""))
		&& (0 <= strFindAfter(TIK, 0, MSG, "\"TIK\":"))
		&& (0 <= strFindAfter(MID, 3, MSG, "\"MID\":\"")))
		return true;
	return false;
}
//---------------------------------------------------------------------
void serial_WSXCHANGE(const char *TITLE, c_WS::e_sens DIR, c_WS *PWSX,
	uint32_t CALL, const char *DATA, const size_t LEN){
	char buffer[64]; // to have only one line at output (multi tasking...)
	int16_t cnt = 0;
	if (CTX.SOFTctx.showWSX_event) {
		if (PWSX)
			SPrintF(sizeof(buffer), cnt, buffer, "%-8s %s:%s %7i",
				CTX.strRoverMode(), TITLE, PWSX->dumpShort(DIR).c_str(), CALL);
		else
			SPrintF(sizeof(buffer), cnt, buffer, "%-8s %s:%s %7i",
				CTX.strRoverMode(), TITLE, "PWSX==nullptr!!!   ", CALL);
		if (!CTX.SOFTctx.showWSX_data)
			_SERIAL_0("\n%s", buffer);
	} else
		buffer[0] = 0; // !!!
	if (CTX.SOFTctx.showWSX_data) {
		if (!CTX.SOFTctx.showWSX_event)
			_SERIAL_0("\n");
		if (DATA) {
			String CHN;
			CHN.reserve(LEN);
			for (size_t i = 0; i < LEN; i++)
				CHN += DATA[i];
			String TYP;
			String TIK;
			String MID;
			//_SERIAL_0("\nheaderString [%s]", CHN.c_str());
			if (!headerString(CHN, TYP, TIK, MID)) {
//				_SERIAL_0("\nheaderString FAILS");
			}
			//_SERIAL_0("\nheaderString [%s] [%s] [%s]", TYP.c_str(), TYK.c_str(), MID.c_str());
			CHN.replace("\"", "");
			CHN.replace("CTL:", "");
			CHN.replace("TYP:", "");
			CHN.replace("TIK:", "");
			CHN.replace("KMD:", "");
			CHN.replace("MID:", "");
			int len;
			bool cut = false;
			if (CHN.length() > 100) {
				len = 100;
				cut = true;
			} else
				len = CHN.length();
			_SERIAL_0("\n%s\t%s%s %3s %3s %s %s%s", (buffer[0]) ? buffer : "",
				(DIR == c_WS::SND) ?
										"≈>" :
										"<-",
				natureMSG(TYP, TIK).c_str(), MID.c_str(),
				TIK.c_str(),
				(DIR == c_WS::SND) ?
										(char* )&blackNum[myWS.waitingSend()] :
										(char* )&whiteNum[myWS.waitingRecv()],
				CHN.substring(0, len).c_str(), cut ? "..." : "");
		} else
			_SERIAL_0("\n%s\t(no data)", (buffer[0]) ? buffer : "");
	}
}
/*
 working -> SEND:rQp   1%1 s∆ e►    1.997s     572	≈>Infos JOB   0 ⓿ {{rRp,0},{JOB,DT:2001,@:100,F:/Learn.work,OP:@}}
 */
///////////////////////////////////////////////////////////////////////
