#ifndef __000_WSCHK__
#define __000_WSCHK__
//#include "FreeRTOS.h"
//#include <000_UTL_SYS_BASIC.h>
#include "ESPAsyncWebServer.h"
#define wsMax  5
/*
 Dans .h pour espace de noms (C & pas C++)!!
 BDOOR_protocol
 */
using namespace std;
class s_wsChecker {
public:
	typedef enum {
		RCV, SND
	} e_sens;
	int event = -2;
	int status = -2;
	int clientID = -2;
	uint64_t _milliTS_LastRCV; // TMS enqueue
	uint64_t _milliTS_LastSND; // TMS enqueue
	AsyncWebSocketClient *client = nullptr;
	String protocol = "???";
	String reversep = "???";
	s_wsChecker();
	void erase(const int EVENT, const int STATUS);
	void record(const int EVENT, AsyncWebSocketClient *CLIENT, const String PROTOCOL);
	String dump();
	String eventUpdate(const e_sens DIR);
	String dumpShort(const s_wsChecker::e_sens DIR);
};
s_wsChecker* openCNX(const int EVENT, AsyncWebSocketClient *CLIENT, const String PROTOCOL);
uint8_t closeAllWSX();
s_wsChecker* getWSX(const uint8_t i);
s_wsChecker* getWSXclient(const AsyncWebSocketClient *client);
String Remote(const char TARGET);
String SEVT(const int EVT);
String SSTA(const int STA);
String dumpAllJUNCTIONS();
String dumpWSX(const String TITLE);
String WWW(s_wsChecker *CHECKER = nullptr);
String listStaClients();
void listHeaders(AsyncWebServerRequest *request);
void serial_WSXDATA(const char *DIR, const char *DATA, int8_t DBGLVL);
void serial_WSXCHANGE(s_wsChecker::e_sens DIR, s_wsChecker *PWSX, uint32_t CALL, int8_t DBGLVL);
bool withCtrlBlock();
bool logWebsocket();
extern s_wsChecker *WSX[wsMax];
#endif //__000_WSCHK__
