#include "000_WIFI.h"
#include "000_LOGS.h"
#include "000_CONTEXT.h"
#include "../rov-CfgH_000_WS3/rov_INC.inc"
///////////////////////////////////////////////////////////////////////
AsyncUDP udp;
c_myUDP myUDP;
//---------------------------------------------------------------------
uint8_t c_myUDP::push(const uint32_t CALLS, const char *BUFFER, const int16_t CNT){
	s_logITM item;
	item.msgID = CALLS;
	item.logMSG = BUFFER;
	item.msgLen = CNT;
	try {
		if (pdPASS != xQueueSend(_outLOGSqueue, &item, 0))
			THROWERR(FIFO_OVERFLOW, CALLS, micro_TS());
		uint8_t siz=uxQueueMessagesWaiting(_outLOGSqueue);
		_SERIAL_0(">u%s",(char*)&whiteNum[siz]);
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
void c_myUDP::postString(const char *BUFFER, const size_t LENGTH){
	lock_Wait();
	#define packLen 1430 // MUST BE LESS THAN EFFECTIVE MTU SIZE: 1436 (-ε=6?)
	size_t lenRemaining=LENGTH+1;
	size_t sumLenSent=0;
	size_t lenToSend;
	size_t lenSent;
	_SERIAL_0("\n{%i}",udp.lastErr());
	do{
		if(lenRemaining<=packLen){
			lenToSend=lenRemaining;// Buffer completed
			lenRemaining=0;
		}else{
			lenToSend=packLen;//Part of buffer
			lenRemaining-=packLen;
		}
		lenSent= udp.writeTo((uint8_t*)(BUFFER+sumLenSent), lenToSend, _remoteIP, _remotePort);
//		lenSent= udp.write((uint8_t*)(BUFFER+sumLenSent), lenToSend);
		sumLenSent +=lenSent;
//		_SERIAL_0("\nudp sent(%i/%i,heap:%i)",sumLenSent,LENGTH + 1,ESP.getMaxAllocHeap());
//		vTaskDelay(2);
	}while((lenToSend==lenSent)&&(sumLenSent!=(LENGTH + 1)));
//	if(lenSent>2000)
	if(lenSent!=lenToSend){
		_SERIAL_0("\nUDPERR(%i)",lenSent-2000);
		uint32_t heap=ESP.getMaxAllocHeap();
		if(heap<65536)
			_SERIAL_0(" HEAP(%i)",heap);
	}
	//A delay seems necessary between post or packet ???
	lock_Clear();
}
//---------------------------------------------------------------------
void c_myUDP::keepAlive(){
	if(!isOpen())
		return;
	uint64_t now = milli_TS();
	uint64_t dtServer = now - _last_serverReady;
	uint64_t dtClient = now - _last_clientReady;
	if (1000 < dtServer) {
		_SERIAL_0("\nC/S%6.3f/%6.3f>%s",dtClient/1000.0,dtServer/1000.0,DEF_UDP_SERVER_HANDSHAKE);
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
	_SERIAL_0("\n\t§§§§ Closing myUDP (%s)",REASON.c_str());
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
bool c_myUDP::Begin(uint16_t port, QueueHandle_t *kmdsQueue){
	lock_Wait();
	_kmdsQueue = kmdsQueue;
	_outLOGSqueue = xQueueCreate(10, sizeof(s_logITM));

	if (udp.listen(port)) {
		_SERIAL_0("\nUDP Listening on port %i", port);
		udp.onPacket([this](AsyncUDPPacket packet){
			static int8_t &this_DebugLVL = *registerFCT(e_tasks::RUDP, "Rudp",
				&CTX.SOFTctx.TSK_DBGMAX_RECV, DEF_PRIO_RUDP);
			uint64_t now = milli_TS();
			uint64_t dtServer = now - this->_last_serverReady;
			uint64_t dtClient = now - this->_last_clientReady;
			_SERIAL_0("\nC/S%6.3f/%6.3f<%s",dtClient/1000.0,dtServer/1000.0,(char*)packet.data());// no write packet.length() includes final '\0' from client
			if (String((char*)packet.data()).equals(DEF_UDP_CLIENT_HANDSHAKE)) {
				this->_last_clientReady=now;
				if (this->_remotePort != packet.remotePort()) 
					this->Open(packet);
				return;
			} 
			try {
				assert(packet.length() < DEF_KMDSMSGLEN - 1);
				s_kmdsMSG item;
				strcpy(item.kmdsMSG, (char*)packet.data());
				if (pdPASS != xQueueSend(*_kmdsQueue, &item, 0)) {
					throw String("onPacket UDP FIFO OVERFLOW !");
				}
			} catch (String const &chaine) {
				_DEBUG_ERR("\n%s", chaine.c_str());
			}
		});
		return true;
	} else
		_SERIAL_0("\nUDP CAN'T listen on  port %i", port);
	return false;
}
///////////////////////////////////////////////////////////////////////
