/// @file
// @formatter:off
#ifdef ESP8266
	#include <Updater.h>
	#include <ESP8266mDNS.h>
#else
	#include <Update.h>
	#include <ESPmDNS.h>
#endif
// @formatter:on
#include "000_Rover_webServer.h"
#include "esp32_LOGS.h"
#include "esp32_FSTR.h"
#include "SPIFFS.h"
#include "rov.CONTEXT.h"
#include "000_WSCHK.h"
#include "rov-Kmds_000_WS3.h"
///////////////////////////////////////////////////////////////////////
s_uploadControl upCtrl = {
	.content_len = 0,
	.progress_len = 0,
	.size_len = 0,
	.error = 0,
	.MSG = "OK",
};
std::vector<String> uries {
	"/pil-Slots.js",
	"/bib-Mod.js",
	"/bib-Mod_util.js",
	"/bib-Util.js",
	"/favicon.ico",
	"/pil-APP_init.js",
	"/pil-APP_run.js",
	"/pil-Canvas.js",
	"/pil-Canvas2.js",
	"/pil-CFG.js",
	"/pil-CfgJS(000_WS3).js",
	"/pil-Dialogs.js",
	"/pil-Joystick.js",
	"/pil-Slots.js",
	"/pil-Socket.js",
	"/pil-Tpid.js",
	"/pil.css",
};
///////////////////////////////////////////////////////////////////////////////
void dumpHeaders(AsyncWebServerRequest *request){
	/*
	 request->version();       // uint8_t: 0 = HTTP/1.0, 1 = HTTP/1.1
	 request->method();        // enum:    HTTP_GET, HTTP_POST, HTTP_DELETE, HTTP_PUT, HTTP_PATCH, HTTP_HEAD, HTTP_OPTIONS
	 request->url();           // String:  URL of the request (not including host, port or GET parameters)
	 request->host();          // String:  The requested host (can be used for virtual hosting)
	 request->contentType();   // String:  ContentType of the request (not avaiable in Handler::canHandle)
	 request->contentLength(); // size_t:  ContentLength of the request (not avaiable in Handler::canHandle)
	 request->multipart();     // bool:    True if the request has content type "multipart"
	 */
	if (request->hasHeader("download"))
		_SERIAL_0("\nFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFffff");
	else
		_SERIAL_0("\nèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèè");
	int args = request->args();
	for (int i = 0; i < args; i++) {
		_SERIAL_0("\nARG[%s]: %s", request->argName(i).c_str(), request->arg(i).c_str());
	}
	_SERIAL_0("\n=================================");
	_SERIAL_0("\nrequest->version       : '%i'", request->version());
	_SERIAL_0("\nrequest->method        : '%i'", request->method());
	_SERIAL_0("\nrequest->url           : '%s'", request->url().c_str());
	_SERIAL_0("\nrequest->host          : '%s'", request->host().c_str());
	_SERIAL_0("\nrequest->contentType   : '%s'", request->contentType().c_str());
	_SERIAL_0("\nrequest->contentLength : '%i'", request->contentLength());
	_SERIAL_0("\nrequest->multipart     : '%i'", request->multipart());

	int headers = request->headers();
	int i;
	for (i = 0; i < headers; i++) {
		AsyncWebHeader *h = request->getHeader(i);
		_SERIAL_0("\nHEADER[%s]: %s", h->name().c_str(), h->value().c_str());
	}
}
/**
 * https://developer.mozilla.org/fr/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types
 * @param filname
 * @return
 */
String contentType(const String filname){
	string name;
	string ext;
	SplitOnLast(string(filname.c_str()), name, ext, ".");
	for (basic_string<char>::iterator p = ext.begin(); p != ext.end(); ++p) {
		*p = toupper(*p); // toupper is for char
	}
	if (ext == "HTML")
		return "text/html";
	else if (ext == "CSV")
		return "text/csv";
	else if (ext == "CSS")
		return "text/css";
	else if (ext == "ICO")
		return "image/x-icon";
	else if (ext == "gif")
		return "image/gif";
	else if ((ext == "JPG") || (ext == "jpg"))
		return "image/jpeg";
	else if (ext == "JS")
		return "text/javascript";
//		return "application/javascript";
	else if (ext == "JSON")
		return "application/json";
	else if (ext == "PDF")
		return "application/pdf";
	else if (ext == "PNG")
		return "image/png";
	else
		return "text/html";
}
void printProgress(size_t prg, size_t sz){
	upCtrl.progress_len = prg;
	upCtrl.size_len = sz;
	_SERIAL_0("Progress: %d%%\n", (prg * 100) / upCtrl.content_len);
}
/**
 * @fn String processor(const String &var)
 * @brief Substitute template by corresponding html content

 * @param var
 * @return String
 */
String processor(const String &var){
	String xxx;
	if (var == "UPDATE_STATUS")
		xxx = upCtrl.MSG;
	return xxx;
}
///////////////////////////////////////////////////////////////////////////////
/*
 DOWNLOAD FILES
 */
///////////////////////////////////////////////////////////////////////////////
/*
 UPLOAD FILES
 */
void handleUpload(AsyncWebServerRequest *request){
	char *html =
		"<div id='divProgress'>divProgress</div> \
		<form method='POST' action='/doUpload' enctype='multipart/form-data'> \
			<input type='file' name='upload'> \
			<input type='submit' value='Upload'> \
		</form>";
	request->send(200, "text/html", html);
}
void handleDoUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data,
	size_t len, bool final){
	if (!index) {
		upCtrl.MSG = "#### UPLOAD en cours...";
		request->_tempFile = SPIFFS.open("/" + filename, FILE_WRITE);
	}
	//for(size_t i=0; i<len; i++){		Serial.write(data[i]);	}
	if (len)
		request->_tempFile.write(data, len);
	if (final) {
		//request->file.flush();
		request->_tempFile.close();
//		request->send(200, "text/plain", "File Uploaded !");
		upCtrl.MSG = "#### UPLOAD complet";
		request->send(SPIFFS, "/index.html", "text/html", false, processor);
	}
}
///////////////////////////////////////////////////////////////////////////////
/*
 UPDATE SOFTWARE
 */
void handleUpdate(AsyncWebServerRequest *request){
	char *html =
		"<div id='divProgress'>divProgress</div> \
		<form method='POST' action='/doUpdate' enctype='multipart/form-data'> \
			<input type='file' name='update' accept='.bin'> \
			<input type='submit' value='Update'> \
		</form>";
	request->send(200, "text/html", html);
}
void handleDoUpdate(AsyncWebServerRequest *request, const String &filename, size_t index,
	uint8_t *data, size_t len, bool final){
	if (!index) {
		upCtrl.MSG = "**** UPGRADE en cours...";
		_SERIAL_0("\n-----------------------------------Update begins");
		upCtrl.content_len = request->contentLength();
		// if filename includes spiffs, update the spiffs partition
		int cmd = (filename.indexOf("spiffs") > -1) ? U_SPIFFS : U_FLASH;
#ifdef ESP8266
		Update.runAsync(true);
		if (!Update.begin(upCtrl.content_len, cmd)) {
#else
		if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd)) {
#endif
			Update.printError(Serial);
		}
	}
	upCtrl.error = Update.getError();
	if (Update.write(data, len) != len) {
		Update.printError(Serial);
#ifdef ESP8266
	} else {
		_SERIAL_0("Progress: %d%%\n", (Update.progress()*100)/Update.size());
#endif
	}
	if (final) {
		upCtrl.error = Update.getError();
		if (upCtrl.error == 0) {
			upCtrl.MSG = "**** UPGRADE complet, rover redémarre...";
		} else {
			upCtrl.MSG = "*** UPGRADE erreur !!!";
		}
		AsyncWebServerResponse *response = request->beginResponse(302, "text/plain",
			"Please wait while the device reboots");
		response->addHeader("Refresh", "20");
		response->addHeader("Location", "/");
		request->send(response);
		if (!Update.end(true)) {
			upCtrl.MSG = "UPGRADE erreur !!!";
			//yield();			delay(1000);
			Update.printError(Serial);
		} else {
			upCtrl.MSG = "UPGRADE complet, rover redémarre...";
//		    yield();			delay(1000);
			_SERIAL_0("\n-----------------------------------Update complete");
			ESP.restart();
		}
	}
}
///////////////////////////////////////////////////////////////////////
static uint8_t reqCnt = 0;
void lock(const char *PAGE){
	reqCnt++;
	_SERIAL_0("\n>%2i request %s", reqCnt, PAGE);
	vTaskSuspend(findCORE()->TH);
}
void unlock(const char *PAGE){
	vTaskResume(findCORE()->TH);
	reqCnt--;
	_SERIAL_0("\n<%2i request %s", reqCnt, PAGE);
}

bool Referer(AsyncWebServerRequest *request){
	/*
	 String ref = "Referer";
	 if (!request->hasHeader(ref))
	 return false;
	 AsyncWebHeader *referer = request->getHeader(ref);
	 request->addHeader("Access-Control-Allow-Origin", "http://192.168.13.12:3001");
	 //	request->addHeader("Access-Control-Allow-Origin", "127.0.0.1:3001");
	 request->addHeader("Access-Control-Allow-Credentials", "true");
	 AsyncWebServerResponse *response;
	 response = request->beginResponse(200, "text/plain", shouldReboot ? "OK" : "FAIL");
	 response->addHeader("Connection", "close");
	 request->send(response);
	 */
	return true;
}
/*
 if(request->hasHeader(WS_STR_PROTOCOL)){
 AsyncWebHeader* protocol = request->getHeader(WS_STR_PROTOCOL);
 BDOOR_protocol=protocol->value();
 response->addHeader(WS_STR_PROTOCOL, protocol->value());
 }
 else
 BDOOR_protocol="---";
 ///////////////////////////////////////////////////////////////////////
 request->send(response);

 */
/**
 * @fn bool webInit(AsyncWebServer &server,	AsyncWebSocket &webSokSrv, AwsEventHandler onWSMessage)
 * @brief Launches the web server
 *
 * Installs CORS cross origin to support interface calls remote pilot
 * @n addHeader("Access-Control-Allow-Origin", "http://192.168.13.12:3001");
 * @n addHeader("Access-Control-Allow-Credentials", "true");
 * @n
 * @n Responds to the queries below:
 * - /favicon.ico (pilot browser tab icon)
 * - / (pilot browser admin page vs index.html)
 * - /pil.html (pilot browser page)
 * - /pil.css (pilot browser style)
 * - /pil.js (pilot javascript script)
 * - /CENTER.jpg (pilot browser page background image)
 * - /EMERGENCY.gif (image of the emergency stop button on the pilot browser page)
 * -
 * - /update (OTA application form)
 * - /doUpdate (OTA application upgrading)
 * - /upload (upload form)
 * - /doUpload (uploading)
 * - /listFiles (spiffs files list)
 * - /workFiles (avalaible jobs to run in javascript select)
 * -
 *
 * @param server asynchronous TCP web server
 * @param webSokSrv web socket server
 * @param onWSMessage web socket listener for incoming messages
 * @return true
 */
bool webInit(AsyncWebServer &server,
	AsyncWebSocket &webSokSrv, AwsEventHandler onWSMessage){
	static int8_t &this_DebugLVL = *registerFCT(e_tasks::SERV, "Serv",
		&CTX.SOFTctx.FCT_DBGMAX_SERV);
//	PROGRESS ----------------------------------------------------------
	server.on("/progress", HTTP_GET, [](AsyncWebServerRequest *request){
		_SERIAL_0("\nprogress");
		String p = String(upCtrl.progress_len) + String("/") + String(upCtrl.content_len);
		p += String(" (max=") + String(upCtrl.size_len);
		p += String(") ") + String(upCtrl.error);
		p += String(" ") + String(upCtrl.MSG);
		request->send(200, "text/plain", p);
	});
//	UPDATE ------------------------------------------------------------
	server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request){
		handleUpdate(request);
	});
	server.on("/doUpdate", HTTP_POST, [](AsyncWebServerRequest *request){
		_SERIAL_0("++++++++++++");
	},
	[](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data,
		size_t len, bool final){ //SPIFFS.end();
		handleDoUpdate(request, filename, index, data, len, final);
	});
//	UPLOAD ------------------------------------------------------------
	server.on("/upload", HTTP_GET, [](AsyncWebServerRequest *request){
		dumpHeaders(request);
		handleUpload(request);
	});
	server.on("/doUpload", HTTP_POST,
		[](AsyncWebServerRequest *request){
			dumpHeaders(request);
		},
		[](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data,
			size_t len, bool final){
			//_SERIAL_0("---- UPLOAD \t. '%s' '%i' '%p' '%i' '%i'\n",filename.c_str(),	index, data,	len,	final);
			if (!index) {
				//	listDir_old(SPIFFS, "/", 0);
				if (SPIFFS.exists("/" + filename)) {
					File file = SPIFFS.open("/" + filename, FILE_READ);
					_SERIAL_0("---- UPLOAD \t. %7u Bytes '%s' exists -> removing\n",
						file.size(), file.name());
					file.close();
					SPIFFS.remove("/" + filename);
				}
			else
				_SERIAL_0("---- UPLOAD \t. ....... ..... '/%s' new file\n", filename.c_str());
		}
		_SERIAL_0("---- UPLOAD \t. %7u Bytes (%s)\n", index + len, final ? "complete" : "...");
		upCtrl.progress_len = index + len;
		handleDoUpload(request, filename, index, data, len, final);
	});
//	DONWLOAD ------------------------------------------------------
	server.on("/listFiles", HTTP_GET, [](AsyncWebServerRequest *request){
		dumpHeaders(request);
		request->send(200, "text/html", Lang.listDir2download(SPIFFS, "/", 0));
	});
//	CHOOSE WORK FILE ----------------------------------------------
	server.on("/workFiles", HTTP_GET, [](AsyncWebServerRequest *request){
		_SERIAL_0("\nworkFiles request");
		dumpHeaders(request);
		String f = Lang.listDir2select(SPIFFS, "/", 0);
		//Serial.printf("\n[%s]", f.c_str());
		request->send(200, "application/json", f);
	});
//	DEBUG LEVELS --------------------------------------------------
	server.on("/jsonCONTEXT", HTTP_GET, [](AsyncWebServerRequest *request){
		_SERIAL_0("\njsonCONTEXT request");
		dumpHeaders(request);
		String f = CTX.jsonCONTEXT();
		//Serial.printf("\n[%s]", f.c_str());
		request->send(200, "application/json", f);
	});
//	PAGES -------------------------------------------------------------
	server.on("/bib-Mod.js", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/bib-Mod.js");
		dumpHeaders(request);
		request->send(SPIFFS, "/bib-Mod.js", "text/javascript");
		unlock("/bib-Mod.js");
	});
	server.on("/bib-Mod_util.js", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/bib-Mod_util.js");
		dumpHeaders(request);
		request->send(SPIFFS, "/bib-Mod_util.js", "text/javascript");
		unlock("/bib-Mod_util.js");
	});
	server.on("/bib-Util.js", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/bib-Util.js");
		dumpHeaders(request);
		request->send(SPIFFS, "/bib-Util.js", "text/javascript");
		unlock("/bib-Util.js");
	});
//		HOME PAGE & ICONE
	server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/favicon.ico");
		dumpHeaders(request);
		request->send(SPIFFS, "/favicon.ico", "text/html");
		unlock("/favicon.ico");
	});
	server.on("/PILOTE", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/pil.html");
		dumpHeaders(request);
		request->send(SPIFFS, "/pil.html", "text/html");
		unlock("/pil.html");
	});
	server.on("/pil-APP_init.js", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/pil-APP_init.js");
		dumpHeaders(request);
		request->send(SPIFFS, "/pil-APP_init.js", "text/javascript");
		unlock("/pil-APP_init.js");
	});
	server.on("/pil-APP_run.js", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/pil-APP_run.js");
		dumpHeaders(request);
		request->send(SPIFFS, "/pil-APP_run.js", "text/javascript");
		unlock("/pil-APP_run.js");
	});
	server.on("/pil-Canvas.js", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/pil-Canvas.js");
		dumpHeaders(request);
		request->send(SPIFFS, "/pil-Canvas.js", "text/javascript");
		unlock("/pil-Canvas.js");
	});
	server.on("/pil-Canvas2.js", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/pil-Canvas2.js");
		dumpHeaders(request);
		request->send(SPIFFS, "/pil-Canvas2.js", "text/javascript");
		unlock("/pil-Canvas2.js");
	});
	server.on("/pil-CFG.js", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/pil-CFG.js");
		dumpHeaders(request);
		request->send(SPIFFS, "/pil-CFG.js", "text/javascript");
		unlock("/pil-CFG.js");
	});
	server.on("/pil-CfgJS(000_WS3).js", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/pil-CfgJS(000_WS3).js");
		dumpHeaders(request);
		request->send(SPIFFS, "/pil-CfgJS(000_WS3).js", "text/javascript");
		unlock("/pil-CfgJS(000_WS3).js");
	});
	server.on("/pil-Dialogs.js", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/pil-Dialogs.js");
		dumpHeaders(request);
		request->send(SPIFFS, "/pil-Dialogs.js", "text/javascript");
		unlock("/pil-Dialogs.js");
	});
	server.on("/pil-Joystick.js", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/pil-Joystick.js");
		dumpHeaders(request);
		request->send(SPIFFS, "/pil-Joystick.js", "text/javascript");
		unlock("/pil-Joystick.js");
	});
	server.on("/pil-Slots.js", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/pil-Slots.js");
		dumpHeaders(request);
		request->send(SPIFFS, "/pil-Slots.js", "text/javascript");
		unlock("/pil-Slots.js");
	});
	server.on("/pil-Socket.js", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/pil-Socket.js");
		dumpHeaders(request);
		request->send(SPIFFS, "/pil-Socket.js", "text/javascript");
		unlock("/pil-Socket.js");
	});
	server.on("/pil-Tpid.js", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/pil-Tpid.js");
		dumpHeaders(request);
		request->send(SPIFFS, "/pil-Tpid.js", "text/javascript");
		unlock("/pil-Tpid.js");
	});
	server.on("/TEST", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/tst.html");
		dumpHeaders(request);
		request->send(SPIFFS, "/tst.html", "text/html");
		unlock("/tst.html");
	});
	server.on("/tst.js", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/tst.js");
		dumpHeaders(request);
		request->send(SPIFFS, "/tst.js", "text/javascript");
		unlock("/tst.js");
	});
	server.on("/pil.js", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/pil.js");
		request->send(SPIFFS, "/pil.js", "text/javascript");
		unlock("/pil.js");
	});
	server.on("/CENTER.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/CENTER.jpg");
		dumpHeaders(request);
		request->send(SPIFFS, "/CENTER.jpg", contentType("/CENTER.jpg"));
		unlock("/CENTER.jpg");
	});
	server.on("/EMERGENCY.gif", HTTP_GET, [](AsyncWebServerRequest *request){
		lock("/EMERGENCY.gif");
		dumpHeaders(request);
		request->send(SPIFFS, "/EMERGENCY.gif", contentType("/EMERGENCY.gif"));
		unlock("/EMERGENCY.gif");
	});
	/*
	 server.on("/three.js", HTTP_GET, [](AsyncWebServerRequest *request){
	 lock("/three.js");
	 request->send(SPIFFS, "/three.js", "text/javascript");
	 unlock("/three.js");
	 });
	 */
	/*
	 for (int i = 0; i < uries.size(); i++){
	 server.on(uries[i].c_str(), HTTP_GET, [i](AsyncWebServerRequest *request){
	 _SERIAL_0("\nGET '%s'\t'%s'",uries[i].c_str(), contentType(uries[i]).c_str());
	 request->send(SPIFFS, uries[i].c_str(), contentType(uries[i]));
	 });
	 } 
	 */
	server.on("/pil.css", HTTP_GET, [](AsyncWebServerRequest *request){
		_SERIAL_0("\n@@@ /pil.css request->url().c_str()='%s'", request->url().c_str());
		if (request->params()) {
			AsyncWebParameter *p = request->getParam(0);
			lock(p->value().c_str());
			dumpHeaders(request);
			request->send(SPIFFS, p->value(), contentType(p->value()));
			unlock(p->value().c_str());
		} else {
			lock("/pil.css");
			request->send(SPIFFS, "/pil.css", "text/css");
			unlock("/pil.css");
		}
	});
	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
		if (WSA.closeAllWSX())
			_SERIAL_0("\n@@@ WS server closes all sockets");
		DETACH(P_imuISR_); // before http client !!!!!
		//dumpHeaders(request);
		_SERIAL_0("\n****************************");
		//	 https://techtutorialsx.com/2017/12/17/esp32-arduino-http-server-getting-query-parameters/
		int paramsNr = request->params();
		if (paramsNr == 0) {
			myKMDS.push("Z1000"); // to delay 1 seconds
			myKMDS.push("IA"); // attach_ISR()
			//_SERIAL_0("\n\tNo parameter => send \"index.html\"");
			lock("/index.html");
			dumpHeaders(request);
			request->send(SPIFFS, "/index.html", "text/html", false, processor);
			unlock("/index.html");
		} else {
			for (int i = 0; i < paramsNr; i++) {
				AsyncWebParameter *p = request->getParam(i);
				_SERIAL_0("\n\t%i:%s\t=[%s]", i, p->name().c_str(), p->value().c_str());
			}
			AsyncWebParameter *p = request->getParam(0);
			if (p->name() == "p") {
//				_SERIAL_0("\n\t0) param `p` => send \"%s\"", p->value().c_str());
				myKMDS.push("Z5000"); // to delay 3 seconds
				myKMDS.push("IA"); // attach_ISR()

				lock(p->value().c_str());
				dumpHeaders(request);
				request->send(SPIFFS, p->value(), contentType(p->value()), false, processor);
				unlock(p->value().c_str());
			}
		else if (p->name() == "download") {
//				_SERIAL_0("\n\t0) param `p` => send \"%s\"", p->value().c_str());
			myKMDS.push("Z5000"); // to delay 3 seconds
			myKMDS.push("IA"); // attach_ISR()

			lock(p->value().c_str());
			dumpHeaders(request);
			request->send(SPIFFS, p->value(), contentType(p->value()), true);
			unlock(p->value().c_str());
		}
		else if (p->name() == "remove") {
			SPIFFS.remove(p->value());
		}
	}
//			request->send(200, "text/plain", "message received");
	_SERIAL_0("\n----------------------------");
}	);
//	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {request->redirect("/update");});
//	NOTFOUND ----------------------------------------------------------
	server.onNotFound([](AsyncWebServerRequest *request){
		if (request->method() == HTTP_OPTIONS) { //preflight CORS (unused for GET method)
			request->send(200);
		}
		else {
			request->send(404, "text/html", "ERR 404 FILE NOT FOUND");
//		 request->send(404);
		}
	});
//	WEBSOCKET ---------------------------------------------------------
	webSokSrv.onEvent(onWSMessage);
	server.addHandler(&webSokSrv);
//	BEGIN -------------------------------------------------------------
	/**
	 * Add CORS headers
	 * if addHeader("Access-Control-Allow-Origin", "*")
	 * 		needs addHeader("Access-Control-Allow-Credentials", "false")
	 * 		and xhttp.withCredentials = false in node_pil.js jsonXHR(REQUEST)
	 * else if addHeader("Access-Control-Allow-Origin", "http://...")
	 * 		needs addHeader("Access-Control-Allow-Credentials", "true")
	 * 		and xhttp.withCredentials = true in node_pil.js jsonXHR(REQUEST)
	 */
//
	/*
	 DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "http://10.42.0.1:3001");
	 //	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "http://localhost:3001");
	 //	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "127.0.0.1:3001");

	 DefaultHeaders::Instance().addHeader(
	 String("Vary"),
	 String("Origin"));
	 DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
	 DefaultHeaders::Instance().addHeader("Access-Control-Allow-Credentials", "true");
	 */
// @formatter:off
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers","Origin, X-Requested-With, Content-Type, Accept");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods","GET, POST, PATCH, DELETE, OPTIONS");
	DefaultHeaders::Instance().addHeader(String("Access-Control-Allow-Origin"),String("http://192.168.13.67:3001"));
	//DefaultHeaders::Instance().addHeader(String("Access-Control-Allow-Credentials"),String("true"));
// @formatter:on
	server.begin();
#ifdef ESP32
	Update.onProgress(printProgress);
#endif
	return true;
}
///////////////////////////////////////////////////////////////////////
