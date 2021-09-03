/** @file */
#pragma once
#include <AsyncWebSocket.h>
typedef struct {
	size_t content_len;
	size_t progress_len;
	size_t size_len;
	uint8_t error;
	String MSG;
} s_uploadControl;
bool webInit(AsyncWebServer &server, AsyncWebSocket &webSokSrv, AwsEventHandler onWSMessage);
///////////////////////////////////////////////////////////////////////
