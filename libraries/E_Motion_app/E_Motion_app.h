#ifndef __E_Motion_app__
#define __E_Motion_app__
#include "esp_http_server.h"
///////////////////////////////////////////////////////////////////////
/**
 * @fn void start_MOTION_webservices(httpd_handle_t server_httpd, httpd_handle_t stream_httpd)
 * @brief register handlers on servers
 *
 * The number of handlers supported for all services is limited to 8 !!!
 * @param server_httpd pages server
 * @param stream_httpd streaming server
 */
void start_MOTION_webservices(httpd_handle_t server_httpd, httpd_handle_t stream_httpd);
///////////////////////////////////////////////////////////////////////
#endif
