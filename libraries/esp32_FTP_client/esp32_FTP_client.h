/** @file */
#ifndef __000_ESP32_FTPCLI__
#define __000_ESP32_FTPCLI__
///////////////////////////////////////////////////////////////////////
#include "WiFiClient.h"
#include <vector>
///////////////////////////////////////////////////////////////////////
/**
 * @class ESP32_FTPClient
 */
class ESP32_FTPClient {
private:
	char outBuf[256];
	unsigned char outCount;
	WiFiClient ctrlChannel;
	WiFiClient dataChannel;
	uint8_t verbose;
	const char *userName;
	const char *passWord;
	const char *serverAdress;
	uint16_t port;/** < communication port */
	bool _isConnected = false;
	uint32_t timeout = 10000;
	WiFiClient* GetDataClient();
	template<typename T>
	void FTPdbgn(T msg){
		if (verbose == 2)
			Serial.println(msg);
	}
	/**
	 * @fn bool waitforAnswer(WiFiClient &CHANNEL)
	 * @brief waits upto time out for at least 1 byte to be available for reception on the channel
	 * @param CHANNEL (data or control)
	 * @return true if the time out is not reached, false otherwise
	 */
	bool waitforAnswer(WiFiClient &CHANNEL);
	/**
	 * @fn bool GetFTPAnswer(char *result = nullptr, int offsetStart = 0)
	 * @brief Gets on control channel the response to a sent command
	 * @param result buffer for response
	 * @param offsetStart position in buffer to start response
	 * @return true if no timeout, false otherwise
	 */
	bool GetFTPAnswer(char *result = nullptr, int offsetStart = 0);
	/**
	 * @fn cliCMD(const char *CMD, const char *PARAM, char *RESULT = nullptr, int OFFSET = 0)
	 * @brief Sends a command & recieves the response
	 * @param CMD ftp command
	 * @param PARAM command parameters if any, "" otherwise
	 * @param RESULT buffer
	 * @param OFFSET buffer offset to get response
	 * @return true if no timeout, false otherwise
	 */
	bool cliCMD(const char *CMD, const char *PARAM, char *RESULT = nullptr, int OFFSET = 0);

public:
	ESP32_FTPClient(const char *_serverAdress, const uint16_t _port, const char *_userName,
		const char *_passWord, const uint16_t _timeout = 10000, const uint8_t _verbose = 2);
	ESP32_FTPClient(const char *_serverAdress, const char *_userName, const char *_passWord,
		const uint16_t _timeout = 10000, const uint8_t _verbose = 2);
	bool OpenConnection();
	bool CloseConnection();
	bool isConnected(const bool VERBOSE = true);
	bool NewFile(const char *fileName);
	bool AppendFile(char *fileName);
	/**
	 * @fn int WriteData(uint8_t *data, int dataLength)
	 * @brief Sends buffer of dataLenth bytes to FTP server
	 * @param data
	 * @param dataLength
	 * @return numbers of written bytes or 0 if not connected
	 */
	int WriteData(uint8_t *data, int dataLength);
	/**
	 * @fn
	 * @brief Closes a data file channel previously open
	 * @return true if no error, false otherwise
	 */
	bool CloseFile();
	bool GetLastModifiedTime(const char *fileName, char *result);
	bool RenameFile(char *from, char *to);
	bool Write(const char *str);
	/**
	 * @fn InitFile(const char *type)
	 * @brief Opens a data channel for file uploading, downloading or others commands as MLSD vs @ref ContentList
	 * @param type "A" for {A}scii, "I" for b{I}nary
	 * @return true if no error, false otherwise
	 */
	bool InitFile(const char *type);
	/**
	 * @fn ChangeWorkDir(const char *dir)
	 * @brief Changes the ftp remote folder
	 * @param dir targeted folder
	 * @return true if no error, false otherwise
	 */
	bool ChangeWorkDir(const char *dir);
	bool DeleteFile(const char *file);
	bool MakeDir(const char *dir);
	/**
	 * @fn ContentList(const char *dir, std::vector<String> &list)
	 * @brief returns a vector of strings of the dir remote folder (files & sub-folders)
	 * @param dir targeted folder
	 * @param list response vector strings
	 * @return true if no error, false otherwise
	 */
	bool ContentList(const char *dir, std::vector<String> &list);
	bool ContentListWithListCommand(const char *dir, std::vector<String> &list);
	bool DownloadString(const char *filename, String &str);
	bool DownloadFile(const char *filename, unsigned char *buf, size_t length, bool printUART =
		false);
};
///////////////////////////////////////////////////////////////////////
#endif
