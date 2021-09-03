/// @file
#pragma once
#include "ESPAsyncWebServer.h"
#include <atomic>
//#include "rov.CONTEXT.h"
using namespace std;
///////////////////////////////////////////////////////////////////////
#define wsMax  5
typedef char protoType[4];
///////////////////////////////////////////////////////////////////////
/*
 Dans .h pour espace de noms (C & pas C++)!!
 BDOOR_ protocol
 */
class c_WS: public Core {
public:
	typedef enum {
		RCV,
		SND
	} e_sens;
	int event = -2; ///< ping, pong, connect, disconnect, error, message...
	int status = -2; ///< connected, disconnected...
	int clientID = -2; ///< Client ID from the web socket server
	uint64_t _milliTS_LastRCV; ///< Last TMS receiver enqueue
	uint64_t _milliTS_LastSND; ///< Last TMS sender enqueue
	AsyncWebSocketClient *client = nullptr;
	protoType protocol; ///< WS client sub-protocol for example pQr = pilot TO rover at connection, query, reply or info
	protoType reversep; ///< WS server sub-protocol for example rQp = rover TO pilot at query, reply or info
	c_WS();
	void erase(const int EVENT, const int STATUS);
	void record(const int EVENT, AsyncWebSocketClient *CLIENT, const String PROTOCOL);
	String dumpLong();
	String eventUpdate(const e_sens DIR);
	String dumpShort(const c_WS::e_sens DIR);
};
///////////////////////////////////////////////////////////////////////
c_WS* openCNX(const int EVENT, AsyncWebSocketClient *CLIENT, const String PROTOCOL);
c_WS* getWSX(const uint8_t i);
String Remote(const char TARGET);
String SEVT(const int EVT);
String SSTA(const int STA);
String dumpAllJUNCTIONS();
String dumpWSX(const String TITLE);
//String WWW(c_WS *CHECKER = nullptr);
String listStaClients();
void listHeaders(AsyncWebServerRequest *request);
//void serial_WSXDATA(const char *DIR, const char *DATA, int8_t DBGLVL);
void serial_WSXCHANGE(const char *TITLE, c_WS::e_sens DIR, c_WS *PWSX, uint32_t CALL,
	const char *DATA, const size_t LEN);
///////////////////////////////////////////////////////////////////////
class c_wsxAll {
private:
	std::atomic_flag _wsaLocker = ATOMIC_FLAG_INIT; // set to clear state = FALSE;

public:
	c_WS* getWSXclient(const AsyncWebSocketClient *client);
	c_WS* lookForWSX(const char *PROTOCOL);
	uint8_t closeAllWSX();
	void lock_Clear();
	/**
	 * @fn void lock_Wait()
	 * @brief waits for the atomic_flag is unlocked and locks it again
	 */
	void lock_Wait();
	void lock_Set();
};
//---------------------------------------------------------------------
inline void c_wsxAll::lock_Set(){
	_wsaLocker.test_and_set(); // set to true & return previous state
}
//---------------------------------------------------------------------
inline void c_wsxAll::lock_Wait(){
	while (_wsaLocker.test_and_set()) { // set to true & return previous state
		if (CTX.SOFTctx.trace_UDP)
			_SERIAL_0("*");
		vTaskDelay(2);
	}
}
//---------------------------------------------------------------------
inline void c_wsxAll::lock_Clear(){
	_wsaLocker.clear(); // clear to false
}
///////////////////////////////////////////////////////////////////////
extern c_WS *WSX[wsMax];
extern c_wsxAll WSA;
///////////////////////////////////////////////////////////////////////

