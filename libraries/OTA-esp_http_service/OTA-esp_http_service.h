#pragma once
#include <esp_http_server.h>
///////////////////////////////////////////////////////////////////////
/**
 * @fn start_OTA_webservice(httpd_handle_t OTA_server,const char *PROGFILE)
 * @brief register handlers on server
 *
 * The number of handlers supported for all services is limited to 8 !!!
 * @param OTA_server pages server
 * @param PROGFILE bin program file name vs __FILE__ for .ino ota update
 */
void start_OTA_webservice(httpd_handle_t OTA_server,const char *PROGFILE);//,const char *fsBasePath="/spiffs");
///////////////////////////////////////////////////////////////////////
