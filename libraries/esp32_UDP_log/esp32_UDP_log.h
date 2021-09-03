#ifndef __esp32_UDP_log__
#define __esp32_UDP_log__
#include "Arduino.h"
#include "AsyncUDP.h"
#include <atomic>
#include <esp32_SYS_basic.h>
#include "rov.CONTEXT.h"
//#include <functional>
//#include "AsyncUDP.h"
//typedef std::function<bool(const String)> f_udplCB;
typedef struct {
	TaskHandle_t tsk = nullptr;
	//s_tskInfo tsk Info;//={nullptr,0,0,0} ;
	int16_t msgLen = 0;
	uint32_t msgID = 99999999;
	char *logMSG = nullptr;
	bool ok = false;
	int msWaitNotify = 0;
} s_logITM;
class c_myUDP: public Core {
private:
	std::atomic_flag _udpLocker = ATOMIC_FLAG_INIT; // set to clear state = FALSE;
	uint16_t _remotePort = 0;
	uint8_t _remoteMAC[6] = { 0, 0, 0, 0, 0, 0 };
	IPAddress _remoteIP = IPAddress();
	QueueHandle_t _outLOGSqueue;
	uint64_t _last_serverReady = 0;
	uint64_t _last_clientReady = 0;
	int8_t _recvDebugLVL = 5;
	TaskHandle_t udpl_HDL=nullptr;
	int8_t _sendDebugLVL = 5;

public:
	c_myUDP();///< private constructor, disallow creating an instance of this class
	void lock_Clear();
	void lock_Wait();
	//IPAddress ip_AP = IPAddress();
	//IPAddress ip_STA = IPAddress();
	bool Begin();
	void Open(AsyncUDPPacket &packet);
	bool isOpen();
	void Close(const char * FORMAT,...);
	uint8_t* macAddrs();
	void postString(const char *BUFFER, const size_t LENGTH);
	uint8_t push(const uint32_t CALLS, char *BUFFER, const int16_t CNT);
//	void push_KMD(char *KMD);
	uint8_t pull(s_logITM *ITEM);
	void keepAlive();
};
//---------------------------------------------------------------------
inline void c_myUDP::lock_Wait(){
	while (_udpLocker.test_and_set()) {
		if (CTX.SOFTctx.trace_UDP)
			_SERIAL_0("&");
		vTaskDelay(1);
	}
}
//---------------------------------------------------------------------
inline void c_myUDP::lock_Clear(){
	_udpLocker.clear();
}
//---------------------------------------------------------------------
inline uint8_t* c_myUDP::macAddrs(){
	return &_remoteMAC[0];
}
//---------------------------------------------------------------------
inline bool c_myUDP::isOpen(){
	return _remotePort != 0;
}
///////////////////////////////////////////////////////////////////////
extern c_myUDP myUDP;
///////////////////////////////////////////////////////////////////////
#endif
