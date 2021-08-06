/*
 3171028
 3170992
 2945200 without  url_encode  
 #include <string>
 #include <iomanip>
 #include <sstream>
 using namespace std;


 3145728

 Personnalisation du fichier index

 https://www.esp32.com/viewtopic.php?t=11190
 https://github.com/Hermann-SW/Raspberry_v1_camera_global_external_shutter/tree/master/FCameraWebServer/index

 standard encode_url win
 https://translate.google.com/translate?hl=fr&sl=en&u=https://www.w3schools.com/tags/ref_urlencode.ASP&prev=search&pto=aue



 FLASH frequency = 40 MHz ????
 */

#include "CAM_SERIAL.h"
#include "esp32_GOODIES.h"
#include "CAM_CONTEXT.h"
#include "CAM_WIFI.h"
#include "CAM_FILSYS.h"
#include "CAM_FTPCLI.h"
#include "esp32_SMS_client.h"
#include "CAM_MOTION.h"// ?? before to allocate motion buffers on heap!!!!
#include "esp32_HTTP_server.h"
#include "MOTION_esp_http_service.h"
#include "ESP32_OTA-http_service.h"
///////////////////////////////////////////////////////////////////////
//#define WIFI_SSID "bdoor"
//
#define WIFI_SSID "BazUbu"
#define WIFI_PSWD "0123456789"
#define SMTP_HOST "smtp.free.fr"
#define SMTP_PORT esp_mail_smtp_port_587 // esp_mail_smtp_port_25 esp_mail_smtp_port_465 esp_mail_smtp_port_587
#define SMTP_LOGIN "testesp@free.fr"
#define SMTP_PSWD "Test1234*"
#define SMTP_USER_DOMAIN "mydomain.net" // "" || 82.66.16.48 || "mydomain.net" || "free.fr"
#define SMS_SERVER "smsapi.free-mobile.fr"  // Server URL
#define SMS_PSWD "Mg046fIyWpLmck"  // Free Mobile pass
#define SMS_USER "10790559"  // Free Mobile user
#define FTP_HOST "ftp.scadis.com"  
#define FTP_PORT 21
#define FTP_LOGIN "ju23010"
#define FTP_PSWD "uNRDWGbk"
#define CTX_RESET true
httpd_handle_t stream_httpd = nullptr;
httpd_handle_t camera_httpd = nullptr;
///////////////////////////////////////////////////////////////////////
void setup(){

	if (!SETUP_SERIAL())
		halt("Erreur SETUP_SERIAL");

	if (!SETUP_CONTEXT(CTX_RESET))
		halt("Erreur SETUP_CONTEXT");

	if (!SETUP_ESPINFO())
		halt("Erreur SETUP_ESPINFO");

	if (!SETUP_FILSYS())
		halt("Erreur SETUP_FILSYS");

	if (!SETUP_WIFI(WIFI_SSID, WIFI_PSWD))
		halt("Erreur SETUP_WIFI");

	if (!SETUP_FTPCLI( FTP_HOST, FTP_PORT, FTP_LOGIN, FTP_PSWD))
		halt("Erreur SETUP_FTPCLI");

	if (!SETUP_SMS(SMS_SERVER, SMS_USER, SMS_PSWD))
		halt("Erreur SETUP_SMS"); //SEND_SMS("Alerte à Barbe d'Or...");

	printSysState();

	if (!SETUP_CAMERA((framesize_t)DEF_CAMERA_FRAMESIZE, DEF_CAMERA_PIXEL_FORMAT))
		halt("Erreur SETUP_CAMERA");

	if (!SETUP_MOTION())
		halt("Erreur SETUP_MOTION");

	if (!SETUP_HTTPSERVER())
		halt("Erreur SETUP_HTTPSERVER");

	httpd_config_t serverConfig = HTTPD_DEFAULT_CONFIG();
	serverConfig.server_port = 80;
	serverConfig.stack_size = 9000; //8192; //8192 new class template...;//4096 wifisecureclient sms stack canary...
	/* Use the URI wildcard matching function in order to
	 * allow the same handler to respond to multiple different
	 * target URIs which match the wildcard scheme */
	serverConfig.uri_match_fn = httpd_uri_match_wildcard;
	_DEBUG_Motion_(0, "\nStarting web server on port: '%d'\n", serverConfig.server_port);
	if (httpd_start(&camera_httpd, &serverConfig) != ESP_OK)
		halt("Erreur STARTING HTML SERVER");
	serverConfig.server_port += 1;
	serverConfig.ctrl_port += 1;
	_DEBUG_Motion_(0, "Starting stream server on port: '%d'\n", serverConfig.server_port);
	if (httpd_start(&stream_httpd, &serverConfig) != ESP_OK)
		halt("Erreur STARTING STREAM SERVER");

	start_OTA_webservice(camera_httpd,__FILE__);
	start_MOTION_webservices(camera_httpd, stream_httpd);

	Serial.setDebugOutput(false);
	Serial.printf("\nCamera Ready at http://%s", WiFi.localIP().toString().c_str());
	printSysState();
	Serial.printf("\n");
	delay(2000); // to give time to start server tasks ???

}
//---------------------------------------------------------------------
void loop(){
	/* 
	 if ((motionCtx.active()) && (!browser.Running())
	 && (cameraCtx.pixFormat() != PIXFORMAT_JPEG)) {
	 uint64_t start = micro_TS();
	 camera_fb_t *fb = esp_camera_fb_get();
	 motionDetect(fb, "RUNNING");
	 esp_camera_fb_return(fb); //		fb = nullptr;
	 Serial.printf(" (%.2f f/s)", 1000000.0 / (micro_TS() - start));
	 } else
	 delay(1000);
	 */
	delay(1000);
}
///////////////////////////////////////////////////////////////////////
