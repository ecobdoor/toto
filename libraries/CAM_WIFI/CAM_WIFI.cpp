#include "CAM_WIFI.h"
//#include <WiFiClientSecure.h>
///////////////////////////////////////////////////////////////////////
bool SETUP_WIFI(const char *ssid, const char *password){
	WiFi.begin(ssid, password);
	Serial.printf("\n\nWiFi");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	WiFi.setSleep(false);
	Serial.printf("connected");
	Serial.printf("\n\tMAC:'%s'", WiFi.macAddress().c_str());
	Serial.printf("\n\tIP :'%s'", WiFi.localIP().toString().c_str());
	return true;
}
///////////////////////////////////////////////////////////////////////
