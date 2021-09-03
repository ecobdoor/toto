/**
 * @file
 * @brief This unit manages web sockets
 *
 * ssgsdddddddddddddddddddddddddddddddddddddddddddddddddddd
 * \n ggggggggggggggggggggggggggggggggggggggggg
 */
#include <esp32_LOGS.h>
#include "esp32_SPRINT.h"
//#include <rov.CONTEXT.h>
#include <000_WSCHK.h>
#include "esp_wifi.h"
///////////////////////////////////////////////////////////////////////
/**
 * @brief Global web socket application manager
 */
c_WS *WSX[wsMax];
c_wsxAll WSA;
//---------------------------------------------------------------------
int8_t c_wsChecker_dbglvl = 0;
c_WS::c_WS() :
	Core("WSX", &c_wsChecker_dbglvl){
	erase(-1, -1);
}
//---------------------------------------------------------------------
c_WS* getWSX(const uint8_t i){
	return WSX[i];
}
//---------------------------------------------------------------------
void c_WS::erase(const int EVENT, const int STATUS){
	clientID = 0;
	static int this_DebugLVL = -1;
	//_SERIAL_0("\n%s ERASING... EVT:%s STA:%s", dump().c_str(),SEVT(EVENT).c_str(), SSTA(STATUS).c_str());
	event = EVENT;
	status = STATUS;
	client = nullptr;
	strcpy((char*)protocol, "***");
	strcpy((char*)reversep, "***");
	//_SERIAL_0("\n%s ERASED     EVT:%s STA:%s", dump().c_str(),SEVT(EVENT).c_str(), SSTA(STATUS).c_str());
}
//---------------------------------------------------------------------
void c_WS::record(const int EVENT, AsyncWebSocketClient *CLIENT, const String PROTOCOL){
	static int this_DebugLVL = -1;
	_SERIAL_7("\n%s RECORDING... EVT:%s STA:%s PRO:\"%s\"", dumpLong().c_str(),
		SEVT(EVENT).c_str(), SSTA(CLIENT->status()).c_str(), PROTOCOL.c_str());
	event = EVENT;
	status = CLIENT->status();
	clientID = CLIENT->id();
	client = CLIENT;
	strcpy((char*)&protocol, PROTOCOL.c_str());
	strcpy((char*)&reversep, "*Q*");
	//_SERIAL_0("\nprotocol %s reversep %s", protocol, reversep);
	if ((PROTOCOL.length() == 3) && (PROTOCOL[1] == 'Q')) {
		reversep[0] = PROTOCOL[2];
		reversep[2] = PROTOCOL[0];
		//_SERIAL_0("\nprotocol %s reversep %s", protocol, reversep);
	}
	//_SERIAL_1( "\nws[%s][%u] connect protolcol:\"%s\"", server->url(),	client->id(), BDOOR_protocol.c_str());
	_SERIAL_3("\n%s", dumpWSX("RECORDED").c_str());
}
//---------------------------------------------------------------------
c_WS* openCNX(const int EVENT, AsyncWebSocketClient *CLIENT, const String PROTOCOL){
	static int this_DebugLVL = -1;
	for (size_t i = 0; i < wsMax; i++) {
		c_WS *pwsx = getWSX(i);
		if (pwsx->client == nullptr) {
			pwsx->_milliTS_LastRCV = milli_TS();
			pwsx->_milliTS_LastSND = milli_TS();
			//_SERIAL_7("\n%s %s", WWW().c_str(), dumpWSX("openCNX begin").c_str());
			pwsx->record(EVENT, CLIENT, PROTOCOL.c_str());
//			_SERIAL_0("\n%s:%s", __FUNCTION__, pwsx->dumpShort(c_WS::RCV).c_str());
			return pwsx;
		}
	}
	return nullptr;
}
///////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
String c_WS::dumpLong(){
	int cnt = 0;
	char BUF[200] = "";
	cnt += sprintf(&(BUF[cnt]), "{%+03i  %s %s } '%s⇔%s' %-10p",
		clientID, SSTA(status).c_str(), SEVT(event).c_str(), protocol, reversep, client);
	return String(BUF);
}
//---------------------------------------------------------------------
String Remote(const char TARGET){
	if (TARGET == 'p')
		return "Pilot";
	else if (TARGET == 'b')
		return "Basis";
	else if (TARGET == 'l')
		return "DBlog";
	else
		return "?????";
}
//---------------------------------------------------------------------
String SEVT(const int EVT){
	String CHN; // = String((EVT > 0) ? "+" : "") + String(EVT) + ":";
	if (EVT == WS_EVT_DATA)
		return String("◄"); //"DATA......";
	if (EVT == 99)
		return String("►"); // "SEND......";
	if (EVT == 98)
		return String("ᐅ"); // "PING......";
	if (EVT == WS_EVT_PONG)
		return String("ᐊ"); // "PONG......";
	if (EVT == WS_EVT_CONNECT)
		return String("▲"); //CHN +="🇨";// "CONNECT...";
	if (EVT == WS_EVT_DISCONNECT)
		return String("▼"); // "DISCONNECT";
	if (EVT == WS_EVT_ERROR)
		return String("▬"); // "ERROR.....";
	if (EVT == -1)
		return String("†"); //"..........";
	else
		return String("¿"); //"??????????";
	return CHN;
}
//---------------------------------------------------------------------
//"⇫⇪∆∇≚∞⋀🇼⟰⟱⚺⚻▴⮛⮟⮝⑬⋁ᗄᗐထܠݎݏ߇߈ƧɅ±⮊∇¤øƟɅɣʬΔΛϪᴓᴧᵛ†‼∆∩⌂▬►◄◊¥Vv□ΥѲ˅˄▲▼ꜛꜜ◊↑↓↕↨ˠØ";
String SSTA(const int STA){
	String CHN; // = String((STA > 0) ? "+" : "") + String(STA) + ":";
	if (STA == WS_DISCONNECTED)
		CHN += "∇"; // "Disconnected ";
	else if (STA == WS_CONNECTED)
		CHN += "∆"; //"Connected    ";
	else if (STA == WS_DISCONNECTING)
		CHN += "ܠ"; //"Disconnecting";
	else if (STA == -1)
		CHN += "†"; //.............";
	else
		CHN += "¿"; //"?????????????";
	return CHN;
}
//---------------------------------------------------------------------
String dumpAllJUNCTIONS(){
	int16_t cnt = 0;
	char buffer[256];
	SPrintF(sizeof(buffer), cnt, buffer, "####  * {wId ST-EV}  pro⇔rev  Address");
	for (int i = 0; i < wsMax; i++) {
		SPrintF(sizeof(buffer), cnt, buffer, "\n\t%i %s", i, WSX[i]->dumpLong().c_str());
	}
	return String(buffer);
}
//---------------------------------------------------------------------
String dumpWSX(const String TITLE){
	String CHN = "" + TITLE + " --------------";
	for (int i = 0; i < wsMax; i++) {
		CHN += "\n" + String(i) + " " + WSX[i]->dumpLong();
	}
	return CHN + "\n";
}
//---------------------------------------------------------------------
String listStaClients(){
	wifi_sta_list_t wifi_sta_list;
	tcpip_adapter_sta_list_t adapter_sta_list;
	memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
	memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));

	esp_wifi_ap_get_sta_list(&wifi_sta_list);
	tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);
	char buffer[256];
	int16_t cnt = 0;
	SPrintF(sizeof(buffer), cnt, buffer, "\n---- STA MAC address   IP address");
	if (adapter_sta_list.num == 0)
		SPrintF(sizeof(buffer), cnt, buffer, "\nNo station leasing");
	else
		for (int i = 0; i < adapter_sta_list.num; i++) {
			tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
			SPrintF(sizeof(buffer), cnt, buffer, "\n    %i ", i);
			for (int i = 0; i < 6; i++) {
				SPrintF(sizeof(buffer), cnt, buffer, "%02X", station.mac[i]);
				if (i < 5)
					SPrintF(sizeof(buffer), cnt, buffer, ":");
			}
			SPrintF(sizeof(buffer), cnt, buffer,
				" PB cannot convert 'esp_ip4_addr_t*' {aka 'esp_ip4_addr*'} to 'const ip4_addr_t*' {aka 'const ip4_addr*'}");
			/*
			 const ip4_addr_t * iipp=&(station.ip);
			 SPrintF(sizeof(buffer), cnt, buffer, " %s",ip4addr_ntoa(&(station.ip));
			 uint32_t ip = ipaddr_addr(ip4addr_ntoa(&(station.ip)));
			 uint8_t ip3 = (uint8_t)((ip & 0xff000000) >> 0x18);
			 uint8_t ip2 = (uint8_t)((ip & 0x00ff0000) >> 0x10);
			 uint8_t ip1 = (uint8_t)((ip & 0x0000ff00) >> 0x08);
			 uint8_t ip0 = (uint8_t)((ip & 0x000000ff) >> 0x00);
			 SPrintF(sizeof(buffer), cnt, buffer, " %03i.%03i.%03i.%03i", ip0, ip1, ip2, ip3);
			 */
		}
	return String(buffer);
}
//---------------------------------------------------------------------
void listHeaders(AsyncWebServerRequest *request){
	int headers = request->headers();
	int i;
	_SERIAL_0("\n-----------");
	if (headers == 0)
		_SERIAL_0("\nNo request header");
	else
		for (i = 0; i < headers; i++) {
			AsyncWebHeader *h = request->getHeader(i);
			_SERIAL_0("\nHEADER[%s]: %s", h->name().c_str(), h->value().c_str());
		}
	_SERIAL_0("\n-----------");
}
//---------------------------------------------------------------------
String c_WS::dumpShort(const c_WS::e_sens DIR){
	extern AsyncWebSocket webSokSrv;
	int16_t cnt = 0;
	char buffer[64];
	char *prot;
	if (DIR == c_WS::SND)
		prot = (char*)reversep;
	else
		prot = (char*)protocol;
	int64_t now = milli_TS();
	SPrintF(sizeof(buffer), cnt, buffer, "%s %3i%%%i s%s e%s %8.3fs",
		prot, clientID, webSokSrv.count(),
		SSTA(status).c_str(), SEVT(event).c_str(), (now - (int64_t)_milliTS_LastSND) / 1000.0F);
	if (DIR == c_WS::SND)
		_milliTS_LastSND = now;
	else
		_milliTS_LastRCV = now;
	return String(buffer);
}
///////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
uint8_t c_wsxAll::closeAllWSX(){
	DETACH(P_imuISR_); // out side of throw !!!!!
	WSA.lock_Wait();
	uint8_t cnt = 0;
	_SERIAL_0("\n\t@@@@ >%s()\n%s", __FUNCTION__, dumpAllJUNCTIONS().c_str());
	for (size_t i = 0; i < wsMax; i++) {
		//_SERIAL_3( "\nScan WSX %i, pwsx=[%s]", i,pwsx->dump().c_str());
		if (WSX[i]->client) {
			WSX[i]->client->close(4000, "Closed by server...");
			WSX[i]->erase(-8, WSX[i]->client->status());
			cnt++;
			//_SERIAL_7( "\n%s@@@@ FOUND[idx(%02i)%s", WWW(pwsx).c_str(), i,pwsx->dump().c_str());
		}
	}
	_SERIAL_0("\n\t@@@@ <%s() %i ws closed", __FUNCTION__, cnt);
	WSA.lock_Clear();
	ATTACH(P_imuISR_); // out side of throw !!!!!
	return cnt;
}
//---------------------------------------------------------------------
c_WS* c_wsxAll::getWSXclient(const AsyncWebSocketClient *client){
	c_WS *pwsx = nullptr;
	WSA.lock_Wait();
	for (size_t i = 0; i < wsMax; i++) {
		//_SERIAL_3( "\nScan WSX %i, pwsx=[%s]", i,pwsx->dump().c_str());
		if (WSX[i]->client == client) {
			pwsx = WSX[i];
			//_SERIAL_7( "\n%s@@@@ FOUND[idx(%02i)%s", WWW(pwsx).c_str(), i,pwsx->dump().c_str());
			break;
		}
	}
	WSA.lock_Clear();
	return pwsx;
}
//---------------------------------------------------------------------
c_WS* c_wsxAll::lookForWSX(const char *PROTOCOL){
	protoType tmp;
	strcpy((char*)tmp, PROTOCOL);
	tmp[1] = 'Q'; // in case of sending a reply rRp etc...
	c_WS *pwsx = nullptr;
	WSA.lock_Wait();
	for (size_t i = 0; i < wsMax; i++) {
		//_SERIAL_3( "\nScan WSX %i, pwsx=[%s]", i,pwsx->dump().c_str());
		if (0 == strcmp(WSX[i]->reversep, tmp)) {
			pwsx = WSX[i];
			//_SERIAL_7( "\n%s@@@@ FOUND[idx(%02i)%s", WWW(pwsx).c_str(), i,pwsx->dump().c_str());
			break;
		}
	}
	WSA.lock_Clear();
	return pwsx;
}
///////////////////////////////////////////////////////////////////////
