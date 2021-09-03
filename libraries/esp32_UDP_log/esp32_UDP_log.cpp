#include "Arduino.h"
#include "esp32_WIFI.h"
#include "esp32_LOGS.h"
#include "esp32_UDP_log.inc"
///////////////////////////////////////////////////////////////////////
AsyncUDP udp;
c_myUDP myUDP;
//---------------------------------------------------------------------
uint8_t c_myUDP::pull(s_logITM *ITEM){
	if (pdPASS == xQueueReceive(_outLOGSqueue, ITEM, 0)) {
		uint8_t siz = 1 + uxQueueMessagesWaiting(_outLOGSqueue);
		_SERIAL_0("<u%s", (char* )&whiteNum[siz]);
		return siz;
	}
	else
		return 0;
}
//---------------------------------------------------------------------
uint8_t c_myUDP::push(const uint32_t CALLS, const char *BUFFER, const int16_t CNT){
	s_logITM item;
	item.msgID = CALLS;
	item.logMSG = BUFFER;
	item.msgLen = CNT;
	try {
		if (pdPASS != xQueueSend(_outLOGSqueue, &item, 0))
			THROWERR(FIFO_OVERFLOW, CALLS, micro_TS());
		uint8_t siz = uxQueueMessagesWaiting(_outLOGSqueue);
		_SERIAL_0(">u%s", (char* )&whiteNum[siz]);
		return siz;
	} catch (EXCEPT const &e) {
		THROW_UP();
	}
}
//---------------------------------------------------------------------
/**
 Definitions for error constants.
 typedef enum {
 ERR_OK         = 0,// No error, everything OK.
 ERR_MEM        = -1,// Out of memory error.
 ERR_BUF        = -2,// Buffer error.
 ERR_TIMEOUT    = -3,// Timeout.
 ERR_RTE        = -4,// Routing problem.
 ERR_INPROGRESS = -5,// Operation in progress
 ERR_VAL        = -6,// Illegal value.
 ERR_WOULDBLOCK = -7,// Operation would block.
 ERR_USE        = -8,// Address in use.
 ERR_ALREADY    = -9,// Already connecting.
 ERR_ISCONN     = -10,// Conn already established.
 ERR_CONN       = -11,// Not connected.
 ERR_IF         = -12,// Low-level netif error
 ERR_ABRT       = -13,// Connection aborted.
 ERR_RST        = -14,// Connection reset.
 ERR_CLSD       = -15,// Connection closed.
 ERR_ARG        = -16// Illegal argument.
 */
/**
 * Sends buffer with the '\0' at end. buffer is splitted in intermediate packet if too loon comparing with MTU possible
 * @param BUFFER
 * @param LENGTH
 */
void c_myUDP::postString(const char *BUFFER, const size_t LENGTH){
	lock_Wait();
	size_t lenRemaining = LENGTH + 1;
	size_t sumLenSent = 0;
	size_t lenToSend;
	size_t lenSent;
	_SERIAL_0("\n{%i}", udp.lastErr());
	do {
		if (lenRemaining <= DEF_UDP_PACKLEN) {
			lenToSend = lenRemaining; // Buffer completed
			lenRemaining = 0;
		} else {
			lenToSend = DEF_UDP_PACKLEN; //Part of buffer
			lenRemaining -= DEF_UDP_PACKLEN;
		}
		lenSent = udp.writeTo((uint8_t*)(BUFFER + sumLenSent), lenToSend, _remoteIP, _remotePort);
//		lenSent= udp.write((uint8_t*)(BUFFER+sumLenSent), lenToSend);
		sumLenSent += lenSent;
//		_SERIAL_0("\nudp sent(%i/%i,heap:%i)",sumLenSent,LENGTH + 1,ESP.getMaxAllocHeap());
//		vTaskDelay(2);
	} while ((lenToSend == lenSent) && (sumLenSent != (LENGTH + 1)));
//	if(lenSent>2000)
	if (lenSent != lenToSend) {
		_SERIAL_0("\nUDPERR(%i)", lenSent - 2000);
		uint32_t heap = ESP.getMaxAllocHeap();
		if (heap < 65536)
			_SERIAL_0(" HEAP(%i)", heap);
	}
	//A delay seems necessary between post or packet ???
	lock_Clear();
}
//---------------------------------------------------------------------
void c_myUDP::keepAlive(){
	if (!isOpen())
		return;
	uint64_t now = milli_TS();
	uint64_t dtServer = now - _last_serverReady;
	uint64_t dtClient = now - _last_clientReady;
	if (1000 < dtServer) {
		_SERIAL_0("\nC/S%6.3f/%6.3f>%s", dtClient / 1000.0, dtServer / 1000.0,
			DEF_UDP_SERVER_HANDSHAKE);
		String chn = DEF_UDP_SERVER_HANDSHAKE;
		postString(chn.c_str(), chn.length());
		_last_serverReady = now;
		if (2000 < dtClient) {
			Close("dtClient timeout");
			return;
		}
	}
}
//---------------------------------------------------------------------
void c_myUDP::Close(const String REASON){
	lock_Wait();
	_SERIAL_0("\n\t§§§§ Closing myUDP (%s)", REASON.c_str());
	_remotePort = 0;
	for (size_t m = 0; m < 6; m++)
		_remoteMAC[m] = 0;
	_remoteIP = IPAddress();
}
//---------------------------------------------------------------------
void c_myUDP::Open(AsyncUDPPacket &packet){
	if (0 != this->_remotePort)
		lock_Wait();
	_SERIAL_0("\n\t§§§§ UDP(%s) @ %s:%i",
		StringMAC(&_remoteMAC[0]).c_str(), _remoteIP.toString().c_str(), _remotePort);
	_remotePort = packet.remotePort();
	_remoteIP = packet.remoteIP();
	packet.remoteMac(&_remoteMAC[0]);
	_SERIAL_0("\n\t§§§§ UDP(%s) @ %s:%i",
		StringMAC(&_remoteMAC[0]).c_str(), _remoteIP.toString().c_str(), _remotePort);
	_last_clientReady = milli_TS();
	lock_Clear();
}
//---------------------------------------------------------------------
/**
 * @fn TASK void sudp_TASK(void * pvParameters)
 * @brief task handling messages to send via udp
 *
 * This task waits indefinitely to be notified by a sender function (itself in an other task)
 * When notification arrives, ulNotifiedValue is the item to send & it runs \ref SND_UDP.
 * Finally it notify the caller task with a status included in the item
 * @param void * pvParameters to get its tskInfo
 */
void sudp_TASK(void *pvParameters){
	s_tskInfo_X &tskInfo = *(reinterpret_cast<s_tskInfo_X*>(pvParameters));
	int8_t &this_DebugLVL = *(tskInfo.DebugLVL);
	extern unsigned char gre5PIN_Sudp;
	for (;;) {
		myUDP.keepAlive();
		s_logITM item;
		while (myUDP.pull(&item)) {
			digitalWrite(gre5PIN_Sudp, HIGH);
			myUDP.postString(item.logMSG, item.msgLen);
			/* A delay seems necessary between post or packet ???
			 * Maybe time of AsyncUPD to copy the buffer????
			 * */
//			vTaskDelay(1);
			delete[] item.logMSG;
			digitalWrite(gre5PIN_Sudp, LOW);
		}
		vTaskDelay(10); //think about other lower priorities tasks timing...
	}
}
//---------------------------------------------------------------------
bool c_myUDP::Begin(){
	lock_Wait();
	_inKMDSqueue = xQueueCreate(10, sizeof(s_kmdITM));
	_outLOGSqueue = xQueueCreate(10, sizeof(s_logITM));

	if (udp.listen(DEF_UDP_PORT)) {
		_SERIAL_0("\nUDP Listening on port %i", DEF_UDP_PORT);
		udp.onPacket(
			[this](AsyncUDPPacket packet){
				static int8_t &this_DebugLVL = *registerFCT(e_tasks::RUDP, "Rudp",
					&_recvDebugLVL, DEF_PRIO_RUDP);
				uint64_t now = milli_TS();
				uint64_t dtServer = now - this->_last_serverReady;
				uint64_t dtClient = now - this->_last_clientReady;
				_SERIAL_0("\nC/S%6.3f/%6.3f<%s", dtClient / 1000.0, dtServer / 1000.0,
					(char* )packet.data()); // no write packet.length() includes final '\0' from client
				if (String((char*)packet.data()).equals(DEF_UDP_CLIENT_HANDSHAKE)) {
					this->_last_clientReady = now;
					if (this->_remotePort != packet.remotePort())
						this->Open(packet);
					return;
				}
				try {
					assert(packet.length() < DEF_KMDSMSGLEN - 1);
					s_kmdITM item;
					strcpy(item.kmdsMSG, (char*)packet.data());
					if (pdPASS != xQueueSend(_inKMDSqueue, &item, 0)) {
						throw String("onPacket UDP FIFO OVERFLOW !");
					}
				} catch (String const &chaine) {
					_DEBUG_ERR("\n%s", chaine.c_str());
				}
			});
		launchTASK(e_tasks::SUDP, udpl_HDL, "Sudp", &sudp_TASK, nullptr,
			&_sendDebugLVL, 5000, 1, DEF_PRIO_SUDP);
		return true;
	}
	else
		_SERIAL_0("\nUDP CAN'T listen on  port %i", DEF_UDP_PORT);
	return false;
}
//---------------------------------------------------------------------
char* c_myUDP::has_KMD(){
	static s_kmdITM item; // To keep item during KMD processing !!!
	if (xQueueReceive(_inKMDSqueue, &item, 0) == pdPASS) {
		_SERIAL_0("\nUDP item.kmdsMSG=`%s`", item.kmdsMSG);
		return item.kmdsMSG;
	}
	return nullptr;
}
///////////////////////////////////////////////////////////////////////
