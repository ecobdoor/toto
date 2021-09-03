#include "Arduino.h"
#include "esp32_WIFI.h"
#include "esp32_LOGS.h"
#include "esp32_UDP_log.h"
#include "esp32_KMDS.h"
#include "esp32_FSTR.h"
#define DEF_ESP_WIFI_AP
// LED pin is IO2 ???

int vio7PIN_Serial = 13; // _DEBUG_ Serial extern...
int blu6PIN_Logs = 12; // _LOG_ extern...
#define gre5PIN_Sudp 14
#define yel4PIN_ 27
#define ora3PIN_imu 16
#define red2PIN_modules  17
#define bro1PIN_Vrcv 25
#define whi0PIN_Wsnd  26
int8_t debugLVL=5;;
int8_t &this_DebugLVL=debugLVL;
///////////////////////////////////////////////////////////////////////
void setup(){
	Serial.begin(115200);
	// SPIFFS =============================================================
		_DBG_APP_BOOT("\n#### SPIFFS ");
		if (!SPIFFS.begin(false, "/spiffs")) {
			_DBG_APP_BOOT("FAILS while mounting");
			return;
		} else
			_DBG_APP_BOOT("started");
		Lang.listDir_Old(SPIFFS, "/", 0);
	// LangSTR =============================================================
		Lang.Begin("/rov-Sen_000_WS3(french).csv");
#ifdef DEF_ESP_WIFI_AP
	const char *ssid = "rover";
	const char *password = "0123456789";
	WiFi.softAP(ssid, password, 3); //,1);//,0,3);
	Serial.printf("\n\t Access Point ssid='%s', pswd='%s'", ssid, password);
	delay(2000); // To find channel, etc...
#else
	const char* ssid = "BazUbu";
	const char* password = "0123456789";
	WiFi.begin(ssid, password);
	_SERIAL_3( Lang.CST(WIFI_CONNECTING));
	while (WiFi.status() != WL_CONNECTED)
	{
		USE_SERIAL.printf( ".");
		delay(1000);
	}
	USE_SERIAL.printf( "\n");
	_SERIAL_3( Lang.CST(WIFI_LOCALIP), WiFi.localIP().toString().c_str());
	_SERIAL_3( Lang.CST(WIFI_ATT_DB), WiFi.RSSI());
#endif
// UDP log ====================================================
	if (myUDP.Begin())
		Serial.printf("\n\t UDPL init OK");
	else
		Serial.printf("\n\t UDPL init ERROR");
}
///////////////////////////////////////////////////////////////////////
void loop(){
	int8_t dbg_loop = 8;
	static int8_t &this_DebugLVL = *registerFCT(e_tasks::LOOP, "Loop", &dbg_loop);
	 FIX_ERRORS();
	 KMDS(myUDP.has_KMD());
	vTaskDelay(50 / portTICK_PERIOD_MS);
	_DBG_APP_KMDS("\n\t******TMS*********************\n\t*****************************************");
	_DBG_APP_KMDS("\n\t******SET_UP**************************************************************");
	_DBG_APP_KMDS("\n\t******imu**************************************************************");
}
///////////////////////////////////////////////////////////////////////
