//#include <WiFiClient.h>
#include "WiFiClient.h"
#include "esp32_FTP_client.h"
///////////////////////////////////////////////////////////////////////
#define _DBG_(format,...) \
	do { \
		if(verbose == 2) \
			Serial.printf(format,## __VA_ARGS__); \
	}while(0)
#define _DBG_COMMENT_(format,...) \
	do { \
		if(verbose == 2) \
		Serial.printf("\n- "); \
		Serial.printf(format,## __VA_ARGS__); \
	}while(0)
#define _DBG_ERR_(format,...) \
	do { \
		if(verbose > 0) \
			Serial.printf(format,## __VA_ARGS__); \
	}while(0)
///////////////////////////////////////////////////////////////////////
ESP32_FTPClient::ESP32_FTPClient(const char *_serverAdress, const uint16_t _port,
	const char *_userName, const char *_passWord, const uint16_t _timeout, const uint8_t _verbose){
	userName = _userName;
	passWord = _passWord;
	serverAdress = _serverAdress;
	port = _port;
	timeout = _timeout;
	verbose = _verbose;
}
//---------------------------------------------------------------------
ESP32_FTPClient::ESP32_FTPClient(const char *_serverAdress, const char *_userName,
	const char *_passWord, const uint16_t _timeout, const uint8_t _verbose){
	userName = _userName;
	passWord = _passWord;
	serverAdress = _serverAdress;
	port = 21;
	timeout = _timeout;
	verbose = _verbose;
}
//---------------------------------------------------------------------
WiFiClient* ESP32_FTPClient::GetDataClient(){
	return &dataChannel;
}
//---------------------------------------------------------------------
bool ESP32_FTPClient::waitforAnswer(WiFiClient &CHANNEL){
	uint32_t tend = millis() + timeout;
	while ((!CHANNEL.available()) && (millis() < tend))
		delay(5);
	return CHANNEL.available();
}
//---------------------------------------------------------------------
bool ESP32_FTPClient::isConnected(const bool VERBOSE){
	if ((!_isConnected) && (VERBOSE))
		_DBG_ERR_("\n! ERR not connected:[%s]", outBuf);
	return _isConnected;
}
//---------------------------------------------------------------------
bool ESP32_FTPClient::cliCMD(const char *CMD, const char *PARAM, char *RESULT, int OFFSET){
	Serial.printf("\n> cmd:%s(%s)", CMD, PARAM);
	if (!_isConnected)
		return isConnected();
	if (PARAM[0])
		ctrlChannel.printf("%s %s\n", CMD, PARAM);
	else
		ctrlChannel.printf("%s\n", CMD);
	return GetFTPAnswer(RESULT, OFFSET);
}
//---------------------------------------------------------------------
bool ESP32_FTPClient::GetFTPAnswer(char *result, int offsetStart){
	if (!waitforAnswer(ctrlChannel)) {
		strcpy(outBuf, "CTRL offline");
		_isConnected = false;
		return isConnected();
	}

	memset(outBuf, 0, sizeof(outBuf));
	char thisByte;
	outCount = 0;

	while (ctrlChannel.available()) {
		thisByte = ctrlChannel.read();
		if (outCount < sizeof(outBuf) - 1) {
			outBuf[outCount] = thisByte;
			outCount++;
		}
	}
	char sav = outBuf[strlen(outBuf) - 1];
	outBuf[strlen(outBuf) - 1] = 0;
	Serial.printf("\n< '%s'", outBuf);
	outBuf[strlen(outBuf) - 1] = sav;
	if (outBuf[0] == '4' || outBuf[0] == '5') {
		_isConnected = false;
		return isConnected();
	} else {
		_isConnected = true;
	}

	if (result != NULL) {
		_DBG_COMMENT_("Result start");
		// Deprecated
		for (int i = offsetStart; i < sizeof(outBuf); i++) {
			result[i] = outBuf[i - offsetStart];
		}
		_DBG_("\n<[%s]", outBuf);
	}
	return isConnected();
}
//---------------------------------------------------------------------
bool ESP32_FTPClient::ContentList(const char *dir, std::vector<String> &list){
	char _resp[sizeof(outBuf)];
	uint16_t _b = 0;

	if (!cliCMD("MLSD", dir)) //; //, _resp))
		return false;
	uint8_t cnt = 0;
	for (size_t i = 0; i < strlen(outBuf); i++) {
		Serial.printf("\n%3i %c(%i)", i, outBuf[i], cnt);
		if (outBuf[i] == '\n')
			cnt++;
	}
	// Convert char array to string to manipulate and find response size
	// each server reports it differently, TODO = FEAT
	//String resp_string = _resp;
	//resp_string.substring(resp_string.lastIndexOf('matches')-9);
	//_DBG_COMMENT_(resp_string);

	if (!waitforAnswer(dataChannel)) {
		strcpy(outBuf, "DATA offline");
		_isConnected = false;
		return isConnected();
	}
	while (dataChannel.available()) {
		String chn = dataChannel.readStringUntil('\n');
		list.push_back(chn);
	}
//
	Serial.printf("N°lines(%i)", cnt);
	if (cnt == 1)
		return GetFTPAnswer(); //number of files sometimes arrives before with 150 Accepted...
	else
		return true;
}
//---------------------------------------------------------------------
bool ESP32_FTPClient::InitFile(const char *type){
	if (!cliCMD("TYPE", type))
		return false;
	if (!cliCMD("PASV", ""))
		return false;

	char *tStr = strtok(outBuf, "(,");
	int array_pasv[6];
	for (int i = 0; i < 6; i++) {
		tStr = strtok(NULL, "(,");
		if (tStr == NULL) {
			_DBG_COMMENT_("Bad PASV Answer");
			CloseConnection();
			return false;
		}
		array_pasv[i] = atoi(tStr);
	}
	unsigned int hiPort, loPort;
	hiPort = array_pasv[4] << 8;
	loPort = array_pasv[5] & 255;
	hiPort = hiPort | loPort;

	IPAddress pasvServer(array_pasv[0], array_pasv[1], array_pasv[2], array_pasv[3]);

	_DBG_COMMENT_("Data connect to : %s:%i", pasvServer.toString().c_str(), hiPort);
	if (!dataChannel.connect(pasvServer, hiPort, timeout)) {
		_DBG_COMMENT_("Wifi data channel not connected ");
		return false;
	}
	_DBG_COMMENT_("Data channel established");
	while (!dataChannel.connected()) {
		Serial.print(".");
		delay(10);
	}
	return true; //GetFTPAnswer();
}
//---------------------------------------------------------------------
bool ESP32_FTPClient::OpenConnection(){
	_DBG_("\n> Wifi connecting to: %s:%i", serverAdress, port);
	//connect return always >0 int else if DNS vs WiFiGenericClass::hostByName return 0
	if (!ctrlChannel.connect(serverAdress, port, timeout)) {
		_DBG_COMMENT_("Wifi control channel not connected ");
		return false;
	}
	_DBG_COMMENT_("Control channel established");
	while (!ctrlChannel.connected()) {
		Serial.print(".");
		delay(10);
	}

	if (!GetFTPAnswer())
		return false;
	if (!cliCMD("USER", userName))
		return false;
	if (!cliCMD("PASS", passWord))
		return false;
	return cliCMD("SYST", "");
}
//---------------------------------------------------------------------
bool ESP32_FTPClient::GetLastModifiedTime(const char *fileName, char *result){
	return cliCMD("MDTM", fileName, result, 4);
}
//---------------------------------------------------------------------
int ESP32_FTPClient::WriteData(uint8_t *data, int dataLength){
//	_DBG_COMMENT_("Writing");
	if (!isConnected())
		return 0;
	return dataChannel.write(data, dataLength);
}
//---------------------------------------------------------------------
bool ESP32_FTPClient::CloseFile(){
	_DBG_COMMENT_("Close File");
	dataChannel.stop();

	if (!_isConnected)
		return false;

	return GetFTPAnswer();
}
//---------------------------------------------------------------------
bool ESP32_FTPClient::Write(const char *str){
	_DBG_COMMENT_("Write File");
	if (!isConnected())
		return false;

	return GetDataClient()->print(str);
}
//---------------------------------------------------------------------
bool ESP32_FTPClient::CloseConnection(){
	if (!cliCMD("QUIT", ""))
		return false;
	ctrlChannel.stop();
	_DBG_COMMENT_("Connection closed");
	return true;
}
//---------------------------------------------------------------------
bool ESP32_FTPClient::RenameFile(char *from, char *to){
	if (!cliCMD("RNFR", from))
		return false;
	return cliCMD("RNTO", to);
}
//---------------------------------------------------------------------
bool ESP32_FTPClient::NewFile(const char *fileName){
	return cliCMD("STOR", fileName);
}
//---------------------------------------------------------------------
bool ESP32_FTPClient::AppendFile(char *fileName){
	return cliCMD("APPE", fileName);
}
//---------------------------------------------------------------------
bool ESP32_FTPClient::ChangeWorkDir(const char *dir){
	return cliCMD("CWD", dir);
}
//---------------------------------------------------------------------
bool ESP32_FTPClient::DeleteFile(const char *file){
	return cliCMD("DELE", file);
}
//---------------------------------------------------------------------
bool ESP32_FTPClient::MakeDir(const char *dir){
	if (!isConnected())
		return false;
	return cliCMD("MKD", dir);
}
//---------------------------------------------------------------------
bool ESP32_FTPClient::ContentListWithListCommand(const char *dir, std::vector<String> &list){
	char _resp[sizeof(outBuf)];
	if (!cliCMD("LIST", dir, _resp))
		return false;

	// Convert char array to string to manipulate and find response size
	// each server reports it differently, TODO = FEAT
	//String resp_string = _resp;
	//resp_string.substring(resp_string.lastIndexOf('matches')-9);
	//_DBG_COMMENT_(resp_string);

	unsigned long _m = millis();
	while (!dataChannel.available() && millis() < _m + timeout)
		delay(1);
	while (dataChannel.available()) {
		String chn = dataChannel.readStringUntil('\n');
		list.push_back(chn);
	}
	return true;
}
//---------------------------------------------------------------------
bool ESP32_FTPClient::DownloadString(const char *filename, String &str){
	if (!cliCMD("RETR", filename))
		return false;

	char _resp[sizeof(outBuf)];
	if (!GetFTPAnswer(_resp))
		return false;

	unsigned long _m = millis();
	while (!GetDataClient()->available() && millis() < _m + timeout)
		delay(1);

	while (GetDataClient()->available()) {
		str += GetDataClient()->readString();
	}
	return true;
}
//---------------------------------------------------------------------
bool ESP32_FTPClient::DownloadFile(const char *filename, unsigned char *buf, size_t length,
	bool printUART){
	if (!cliCMD("RETR", filename))
		return false;

	char _resp[sizeof(outBuf)];
	if (!GetFTPAnswer(_resp))
		return false;

	char _buf[2];

	unsigned long _m = millis();
	while (!dataChannel.available() && millis() < _m + timeout)
		delay(1);

	while (dataChannel.available()) {
		if (!printUART)
			dataChannel.readBytes(buf, length);
		else {
			for (size_t _b = 0; _b < length; _b++) {
				dataChannel.readBytes(_buf, 1), Serial.print(_buf[0], HEX);
			}
		}
	}
	return true;
}
///////////////////////////////////////////////////////////////////////
