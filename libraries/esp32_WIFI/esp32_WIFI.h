#ifndef __000_WIFI__
#define __000_WIFI__
#include <WiFi.h>
class c_myWIFI{
public:
	void Close();
	void Status();
};
///////////////////////////////////////////////////////////////////////
void WiFiEvents(WiFiClass &WiFi);
String StringMAC(const uint8_t *  MAC);
extern c_myWIFI myWIFI;
#endif
