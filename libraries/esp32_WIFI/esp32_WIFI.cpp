#include "esp32_WIFI.h"
#include "esp32_LOGS.h"
#include "esp32_SPRINT.h"
#include "000_WSCHK.h"
#include "esp32_FSTR.h"
c_myWIFI myWIFI;
//---------------------------------------------------------------------
void c_myWIFI::Status(){
	_SERIAL_0(Lang.CST(WIFI_LOCALIP), WiFi.localIP().toString().c_str());
	_SERIAL_0(Lang.CST(WIFI_ATT_DB), WiFi.RSSI());
}
//---------------------------------------------------------------------
void c_myWIFI::Close(){
	_SERIAL_0("\n\tClosing myWIFI");
	myUDP.Close("c_myWIFI::Close");
}
/**
 * https://github.com/espressif/arduino-esp32/blob/master/tools/sdk/include/esp32/esp_event_legacy.h
 * https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/WiFiClientEvents/WiFiClientEvents.ino
 * https://github.com/espressif/arduino-esp32/blob/master/tools/sdk/include/esp32/esp_event_legacy.h
 *
 */
void WiFiEvents(WiFiClass &WiFi){
//---------------------------------------------------------------------
	/**
	 * @brief ACCESS POINT breakdown: SYSTEM_EVENT_AP_STOP
	 */
	WiFi.onEvent([&myWIFI](WiFiEvent_t event, WiFiEventInfo_t info){
		_SERIAL_0("\nwifiEVT:WiFi stop AP");
		myWIFI.Close();
	}, ARDUINO_EVENT_WIFI_AP_STOP);
//---------------------------------------------------------------------
	/**
	 * @brief ACCESS POINT activation: SYSTEM_EVENT_AP_START
	 */
	WiFi.onEvent([&WiFi](WiFiEvent_t event, WiFiEventInfo_t info){
		uint8_t *adip = (uint8_t*)(&info.wifi_ap_staipassigned.ip);
//		myNet.ip_AP = WiFi.softAPIP();
		int8_t this_DebugLVL = 0;
		_SERIAL_0("\nwifiEVT:WiFi find AP connection: ");
		_SERIAL_0("\n\tIP ='%s'", WiFi.softAPIP().toString().c_str());
		_SERIAL_0("\n\tMAC='%s'", WiFi.softAPmacAddress().c_str());
	}, ARDUINO_EVENT_WIFI_AP_START);
//---------------------------------------------------------------------
	/**
	 * @brief ACCESS POINT station disconnection: SYSTEM_EVENT_AP_STADISCONNECTED
	 */
	WiFiEventId_t eventID = WiFi.onEvent(
		[](WiFiEvent_t event, WiFiEventInfo_t info){
			_SERIAL_0("\nwifiEVT:DISCONNECT MAC(%s)",
				StringMAC(&info.wifi_ap_stadisconnected.mac[0]).c_str());
			WSA.closeAllWSX();
			if (0 == memcmp((void*)&info.wifi_ap_stadisconnected.mac, (void*)myUDP.macAddrs(), 6)) {
				myUDP.Close("WIFI sta_disconnected");
			}
		}, ARDUINO_EVENT_WIFI_AP_STADISCONNECTED);
	// WiFi.removeEvent(eventID);
//---------------------------------------------------------------------
	/**
	 * @brief ACCESS POINT station connection: SYSTEM_EVENT_AP_STACONNECTED
	 */
	WiFi.onEvent(
		[](WiFiEvent_t event, WiFiEventInfo_t info){
			_SERIAL_0("\nwifiEVT:CONNECT -> MAC(%s)",
				StringMAC(&info.wifi_ap_staconnected.mac[0]).c_str());
			myUDP.lock_Clear();
		}, ARDUINO_EVENT_WIFI_AP_STACONNECTED);
	//---------------------------------------------------------------------
	/**
	 * @brief ACCESS POINT station IP assignation: SYSTEM_EVENT_AP_STAIPASSIGNED
	 */
	WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info){
		uint8_t *adip = (uint8_t*)(&info.wifi_ap_staipassigned.ip);
		IPAddress *updIP = new IPAddress(adip[0], adip[1], adip[2], adip[3]);
		_SERIAL_0("\nwifiEVT:ASSIGN IP(%s)", updIP->toString().c_str());
	}, ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED);
	//---------------------------------------------------------------------
	/**
	 * @brief STATION station IP assignation: SYSTEM_EVENT_WIFI_STA_GOT_IP
	 */
	WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info){
		uint8_t *adip = (uint8_t*)(&info.got_ip.ip_info.ip);
		IPAddress *IP = new IPAddress(adip[0], adip[1], adip[2], adip[3]);
		_SERIAL_0("\nwifiEVT:OBTAIN IP(%s)", IP->toString().c_str());
		adip = (uint8_t*)(&info.got_ip.ip_info.netmask);
		IPAddress *MASK = new IPAddress(adip[0], adip[1], adip[2], adip[3]);
		_SERIAL_0("  NETMASK(%s)", MASK->toString().c_str());
		adip = (uint8_t*)(&info.got_ip.ip_info.gw);
		IPAddress *GATE = new IPAddress(adip[0], adip[1], adip[2], adip[3]);
		_SERIAL_0("  GATEAWAY(%s)", GATE->toString().c_str());
	}, ARDUINO_EVENT_WIFI_STA_GOT_IP);
	//---------------------------------------------------------------------
	/**
	 * @brief STATION station IP lost IP: SYSTEM_EVENT_WIFI_STA_LOST_IP
	 */
	WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info){
		_SERIAL_0("\nwifiEVT:LOST IP(0.0.0.0)");
	}, ARDUINO_EVENT_WIFI_STA_LOST_IP);
}
///////////////////////////////////////////////////////////////////////
String StringMAC(const uint8_t *MAC){
	char buffer[19];
	int16_t cnt = 0;
	for (int j = 0; j < 6; j++)
		SPrintF(sizeof(buffer), cnt, buffer, "%02x:", (uint8_t)(*(MAC + j)));
	buffer[17] = '\0';
	return String(buffer);
}
///////////////////////////////////////////////////////////////////////
