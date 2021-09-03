#include <esp32_SYS_basic.h>
#include <sstream>
#include <esp32_DEBUG.h>
#include <vector>
//#include "FreeRTOS.h"
#include <ctime>
#include "esp32_GOODIES.h"
//#include <string.h>
using namespace std;
//	http://www.alanwood.net/unicode/enclosed_alphanumerics.html
const char blackNum[10][4] = {/*"⓿"*/"⓿", "➊", "➋", "➌", "➍", "➎", "➏", "➐", "➑", "➒" };
/*
blackNum[0][0] = 0xe2;
blackNum[0][1] = 0x93;
blackNum[0][2] = 0xbf;
blackNum[0][3] = '\0';
*/
const char whiteNum[10][4] = { "⓪", "➀", "➁", "➂", "➃", "➄", "➅", "➆", "➇", "➈" };
/*
 const char upperAlpha[26][4] = {"Ⓐ","Ⓑ","Ⓒ","Ⓓ","Ⓔ","Ⓕ","Ⓖ","Ⓗ","Ⓘ","Ⓙ","Ⓚ","Ⓛ","Ⓜ","Ⓝ","Ⓞ","Ⓟ","Ⓠ","Ⓡ","Ⓢ","Ⓣ","Ⓤ","Ⓥ","Ⓦ","Ⓧ","Ⓨ","Ⓩ"};
 const char lowerAlpha[26][4] = {"⒜","⒝","⒞","⒟","⒠","⒡","⒢","⒣","⒤","⒥","⒦","⒧","⒨","⒩","⒪","⒫","⒬","⒭","⒮","⒯","⒰","⒱","⒲","⒳","⒴","⒵"};
 */
/*
 const char blackNum[10][4] = {
 "\xE2\x97\x89",
 "\xe2\x9E\x8A",
 "\xe2\x9E\x8B",
 "\xe2\x9E\x8C",
 "\xe2\x9E\x8D",
 "\xe2\x9E\x8E",
 "\xe2\x9E\x8F",
 "\xe2\x9E\x90",
 "\xe2\x9E\x91",
 "\xe2\x9E\x92",
 };
 const char whiteNum[10][4] = {
 "\xE2\x93\xAA",
 "\xe2\x9E\x80",
 "\xe2\x9E\x81",
 "\xe2\x9E\x82",
 "\xe2\x9E\x83",
 "\xe2\x9E\x84",
 "\xe2\x9E\x85",
 "\xe2\x9E\x86",
 "\xe2\x9E\x87",
 "\xe2\x9E\x88",
 };
 */
//---------------------------------------------------------------------
uint64_t UI64S(const String CHN){
	uint64_t res = 0;
	int deb;
	if (CHN[0] == '-')
		deb = 1;
	else
		deb = 0;
	//_SERIAL_0("%s:", CHN.c_str());
	for (int i = deb; i < CHN.length(); i++) {
		//int x=CHN[i]-48;
		res = 10 * res + (uint8_t)(CHN[i] - 48);
		//_SERIAL_0("%i-%lli>",x, res);
	}
	return res;
}
//---------------------------------------------------------------------
uint32_t UI32S(const String CHN){
	uint32_t res = 0;
	int deb;
	if (CHN[0] == '+')
		deb = 1;
	else
		deb = 0;
	//_SERIAL_0("%s:", CHN.c_str());
	for (int i = deb; i < CHN.length(); i++) {
		//int x=CHN[i]-48;
		res = 10 * res + (uint8_t)(CHN[i] - 48);
		//_SERIAL_0("%i-%lli>",x, res);
	}
	return res;
}
/*
 *  18 446744073 709551615 max value !!!
 *  18 446744073 709461...
 * BBB
 */
String SUI64(const uint64_t VAL){
	std::ostringstream ss;
	ss << VAL;
	return String(ss.str().c_str());
//    return ss.str(); // for std::string !!
}
uint64_t tmsOffset = 0;
uint64_t milli_TS(){
	return esp_timer_get_time() / 1000;
}
uint64_t micro_TS(){
	return esp_timer_get_time();
}
/**
 * @fn uint64_t micro_TS()
 * @brief Measures microtime
 *
 * Because micro_TS() is called from ISR the IRAM_ATTR attribute forces to be load in ram
 * Cf. https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/spi_flash.html#iram-safe-interrupt-handlers
 * @return esp_timer_get_time() in microsecond since boot
 *//*
 uint64_t IRAM_ATTR micro_TS(){
 return (uint64_t)(esp_timer_get_time()); // (uint64_t)µicroseconds();
 }*/
/**
 * @fn char inKey(const String CHN)
 * @brief get a char from keyboard whihout waiting for if CHN=="" else wait for char
 * @param const String CHN if (CHN.length() != 0) wait for char else return immediately
 *
 * @return hitten char or '\0'
 */
char inKey(const String CHN){
	char CHAR = 0;
	if (CHN.length() == 0) {
		if (USE_SERIAL.available() > 0) // read on the fly
			CHAR = USE_SERIAL.read();
	} else {
		USE_SERIAL.print(CHN);
		while (!USE_SERIAL.available())
			// wait for char(s)
			;
		CHAR = USE_SERIAL.read();
	}
	while (USE_SERIAL.available()) { // drain the keyboard input for next time
		USE_SERIAL.read();
	}
	/*
	 if(CHAR!='\0'){
	 USE_SERIAL.print("\n--- Receive:'");
	 USE_SERIAL.print(CHAR,HEX);
	 USE_SERIAL.println("'");
	 }
	 */
	return CHAR;
}
String inString(const char *PROMPT){
	if (PROMPT != nullptr) {
		_SERIAL_0("%s", PROMPT);
		while (!(0 < USE_SERIAL.available()))
			//wait for char(s) entry
			;
	}
	uint16_t cnt = 0;
	char BUF[64];
	if (USE_SERIAL.available() > 0) {
		while (USE_SERIAL.available()) {
			BUF[cnt] = USE_SERIAL.read();
			cnt++;
			ASSERT(cnt < 64);
			delay(1); //... to reveive full line kmd when prompt
		}
		// clean end of string
		if (cnt > 0 && (BUF[cnt - 1] == '\n' || BUF[cnt - 1] == '\r'))
			cnt--;
		if (cnt > 0 && (BUF[cnt - 1] == '\n' || BUF[cnt - 1] == '\r'))
			cnt--;
	}
	BUF[cnt] = '\0';
	//if (PROMPT != nullptr)		_SERIAL_0("`%s`", BUF);
	return String(BUF);
}
//=============================================================================
uint64_t Uint64(const char *str){
	return (uint64_t)atol(str);
}
int16_t Int16(const char *str){
	return (int16_t)atol(str);
}
int64_t Int64(const char *str){
	return (int64_t)atol(str);
}
int Int(const char *str){
	return atoi(str);
}
float Float(const char *str){
	return String(str).toFloat();
}
/*
 https://github.com/espressif/arduino-esp32/tree/master/libraries/SPIFFS
 */
void SplitOnLast(const std::string str, std::string &first, std::string &second,
	const char *charPatterns){
	std::size_t found = str.find_last_of(charPatterns);
	second = str.substr(found + 1);
	first = str.substr(0, found);
}
char splitString(vector<String> &ARGS, const String &CHN, const char *SEPARATORS, const bool debug){
	ARGS.clear();
	char *writable = new char[CHN.length() + 1];
	memcpy(writable, CHN.c_str(), CHN.length() + 1);
	//_SERIAL_0("\nSizes      CHN(%i),   SEPARATORS(%i)",CHN.length(), strlen(SEPARATORS));
	//_SERIAL_0("\nSizes writable(%i)\"%s\", SEPARATORS(%i)\"%s\"",strlen(writable), writable,strlen(SEPARATORS),SEPARATORS);
	char *pch = strtok(writable, SEPARATORS);
	while (pch != NULL) {
		//_SERIAL_0("\n%s", pch);
		ARGS.push_back(String(pch));
		pch = strtok(NULL, SEPARATORS);
	}
	delete[] writable; // don't forget to free the String after finished using it
	if (debug) {
		_SERIAL_0("\n====Splitting \"%s\" with '%s'", writable, SEPARATORS);
		if (ARGS.size() == 0)
			_SERIAL_0("\n----Pas d'argument");
		else
			for (size_t i = 0; i < ARGS.size(); i++)
				_SERIAL_0("\n    %i:`%s` ", i, ARGS[i].c_str());
		_SERIAL_0("\n");
	}
	return ARGS.size() == 0 ? '\0' : ARGS[0][0];
}
Core::Core(const char *classType, int8_t *dbgLvlMax) :
	class_Type(classType), this_DebugLVL(*dbgLvlMax){
}
/**
 * @fn String datim(uint64_t TMS, const int SHIFTHOURS = 1)
 * @param TMS timestamp in micro seconds
 * @param SHIFTHOURS from GMT to country meridian
 * @return human readable date & time
 *//*
 String datim(uint64_t TMS, const int SHIFTHOURS){
 const time_t rawtime = (const time_t)(TMS / 1000000);

 struct tm *dt;
 char timestr[30];
 char buffer[30];

 dt = gmtime(&rawtime);
 dt->tm_hour = (dt->tm_hour + SHIFTHOURS) % 24;
 // use any strftime format spec here
 size_t cnt = strftime(timestr, sizeof(timestr), "%d/%m/%Y %H:%M:%S", dt);
 sprintf(buffer, "%s.%03i", timestr, (uint16_t)(TMS % 1000000));
 //std::string stdBuffer(buffer);
 return String(buffer);
 }*/
