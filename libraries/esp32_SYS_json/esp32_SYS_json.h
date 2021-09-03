#pragma once
#define ARDUINOJSON_USE_LONG_LONG 1
#include "ArduinoJson.h"
///////////////////////////////////////////////////////////////////////
// Approximately 32 bytes by field
typedef StaticJsonDocument<JSON_OBJECT_SIZE(50) + 60> t_jsonDoc;
///////////////////////////////////////////////////////////////////////
enum class e_hasMsg {
	No, ///< no message to send
	Ack, ///< a single acknowledge message to send
	Full, ///< a completed message to send
};
static String ShasMsg(const e_hasMsg HAS){
	if (HAS == e_hasMsg::No)
		return String("NO  ");
	else if (HAS == e_hasMsg::Ack)
		return String("ACK ");
	else if (HAS == e_hasMsg::Full)
		return String("FULL");
	else
		return String("+-+-");
}
///////////////////////////////////////////////////////////////////////
extern t_jsonDoc *_jsonDoc;
///////////////////////////////////////////////////////////////////////
//String dumpJSON();

static String dumpJSON(){
	char buffer[512];
	serializeJson(*_jsonDoc, &buffer, 511);
	return String(buffer);
}
///////////////////////////////////////////////////////////////////////
