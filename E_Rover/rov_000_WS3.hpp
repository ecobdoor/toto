/**
 * @file





## Using esp32-arduino-lib-builder to compile custom libraries

Espressif has provided a [tool](https://github.com/espressif/esp32-arduino-lib-builder) to simplify building your own compiled libraries for use in Arduino IDE (or your favorite IDE).
To use it to generate custom libraries, follow these steps:
1. `git clone https://github.com/espressif/esp32-arduino-lib-builder`
2. `cd esp32-arduino-lib-builder`
3. `./tools/update-components.sh`
4. `./tools/install-esp-idf.sh` (if you already have an $IDF_PATH defined, it will use your local copy of the repository)
5. `make menuconfig` or directly edit sdkconfig.
6. `./build.sh`

The script automates the process of building [arduino as an ESP-IDF component](https://github.com/espressif/arduino-esp32/blob/master/docs/esp-idf_component.md).
Once it is complete, you can cherry pick the needed libraries from `out/tools/sdk/lib`, or run `tools/copy-to-arduino.sh` to copy the entire built system.
`tools/config.sh` contains a number of variables that control the process, particularly the $IDF_BRANCH variable.  You can adjust this to try building against newer versions, but there are absolutely no guarantees that any components will work or even successfully compile against a newer IDF.






 * @page todo_tofix _TODO | _TOFIX
 * - Tasks priorities & cores:
 * @todo ASync receivers:
 * @n CONFIG_ASYNC_TCP_RUNNING_CORE =-1//any available core (take 0 !!!)
 * @n CONFIG_ARDUINO_UDP_RUNNING_CORE=1
 * @n CONFIG_ARDUINO_UDP_TASK_PRIORITY=3
 *
 * - ISR attach & detach:
 * @todo attach after delay when html client loads pages
 * @n or OTA upgrades | uploads | downloads
 *
 * - A LIRE ABSOLUMENT:
 * - http://marionpatrick.free.fr/man_html/html/tuto_code_unicode:2000-3fff.html (utf8)
 * - https://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format
 * - http://vidalc.chez.com/lf/socket.html
 * - https://stackoverflow.com/questions/18277304/using-stdcout-in-multiple-threads (detail : PrintThread{})
 */

/*********
 *
 * CHECK FOR
 *
 *
 *
 * 	CONFIG_ASYNC_TCP_RUNNING_CORE =-1//any available core (take 0 !!!)
 * 	CONFIG_ARDUINO_UDP_RUNNING_CORE=1
 * 	CONFIG_ARDUINO_UDP_TASK_PRIORITY=3
 sudo iptraf-ng !!!!!!!!!!!!!
 sudo wireshark
 Rui Santos
 Complete project details at https://randomnerdtutorials.com
 eric@BazUbu:~$ date +%s
 1617182061
 eric@BazUbu:~$

 A METTRE dnans readme.md

 Verify the networking
 sudo lshw -class network -short && nmcli device status

 A PARTIR DE -70dB, IL Y A PERTE D'ACKNOLEDGE !!!!!!!!!!!


 En mode debug esp verbose,etc... limiter

 /media/DEV/ROB_000/rover/libraries/ESPAsyncWebServer-master/src/AsyncEventSource.h:
 24  #ifdef ESP32
 25  #include <AsyncTCP.h>
 26: #define SSE_MAX_QUEUED_MESSAGES 32
 27  #else
 28  #include <ESPAsyncTCP.h>
 29: #define SSE_MAX_QUEUED_MESSAGES 8
 30  #endif
 31  #include <ESPAsyncWebServer.h>

 Limiter la longueur aceptable en fifo javascript sachant que les messages de requête doivent avoir un message de réponse dans un temps acceptable
 De plus les queues de messages des librairies Async socket gèrent des allocations et des libérations mémoires sur le heap
 qui peuvent entrîner des ralentissement lors des réorganisations mémoire


 ---------------------------------------------------------------------------------
 | dbg lvl   			|	 0	|	3int this_DebugLVL
 | cnt char  			|	 0	|
 | modules 	 			|	 1	|
 ------------------------------------------------------------static---------------------
 | event 	 			|    6
 | queue 	 			|	 1
 | module 	MOT<>PILOT 			|	 1
 ---------------------------------------------------------------------------------
 | event 	 			|    6
 | queue 	 			|	 1
 | modules TMS+MOT*PILOT|	 1
 DebugLVL 3 sans devtools sur browser
 Saturation pour 2 fenêtres (1 firefox & 1 chrome)
 Pertes fifo JS (transactions/secondes
 1
 Débit wifi   8 Ko/s dans les deux sens
 80 = (8000 o/s)/(100o/Json/s) moitié query / moitié reply
 Soit 40 transactions / seconde

 ---------------------------------------------------------------------------------
 https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/generic-class.html
 https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/generic-examples.html
 https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/WiFiClientEvents/WiFiClientEvents.ino

 * WiFi Events
 0  SYSTEM_EVENT_WIFI_READY               < ESP32 WiFi ready
 1  SYSTEM_EVENT_SCAN_DONE                < ESP32 finish scanning AP
 2  SYSTEM_EVENT_STA_START                < ESP32 station start
 3  SYSTEM_EVENT_STA_STOP                 < ESP32 station stop
 4  SYSTEM_EVENT_STA_CONNECTED            < ESP32 station connected to AP
 5  SYSTEM_EVENT_STA_DISCONNECTED         < ESP32 station disconnected from AP
 6  SYSTEM_EVENT_STA_AUTHMODE_CHANGE      < the auth mode of AP connected by ESP32 station changed
 7  SYSTEM_EVENT_STA_GOT_IP               < ESP32 station got IP from connected AP
 8  SYSTEM_EVENT_STA_LOST_IP              < ESP32 station lost IP and the IP is reset to 0
 9  SYSTEM_EVENT_STA_WPS_ER_SUCCESS       < ESP32 station wps succeeds in enrollee mode
 10 SYSTEM_EVENT_STA_WPS_ER_FAILED        < ESP32 station wps fails in enrollee mode
 11 SYSTEM_EVENT_STA_WPS_ER_TIMEOUT       < ESP32 station wps timeout in enrollee mode
 12 SYSTEM_EVENT_STA_WPS_ER_PIN           < ESP32 station wps pin code in enrollee mode
 13 SYSTEM_EVENT_AP_START                 < ESP32 soft-AP start
 14 SYSTEM_EVENT_AP_STOP                  < ESP32 soft-AP stop
 15 SYSTEM_EVENT_AP_STACONNECTED          < a station connected to ESP32 soft-AP
 16 SYSTEM_EVENT_AP_STADISCONNECTED       < a station disconnected from ESP32 soft-AP
 17 SYSTEM_EVENT_AP_STAIPASSIGNED         < ESP32 soft-AP assign an IP to a connected station
 18 SYSTEM_EVENT_AP_PROBEREQRECVED        < Receive probe request packet in soft-AP interface
 19 SYSTEM_EVENT_GOT_IP6                  < ESP32 station or ap or ethernet interface v6IP addr is preferred
 20 SYSTEM_EVENT_ETH_START                < ESP32 ethernet start
 21 SYSTEM_EVENT_ETH_STOP                 < ESP32 ethernet stop
 22 SYSTEM_EVENT_ETH_CONNECTED            < ESP32 ethernet phy link up
 23 SYSTEM_EVENT_ETH_DISCONNECTED         < ESP32 ethernet phy link down
 24 SYSTEM_EVENT_ETH_GOT_IP               < ESP32 ethernet got IP from connected AP
 25 SYSTEM_EVENT_MAX


 void WiFiEvent(WiFiEvent_t event)
 {
 Serial.printf("[WiFi-event] event: %d\n", event);

 switch (event) {
 case SYSTEM_EVENT_WIFI_READY:
 Serial.println("WiFi interface ready");
 break;
 case SYSTEM_EVENT_SCAN_DONE:
 Serial.println("Completed scan for access points");
 break;
 case SYSTEM_EVENT_STA_START:
 Serial.println("WiFi client started");
 break;
 case SYSTEM_EVENT_STA_STOP:
 Serial.println("WiFi clients stopped");
 break;
 case SYSTEM_EVENT_STA_CONNECTED:
 Serial.println("Connected to access point");
 break;
 case SYSTEM_EVENT_STA_DISCONNECTED:
 Serial.println("Disconnected from WiFi access point");
 break;
 case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
 Serial.println("Authentication mode of access point has changed");
 break;
 case SYSTEM_EVENT_STA_GOT_IP:
 Serial.print("Obtained IP address: ");
 Serial.println(WiFi.localIP());
 break;
 case SYSTEM_EVENT_STA_LOST_IP:
 Serial.println("Lost IP address and IP address is reset to 0");
 break;
 case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
 Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
 break;
 case SYSTEM_EVENT_STA_WPS_ER_FAILED:
 Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
 break;
 case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
 Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
 break;
 case SYSTEM_EVENT_STA_WPS_ER_PIN:
 Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
 break;
 case SYSTEM_EVENT_AP_START:
 Serial.println("WiFi access point started");
 break;
 case SYSTEM_EVENT_AP_STOP:
 Serial.println("WiFi access point  stopped");
 break;
 case SYSTEM_EVENT_AP_STACONNECTED:
 Serial.println("Client connected");
 break;
 case SYSTEM_EVENT_AP_STADISCONNECTED:
 Serial.println("Client disconnected");
 break;
 case SYSTEM_EVENT_AP_STAIPASSIGNED:
 Serial.println("Assigned IP address to client");
 break;
 case SYSTEM_EVENT_AP_PROBEREQRECVED:
 Serial.println("Received probe request");
 break;
 case SYSTEM_EVENT_GOT_IP6:
 Serial.println("IPv6 is preferred");
 break;
 case SYSTEM_EVENT_ETH_START:
 Serial.println("Ethernet started");
 break;
 case SYSTEM_EVENT_ETH_STOP:
 Serial.println("Ethernet stopped");
 break;
 case SYSTEM_EVENT_ETH_CONNECTED:
 Serial.println("Ethernet connected");
 break;
 case SYSTEM_EVENT_ETH_DISCONNECTED:
 Serial.println("Ethernet disconnected");
 break;
 case SYSTEM_EVENT_ETH_GOT_IP:
 Serial.println("Obtained IP address");
 break;
 default: break;
 }}

 *********/
/*
 * DON'T send directly serialize in a wifi or serial stream because arduino json may make a packet by character
 https://arduinojson.org/v6/api/json/serializejson/
 See to serial or wifi & file slowing write
 */
//
#define DEF_ESP_WIFI_AP
// initial stack
int loop_Delay = 250;
char *stack_start;

#if !defined(__cplusplus)
#error >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>C++ compiler required.
#endif
/*
 #include "bib_view.h"
 remWorld remWorld0 *=new remWorld();
 */
//#include "esp_wifi.h"
//#include "time.h"
#include <ESPmDNS.h>
//#include "Esp.h"
#include "000_I2C.h"
#include "000_Rover_webServer.h"
//#include "rov-Mod_000_WS3.h"
#include "rov-Mod_Synchro_000_WS3.h"
#include "rov-Mod_Motors_000_WS3.h"
#include "rov-Mod_Jobgoal_000_WS3.h"
#include "rov-Mod_Watchdog_000_WS3.h"
#include "rov-Mod_Driver_000_WS3.h"
#include "rov-CfgHPP(000_WS3).hpp"
#include "rov-Kmds_000_WS3.h"
#include "000_WIFI.h"
#include "000_LOGS.h"
#include "000_WSOK.h"

struct asmALL {
	asmPWMs PWMs; // 	Motors PWM
	asmPIDs PIDs; //	Servos PID
	asmIMU IMU; //
	asmUSD USD; //
} asmALL;
//return
#define DEF_FACTORY_RESET // takes default contexts

/**
 * https://translate.google.com/translate?hl=fr&sl=en&u=https://www.freertos.org/FreeRTOS_Support_Forum_Archive/August_2017/freertos_Static_variables_and_Tasks_90861d42j.html&prev=search&pto=aue
 * Tout ce qui est global ou statique sera référencé par toutes les tâches.
 * Il y a donc un seul &this_DebugLVL pour tout le main !!!!
 * ce qui n'est "pas" gênant car main, setup (vs boot), loop, kmds, on_message et totes les fonctions
 * ci dessous ... (à vérifier!!!!)
 */
int8_t &this_DebugLVL = *registerFCT(e_tasks::MAIN, "Main", &CTX.SOFTctx.FCT_DBGMAX_MAIN);

// LED pin is IO2 ???

int vio7PIN_Serial = 13; // _DEBUG_ Serial extern...
int blu6PIN_Logs = 12; // _LOG_ extern...
#define gre5PIN_Sudp 14
#define yel4PIN_ 27
#define ora3PIN_imu 16
#define red2PIN_modules  17
#define bro1PIN_Vrcv 25
#define whi0PIN_Wsnd  26
///////////////////////////////////////////////////////////////////////
String BDOOR_protocol; // PATCH sur la bib asynchrone !!!
uint64_t dt_modules = 0;
uint64_t dt_event = 0;
uint64_t dt_queue = 0;
///////////////////////////////////////////////////////////////////////
bool imuFlag = false;
uint64_t isr_microTimeStamp = 0;
///////////////////////////////////////////////////////////////////////
t_map_QSRC sourceMap = {
	{ DEF_QUERY_BASIS_TO_ROVER, { reply2query_BASIS, nullptr, 123 } },
	{ DEF_REPLY_BASIS_TO_ROVER, { ackno2reply_BASIS, nullptr, 123 } },
	{ DEF_REPLY_PILOT_TO_ROVER, { ackno2reply_PILOT, nullptr, 123 } },
	{ DEF_QUERY_PILOT_TO_ROVER, { reply2query_PILOT, nullptr, 456 } },
};
Flow *qFlow;
extern t_map_MOD modulesMap;
TaskHandle_t core_HDL;
TaskHandle_t zlog_HDL;
TaskHandle_t ylog_HDL;
TaskHandle_t ximu_HDL;
TaskHandle_t wsnd_HDL;
TaskHandle_t udpl_HDL;
TaskHandle_t onEvent;

/*
 const char* ssid = "rover";
 const char* password =  "0123456789";
 */
AsyncWebServer server(80);
AsyncWebSocket webSokSrv("/ws");
JsonObject *jsonPtr;
//Internals::StaticJsonB/media/DEV/ROB_000/setup/SET_000_WS3/rov_000_WS3.hppufferBase * _jsonBuffer;
//int outMSGqueueSIZE = 5;
int inMSGqueueSIZE = 10;
QueueHandle_t inMSGqueue;
int inKMDSqueueSIZE = 10;
QueueHandle_t inKMDSqueue;
//QueueHandle_t outMSGqueue;
int imuDATAqueueSIZE = 40;
QueueHandle_t _IMUqueue;
char ptrTaskList[250];
/*
 const char* ntpServer = "pool.ntp.org";
 const long  gmtOffset_sec = 3600;
 const int   daylightOffset_sec = 3600;
 */
String msg = "";
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//void vTaskGetRunTimeStats(char *buffer);
///////////////////////////////////////////////////////////////////////
typedef struct {
	s_wsChecker *PWSX;
	char msgJSON[200];
} s_itemWS;
/*
 typedef enum{
 udp,
 ws,
 serial,
 spiffs,
 }e_msgPro;
 typedef struct {
 e_msgPro msgPro; // Discriminant
 union {
 s_itemWS itemWS;
 s_kmdsMSG itemUDP;
 };
 } t_datagram;
 */
///////////////////////////////////////////////////////////////////////
void onWSMessage(AsyncWebSocket *server, AsyncWebSocketClient *client,
	AwsEventType type, void *arg, uint8_t *data, size_t len){
	static const char *class_Type = "RCV";
	static int8_t &this_DebugLVL = *registerFCT(e_tasks::VRCV, "Vrcv",
		&CTX.SOFTctx.TSK_DBGMAX_RECV, DEF_PRIO_VRCV);
	static uint32_t _call_ = 0;
	_call_++;
	uint64_t start_event = milli_TS();
	digitalWrite(bro1PIN_Vrcv, HIGH);
	_SERIAL_7("\n%s@@@@%s", WWW().c_str(),
		dumpWSX(
			"BEGIN onWSMessage " + String(client->id()) + String(", ")
				+ SEVT(type) + ", " + SSTA(client->status())).c_str());
	//
	//	CONNECT (creates a new PWX or close if MAX connections---------
	//
	if (type == WS_EVT_CONNECT) {
		_SERIAL_0("\n@@@@ FOUND");
		s_wsChecker *pwsx = openCNX(type, client, BDOOR_protocol);
		_SERIAL_0("\n%s", WWW(pwsx).c_str());
		if (pwsx != nullptr) {
			client->text(
				"ROVER client " + String(client->id())
					+ " connected :-) protocol=`" + BDOOR_protocol
					+ "`");
			client->ping();
		} else {
			client->close(1011, "MAX connections, no more available");
		}
		return digitalWrite(bro1PIN_Vrcv, LOW);
	}
	//
	//	FIND PWX  ---------------------------------------------
	//
	bool msgToqueue = false;
	s_wsChecker *pwsx = getWSXclient(client);
	if (pwsx == nullptr) {
		_DEBUG_ERR("\n%s@@@@ ERROR !!!!!!!!!!!!!!!! pwsx NOT FOUND !!!\n",
			WWW().c_str());
		return digitalWrite(bro1PIN_Vrcv, LOW);
	}
	msg = WWW(pwsx); // Old pwsx just for debug !!! msg="" later...
	pwsx->status = client->status();
	pwsx->event = type;
	_SERIAL_WSXCHANGE(s_wsChecker::RCV, pwsx, _call_);
	//
	//	DISCONNECT ----------------------------------------------------
	//
	if (type == WS_EVT_DISCONNECT) {
		//vTaskSuspendAll();
		pwsx->clientID *= -1; // Urgentissime
		USE_SERIAL.printf("\nId[%u] disconnect", client->id());
		pwsx->erase(type, client->status());
		server->cleanupClients();
//		client->close();
		//if (!xTaskResumeAll()) {	//taskYIELD();		}
	}
	//
	//	ERROR  --------------------------------------------------------
	//
	else if (type == WS_EVT_ERROR) {
		pwsx->clientID *= -1; // Urgentissime
		USE_SERIAL.printf("\nId(%u) error(%u):", client->id(),
			*((uint16_t*)arg));
//			USE_SERIAL.printf("\nId(%u) error(%u): %s", client->id(), *((uint16_t*) arg),(char*) data);
		pwsx->erase(type, client->status());
		client->close();
		server->cleanupClients();
	}
	//
	//	PONG  ---------------------------------------------------------
	//
	else if (type == WS_EVT_PONG) {
		USE_SERIAL.printf("\nId(%u) pong[%u]: %s", client->id(), len,
			((len) ? (char*)data : "no data"));
	}
	//
	//	DATA ----------------------------------------------------------
	//
	else if (type == WS_EVT_DATA) {
		AwsFrameInfo *info = (AwsFrameInfo*)arg;
		//
		//  The whole message is in a single frame and we got all of it's data
		//  ------------------------------------------------------------------
		if (info->final && info->index == 0 && info->len == len) {
			msg = "";
			//_SERIAL_1("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);
			if (info->opcode == WS_TEXT) {
				for (size_t i = 0; i < info->len; i++) {
					msg += (char)data[i];
				}
			} else {
				char buff[3];
				for (size_t i = 0; i < info->len; i++) {
					sprintf(buff, "%02x ", (uint8_t)data[i]);
					msg += buff;
				}
			}
			/*
			 ///////// ATTENTION client->text prend 50 ms  !!!!!!!!!!!!!
			 if(info->opcode == WS_TEXT) client->text(String(cntQUERIES)+" I got your SINGLE text message");
			 else client->binary("I got your binary message");
			 */
//      p wsx->msgAvailable=true;
			if (msg == "FIFO_FULL") {
				_DEBUG_ERR("\n%s <===[%s]##############",
					WWW(pwsx).c_str(), msg.c_str());
//				webSokSrv.close(client->id(),4001,"blabla");
				/*
				 pwsx->erase(type,client->status());
				 client->close(4001,"blabla");//4001,"blabla"
				 */
				return digitalWrite(bro1PIN_Vrcv, LOW);
			} else {
				msgToqueue = true;
			}

		} // end of single
		  //
		  //  message is comprised of multiple kfs or the frame is split into multiple packets
		  //  -----------------------------------------------------------------------------------
		else {
			if (info->index == 0) {
				if (info->num == 0) {
					msg = "";
					//_SERIAL_1("\nws[%s][%u] %s-message start", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
				}
				//_SERIAL_1("\nws[%s][%u] frame[%u] start[%llu]", server->url(), client->id(), info->num, info->len);
			}
			//_SERIAL_1("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);
			if (info->opcode == WS_TEXT) {
				for (size_t i = 0; i < info->len; i++) {
					msg += (char)data[i];
				}
			} else {
				char buff[3];
				for (size_t i = 0; i < info->len; i++) {
					sprintf(buff, "%02x ", (uint8_t)data[i]);
					msg += buff;
				}
			}
			if ((info->index + len) == info->len) {
				//_SERIAL_1("\nws[%s][%u] frame[%u] end[%llu]", server->url(), client->id(), info->num, info->len);
				if (info->final) {
					//_SERIAL_1("\nws[%s][%u] %s-message end", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
					if (info->message_opcode != WS_TEXT)
						client->binary("I got your binary message");
					//         pw sx->msgAvailable=true;
					msgToqueue = true;
				}
			}
		} // end of multiple
//    _SERIAL_1("\n   [%s]",msg.c_str());
		if (msgToqueue) {
			_SERIAL_WSXDATA("<≈", msg.c_str());
			s_itemWS recItem;
			recItem.PWSX = pwsx;
			//recItem.checker = *pwsx;
			strcpy(recItem.msgJSON, msg.c_str());
			msgToqueue = false;
			try {
				if (pdPASS != xQueueSend(inMSGqueue, &recItem, 0))
					THROWERR(FIFO_OVERFLOW, rand() % 2 + 5, micro_TS());
			} catch (EXCEPT const &e) {
				FIX_ERROR();
			}
		}
	} // end if WS_EVT_DATA
	else {
		_SERIAL_1("*");
	}
	dt_event = milli_TS() - start_event;
	digitalWrite(bro1PIN_Vrcv, LOW);
}
///////////////////////////////////////////////////////////////////////
/*
 Traite les messages en attente pour tous les clients en W SX
 Order
 Answer
 Debug, Log
 ...
 */
/**
 * @fn void core_TASK(void *pvParameters)
 * @param void * pvParameters to get its tskInfo
 */
void core_TASK(void *pvParameters){
	s_tskInfo_X &tskInfo = *(reinterpret_cast<s_tskInfo_X*>(pvParameters));
	int8_t & this_DebugLVL = *(tskInfo.DebugLVL);
	for (;;) {
		//digitalWrite(red2PIN_modules, HIGH);
		uint64_t start_queue = milli_TS();
		/*
		 * ==========================================
		 * 1) Traiter les requêtes ou réponses reçues
		 * ------------------------------------------
		 */
		s_itemWS item;
		if (xQueueReceive(inMSGqueue, &item, 0) == pdPASS) {
			digitalWrite(DEF_BLUE_LED, HIGH);
			try {
				extern t_jsonDoc *_jsonDoc;
				String keeper = String((char*)item.msgJSON);
				// détruit la source which must stay available during _sonDoc used !!!)
				if (0 != deserializeJson(*_jsonDoc, (char*)item.msgJSON))
					THROWERR(JSON_ERR_DESERIALIZE, (char* )item.msgJSON);
				JsonObject root = (*_jsonDoc).as<JsonObject>(); // keep _jsonDoc of course...
				String header = Header(+"POPF " + root["KMD"]["MID"].as<String>() + " << ",
					Remote(item.PWSX->protocol[0]));
				//_SERIAL_3( "\n%s%s", header.c_str(), keeper.c_str());

				//--------------------------------------------------
				//	1)  TRAITER LA FILE D'ATTENTE DE RECEPTION SOCKET
				//------------------------------------------------
				//const char * x = root["CTL"]["TYP"].as<string>().c_str();
				/**
				 *  itDisps->second.task returns true if root argument is a query
				 *  then root is modified to send the reply
				 *  But when receiving reply could also need a new query from the rover
				 */
				if (findFCT(root["CTL"]["TYP"].as<string>().c_str())(root)) {
					myWSOK.sendJSON(item.PWSX, root["KMD"]["MID"].as<String>());
				}
				delay(1);
			} catch (EXCEPT const &e) {
				FIX_ERROR();
			}
		}
		else {
			/**< @todo what to do if fifo empty: nothing ??? */
		}
		dt_queue = milli_TS() - start_queue;
		/*
		 * ==========================================
		 * 2) Envoyer des requêtes aux sockets actives suivant le timing exigé !!!
		 * ------------------------------------------
		 * plusieurs requêtes sur la même socket peuvent avoir des timing differents!!!!
		 * on est dans le cas simple !!!!!!!!!!!
		 * Déclarer les modules les plus rapides en tête dans le vecteur des modules pour
		 * les traiter en priorité
		 */
		uint64_t start_modules = milli_TS();
		//vTaskDelay( x Delay );
		try {
			for (auto itMod = modulesMap.begin(); itMod != modulesMap.end(); ++itMod) {
				Module &module = *(itMod->second); //*MODULES[m];
				//_SERIAL_0( "\n[%s]", module._Mid.c_str());
//				if (false/*module.has_MSG()*/) {
				if (module.has_MSG()) {
					digitalWrite(DEF_BLUE_LED, HIGH);
					digitalWrite(red2PIN_modules, HIGH);
					//_SERIAL_0( "\n%s %s",WWW().c_str(),module.dump("").c_str());
					bool found = false;
					for (int i = 0; i < wsMax; i++) {
						s_wsChecker *pwsx = getWSX(i);
						//_SERIAL_0( " [%s:%i]", pwsx->reversep.c_str(), pwsx->status);
						String explain;
						if ((pwsx->client != NULL)
							&& (pwsx->status == WS_CONNECTED)
							&& (module._protocol == pwsx->reversep)) {
							found = true;
							//module.tmsout_MSG(); // May be sent on an other socket !!!
							myWSOK.sendJSON(pwsx, module._Mid);
							//break;
							break;
						}
					}
					if (!found) {
						extern t_jsonDoc *_jsonDoc;
						String header = "!!!! " + module._Mid + Header(" -> ",
							Remote((*_jsonDoc).as<JsonObject>()["CTL"]["TYP"].as<String>()[2]));
//						_DBG_APP_BOOT("\n%s %s", header.c_str(), dumpJSON().c_str());
						_DBG_TSK_ROOT("\n%s No pilot connected", header.c_str());
					}
					digitalWrite(red2PIN_modules, LOW);
				}
			}
		} catch (EXCEPT const &e) {
			FIX_ERROR();
		}
		//END_MODULES:
		dt_modules = milli_TS() - start_modules;
		vTaskDelay(10);
		digitalWrite(DEF_BLUE_LED, LOW);
//		digitalWrite(red2PIN_modules, LOW);
	}
}
///////////////////////////////////////////////////////////////////////
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
	int8_t & this_DebugLVL = *(tskInfo.DebugLVL);
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
///////////////////////////////////////////////////////////////////////
/**
 * @fn TASK void wsnd_TASK(void * pvParameters)
 * @brief task handling messages to send via websocket
 *
 * This task waits indefinitely to be notified by a sender function (itself in an other task)
 * When notification arrives, ulNotifiedValue is the item to send & it runs \ref SND_DOIT.
 * Finally it notify the caller task with a status included in the item
 * @param void * pvParameters to get its tskInfo
 */
void wsnd_TASK(void *pvParameters){
	s_tskInfo_X &tskInfo = *(reinterpret_cast<s_tskInfo_X*>(pvParameters));
	int8_t & this_DebugLVL = *(tskInfo.DebugLVL);
	for (;;) {
		s_OUTfifoItem item;
		while (myWSOK.pull(&item)) {
			try {
				digitalWrite(whi0PIN_Wsnd, HIGH);
				if ((item.pwsx->clientID <= 0) || (item.pwsx->status != WS_CONNECTED))
					THROWERR(ERR_SND_NOTCONNECT, item.pwsx->dumpShort(s_wsChecker::SND).c_str());
				if (!item.pwsx->client->canSend())
					THROWERR(WS_ERR_CANTSEND, item.pwsx->dumpShort(s_wsChecker::SND).c_str());
				item.pwsx->client->text((char*)item.outMSG/* + ITM.SCBlen*/); //by server better No...)
				//webSokSrv(ITM.pwsx->clientID,(char *) &ITM.outMSG[0] + ITM.SCBlen);
				item.pwsx->event = 99; // Specific for send
				_SERIAL_WSXCHANGE(s_wsChecker::SND, item.pwsx, item.msgID);
				delete[] item.outMSG; // Make an ack fifo to every time delete in the same task than new !!!!
				digitalWrite(whi0PIN_Wsnd, LOW);
				vTaskDelay(10); //think about webSocket server to send... (10ms => 100 Hz)
			} catch (EXCEPT const &e) {
				delete[] item.outMSG;
				FIX_ERROR();
			}
		}
		vTaskDelay(10); //think about other lower priorities tasks timing...
	}
	/*
	 for (;;) {
	 //	WAIT FOR NOTIFICATION
	 xTaskNotifyWait(0x00, ULONG_MAX, &ulNotifiedValue, portMAX_DELAY); // Block indefinitely.
	 digitalWrite(whi0PIN_Wsnd, HIGH);
	 // SEND THE WS MESSAGE
	 s_OUTfifoItem &ITM = *((s_OUTfifoItem*)ulNotifiedValue);
	 ITM.pwsx->event = 99; // Specific for send
	 _SERIAL_WSXCHANGE(s_wsChecker::SND, ITM.pwsx, ITM.msgID);
	 SND_DOIT(ITM); // to stop IMU interrupt ???

	 while (pdTRUE != xTaskNotify((TaskHandle_t)ITM.tsk, (uint32_t)xTaskGetCurrentTaskHandle(),
	 eSetValueWithoutOverwrite)) {
	 USE_SERIAL.printf("£");
	 vTaskDelay(1);
	 }
	 //vTaskDelay(3);//????????????????????????????????????????
	 digitalWrite(whi0PIN_Wsnd, LOW);
	 }
	 */
}
///////////////////////////////////////////////////////////////////////
/// I M U   O N   I 2 C   &   S I M U L A T I O N
///////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
/**
 * @fn void iimu_TASK(void *pvParameters)
 * @brief Task handling IMU data get from I²C \ref iimu_ISR interrupt
 *
 * Push IMU real data in \ref _IMUqueue each time an ISR notification is received
 * and reads I²C ultrasonic sensor once in 10 times
 * @param void * pvParameters to get its tskInfo & module Driver reference
 */
void iimu_TASK(void *pvParameters){
	s_tskInfo_X &tskInfo = *(reinterpret_cast<s_tskInfo_X*>(pvParameters));
	int8_t & this_DebugLVL = *(tskInfo.DebugLVL);
	Driver &_driver = *reinterpret_cast<Driver*>(tskInfo.Param);
//_SERIAL_0( "\n\tModule idM= '%s'", _driver->_Mid.c_str());
// Initialise the xLastWakeTime variable with the current time.
	TickType_t xLastWakeTime = xTaskGetTickCount();
//const TickType_t xFrequency = 6;
	uint32_t ulInterruptStatus;
	for (;;) {
		xTaskNotifyWait(0x00, ULONG_MAX, &ulInterruptStatus, portMAX_DELAY); // Bloquer indéfiniment.
		if (imuFlag == true) { // On interrupt, read data
			imuFlag = false; // reset n/media/DEV/ROB_000/setup/SET_000_WS3/rov-Mod_000_WS3.cppewData flag
			digitalWrite(ora3PIN_imu, HIGH);
			_driver.acq_trueIMU(isr_microTimeStamp);
			digitalWrite(ora3PIN_imu, LOW);
			delayMicroseconds(50); // Bitscope 1% 100µs/10ms
			digitalWrite(ora3PIN_imu, HIGH);
			_driver.push_RTdata();
			delayMicroseconds(100); // Bitscope 1% 100µs/10ms
			digitalWrite(ora3PIN_imu, LOW);
		}
	}
}
//---------------------------------------------------------------------
/**
 * @fn void simu_TASK(void *pvParameters)
 * @brief Task handling IMU data simulation
 *
 * Push IMU simulated data in \ref _IMUqueue at 100 Hz
 * and reads I²C ultrasonic sensor once in 10 times
 * \n This task may be useful to verify the kinematic model of rover or to pilot it from IMU calculated data
 * \n WARNING: 	isr_microTimeStamp will be overide by simulation MTS
 * @param void * pvParameters to get its tskInfo & module Driver reference
 */
void simu_TASK(void *pvParameters){
	s_tskInfo_X &tskInfo = *(reinterpret_cast<s_tskInfo_X*>(pvParameters));
	int8_t & this_DebugLVL = *(tskInfo.DebugLVL);
	Driver &_driver = *reinterpret_cast<Driver*>(tskInfo.Param);
	const TickType_t xFrequency = 5100; /**< @todo Adapt with simu sample period !!! */
	TickType_t xLastWakeTime = xTaskGetTickCount(); // Initialize the xLastWakeTime variable with the current time.
	for (;;) {
		vTaskDelayUntil(&xLastWakeTime, xFrequency); // Attend le cycle suivant.
		digitalWrite(ora3PIN_imu, HIGH);
		_driver.acq_simuIMU();
		digitalWrite(ora3PIN_imu, LOW);
		delayMicroseconds(50); // Bitscope 1% 100µs/10ms
		digitalWrite(ora3PIN_imu, HIGH);
		_driver.push_RTdata();
		delayMicroseconds(100); // Bitscope 1% 100µs/10ms
		digitalWrite(ora3PIN_imu, LOW);
	}
}
/*
 //---------------------------------------------------------------------
 void attach_ISR(){
 Serial.printf("\nATTACHING ISR...");
 attachInterrupt(hardPIN_isr, iimu_ISR, RISING); // define interrupt for intPin output of MPU9250
 }
 //---------------------------------------------------------------------
 void detach_ISR(){
 detachInterrupt(hardPIN_isr); // define interrupt for intPin output of MPU9250
 Serial.printf("\nDETACHING ISR...");
 }
 */
///////////////////////////////////////////////////////////////////////
/// L O G   T E S T
///////////////////////////////////////////////////////////////////////
/**
 * @fn void ylog_TASK(void *pvParameters)
 * @brief Task testing webSocket log
 * @param void * pvParameters to get its tskInfo
 */
void ylog_TASK(void *pvParameters){
	s_tskInfo_X &tskInfo = *(reinterpret_cast<s_tskInfo_X*>(pvParameters));
	int8_t & this_DebugLVL = *(tskInfo.DebugLVL);
	tskInfo.scratch = rtnThrow(TEST_INFO, __FUNCTION__, __LINE__, __FILE__);
	const TickType_t xFrequency = 100;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for (;;) {
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		_DBG_APP_BOOT("\nYlog_TASK %15f8 s", micro_TS() / 1000000.0);
		if (myUDP.isOpen())
			for (size_t i = 0; i < 3; i++)
				_DBG_APP_BOOT(
					"**************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			******************************************************************** \
			");
	}
}
/**
 * @fn void zlog_TASK(void *pvParameters)
 * @brief Task testing webSocket log
 * @param void * pvParameters to get its tskInfo
 */
void zlog_TASK(void *pvParameters){
	s_tskInfo_X &tskInfo = *(reinterpret_cast<s_tskInfo_X*>(pvParameters));
	int8_t & this_DebugLVL = *(tskInfo.DebugLVL);
	const TickType_t xFrequency = 201;
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for (;;) {
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		_DBG_APP_BOOT("\nZlog_TASK %15f8 s", micro_TS() / 1000000.0);
		if (myUDP.isOpen())
			for (size_t i = 0; i < 3; i++)
				_DBG_APP_BOOT(
					"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \
			");
	}
}
///////////////////////////////////////////////////////////////////////
/// 	S E T U P
///////////////////////////////////////////////////////////////////////
/**
 * @fn void setup()
 * @brief Boot of application
 *
 *
 * @n Boot declare REPORT buffer report
 * @n Boot def  MTMS time		(SERIAL  possible
 * @n Boot def SERIAL   Serial          (SERIAL English
 * @n Boot EEPROM CONTEXT (0,1 or 2) bool avec sortie sur buffer report
 * @n Boot ctx SERIAL   Serial          (SERIAL English
 * @n Boot ctx MTMS time		(SERIAL  possible
 * @n Boot SPIFFS          (SERIAL English
 * @n Boot LSTR Lang       (SERIAL Start in English then continue in Localized STRing
 * @n Boot WIFI	 (AP or STA)		(SERIAL localized
 * @n Boot DATAG(fifo flow) & IMU_queue & inMSGqueue
 * @n Boot SUDP			(SERIAL localized
 * @n Boot LOGS  cvs & telemetry log serial to udp start boot report
 * 			@n Boot wsnd_HDL
 * @n Boot display of system informations
 * @n Boot Esp32 pins
 * @n Boot Bitscop pins test
 * @n Boot Webserver & Websocket server
 * @n Boot I2C
 * @n Boot inMSGqueue modules
 * @n Boot inMSGqueue tasks
 *
 */
void setup(){
	extern uint64_t tmsOffset;
	tmsOffset = 1601299940009;
// init record of stack
	char stack;
	stack_start = &stack;
	Serial.begin(115200);
//USE_SERIAL.printf("\n** this_DebugLVL '%i'\n",this_DebugLVL);
	_DBG_APP_BOOT("\n#### ROVER starts at '%s'", datim(micro_TS(), 2).c_str());
	int8_t & this_DebugLVL = *registerFCT(e_tasks::BOOT, "Boot", &CTX.SOFTctx.FCT_DBGMAX_BOOT);
//USE_SERIAL.printf("\n** this_DebugLVL '%i'\n",this_DebugLVL);
//Serial.println(datim(1617182061001001,2));
// SPIFFS =============================================================
	_DBG_APP_BOOT("\n#### SPIFFS ");
	if (!SPIFFS.begin()) {
		_DBG_APP_BOOT("FAILS while mounting");
		return;
	} else
		_DBG_APP_BOOT("started");
	Lang.listDir_Old(SPIFFS, "/", 0);
// LangSTR =============================================================
	Lang.Begin("/rov-Sen_000_WS3(french).csv");
	/*
	 char c;
	 c=inKey();
	 c=inKey("Entrez un caractère");
	 c=inKey();
	 c=inKey("Entrez un caractère");
	 */
	_SERIAL_0("\nUTF8 support:");
	_SERIAL_0(
		"\nLook correct:{↡↠↣⇫ε⇪∆∇≚∞⋀🇼⟰⟱⚺⚻▴⑬⋁ᗄᗐထܠݎݏ߇߈ƧɅ±∇¤øƟɅɣʬΔΛϪᴓᴧᵛ†‼∆∩⌂▬►◄◊¥Vv□ΥѲ˅˄▲▼ꜛꜜ◊↑↓↕↨ˠØ}\n");
	_SERIAL_0("\nSeem hazardous:{⮛⮟⮝   ߇߈  ⮊}\n");
	_DBG_APP_BOOT(Lang.CST(KERN_), Lang.CST(____LINE_40_EQUAL____));
	_DBG_APP_BOOT(Lang.CST(KERN_CPP), __cplusplus);
	_DBG_APP_BOOT(Lang.CST(KERN_ARDUINOJSON_USE_LONG_LONG),
		ARDUINOJSON_USE_LONG_LONG);
	_DBG_APP_BOOT(Lang.CST(KERN_sizeof_t_jsonDoc), sizeof(t_jsonDoc));
	_DBG_APP_BOOT(Lang.CST(KERN_WS_MAX_QUEUED_MESSAGES), WS_MAX_QUEUED_MESSAGES);
	_DBG_APP_BOOT(Lang.CST(KERN_configUSE_TRACE_FACILITY),
		configUSE_TRACE_FACILITY);
	_DBG_APP_BOOT(Lang.CST(KERN_configUSE_STATS_FORMATTING_FUNCTIONS),
		this_DebugLVL, configUSE_STATS_FORMATTING_FUNCTIONS);
//INCLUDE_uxTaskPriorityGet
	_DBG_APP_BOOT(Lang.CST(KERN_INCLUDE_vTaskDelay), INCLUDE_vTaskDelay);
	_DBG_APP_BOOT(Lang.CST(KERN_portTICK_PERIOD_MS), portTICK_PERIOD_MS);
	_DBG_APP_BOOT(Lang.CST(KERN_ESP_TASKS));
	_DBG_APP_BOOT(Lang.CST(KERN_ESP_TASK_PRIO_MAX), ESP_TASK_PRIO_MAX);
	_DBG_APP_BOOT(Lang.CST(KERN_ESP_TASK_TIMER_PRIO), ESP_TASK_TIMER_PRIO,
		ESP_TASK_TIMER_STACK);
	_DBG_APP_BOOT(Lang.CST(KERN_ESP_TASKD_EVENT_PRIOevent), ESP_TASKD_EVENT_PRIO,
		ESP_TASKD_EVENT_STACK);
	_DBG_APP_BOOT(Lang.CST(KERN_ESP_TASK_TCPIP_PRIO), ESP_TASK_TCPIP_PRIO,
		ESP_TASK_TCPIP_STACK);
	_DBG_APP_BOOT(Lang.CST(KERN_ESP_TASK_MAIN_PRIO), ESP_TASK_MAIN_PRIO,
		ESP_TASK_MAIN_STACK);
// EEPROM CONTEXT =====================================================
	_DBG_APP_BOOT(Lang.CST(EEPROM_), Lang.CST(____LINE_40_EQUAL____));
	bool ee = true;
	ee = ee & Ebegin(CTX.E_BOOT0, "BOOT 0");
	ee = ee & Ebegin(CTX.E_SOFT1, "SOFT 1");
	ee = ee & Ebegin(CTX.E_SOFT2, "SOFT 2");
	ee = ee & Ebegin(CTX.E_HARD1, "HARD 1");
	ee = ee & Ebegin(CTX.E_HARD2, "HARD 2");
	if (!ee) {
		_DBG_APP_BOOT("\n  FAILS to start");
	} else {
		_DBG_APP_BOOT("\n  Started");
#ifdef DEF_FACTORY_RESET
		Esave(CTX.E_BOOT0, CTX.BOOTctx);
		Esave(CTX.E_SOFT1, CTX.SOFTctx);
		Esave(CTX.E_SOFT2, CTX.SOFTctx);
		Esave(CTX.E_HARD1, CTX.HARDctx);
		Esave(CTX.E_HARD2, CTX.HARDctx);
		_DBG_APP_BOOT(" with default %s", CTX.dumpBOOTctx().c_str());
#else
		Eload(CTX.E_BOOT0, &CTX.BOOTctx);
		_DBG_APP_BOOT( "with EEPROM %s", CTX.dumpBOOTctx().c_str());
		if (CTX.BOOTctx.SOFT == 1)
		Eload(CTX.E_SOFT1, &CTX.SOFTctx);
		else if (CTX.BOOTctx.SOFT == 2)
		Eload(CTX.E_SOFT2, &CTX.SOFTctx);
		else
		_SERIAL_0( "\nKEEP DEFAULT CONTEXT");
		if (CTX.BOOTctx.HARD == 1)
		Eload(CTX.E_HARD1, &CTX.HARDctx);
		else if (CTX.BOOTctx.HARD == 2)
		Eload(CTX.E_HARD2, &CTX.HARDctx);
		else
		_SERIAL_0( "\nKEEP DEFAULT SENSORS");
#endif
		_DBG_APP_BOOT("\n%s", CTX.dumpHARDctx().c_str());
		_DBG_APP_BOOT("\n%s", CTX.dumpSOFTctx().c_str());
	}
//	if(!ee){delay(1000);ESP.restart();}
	/*
	 uint64_t tms; //
	 //tms = 1617182061000000;E_CALIMU.put(0, tms);	E_CALIMU.commit();tms = 0;
	 E_CALIMU.get(0, tms);
	 _SERIAL_0( Lang.CST(EEPROM_CALIMU_OK), datim(tms).c_str(), tms);
	 */
// WIFI setup ====================================================
	_DBG_APP_BOOT("\n==== WIFI ========================================");
	/**
	 * https://github.com/espressif/arduino-esp32/blob/master/tools/sdk/include/esp32/esp_event_legacy.h
	 */
	WiFiEvents (WiFi);
#ifdef DEF_ESP_WIFI_AP
	const char *ssid = "rover";
	const char *password = "0123456789";
	WiFi.softAP(ssid, password, 3); //,1);//,0,3);
	_DBG_APP_BOOT("\n\t Access Point ssid='%s', pswd='%s'", ssid, password);
	;
	delay(2000); // To find channel, etc...
//while (WiFi.softAPIP() == IPAddress(0, 0, 0, 0))_SERIAL_3( "\n\t'%s' : Access Point IP", WiFi.softAPIP().toString().c_str());
	/*
	 if (MDNS.begin("esp32")) {
	 _DBG_APP_BOOT(" & MDNS responder started");
	 }
	 int8_t power;
	 esp_wifi_get_max_tx_power(&power);
	 _DBG_APP_BOOT(Lang.CST(WIFI_POWER), power);
	 */
//esp_wifi_set_max_tx_power(power+1);esp_wifi_get_max_tx_power(&power);_SERIAL_3( Lang.CST(WIFI_POWER), power);
#else
	const char* ssid = "bdoor";
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
	/*
	 //init and get the time
	 configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
	 delay(1000);
	 struct tm timeinfo;
	 if(!getLocalTime(&timeinfo)){
	 Serial.println("Failed to obtain time");
	 //	     return;
	 }
	 Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
	 */
// UDP listener ====================================================
	_DBG_APP_BOOT("\n==== UDP  ========================================");
	inKMDSqueue = xQueueCreate(inKMDSqueueSIZE, sizeof(s_kmdsMSG));
	if (myUDP.Begin(DEF_UDP_PORT, &inKMDSqueue)) {
		_DBG_APP_BOOT("\n\t UDPL init OK");
//	==== TASK SUDP ============================================
#define DEF_TASK_SEND_UDP
#ifdef DEF_TASK_SEND_UDP
		launchTASK(e_tasks::SUDP, udpl_HDL, "Sudp", &sudp_TASK, nullptr,
			&CTX.SOFTctx.TSK_DBGMAX_UDPL, 5000, 1, DEF_PRIO_SUDP);
#endif
		delay(100);
	}
	else
		_DBG_APP_BOOT("\n\t UDPL init error");
// WSOK sender ====================================================
	if (myWSOK.Begin()) {
		_DBG_APP_BOOT("\n\t WSOK init OK");
		// TASK MESSAGE SENDER VIA WEBSOCKET ==================================
#define DEF_TASK_SEND_WS
#ifdef DEF_TASK_SEND_WS
		launchTASK(e_tasks::WSND, wsnd_HDL, "Wsnd", &wsnd_TASK, nullptr,
			&CTX.SOFTctx.TSK_DBGMAX_SEND, 7000, 0, DEF_PRIO_WSND);
#endif
		delay(100);
	}
	else
		_DBG_APP_BOOT("\n\t WSOK init error");
// PINS configuration ====================================================
	_SERIAL_3(Lang.CST(PINS_LED), Lang.CST(____LINE_40_EQUAL____));
	_SERIAL_3(Lang.CST(PINS_ISRIMU), c_linkISR::pin());
	pinMode(c_linkISR::pin(), INPUT); //_PULLUP ? _PULLDOWN? INPUT SOFT INTERRUPT RISING UP !!!
	_SERIAL_3(Lang.CST(PINS_OUTTEST), DEF_BLUE_LED);
	digitalWrite(DEF_BLUE_LED, HIGH);
	pinMode(whi0PIN_Wsnd, OUTPUT);
	pinMode(red2PIN_modules, OUTPUT);
	pinMode(bro1PIN_Vrcv, OUTPUT);
	pinMode(ora3PIN_imu, OUTPUT);
	pinMode(yel4PIN_, OUTPUT);
	pinMode(gre5PIN_Sudp, OUTPUT);
	pinMode(blu6PIN_Logs, OUTPUT);
	pinMode(vio7PIN_Serial, OUTPUT);
	pinMode(DEF_BLUE_LED, OUTPUT);
// BITSCOPE checking ====================================================
	for (size_t i = 0; i < 300; i++) {
		digitalWrite(whi0PIN_Wsnd, HIGH);
		delay(1);
		digitalWrite(bro1PIN_Vrcv, HIGH);
		delay(1);
		digitalWrite(red2PIN_modules, HIGH);
		delay(1);
		digitalWrite(ora3PIN_imu, HIGH);
		delay(1);
		digitalWrite(yel4PIN_, HIGH);
		delay(1);
		digitalWrite(gre5PIN_Sudp, HIGH);
		delay(1);
		digitalWrite(blu6PIN_Logs, HIGH);
		delay(1);
		digitalWrite(vio7PIN_Serial, HIGH);
		delay(1);
		digitalWrite(whi0PIN_Wsnd, LOW);
		delay(1);
		digitalWrite(bro1PIN_Vrcv, LOW);
		delay(1);
		digitalWrite(red2PIN_modules, LOW);
		delay(1);
		digitalWrite(ora3PIN_imu, LOW);
		delay(1);
		digitalWrite(yel4PIN_, LOW);
		delay(1);
		digitalWrite(gre5PIN_Sudp, LOW);
		delay(1);
		digitalWrite(blu6PIN_Logs, LOW);
		delay(1);
		digitalWrite(vio7PIN_Serial, LOW);
		delay(4);
	}
	digitalWrite(DEF_BLUE_LED, LOW);
// I2C checking ====================================================
	_SERIAL_3(Lang.CST(I2C_), Lang.CST(____LINE_40_EQUAL____));
	Wire.begin(); // set master mode, default on SDA/SCL for Ladybug
	delay(100);
//Clock Stretching by SFR02
	Wire.setClock(100000); // I2C frequency at 400 kHz
	_SERIAL_3(Lang.CST(I2C_CLOCK), Wire.getClock());
#define PWR_MGMT_1 0x6B
#define MPU9250_1_ADDRESS 0x68
#define USER_CTRL        0x6A  // Bit 7 enable DMP, bit 3 reset DMP
#define  I2C_MST_EN	0x20
#define WHO_AM_I_MPU9250 0x75 // Should return 0x71
	_SERIAL_3("\nWHOAMI:'%i'", readByte(MPU9250_1_ADDRESS, WHO_AM_I_MPU9250));
	writeByte(MPU9250_1_ADDRESS, USER_CTRL, 0); // disable internal I2C bus
	writeByte(MPU9250_1_ADDRESS, PWR_MGMT_1, 0x80); /**< @todo Find configured values MPU9250_?_ADDRESS & PWR_MGMT_? in 000_MPU9250 */
	delay(100); // Wait for all registers to reset
	writeByte(MPU9250_1_ADDRESS, USER_CTRL, I2C_MST_EN); // re-enable internal I2C bus
	delay(100); // Wait for all registers to reset
	_SERIAL_3("\nWHOAMI 100000:'%i'", readByte(MPU9250_1_ADDRESS, WHO_AM_I_MPU9250));
	Wire.setClock(400000); // I2C frequency at 400 kHz
	delay(50);
	_SERIAL_3("\nWHOAMI 400000:'%i'", readByte(MPU9250_1_ADDRESS, WHO_AM_I_MPU9250));
	delay(50);
	MPU9250 *mpu = new MPU9250(MPU9250_1_ADDRESS);
	mpu->resetMPU9250();
	delay(50);
	float SelfTest[6]; // holds results of gyro and accelerometer self test
	mpu->SelfTest(SelfTest); // Start by performing self test and reporting values
	_DBG_DRV_XIMU(
		"\n\tAcceleration xyz-axis (%6.3f,%6.3f,%6.3f) %% trim within of factory value",
		SelfTest[0], SelfTest[1], SelfTest[2]);
	_DBG_DRV_XIMU(
		"\n\tGyration     xyz-axis (%6.3f,%6.3f,%6.3f) %% trim within of factory value",
		SelfTest[3], SelfTest[4], SelfTest[5]);
	delay(50);
	_SERIAL_3("\nWHOAMI==>'%i'", mpu->getMPU9250ID());
	delay(50);
	delete mpu;

	int ni2c = I2Cscan();
	try {
		if (2 != ni2c)
			THROWERR(I2C_MISSING, ni2c);
	} catch (EXCEPT const &e) {
		FIX_ERROR();
	}

	/*
	 //	List files/jobgoals available
	 Serial.println("*************************************************");
	 File root = SPIFFS.open("/");
	 File jobgoalsFile = SPIFFS.open("/Jobgoals.js", "w");
	 String M = "JOBGOALS={ "; //space if no file -> ]
	 File file = root.openNextFile();
	 int fidx = 0;
	 while (file) {
	 Serial.print("FILE: ");
	 Serial.println(file.name());DLVL
	 String ext = String(file.name());
	 ext = ext.substring(ext.length() - 4);
	 if (ext == ".mis") {
	 M += "\"" + String(file.name()) + "\",";
	 }
	 file = root.openNextFile();
	 fidx++;
	 }
	 Serial.println("*************************************************");
	 M[M.length() - 1] = '}';
	 M += ";";
	 jobgoalsFile.println(M);
	 M = "// /////////////////////////////////////////////////";
	 jobgoalsFile.println(M);
	 jobgoalsFile.close();
	 Serial.println("*************************************************");
	 */
	_SERIAL_3(Lang.CST(SOCKETS_), Lang.CST(____LINE_40_EQUAL____));
	webInit(server, webSokSrv, onWSMessage);
	extern s_wsChecker *WSX[wsMax];
	for (size_t i = 0; i < wsMax; i++) {
		WSX[i] = new s_wsChecker();
	}
	_SERIAL_1("\nAPP SOCKETS =======================");
	_IMUqueue = xQueueCreate(imuDATAqueueSIZE, sizeof(s_fusensors));
	inMSGqueue = xQueueCreate(inMSGqueueSIZE, sizeof(s_itemWS));
//outMSGqueue = xQueueCreate(outMSGqueueSIZE, sizeof(s_OUTfifoItem));
	_SERIAL_1("\n\t'%+15i' : {APP} max number of simultaneous network client  connections",
		wsMax);
	_SERIAL_1("\n\nWORKFLOW ==========================");
	qFlow = new Flow("FLOW", &CTX.SOFTctx.FLOW___DBGMAX_);
	_SERIAL_1("\nQueued on map ack");
	_SERIAL_1("\n\nModules ===========================");

// TIME_MS module =====================================================
	modulesMap.insert(std::pair<const char*, Module*>(TIMEMScfg.MOD.Mid.c_str(),
		new Synchro(qFlow, TIMEMScfg, &CTX.SOFTctx.MOD_DBGMAX_TMS)));
// MOTORS module ======================================================
	modulesMap.insert(
		std::pair<const char*, Module*>(MOTORScfg.MOD.Mid.c_str(),
			new Motors(qFlow, MOTORScfg.MOD,
				new H_pwmMotors(&asmALL.PWMs, MOTORScfg.leftPWM, MOTORScfg.rightPWM,
					MOTORScfg.pwmCnam.c_str(), &CTX.SOFTctx.PWM____DBGMAX_, true),
				new H_pidMotors(&asmALL.PIDs, MOTORScfg.linPID, MOTORScfg.rotPID,
					&CTX.HARDctx.xpidCTX, MOTORScfg.pidCnam.c_str(),
					&CTX.SOFTctx.PID____DBGMAX_,
					true),
				new H_speedSensorMotors("speed", &CTX.SOFTctx.SPEED__DBGMAX_, true),
				new H_setpointSensorMotors("joystick", &CTX.SOFTctx.JSTICK_DBGMAX_, true),
				&CTX.SOFTctx.MOD_DBGMAX_MOT)));
// WATCHDOG module ====================================================
	modulesMap.insert(std::pair<const char*, Module*>(WATCHDOGcfg.MOD.Mid.c_str(),
		new Watchdog(qFlow, WATCHDOGcfg, &CTX.SOFTctx.WATctx,
			&CTX.SOFTctx.MOD_DBGMAX_WAT)));
// DRIVER module ====================================================
	modulesMap.insert(std::pair<const char*, Module*>(DRIVERcfg.MOD.Mid.c_str(),
		new Driver(qFlow, DRIVERcfg.MOD, &CTX.SOFTctx.DRVctx,
			new Kine_1(&CTX.HARDctx.kineCTX,
				"kine", &CTX.SOFTctx.KINE___DBGMAX_),
			new H_AccGyrMag(DRIVERcfg.IMU, &CTX.HARDctx.ximuCTX,
				DRIVERcfg.imuCnam, &CTX.SOFTctx.IMU____DBGMAX_, true),
			new H_ultraSounds(DRIVERcfg.usdCnam,
				&CTX.SOFTctx.USD____DBGMAX_, true),
			&CTX.SOFTctx.MOD_DBGMAX_DRV)));
// JOBGOAL module ====================================================
	modulesMap.insert(std::pair<const char*, Module*>(JOBGOALcfg.MOD.Mid.c_str(),
		new Jobgoal(qFlow, JOBGOALcfg, &CTX.SOFTctx.MOD_DBGMAX_JOB)));
	//#define DEF_TASK_LOG
#ifdef DEF_TASK_LOG
		launchTASK(e_tasks::YLOG, ylog_HDL, "Ylog", &ylog_TASK, nullptr,
			&CTX.SOFTctx.TSK_DBGMAX_LOGS, 4000, 1, DEF_PRIO_LOGS);
		launchTASK(e_tasks::ZLOG, zlog_HDL, "Zlog", &zlog_TASK, nullptr,
			&CTX.SOFTctx.TSK_DBGMAX_LOGS, 4000, 1, DEF_PRIO_LOGS);
		/*
		 */
	#endif

//	==== TASK MODULES LOOP ============================================
#define DEF_TASK_MOD
#ifdef DEF_TASK_MOD
	launchTASK(e_tasks::CORE, core_HDL, "Core", &core_TASK, nullptr,
		&CTX.SOFTctx.TSK_DBGMAX_CORE, 12000, 1, DEF_PRIO_CORE);
#endif
//	==== TASK XIMU ISR|SIMU ============================================
//	c_linkISR::Begin(&attach_ISR, &detach_ISR);
//	c_linkISR::Begin(hardPIN_isr &iimu_ISR);
#define DEF_TASK_IMU
#ifdef DEF_TASK_IMU
	if (CTX.SOFTctx.DRVctx.simul_IMU) {
		launchTASK(e_tasks::XIMU, ximu_HDL, "Simu", &simu_TASK, findModule("DRV"),
			&CTX.SOFTctx.TSK_DBGMAX_XIMU, 10000, 1, DEF_PRIO_XIMU);
		_SERIAL_1("\n\tIMU simulation !!!!!!");
	} else {
		launchTASK(e_tasks::XIMU, ximu_HDL, "Iimu", &iimu_TASK, findModule("DRV"),
			&CTX.SOFTctx.TSK_DBGMAX_XIMU, 5000, 1, DEF_PRIO_XIMU);
	}
	delay(100);
#endif
	_DBG_APP_BOOT("\nTasks =============================\n%s\n--------------------------------",
		dump_TASKS().c_str());
#ifdef DEF_TASK_IMU
	_SERIAL_1("\n\tTrue I²C IMU, ISR attached at pin %i (rising)", c_linkISR::pin());
	c_linkISR::attach_ISR(); // attachInterrupt(hardPIN_isr, iimu_ISR, RISING); interrupt pin output of MPU9250
#endif
}
///////////////////////////////////////////////////////////////////////
/// 	L O O P
///////////////////////////////////////////////////////////////////////
/**
 * @fn void loop()
 * @brief The loop task does nothing excepted it pass fifo commands to @ref KMDS
 * @n examples: attach ISR pin after a vTaskDelay, any command from UDP to modify environment...
 *
 * Intercepts serial keyboard commands every 250 milliseconds
 * this_DebugLVL can't be registred inside the loop task !!!!!!!!!!!!!!!!!!!!!!!
 */
void loop(){
	// Pass to CTX.SOFTctx.FCT_DBGMAX_LOOP (Aslm = App main setup loop)
	static int8_t &this_DebugLVL = *registerFCT(e_tasks::LOOP, "Loop",
		&CTX.SOFTctx.FCT_DBGMAX_LOOP);
	static uint32_t cnt = 0;
	uint64_t lastClean = milli_TS();
	FIX_ERRORS();
	s_kmdsMSG item;
	if (xQueueReceive(inKMDSqueue, &item, 0) == pdPASS)
		KMDS(item.kmdsMSG);
	else
		KMDS();
	vTaskDelay(loop_Delay / portTICK_PERIOD_MS);
	uint64_t newTMS = milli_TS();
	for (size_t i = 0; i < 1; i++) {
		//_SERIAL_1("",   "\n`%5i` ''abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",cnt);
		//_SERIAL_1("$",  "\n`%5i` '$'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",cnt);
		if (CTX.SOFTctx.txtActive)
			_SERIAL_1(
				"\n`%5i` <abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ>",
				cnt);
		if (CTX.SOFTctx.csvActive)
			_CSV_("@%llu;%i;%i", milli_TS(), loop_Delay, cnt);
		//_SERIAL_1("~$", "\n`%5i` '~$'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",cnt);
		//_SERIAL_1("$~", "\n`%5i` '$~abcdefgh/media/DEV/ROB_000/setup/SET_000_WS3/rov_000_WS3.hppijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",cnt);
		cnt++;
	}
	/*
	 if (1000 < (newTMS - lastClean)) {
	 _DBG_ APPLOOP(listStaClients().c_str());
	 _DBG _APPLOOP("\nwebSokSrv.cleanupClients()");
	 webSokSrv.cleanupClients();
	 _DBG_ APPLOOP(listStaClients().c_str());
	 lastClean = newTMS;
	 }
	 */
//vTaskDelete(NULL); delete the task !!!
}
///////////////////////////////////////////////////////////////////////
/*
 /// // This example demonstrates how a human readable table of run time stats
 // information is generated from raw data provided by uxTaskGetSystemState().
 // The human readable table is written to buffer


 // ### Cf. https://www.esp32.com/viewtopic.php?t=3674 to activate uxTaskGetSystemState
 void vTaskGetRunTimeStats(char *buffer) {
 TaskStatus_t *pxTaskStatusArray;
 volatile UBaseType_t uxArraySize, x;
 uint32_t ulTotalRunTime, ulStatsAsPercentage;

 // Make sure the write buffer does not contain a string.
 *buffer = 0x00;

 // Take a snapshot of the number of tasks in case it changes while this
 // function is executing.
 uxArraySize = uxTaskGetNumberOfTasks();

 // Allocate a TaskStatus_t structure for each task.  An array could be
 // allocated statically at compile time.
 pxTaskStatusArray = (TaskStatus_t*) pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));

 if (pxTaskStatusArray != NULL)
 {
 // Generate raw status information about each task.
 uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, &ulTotalRunTime);

 // For percentage calculations.
 ulTotalRunTime /= 100UL;

 // Avoid divide by zero errors.
 if (ulTotalRunTime > 0)
 {
 // For each populated position in the pxTaskStatusArray array,
 // format the raw data as human readable ASCII data
 for (x = 0; x < uxArraySize; x++)
 {
 // What percentage of the total run time has the task used?
 // This will always be rounded down to the nearest integer.
 // ulTotalRunTimeDiv100 has already been divided by 100.
 ulStatsAsPercentage = pxTaskStatusArray[x].ulRunTimeCounter / ulTotalRunTime;

 if (ulStatsAsPercentage > 0UL)
 {
 sprintf(buffer, "%s\t\t%lu\t\t%lu%%\r\n",
 pxTaskStatusArray[x].pcTaskName, pxTaskStatusArray[x].ulRunTimeCounter,
 ulStatsAsPercentage);
 }
 else
 {
 // If the percentage is zero here then the task has
 // consumed less than 1% of the total run time.
 sprintf(buffer, "%s\t\t%lu\t\t<1%%\r\n", pxTaskStatusArray[x].pcTaskName,
 pxTaskStatusArray[x].ulRunTimeCounter);
 }

 buffer += strlen((char*) buffer);
 }
 }

 // The array is no longer needed, free the memory it consumes.
 vPortFree(pxTaskStatusArray);
 }
 }
 */
///
/*
 PATCH de ESPAsyncWebServer-master/src/AsyncWebSocket.cpp

 if(request->hasHeader(WS_STR_PROTOCOL)){
 AsyncWebHeader* protocol = request->getHeader(WS_STR_PROTOCOL);
 //ToDo: check protocol

 //////////////////////////////////////////////////////////////////////////////////////
 extern String BDOOR_protocol;
 BDOOR_proto⇔col=protocol->value();
 //////////////////////////////////////////////////////////////////////////////////////

 response->addHeader(WS_STR_PROTOCOL, protocol->value());
 }
 else{
 //////////////////////////////////////////////////////////////////////////////////////
 extern String BDOOR_protocol;
 BDOOR_protocol="";
 //////////////////////////////////////////////////////////////////////////////////////
 }
 */
