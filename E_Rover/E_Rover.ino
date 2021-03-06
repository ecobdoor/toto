/** @file E_Rover.ino */
//*********************************************************************
/**
 * @page todo_tofix _TODO | _TOFIX
 *
 * - Global architecture:
 * @todo INCLUDE:
 * @n Include hard IMU in Kinetic (not in driver which is the global controller)
 *
 * - Tasks priorities & cores:
 * @todo ASync receivers:
 * @n CONFIG_ASYNC_TCP_RUNNING_CORE =-1//any available core (take 0 !!!)
 * @n CONFIG_ARDUINO_UDP_RUNNING_CORE=1
 * @n CONFIG_ARDUINO_UDP_TASK_PRIORITY=3
 *
 */
//*********************************************************************
/**
 * @page done_fixed _DONE | _FIXED
 *
 * - <b>Tasks priorities & cores (DONE)</b>
 * 	-# Attach ISR after delay
 * 	@n Low level KMD "Zmmm" is passed in KMD fifo where mmm is the delay in millisecond before
 * 	@n executing the next command for example:
 * 		-  when html client loads pages, OTA upgrades | uploads | downloads
 * 		- or during long commands requiring the total stop of interruptions such as keyboard commands with the menu assistance
 * 	-# lkjljljl
 * 	-# lkjljljl
 *
 *
 * - PATCHES (DON'T FORGET)
 *  - lkjljljl
 *  - lkjljljl
 *  - lkjljljl
 *  - lkjljljl
 *
 *
 * @n A LIRE ABSOLUMENT...
 * - http://marionpatrick.free.fr/man_html/html/tuto_code_unicode:2000-3fff.html (utf8)
 * - https://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format
 * - http://vidalc.chez.com/lf/socket.html
 * - https://stackoverflow.com/questions/18277304/using-stdcout-in-multiple-threads (detail : PrintThread{})
 *
 *
 */
//*********************************************************************
/**
 * @page esp32-arduino-lib-builder esp32-arduino-lib-builder
 *
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


 *
 */

/*
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
 Rover

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

 Limiter la longueur aceptable en fifo javascript sachant que les messages de requ??te doivent avoir un message de r??ponse dans un temps acceptable
 De plus les queues de messages des librairies Async socket g??rent des allocations et des lib??rations m??moires sur le heap
 qui peuvent entr??ner des ralentissement lors des r??organisations m??moire


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
 Saturation pour 2 fen??tres (1 firefox & 1 chrome)
 Pertes fifo JS (transactions/secondes
 1
 D??bit wifi   8 Ko/s dans les deux sens
 80 = (8000 o/s)/(100o/Json/s) moiti?? query / moiti?? reply
 Soit 40 transactions / seconde

 ---------------------------------------------------------------------------------
 https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/generic-class.html
 https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/generic-examples.html
 https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/WiFiClientEvents/WiFiClientEvents.ino

 * WiFi Events
 0  SYSTEM_EVENT_WIFI_READY               < ESP32 WiFi ready
 1  SYSTEM_EVENT_SCAN_DONE                < ESP32 finish scanning AP
 2  SYSTEM_EVENT_STA_START                < ESP32 sRovertation start
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
 case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:Rover
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

 */
/*
 * DON'T send directly serialize in a wifi or serial stream because arduino json may make a packet by character
 https://arduinojson.org/v6/api/json/serializejson/
 See to serial or wifi & file slowing write
 */
//#define DEF_ESP_WIFI_AP
// initial stack
int loop_Delay = 250;
//char *stack_start;

#if !defined(__cplusplus)
#error >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>C++ compiler required.
#endif
/*
 #include "bib_view.h"
 remWorld remWorld0 *=new remWorld();
 */
//#include "esp_wifi.h"
//#include "time.h"
//#include "Esp.h"
#include "Arduino.h"
#include <ESPmDNS.h>
#include "esp32_GOODIES.h"
#include "esp32_SPRINT.h"
#include "esp32_LOGS.h"
#include "esp32_WIFI.h"
#include "000_WSOK.h"
#include "000_Rover_webServer.h"
#include "rov-CfgH_000_WS3.h"
#include "rov-CfgH_000_WS3.inc"
#include "rov-Kmds_000_WS3.h"
#include "rov-Mod_Driver_000_WS3.h"
#include "rov-Mod_Motors_000_WS3.h"
#include "rov-Mod_Jobgoal_000_WS3.h"
#include "rov-Mod_Synchro_000_WS3.h"
#include "rov-Mod_Watchdog_000_WS3.h"
//#include <freertos/task.h> // for taskENTER_CRITICAL() taskEXIT_CRITICAL()

struct asmALL {
	asmPWMs PWMs; ///< Motors PWM implementation
	asmPIDs PIDs; ///< Servos PID implementation
	asmIMU IMU; ///< IMU implementation
	asmUSD USD; ///< Ultrasonic radar implementation
} asmALL;
//return
#define DEF_FACTORY_RESET // takes default contexts

/**
 * https://translate.google.com/translate?hl=fr&sl=en&u=https://www.freertos.org/FreeRTOS_Support_Forum_Archive/August_2017/freertos_Static_variables_and_Tasks_90861d42j.html&prev=search&pto=aue
 * Tout ce qui est global ou statique sera r??f??renc?? par toutes les t??ches.
 * Il y a donc un seul &this_DebugLVL pour tout le main !!!!
 * ce qui n'est "pas" g??nant car main, setup (vs boot), loop, kmds, on_message et totes les fonctions
 * ci dessous ... (?? v??rifier!!!!)
 */
int8_t &this_DebugLVL = *registerFCT(e_tasks::MAIN, "Main", &CTX.SOFTctx.FCT_DBGMAX_MAIN);

///////////////////////////////////////////////////////////////////////
t_map_QSRC sourceMap = {
	{ DEF_QUERY_BASIS_TO_ROVER, { reply2query_BASIS, nullptr, 123 } },
	{ DEF_REPLY_BASIS_TO_ROVER, { ackno2reply_BASIS, nullptr, 123 } },
	{ DEF_REPLY_PILOT_TO_ROVER, { ackno2reply_PILOT, nullptr, 123 } },
	{ DEF_QUERY_PILOT_TO_ROVER, { reply2query_PILOT, nullptr, 456 } },
};
c_myFlow *qFlow;
extern t_map_MOD modulesMap;
TaskHandle_t core_HDL; ///< application's kern task
TaskHandle_t zlog_HDL; ///< Log test task
TaskHandle_t ylog_HDL; ///< Log test task
TaskHandle_t ximu_HDL; ///< IMU task
TaskHandle_t wsnd_HDL; ///< WS sender task
//TaskHandle_t onEvent;

/*
 const char* ssid = "rover";
 const char* password =  "0123456789";
 */
AsyncWebServer server(80);
AsyncWebSocket webSokSrv("/ws");
JsonObject *jsonPtr;
H_AccGyrMag *imu9250 = nullptr; ///< @brief IMU pointer for reset_ISR() from KMDS

//Internals::StaticJsonB/media/DEV/ROB_000/setup/SET_000_WS3/rov_000_WS3.hppufferBase * _jsonBuffer;
//int outMSGqueueSIZE = 5;
//QueueHandle_t outMSGqueue;
//char ptrTaskList[250];
/*
 const char* ntpServer = "pool.ntp.org";
 const long  gmtOffset_sec = 3600;
 const int   daylightOffset_sec = 3600;
 */
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
/*
 Traite les messages en attente pour tous les clients en W SX
 Order
 Answer
 Debug, Log
 ...
 */
/**
 * Exception fault when throwing up to a task body!!!!!!!!!
 * @param this_DebugLVL
 */
//---------------------------------------------------------------------
int8_t c_Kernel_dbglvl = 0;
/**
 * @class c_Kernel: public Core
 * @todo
 */
class c_Kernel: public Core {
public:
	c_Kernel() :
		Core("Kern", &c_Kernel_dbglvl){
	}
	/**
	 * @fn void c_Kernel::core_websock_MSG()
	 * @brief Processes the messages received from web sockets fifo & sends a response if needs
	 */
	void core_websock_MSG();
	/**
	 * @fn void core_modules_MSG(int8_t &this_DebugLVL)
	 * @brief Manages the json messages spontaneously generated by modules @ref has_MSG
	 */
	void core_modules_MSG();

private:
};
//---------------------------------------------------------------------
/** *******************************************************************
 * If a message is available in the websocket receive fifo, extract it and:\n
 * - If the text message is correctly deserialized into a JSON object, pass the JSON object\n to the targeted module which will process it and possibly return one (or more) auto_MSG and a response (or not).\n
 * - Otherwise, sends the text message to the KMDS fifo for low-level command execution.
 */
void c_Kernel::core_websock_MSG(){
	/*
	 * ==========================================
	 * 1) Traiter les requ??tes ou r??ponses re??ues
	 * ------------------------------------------
	 */
	uint64_t start_queue = milli_TS();
	s_WS_recvMSG item;
	try {
		if (xQueueReceive(myWS.recv_queue(), &item, 0) == pdPASS) {
			digitalWrite(DEF_BLUE_LED, HIGH);
			String keeper = String((char*)item.inMSG);
			// d??truit la source which must stay available during _sonDoc used !!!)
			if (0 != deserializeJson(*_jsonDoc, (char*)item.inMSG)) {
				myKMDS.push((char*)&item.inMSG);
				_SERIAL_0("\ndeserializeJson ERRROR -> pass toKMDS");
				return;
			}
//				THROWERR(JSON_ERR_DESERIALIZE, (char* )item.inMSG);
			JsonObject root = (*_jsonDoc).as<JsonObject>(); // keep _jsonDoc of course...
			String ppp = root["CTL"]["TYP"].as<String>();
			//_SERIAL_0("\nroot[CTL][TYP]='%s'", ppp.c_str());
			//String header = Head er(+"POPF " + root["KMD"]["MID"].as<String>() + " << ",				Remote(ppp[2]));
//			Remote(item.PWSX->protocol[0]));
			//_SERIAL_3( "\n%s%s", header.c_str(), keeper.c_str());

			//--------------------------------------------------
			//	1)  TRAITER LA FILE D'ATTENTE DE RECEPTION SOCKET
			//------------------------------------------------
			//const char * x = root["CTL"]["TYP"].as<string>().c_str();
			/**
			 *  itDisps->second.task returns true if root argument is a query
			 *  then root is modified to send the reply
			 *  But when receiving reply could also need a new query|info from the rover \n
			 *  ackno2reply_PILOT calls updateATreply
			 */
			if (findFCT(root["CTL"]["TYP"].as<string>().c_str())(root)) {
				myWS.sendJSON(root["KMD"]["MID"].as<String>(),
					root["CTL"]["TYP"].as<String>().c_str(),
					root["CTL"]["TIK"].as<int32_t>() != 0);
			}
			delay(1);
		}
		else {
			/**
			 * @bug HAZARDOUS: if the ws recv fifo is empty for a long time?
			 * @todo What happens if the ws recv fifo is empty for a long time?\n
			 * - it is possible if the rover is:
			 * 	- running on an autonomous mission\n
			 * 	- idle: stopped, power loading, etc...\n
			 * 	- error:
			 * - Otherwise the browser or the wifi is planted and you have to stop!!!\n
			 * because there is no more pilot watchdog message
			 */
		}
	} catch (EXCEPT const &e) {
		FIX_ERROR();
	} catch (std::exception const &e) {
		Serial.printf("\n@@@@@@@@@@@@@@@@@@@@@ %s @@@@@@@@@@", e.what());
	}
}
//---------------------------------------------------------------------
void c_Kernel::core_modules_MSG(){
	/*
	 * ==========================================
	 * 2) Envoyer des requ??tes aux sockets actives suivant le timing exig?? !!!
	 * ------------------------------------------
	 * plusieurs requ??tes sur la m??me socket peuvent avoir des timing differents!!!!
	 * on est dans le cas simple !!!!!!!!!!!
	 * D??clarer les modules les plus rapides en t??te dans le vecteur des modules pour
	 * les traiter en priorit??
	 */
	uint64_t start_modules = milli_TS();
	//vTaskDelay( x Delay );
	try {
		for (auto itMod = modulesMap.begin(); itMod != modulesMap.end(); ++itMod) {
			Module &module = *(itMod->second); //*MODULES[m];
			//if (String("DRV") == module._Mid)continue; // _Mid is a String!!

			//_SERIAL_0( "\n[%s]", module._Mid.c_str());
//				if (false/*module.has_MSG()*/) {
			if (module.has_MSG()) {
				digitalWrite(DEF_BLUE_LED, HIGH);
				digitalWrite(DEF_BLUE_LED, HIGH);

				myWS.sendJSON(module._Mid, module._protocol.c_str());
				digitalWrite(DEF_BLUE_LED, LOW);
			}
		}
	} catch (EXCEPT const &e) {
		FIX_ERROR();
	} catch (std::exception const &e) {
		Serial.printf("\n@@@@@@@@@@@@@@@@@@@@@ %s @@@@@@@@@@", e.what());
	}
}
//---------------------------------------------------------------------
/**
 * @fn void core_TASK(void *pvParameters)
 * @brief Main task of the application managing messages received either from open web sockets
 * or spontaneously generated by modules.
 * @param void * pvParameters to get its tskInfo
 */
void core_TASK(void *pvParameters){
	/**
	 * @todo Actually @ref core_websock_MSG() & @ref core_modules_MSG() should be 2 separate tasks.\n
	 * !!! <b>WORKAROUND: for now any browser message that generates multiple auto_MSGs must be a low level command</b> !!!
	 * - Indeed, a message received from the pilot can generate a process that must generate several @ ref auto_MSG. \n
	 * But the latter are only seen by @ref core_modules_MSG(), they risk crashing into each other without \n
	 * ever being pushed into the sending fifo to the pilot.
	 * - This is not the case if @ref core_websock_MSG() receives a low level message which will be processed by \n
	 * the independent KMDS task because @ref core_modules_MSG() will run normally.
	 *
	 */
	s_tskInfo_X &tskInfo = *(reinterpret_cast<s_tskInfo_X*>(pvParameters));
	int8_t &this_DebugLVL = *(tskInfo.DebugLVL);
	c_Kernel Kernel;
	for (;;) {
		//digitalWrite(red2PIN_modules, HIGH);
		Kernel.core_websock_MSG();
		Kernel.core_modules_MSG();
		vTaskDelay(10);
		digitalWrite(DEF_BLUE_LED, LOW);
//		digitalWrite(red2PIN_modules, LOW);
	}
}
///////////////////////////////////////////////////////////////////////
/**
 * @fn char * outset(char * BUFFER,size_t LEN=60)
 * @brief return the beginning of BUFFER (no thread-safe)
 * @param BUFFER input
 * @param LEN max length to output
 * @return static stacked buffer. Pay attention to reentrant calls !!!
 */
char* outset(char *BUFFER, size_t LEN = 60){
	static char buffer[64];
	if (BUFFER) {
		snprintf(buffer, (LEN < 60) ? 1 + LEN : 60, "%s", BUFFER);
	} else
		sprintf(buffer, "(no data)");
	return buffer;
}
//---------------------------------------------------------------------
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
	int8_t &this_DebugLVL = *(tskInfo.DebugLVL);
	for (;;) {
		myWS.wsnd_websock_MSG();
		vTaskDelay(25); //think about other lower priorities tasks timing...
	}
}
///////////////////////////////////////////////////////////////////////
/// I M U   O N   I 2 C   &   S I M U L A T I O N
///////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
/**
 * @fn void iimu_TASK(void *pvParameters)
 * @brief Task handling IMU data get from I??C \ref iimu_ISR interrupt
 *
 * Every harware interrupt data are read from I??C. If previous data have been proceeded at
 * ISR micro time, the current are treated & pushed to the driver fifo. Else they are forgotten.
 * Indeed, despite a high priority, it is not possible to guarantee that the task will not be
 * interrupted by an other and that the data will be deposited in fifo before 10ms.
 *
 * Push IMU real data in \ref _IMUqueue each time an ISR notification is received
 * and reads I??C ultrasonic sensor once in 10 times
 * @param void * pvParameters to get its tskInfo & module Driver reference
 */
void iimu_TASK(void *pvParameters){
	s_tskInfo_X &tskInfo = *(reinterpret_cast<s_tskInfo_X*>(pvParameters));
	int8_t &this_DebugLVL = *(tskInfo.DebugLVL);
	Driver &_driver = *reinterpret_cast<Driver*>(tskInfo.Param);
	Kine_1 &kine = _driver.imu_Kine();
//_SERIAL_0( "\n\tModule idM= '%s'", _driver->_Mid.c_str());
// Initialise the xLastWakeTime variable with the current time.
	TickType_t xLastWakeTime = xTaskGetTickCount();
//const TickType_t xFrequency = 6;
	uint32_t ulInterruptStatus;
	for (;;) {
		xTaskNotifyWait(0x00, ULONG_MAX, &ulInterruptStatus, portMAX_DELAY); // Wait indefinitely for ISR  interrupt
		// MUST BE PERFORMED BEFORE NEXT ISR (other tasks could delay the end of the fifo push)
		if (imuFlag) {
			imuFlag = false; // reset flag
			digitalWrite(ora3PIN_imu, !digitalRead(ora3PIN_imu));
			delayMicroseconds(100); // Bitscope 1% 100??s/10ms
			digitalWrite(ora3PIN_imu, !digitalRead(ora3PIN_imu));
			delayMicroseconds(100); // Bitscope 1% 100??s/10ms
			_driver.acq_trueIMU(isr_microTimeStamp); // Acquire, set _Ximu._microTS
			if (_driver.pushDone()) {
				digitalWrite(ora3PIN_imu, HIGH);
				//vTaskSuspendAll ();//taskENTER_CRITICAL();
				_driver.push_RTdata();
				//xTaskResumeAll ();//taskEXIT_CRITICAL();
//				delayMicroseconds(50); // Bitscope 1% 100??s/10ms
				digitalWrite(ora3PIN_imu, LOW);
			}
		}
	}
}
//---------------------------------------------------------------------
/**
 * @fn void simu_TASK(void *pvParameters)
 * @brief Task handling IMU data simulation
 *
 * Push IMU simulated data in \ref _IMUqueue at 100 Hz
 * and reads I??C ultrasonic sensor once in 10 times
 * \n This task may be useful to verify the kinematic model of rover or to pilot it from IMU calculated data
 * \n WARNING: 	isr_microTimeStamp will be overide by simulation MTS
 * @param void * pvParameters to get its tskInfo & module Driver reference
 */
void simu_TASK(void *pvParameters){
	s_tskInfo_X &tskInfo = *(reinterpret_cast<s_tskInfo_X*>(pvParameters));
	int8_t &this_DebugLVL = *(tskInfo.DebugLVL);
	Driver &_driver = *reinterpret_cast<Driver*>(tskInfo.Param);
	const TickType_t xFrequency = 5100; /**< @todo Adapt with simu sample period !!! */
	TickType_t xLastWakeTime = xTaskGetTickCount(); // Initialize the xLastWakeTime variable with the current time.
	for (;;) {
		vTaskDelayUntil(&xLastWakeTime, xFrequency); // Attend le cycle suivant.
		digitalWrite(ora3PIN_imu, HIGH);
		_driver.acq_simuIMU();
		digitalWrite(ora3PIN_imu, LOW);
		delayMicroseconds(50); // Bitscope 1% 100??s/10ms
		digitalWrite(ora3PIN_imu, HIGH);
		_driver.push_RTdata();
		delayMicroseconds(100); // Bitscope 1% 100??s/10ms
		digitalWrite(ora3PIN_imu, LOW);
	}
}
///////////////////////////////////////////////////////////////////////
/// L O G   T E S T
///////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
/**
 * @fn void ylog_TASK(void *pvParameters)
 * @brief Task testing webSocket log
 * @param void * pvParameters to get its tskInfo
 */
void ylog_TASK(void *pvParameters){
	s_tskInfo_X &tskInfo = *(reinterpret_cast<s_tskInfo_X*>(pvParameters));
	int8_t &this_DebugLVL = *(tskInfo.DebugLVL);
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
			");
	}
}
//---------------------------------------------------------------------
/**
 * @fn void zlog_TASK(void *pvParameters)
 * @brief Task testing webSocket log
 * @param void * pvParameters to get its tskInfo
 */
void zlog_TASK(void *pvParameters){
	s_tskInfo_X &tskInfo = *(reinterpret_cast<s_tskInfo_X*>(pvParameters));
	int8_t &this_DebugLVL = *(tskInfo.DebugLVL);
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
			");
	}
}
//#####################################################################
// 	S E T U P
//#####################################################################
//---------------------------------------------------------------------
/*
 Module* driver(){
 Kine_1 *ik = new Kine_1(&CTX.HARDctx.kineCTX, "imu_kine", &CTX.SOFTctx.KINE___DBGMAX_);
 Kine_1 *dk = new Kine_1(&CTX.HARDctx.kineCTX, "drv_Kine", &CTX.SOFTctx.KINE___DBGMAX_);
 imu9250 = new H_AccGyrMag(DRIVERcfg.IMU, &CTX.HARDctx.ximuCTX,
 DRIVERcfg.imuCnam, &CTX.SOFTctx.IMU____DBGMAX_, true);
 H_ultraSounds *usd = new H_ultraSounds(DRIVERcfg.usdCnam,
 &CTX.SOFTctx.USD____DBGMAX_, true);
 Module *m = new Driver(qFlow, DRIVERcfg.MOD, &CTX.SOFTctx.DRVctx, ik, dk, imu9250, usd,
 &CTX.SOFTctx.MOD_DBGMAX_DRV);
 return m;
 }
 */
//---------------------------------------------------------------------
H_pwmMotors* new_pwmMotors(){
	pwmMotors = new H_pwmMotors(&asmALL.PWMs, MOTORScfg.leftPWM, MOTORScfg.rightPWM,
		MOTORScfg.pwmCnam.c_str(), &CTX.SOFTctx.PWM____DBGMAX_, true);
	pwmMotors->detachPWMpins();
	return pwmMotors;
}
//---------------------------------------------------------------------
void testPINS(){
	if (!CTX.SOFTctx.delayPINStest)
		return;
	size_t xxx = 3;
	bool finished = false;
	// approximately number of pins to check on logic analyser (pulse 1 ms each)
	if (0 < CTX.SOFTctx.delayPINStest) {
		xxx = CTX.SOFTctx.delayPINStest / 16;
		finished = true;
	}
	digitalWrite(DEF_BLUE_LED, HIGH);
	do {
		for (size_t i = 0; i < xxx; i++) {
			digitalWrite(whi0PIN_wsx, HIGH);
			delay(1);
			digitalWrite(bro1PIN_udp, HIGH);
			delay(1);
			digitalWrite(red2PIN_serial, HIGH);
			delay(1);
			digitalWrite(ora3PIN_imu, HIGH);
			delay(1);
#ifndef DEF_WITHMOTORSPINS
			digitalWrite(yel4PIN_Rpwm, HIGH);
			delay(1);
			digitalWrite(gre5PIN_Rsns, HIGH);
			delay(1);
			digitalWrite(blu6PIN_Lpwm, HIGH);
			delay(1);
			digitalWrite(vio7PIN_Lsns, HIGH);
			delay(1);
#endif
			digitalWrite(whi0PIN_wsx, LOW);
			delay(1);
			digitalWrite(bro1PIN_udp, LOW);
			delay(1);
			digitalWrite(red2PIN_serial, LOW);
			delay(1);
			digitalWrite(ora3PIN_imu, LOW);
			delay(1);
#ifndef DEF_WITHMOTORSPINS
			digitalWrite(yel4PIN_Rpwm, LOW);
			delay(1);
			digitalWrite(gre5PIN_Rsns, LOW);
			delay(1);
			digitalWrite(blu6PIN_Lpwm, LOW);
			delay(1);
			digitalWrite(vio7PIN_Lsns, LOW);
			delay(1);
#endif
			if ((CTX.SOFTctx.delayPINStest < 0) && (inKey()))
				finished = true;
		}
	} while (!finished);
	digitalWrite(DEF_BLUE_LED, LOW);
}
///////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
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
 * @n Boot DATAG(fifo flow) & IMU_queue & WS_recvMSG_queue
 * @n Boot SUDP			(SERIAL localized
 * @n Boot LOGS  cvs & telemetry log serial to udp start boot report
 * 			@n Boot wsnd_HDL
 * @n Boot display of system informations
 * @n Boot Esp32 pins
 * @n Boot Bitscop pins test
 * @n Boot Webserver & Websocket server
 * @n Boot I2C
 * @n Boot WS_recvMSG_queue modules
 * @n Boot WS_recvMSG_queue tasks
 *
 */
#include "rov.Quaternions.hpp"
void setup(){
	extern uint64_t tmsOffset;
	tmsOffset = 1601299940009;
	int16_t cnt;
	char *buffer;
/// ***********************************************************************
/// SERIAL
///
	Serial.begin(115200);
//	test_Quaternions();
	/*
	 /// ***********************************************************************
	 /// PSRAM
	 ///
	 //Initialisation
	 if (psramInit()) {
	 Serial.println("\nLa PSRAM est correctement initialis??e");
	 } else {
	 halt("La PSRAM ne fonctionne pas");
	 }
	 */
//	printSysState();
//USE_SERIAL.printf("\n** this_DebugLVL '%i'\n",this_DebugLVL);
	int8_t &this_DebugLVL = *registerFCT(e_tasks::BOOT, "Boot", &CTX.SOFTctx.FCT_DBGMAX_BOOT);
//USE_SERIAL.printf("\n** this_DebugLVL '%i'\n",this_DebugLVL);
//Serial.println(datim(1617182061001001,2));
/// ***********************************************************************
/// SPIFFS
///
	_DBG_APP_BOOT("\n@@@@ SPIFFS ");
	if (!SPIFFS.begin(false, "/spiffs")) {
		_DBG_APP_BOOT("FAILS while mounting");
		return;
	} else
		_DBG_APP_BOOT("started");
/// ***********************************************************************
/// LangSTR
///
	Lang.Begin("/rov-Sen_000_WS3(french).csv");
	/** *********************************************************************
	 * UTF8 SUPPORT
	 * https://fr.wikipedia.org/wiki/Exposants_et_indices_Unicode

	 ??????????????????????????????


	 https://unicode-table.com/en/blocks/combining-diacritical-marks/
	 X?????????X??X??X????X?? U0303 combinig tilde a?? '??? X??? X?? v??? Z??vZ??Y??
	 */
	char utf8[6] = { 0xe2, 0x93, 0xbf, '\0', '\0', '\0' };
	_SERIAL_0("\n ZERO '%s'", utf8);
	_SERIAL_0("\nUTF8 support:");
// CF. https://unicode-table.com/fr/sets/arrow-symbols/#heavy-arrows
	// https://unicode-table.com/fr/278E/
	_SERIAL_0("\nLook correct:{???????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????"); //???????????????????
	_SERIAL_0(
		"\nLook correct:{X?????????X??X??X????X?? U0303 combinig tilde a?? '??? X??? X?? v??? Z??Z??Y??f???????????????????????????????????????????????????????????????????????????????????????????????????G??????????????????????????????????Vv??????????????????????????????????????????}");
	_SERIAL_0("\nSeem hazard: {??????????????????????????????????????????????????????????????????????}");
	_SERIAL_0("\nSeem strange:{????}");
	_SERIAL_0("\n********************\x24\x60***********************");
	const char bufRight[10] = "???"; // \xe2\x86\xa3
	Serial.printf("\n[%s]= ", (char*)&bufRight);
	for (size_t i = 0; i < strlen(bufRight); i++)
		Serial.printf(" %2i:x%02X", i, bufRight[i]);
	const char bufLeft[10] = "???"; // \xe2\x86\xa2
	Serial.printf("\n[%s]= ", (char*)&bufLeft);
	for (size_t i = 0; i < strlen(bufLeft); i++)
		Serial.printf(" %2i:x%02X", i, bufLeft[i]);
	Serial.printf("\nBLACK:");
	for (size_t i = 0; i < 10; i++)
		Serial.printf("%s", (char*)&blackNum[i]);
	Serial.printf("\nWHITE:");
	for (size_t i = 0; i < 10; i++)
		Serial.printf("%s", (char*)&whiteNum[i]);
	Serial.printf("\n*********************\n");
	/*
	 Serial.printf("\n   8               9               A               B");
	 Serial.printf("\n   ??123456789ABCDEF??123456789ABCDEF??123456789ABCDEF??123456789ABCDEF");
	 for(size_t i=0x80;i<=0xBF;i++){
	 Serial.printf("\n%2X ",i);
	 uint8_t  zzz[4];
	 zzz[0]=0xe2;
	 zzz[1]=i;
	 zzz[3]=0x00;
	 for(size_t j=0x80;j<=0xBF;j++){
	 zzz[2]=j;
	 Serial.printf("%s",(char *)&zzz );
	 }
	 }
	 */
/// ***********************************************************************
/// EEPROM CONTEXT
///
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
		_DBG_APP_BOOT("\n  Started ");
#ifdef DEF_FACTORY_RESET
		// CTX.SAVE(); NO !!!
		_DBG_APP_BOOT("with DEFAULT %s", CTX.dumpBOOTctx().c_str());
#else
	CTX.LOAD();
	_DBG_APP_BOOT("with EEPROM %s", dumpBOOTctx().c_str());
#endif
		_DBG_APP_BOOT("\n%s", CTX.dumpHARDctx().c_str());
		_DBG_APP_BOOT("\n%s\n", CTX.dumpSOFTctx().c_str());
	}
//	if(!ee){delay(1000);ESP.restart();}
	/*
	 uint64_t tms; //
	 //tms = 1617182061000000;E_CALIMU.put(0, tms);	E_CALIMU.commit();tms = 0;
	 E_CALIMU.get(0, tms);
	 _SERIAL_0( Lang.CST(EEPROM_CALIMU_OK), datim(tms).c_str(), tms);
	 */
// WIFI setup ====================================================
/// ***********************************************************************
/// WIFI
///
	buffer = allocBuf(DEF_DBGMSG_BUFSIZE, &cnt);
	SPrintF_tilte(DEF_DBGMSG_BUFSIZE, cnt, buffer, "@@@@ WIFI (delay 1 to 2 sec to find channel)");
	_DBG_APP_BOOT("%s", buffer);
	WiFiEvents(WiFi);
	buffer = allocBuf(DEF_DBGMSG_BUFSIZE, &cnt);
#ifdef DEF_ESP_WIFI_AP
	const char *ssid = "rover";
	const char *password = "0123456789";
	WiFi.softAP(ssid, password, 3); //,1);//,0,3);
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer, "\n\tAccess Point ssid='%s', pswd='%s'", ssid,
		password);
	delay(2001); // To find channel, etc...
	WiFi.setTxPower(WIFI_POWER_15dBm);
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
	const char *ssid = "bdoor"; //"BazUbu";//
	const char *password = "0123456789";
	WiFi.begin(ssid, password);
	_SERIAL_3(Lang.CST(WIFI_CONNECTING));
	_SERIAL_3(" ssid '%s'", ssid);
	while (WiFi.status() != WL_CONNECTED)
	{
		USE_SERIAL.printf(".");
		delay(1000);
	}
	USE_SERIAL.printf("\n");
	_SERIAL_3(Lang.CST(WIFI_LOCALIP), WiFi.localIP().toString().c_str());
	_SERIAL_3(Lang.CST(WIFI_ATT_DB), WiFi.RSSI());
#endif
	_DBG_APP_BOOT("\n%s", buffer);
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
///******************************************************************** DIRECTORY
/// UDP LOGS
///
	buffer = allocBuf(DEF_DBGMSG_BUFSIZE, &cnt);
	SPrintF_tilte(DEF_DBGMSG_BUFSIZE, cnt, buffer, "@@@@ UDP_dashboard (%s)",
		(!CTX.SOFTctx.delayUDPStart) ?
			"No delay for udp log" :
			(String("Waiting for ") + String(CTX.SOFTctx.delayUDPStart / 1000.0, 3)
				+ String(" sec to connect trace log")).c_str());
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer, "\n\tUDP Listening on port %i", DEF_UDP_PORT);
	_DBG_APP_BOOT("\n%s", buffer);
	if (myUDP.Begin()) {
		_DBG_APP_BOOT("\n\tUDP begin OK");
		delay(CTX.SOFTctx.delayUDPStart);
	}
	else
		halt("\n\t UDP init error");
/// ***********************************************************************
/// ESP INFOS
///
	buffer = allocBuf(DEF_DBGMSG_BUFSIZE, &cnt);
	SETUP_ESPINFO(DEF_DBGMSG_BUFSIZE, cnt, buffer);
	_DBG_APP_BOOT("\n%s", buffer);
/// ***********************************************************************
/// ESP CONFIG
///
	buffer = allocBuf(DEF_DBGMSG_BUFSIZE, &cnt);
	SPrintF_tilte(DEF_DBGMSG_BUFSIZE, cnt, buffer, "#### %s", Lang.CST(KERN_));
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer,
		Lang.CST(KERN_CPP), __cplusplus);
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer,
		Lang.CST(KERN_ARDUINOJSON_USE_LONG_LONG), ARDUINOJSON_USE_LONG_LONG);
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer,
		Lang.CST(KERN_sizeof_t_jsonDoc), sizeof(t_jsonDoc));
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer,
		Lang.CST(KERN_WS_MAX_QUEUED_MESSAGES), WS_MAX_QUEUED_MESSAGES);

	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer, "\n");

	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer,
		Lang.CST(KERN_configUSE_TRACE_FACILITY), configUSE_TRACE_FACILITY);
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer,
		Lang.CST(KERN_configUSE_STATS_FORMATTING_FUNCTIONS), configUSE_STATS_FORMATTING_FUNCTIONS);
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer,
		Lang.CST(KERN_configGENERATE_RUN_TIME_STATS), configGENERATE_RUN_TIME_STATS);
	/*
	if (configGENERATE_RUN_TIME_STATS)
		SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer,
			Lang.CST(KERN_configRUN_TIME_STATS_USING_ESP_TIMER),
			configRUN_TIME_STATS_USING_ESP_TIMER);
	else
		SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer,
			Lang.CST(KERN_configRUN_TIME_STATS_USING_ESP_TIMER),
			-1);
	*/
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer, "\n");

	//INCLUDE_uxTaskPriorityGet
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer,
		Lang.CST(KERN_INCLUDE_vTaskDelay), INCLUDE_vTaskDelay);
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer,
		Lang.CST(KERN_portTICK_PERIOD_MS), portTICK_PERIOD_MS);
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer,
		Lang.CST(KERN_ESP_TASKS));
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer,
		Lang.CST(KERN_ESP_TASK_PRIO_MAX), ESP_TASK_PRIO_MAX);
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer,
		Lang.CST(KERN_ESP_TASK_TIMER_PRIO), ESP_TASK_TIMER_PRIO, ESP_TASK_TIMER_STACK);
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer,
		Lang.CST(KERN_ESP_TASKD_EVENT_PRIOevent), ESP_TASKD_EVENT_PRIO, ESP_TASKD_EVENT_STACK);
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer,
		Lang.CST(KERN_ESP_TASK_TCPIP_PRIO), ESP_TASK_TCPIP_PRIO, ESP_TASK_TCPIP_STACK);
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer,
		Lang.CST(KERN_ESP_TASK_MAIN_PRIO), ESP_TASK_MAIN_PRIO, ESP_TASK_MAIN_STACK);
	_DBG_APP_BOOT("\n%s", buffer);
/// ***********************************************************************
/// BENCHMARK
///
	if (CTX.SOFTctx.benchMark) {
		buffer = allocBuf(DEF_DBGMSG_BUFSIZE, &cnt);
		SPrintF_tilte(DEF_DBGMSG_BUFSIZE, cnt, buffer,
			"#### Benchmark by milliseconde ie kHz (wait for computation...)");
		_DBG_APP_BOOT("\n%s", buffer);
		buffer = allocBuf(DEF_DBGMSG_BUFSIZE, &cnt);
		benchMark(DEF_DBGMSG_BUFSIZE, cnt, buffer);
		_DBG_APP_BOOT("\n%s", buffer);
	}
///********************************************************************
/// SPIFFS DIRECTORY
///
	buffer = allocBuf(DEF_DBGMSG_BUFSIZE, &cnt);
	SPrintF_tilte(DEF_DBGMSG_BUFSIZE, cnt, buffer, "#### Listing SPIFFS /");
	_DBG_APP_BOOT("\n%s", buffer);
	Lang.listDir_Old(SPIFFS, "/", 0);
///********************************************************************
/// EEPROM ENVIRONMENT DISPLAY
///
	buffer = allocBuf(DEF_DBGMSG_BUFSIZE, &cnt);
	SPrintF_tilte(DEF_DBGMSG_BUFSIZE, cnt, buffer, "#### EEPROM environment");
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer, "\n%s\n", CTX.dumpBOOTctx().c_str());
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer, "\n%s\n", CTX.dumpHARDctx().c_str());
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer, "\n%s\n", CTX.dumpSOFTctx().c_str());
	_DBG_APP_BOOT("\n%s", buffer);
/// ***********************************************************************
/// PINS configuration
///
	buffer = allocBuf(DEF_DBGMSG_BUFSIZE, &cnt);
	SPrintF_tilte(DEF_DBGMSG_BUFSIZE, cnt, buffer, "#### PINS configuration (%s)",
		(!CTX.SOFTctx.delayPINStest) ?
			"No delay for bitscpoe test" :
			(String("Waiting for ") + String(CTX.SOFTctx.delayPINStest / 1000.0, 3)
				+ String(" sec to test bitscope")).c_str());
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer,
		"\n\tTesting square wave for logic analyzer during %i ms", CTX.SOFTctx.delayPINStest);
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer, "\n\tBLUE LED pin %2i ", DEF_BLUE_LED);
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer, "\n\tIMU ISR  pin %2i", c_linkISR::pin());
#ifdef DEF_WITHMOTORSPINS
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer, "\n\tMOTORS pins activated");
#else
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer, "\n\tMOTORS pins unactivated !!!");
#endif
	_DBG_APP_BOOT("%s\n", buffer);

	pinMode(c_linkISR::pin(), INPUT); //_PULLUP ? _PULLDOWN? INPUT SOFT INTERRUPT RISING UP !!!

	pinMode(DEF_BLUE_LED, OUTPUT);
	digitalWrite(DEF_BLUE_LED, HIGH);

	pinMode(whi0PIN_wsx, OUTPUT);
	digitalWrite(whi0PIN_wsx, LOW);

	pinMode(bro1PIN_udp, OUTPUT);
	digitalWrite(bro1PIN_udp, LOW);

	pinMode(red2PIN_serial, OUTPUT);
	digitalWrite(red2PIN_serial, LOW);

	pinMode(ora3PIN_imu, OUTPUT);
	digitalWrite(ora3PIN_imu, LOW);

	pinMode(yel4PIN_Rpwm, OUTPUT);
	digitalWrite(yel4PIN_Rpwm, LOW);

	pinMode(gre5PIN_Rsns, OUTPUT);
	digitalWrite(gre5PIN_Rsns, LOW);

	pinMode(blu6PIN_Lpwm, OUTPUT);
	digitalWrite(blu6PIN_Lpwm, LOW);

	pinMode(vio7PIN_Lsns, OUTPUT);
	digitalWrite(vio7PIN_Lsns, LOW);

/// BITSCOPE pins testing -----------------------------------------
	if (CTX.SOFTctx.delayPINStest)
		testPINS();
///********************************************************************
/// HTML & WEB SOCKETS SERVERS
///
	buffer = allocBuf(DEF_DBGMSG_BUFSIZE, &cnt);
	SPrintF_tilte(DEF_DBGMSG_BUFSIZE, cnt, buffer, "#### HTML & WEB SOCKETS servers");
	if (!myWS.Begin())
		halt("\n\tWEB SOCKET service ERROR\n");
	for (size_t i = 0; i < wsMax; i++)
		WSX[i] = new c_WS();
	if (!webInit(server, webSokSrv, onWSMessage))
		halt("\n\tHTML SERVER don't start");
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer, "\n\tInit OK (%i client web sockets max)", wsMax);
// TASK MESSAGE SENDER VIA WEBSOCKET ==================================
#define DEF_TASK_SEND_WS
#ifdef DEF_TASK_SEND_WS
	launchTASK(e_tasks::WSND, wsnd_HDL, "Wsnd", &wsnd_TASK, nullptr,
		&CTX.SOFTctx.TSK_DBGMAX_SEND, 7000, 1, DEF_PRIO_WSND);
#endif
	delay(100);
	_DBG_APP_BOOT("\n%s\n", buffer);
/// ***********************************************************************
/// I2C checking
///
	buffer = allocBuf(DEF_DBGMSG_BUFSIZE, &cnt);
	SPrintF_tilte(DEF_DBGMSG_BUFSIZE, cnt, buffer, "#### I??C, IMU & radar starting");
	_DBG_APP_BOOT("%s\n", buffer);
	myKMDS.reset_ISR();
	imu9250 = new H_AccGyrMag(DRIVERcfg.IMU, &CTX.HARDctx.ximuCTX,
		DRIVERcfg.imuCnam, &CTX.SOFTctx.IMU____DBGMAX_, true);
/// ***********************************************************************
/// MODULES
///
	buffer = allocBuf(DEF_DBGMSG_BUFSIZE, &cnt);
	SPrintF_tilte(DEF_DBGMSG_BUFSIZE, cnt, buffer, "#### MODULES insertion");
	qFlow = new c_myFlow("FLOW", &CTX.SOFTctx.FLOW___DBGMAX_);
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer, "\n\tJSON flow(query/reply) control ready");
	_DBG_APP_BOOT("\n%s\n", buffer);
	buffer = allocBuf(DEF_DBGMSG_BUFSIZE, &cnt);
	/// TMS module ------------------------------------------------------------
	modulesMap.insert(std::pair<const char*, Module*>(TIMEMScfg.MOD.Mid.c_str(),
		new Synchro(qFlow, TIMEMScfg, &CTX.SOFTctx.MOD_DBGMAX_TMS)));
	/// MOTORS module ---------------------------------------------------------
	modulesMap.insert(std::pair<const char*, Module*>(MOTORScfg.MOD.Mid.c_str(),
		new Motors(qFlow, MOTORScfg.MOD, &CTX.SOFTctx.MOTctx, new_pwmMotors(),
			new H_pidMotors(&asmALL.PIDs, MOTORScfg.linPID, MOTORScfg.rotPID,
				&CTX.HARDctx.xpidCTX, MOTORScfg.pidCnam.c_str(),
				&CTX.SOFTctx.PID____DBGMAX_, true),
			new H_speedSensorMotors("speed", &CTX.SOFTctx.SPEED__DBGMAX_, true),
			new H_setpointSensorMotors("joystick", &CTX.SOFTctx.JSTICK_DBGMAX_, true),
			&CTX.SOFTctx.MOD_DBGMAX_MOT)));
	pwmMotors->attachPWMpins();
	/// WATCHDOG module -------------------------------------------------------
	modulesMap.insert(std::pair<const char*, Module*>(WATCHDOGcfg.MOD.Mid.c_str(),
		new Watchdog(qFlow, WATCHDOGcfg, &CTX.SOFTctx.WATctx,
			&CTX.SOFTctx.MOD_DBGMAX_WAT)));
	/// JOBGOAL module --------------------------------------------------------
	modulesMap.insert(std::pair<const char*, Module*>(JOBGOALcfg.MOD.Mid.c_str(),
		new Jobgoal(qFlow, JOBGOALcfg, &CTX.SOFTctx.MOD_DBGMAX_JOB)));
	/// DRIVER module ---------------------------------------------------------
	/// Must be the last because uses motors & jobgoal
//	modulesMap.insert(std::pair<const char*, Module*>(DRIVERcfg.MOD.Mid.c_str(), driver()));

	modulesMap.insert(std::pair<const char*, Module*>(DRIVERcfg.MOD.Mid.c_str(),
		new Driver(qFlow, DRIVERcfg.MOD, &CTX.SOFTctx.DRVctx,
			new Kine_1(&CTX.HARDctx.kineCTX, "imu_kine", &CTX.SOFTctx.KINE___DBGMAX_),
			new Kine_1(&CTX.HARDctx.kineCTX, "drv_Kine", &CTX.SOFTctx.KINE___DBGMAX_),
			imu9250,
			new H_ultraSounds(DRIVERcfg.usdCnam, &CTX.SOFTctx.USD____DBGMAX_, true),
			&CTX.SOFTctx.MOD_DBGMAX_DRV)));
	/*
	 Kine_1 *ik =;
	 Kine_1 *dk =;
	 imu9250 = new H_AccGyrMag(DRIVERcfg.IMU, &CTX.HARDctx.ximuCTX,
	 DRIVERcfg.imuCnam, &CTX.SOFTctx.IMU____DBGMAX_, true);
	 H_ultraSounds *usd = new H_ultraSounds(DRIVERcfg.usdCnam,
	 &CTX.SOFTctx.USD____DBGMAX_, true);
	 Module *m = new Driver(qFlow, DRIVERcfg.MOD, &CTX.SOFTctx.DRVctx, ik, dk, imu9250, usd,
	 &CTX.SOFTctx.MOD_DBGMAX_DRV);
	 */
	/// ***********************************************************************
	SPrintF(DEF_DBGMSG_BUFSIZE, cnt, buffer,
		"\n\n\tMODULES mapping ---------------------------------------\n%s\n",
		dumpAllMODULES().c_str());
	_DBG_APP_BOOT("\n%s\n", buffer);
/// ***********************************************************************
/// TASK definitions
/// LOG test tasks --------------------------------------------------------
//#define DEF_TASK_LOG
#ifdef DEF_TASK_LOG
launchTASK(e_tasks::YLOG, ylog_HDL, "Ylog", &ylog_TASK, nullptr,
	&CTX.SOFTctx.TSK_DBGMAX_LOGS, 4000, 1, DEF_PRIO_LOGS);
launchTASK(e_tasks::ZLOG, zlog_HDL, "Zlog", &zlog_TASK, nullptr,
	&CTX.SOFTctx.TSK_DBGMAX_LOGS, 4000, 1, DEF_PRIO_LOGS);
/*
 */
#endif
/// KERN task -------------------------------------------------------------
#define DEF_TASK_MOD
#ifdef DEF_TASK_MOD
	launchTASK(e_tasks::CORE, core_HDL, "Core", &core_TASK, nullptr,
		&CTX.SOFTctx.TSK_DBGMAX_CORE, 12000, 1, DEF_PRIO_CORE);
#endif
///	XIMU task -------------------------------------------------------------
#define DEF_TASK_IMU
#ifdef DEF_TASK_IMU
	if (CTX.SOFTctx.DRVctx.simul_IMU) {
		launchTASK(e_tasks::XIMU, ximu_HDL, "Simu", &simu_TASK, findModule("DRV"),
			&CTX.SOFTctx.TSK_DBGMAX_XIMU, 10000, 1, DEF_PRIO_XIMU);
		_SERIAL_1("\n\tIMU simulation !!!!!!");
	} else {
		c_linkISR::Begin();
		launchTASK(e_tasks::XIMU, ximu_HDL, "Iimu", &iimu_TASK, findModule("DRV"),
			&CTX.SOFTctx.TSK_DBGMAX_XIMU, 8000, 1, DEF_PRIO_XIMU);
	}
	delay(100);
#endif
	_DBG_APP_BOOT(
		"\n#### TASKS mapping #################################################\n\n%s\n\n#### HEAP bytes %s\n%s%s\n",
		dump_TASKS().c_str(), heapState().c_str(), "\n#### ROVER starts at ",
		datim(micro_TS(), 2).c_str());
#ifdef DEF_TASK_IMU
	_SERIAL_1("\n#### True I??C IMU, ISR attached at pin %i (rising)", c_linkISR::pin());
	ATTACH(P_imuISR_); // attachInterrupt(hardPIN_isr, iimu_ISR, RISING); interrupt pin output of MPU9250
#endif
/// ***********************************************************************
/// END OF SETUP
///
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
//delay(1000);return;
//_SERIAL_0("\n*** void loop() starts***********************");
// Pass to CTX.SOFTctx.FCT_DBGMAX_LOOP (Aslm = App main setup loop)
	static int8_t &this_DebugLVL = *registerFCT(e_tasks::LOOP, "Loop",
		&CTX.SOFTctx.FCT_DBGMAX_LOOP);
	uint64_t lastClean = milli_TS();
//FIX_ERRORS();
	myKMDS.KMDS(myKMDS.has_KMD());
	vTaskDelay(loop_Delay / portTICK_PERIOD_MS);
	uint64_t newTMS = milli_TS();
}
///////////////////////////////////////////////////////////////////////

