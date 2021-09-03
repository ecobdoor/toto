#include <esp32_LOGS.h>
#include <rov.CONTEXT.h>
#include <000_WSCHK.h>
#include "esp_wifi.h"
s_wsChecker *WSX[wsMax];
s_wsChecker* getWSX(const uint8_t i){
	return WSX[i];
}
uint8_t closeAllWSX(){
	uint8_t cnt = 0;
	_SERIAL_0("\n@@@ >%s(%i)",__FUNCTION__,cnt);
	for (size_t i = 0; i < wsMax; i++) {
		//_SERIAL_3( "\nScan WSX %i, pwsx=[%s]", i,pwsx->dump().c_str());
		if (WSX[i]->client) {
			WSX[i]->client->close(4000, "Closed by server");
			WSX[i]->erase(-8, WSX[i]->client->status());
			cnt++;
			//_SERIAL_7( "\n%s@@@@ FOUND[idx(%02i)%s", WWW(pwsx).c_str(), i,pwsx->dump().c_str());
		}
	}
	if(cnt)
		_SERIAL_0("\n%s", WWW().c_str(), dumpWSX(__FUNCTION__).c_str());
	_SERIAL_0("\n@@@ <%s(%i)",__FUNCTION__,cnt);
	return cnt;
}
s_wsChecker* getWSXclient(const AsyncWebSocketClient *client){
	for (size_t i = 0; i < wsMax; i++) {
		//_SERIAL_3( "\nScan WSX %i, pwsx=[%s]", i,pwsx->dump().c_str());
		if (WSX[i]->client == client) {
			return WSX[i];
			//_SERIAL_7( "\n%s@@@@ FOUND[idx(%02i)%s", WWW(pwsx).c_str(), i,pwsx->dump().c_str());
		}
	}
	return nullptr;
}
s_wsChecker::s_wsChecker(){
	erase(-1, -1);
}
void s_wsChecker::erase(const int EVENT, const int STATUS){
	clientID = 0;
	static int this_DebugLVL = -1;
	_SERIAL_6("\n%s ERASING... EVT:%s STA:%s", dump().c_str(),
		SEVT(EVENT).c_str(), SSTA(STATUS).c_str());
	event = EVENT;
	status = STATUS;
	client = nullptr;
	protocol = "***";
	reversep = "***";
	_SERIAL_7("\n%s ERASED     EVT:%s STA:%s", dump().c_str(),
		SEVT(EVENT).c_str(), SSTA(STATUS).c_str());
}
void s_wsChecker::record(const int EVENT, AsyncWebSocketClient *CLIENT, const String PROTOCOL){
	static int this_DebugLVL = -1;
	_SERIAL_7("\n%s RECORDING... EVT:%s STA:%s PRO:\"%s\"", dump().c_str(),
		SEVT(EVENT).c_str(), SSTA(CLIENT->status()).c_str(), PROTOCOL.c_str());
	event = EVENT;
	status = CLIENT->status();
	clientID = CLIENT->id();
	client = CLIENT;
	protocol = PROTOCOL;
	reversep = "*Q*";
	if ((PROTOCOL.length() == 3) && (PROTOCOL[1] == 'Q')) {
		reversep[0] = PROTOCOL[2];
		reversep[2] = PROTOCOL[0];
	}
	//_SERIAL_1( "\nws[%s][%u] connect protolcol:\"%s\"", server->url(),	client->id(), BDOOR_protocol.c_str());
	_SERIAL_3("\n%s", dumpWSX("RECORDED").c_str());
}
///////////////////////////////////////////////////////////////////////
s_wsChecker* openCNX(const int EVENT, AsyncWebSocketClient *CLIENT, const String PROTOCOL){
	static int this_DebugLVL = -1;
	for (size_t i = 0; i < wsMax; i++) {
		s_wsChecker *pwsx = getWSX(i);
		if (pwsx->client == nullptr) {
			pwsx->_milliTS_LastRCV = milli_TS();
			pwsx->_milliTS_LastSND = milli_TS();
			_SERIAL_7("\n%s %s", WWW().c_str(), dumpWSX("openCNX begin").c_str());
			pwsx->record(EVENT, CLIENT, PROTOCOL.c_str());
			return pwsx;
		}
	}
	_DEBUG_ERR("\n%s@@@@ ERROR !!!!!!!!!!!!openCNX impossible!!!!!!!!!!!!!!",
		WWW().c_str());
	return nullptr;
}
///////////////////////////////////////////////////////////////////////
String s_wsChecker::dump(){
	int cnt = 0;
	char BUF[200] = "";
	cnt += sprintf(&(BUF[cnt]), "{%+03i  %s %s } '%s⇔%s' %-10p",
		clientID, SSTA(status).c_str(), SEVT(event).c_str(), protocol.c_str(), reversep.c_str(),
		client);
	return String(BUF);
}
String s_wsChecker::dumpShort(const s_wsChecker::e_sens DIR){
	extern AsyncWebSocket webSokSrv;
	int16_t cnt = 0;
	char buffer[64];
	const char *prot;
	if (DIR == s_wsChecker::SND)
		prot = reversep.c_str();
	else
		prot = protocol.c_str();
	int64_t now = milli_TS();
	SPrintF(sizeof(buffer), cnt, buffer, "%s %i/%i %s %s %.3fs", prot, clientID, webSokSrv.count(),
		SSTA(status).c_str(), SEVT(event).c_str(), (now - (int64_t)_milliTS_LastSND) / 1000.0F);
	if (DIR == s_wsChecker::SND)
		_milliTS_LastSND = now;
	else
		_milliTS_LastRCV = now;
	return String(buffer);
}
String dumpAllJUNCTIONS(){
	int16_t cnt = 0;
	char buffer[256];
	SPrintF(sizeof(buffer), cnt, buffer, "####  * {wId ST-EV}  pro⇔rev  Address");
	for (int i = 0; i < wsMax; i++) {
		SPrintF(sizeof(buffer), cnt, buffer, "\n\t%i %s", i, WSX[i]->dump().c_str());
	}
	return String(buffer);
}
///////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////
String SEVT(const int EVT){
	String CHN; // = String((EVT > 0) ? "+" : "") + String(EVT) + ":";
	if (EVT == WS_EVT_DATA)
		return String("◄"); //"DATA......";
	if (EVT == 99)
		return String("►"); // "PONG......";
	if (EVT == WS_EVT_PONG)
		return String("◊"); // "PONG......";
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
///////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////
String dumpWSX(const String TITLE){
	String CHN = "" + TITLE + " --------------";
	for (int i = 0; i < wsMax; i++) {
		CHN += "\n" + String(i) + " " + WSX[i]->dump();
	}
	return CHN + "\n";
}
///////////////////////////////////////////////////////////////////////
/**
 * When Who What
 */
String WWW(s_wsChecker *CHECKER){
	extern uint64_t dt_modules;
	extern uint64_t dt_event;
	extern uint64_t dt_queue;

//	return "";
	char buffer[164];
	int cnt = 0;
	if (CHECKER != nullptr) {
		cnt += sprintf(&buffer[cnt], "%02i:%s", CHECKER->clientID, SSTA(CHECKER->status).c_str());
	} else {
		cnt += sprintf(&buffer[cnt], "%02i:%s", -2, "?????????????");
	}
	//cnt += sprintf(&buffer[cnt], " (%3i~%3i~%3i)", (uint32_t) dt_event,(uint32_t) dt_queue, (uint32_t) dt_modules);
	/*
	 cnt += sprintf(&buffer[cnt], "flow(%i>%2i/%3i)", uxQueueMessagesWaiting(inMSGqueue),
	 qFlow->waitSize(), qFlow->lostSize());
	 */
//	cnt += sprintf(&buffer[cnt], "flow(flow %i>)", uxQueueMessagesWaiting(inMSGqueue));
	cnt += sprintf(&buffer[cnt], "\t");
	return String(buffer);
	/*
	 cnt += sprintf(&buffer[cnt], "%s:", SUI64(milli_TS()).c_str());
	 cnt += sprintf(&buffer[cnt], " %idB", WiFi.RSSI());
	 //	cnt += sprintf(&buffer[cnt], "COR(%i)", xPortGetCoreID());
	 if (CHECKER != nullptr) {
	 cnt += sprintf(&buffer[cnt], " %s", CHECKER->dump().c_str());
	 } else {
	 cnt += sprintf(&buffer[cnt],
	 "                                                            ");
	 }
	 //	cnt += sprintf(&buffer[cnt], "FRE(%i)", ESP.getFreeHeap());
	 cnt += sprintf(&buffer[cnt], "Queue(%i)", uxQueueMessagesWaiting(inMSGqueue));
	 cnt += sprintf(&buffer[cnt], "\t");
	 return String(buffer);
	 */
}
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
void serial_WSXDATA(const char *DIR, const char *DATA, int8_t DBGLVL){
	if (CTX.SOFTctx.showWSX_data) {
		if (!CTX.SOFTctx.showWSX_event)
			_SERIAL_0("\n%2i:%s%s", DBGLVL, DIR, DATA);
	}
}
void serial_WSXCHANGE(s_wsChecker::e_sens DIR, s_wsChecker *PWSX, uint32_t CALL, int8_t DBGLVL){
	if (CTX.SOFTctx.showWSX_event) {
		_SERIAL_0("\n%2i:", DBGLVL);
		if (PWSX != nullptr)
			_SERIAL_0("%s %7i", PWSX->dumpShort(DIR).c_str(), CALL);
		else
			_SERIAL_0("%s %7i", "PWSX==nullptr!!!   ", CALL);
	}
}
bool logWebsocket(){
	return CTX.SOFTctx.logWebsocket;
}
bool withCtrlBlock(){
	return CTX.SOFTctx.withCtrlBlock;
}

