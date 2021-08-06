/** @file */
#ifndef __ESP32_HTTP_SERVER__
#define __ESP32_HTTP_SERVER__
#include "Arduino.h"
///////////////////////////////////////////////////////////////////////
class c_browser {
private:
	volatile bool _running;/** < indicator to freeze main loop !!!volatile!!!*/
public:
	c_browser();
	esp_err_t Off(const esp_err_t ESP_ERR=0);
	void On();
	bool Running();
};
inline esp_err_t c_browser::Off(const esp_err_t ESP_ERR){
	Serial.printf("\nbrowser.Off(%X)",ESP_ERR);
	_running = false;
	return ESP_ERR;
}
inline void c_browser::On(){
	_running = true;
	Serial.printf("\nbrowser.On()");
	delay(1000); // To wait for main loop free the heap...
}
inline bool c_browser::Running(){
	return _running;
}
bool SETUP_HTTPSERVER();
///////////////////////////////////////////////////////////////////////
extern c_browser browser;
#endif
