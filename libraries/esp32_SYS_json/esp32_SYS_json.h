#ifndef _esp32_SYS_json_
#define _esp32_SYS_json_
#define ARDUINOJSON_USE_LONG_LONG 1
#include "ArduinoJson.h"
///////////////////////////////////////////////////////////////////////
// Approximately 32 bytes by field
typedef StaticJsonDocument<JSON_OBJECT_SIZE(50) + 60> t_jsonDoc;
extern t_jsonDoc *_jsonDoc;
#endif
