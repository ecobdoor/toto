#ifndef __I_UDP__
#define __I_UDP__
/** @file */
//#include "../CORE/c_APP.h"
//#include "../CORE/c_APP_Colors.h"
#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <string.h>
//#include<opencv2/core/mat.hpp>
#include <sstream>
using namespace std;
#define DEF_UDP_DATA_BUFLEN 2000
#define DEF_UDP_SOCKET_BUFMAX 100000

/**
 * \class socketUDP
 */
class socketUDP {
private:
	uint32_t _len;
	uint32_t _cntRead = 0;
	uint32_t _cntWrite = 0;
	struct sockaddr_in _REMOTEADDR;
	string _lastRcvPID = "";
	string _lastSndPID = "";
	bool _isClient = false; //for reset & packet ID management differentiation

protected:
	int _sockfd;
	std::string _name;
	std::string _caller;
	//int checkError(const std::string CHN, const int ERR = 0, const int LVL = 0);
	void setRecvTimeout(uint32_t TIMEOUT);
public:
	bool isClient();
	void setCaller(const string CALLER);
	socketUDP(const std::string NAME, int domain, int type, int protocol,
			uint16_t TIMEOUT);
	~socketUDP();
	void connect2server(const string IP, const int PORT);
	bool sendBuf(char * BUF, const uint16_t LEN, const bool checkError = true);
	int recvBuf(char * BUF, const uint16_t BUFLEN, const bool checkError = true);
	void setRecvBufSiz(const uint32_t Size);
	void setSendBufSiz(const uint32_t Size);
	void clear(const int TIMEOUT_MILLIS = 0);
	void end();
	void check();
	/**
	 * @fn string socketUDP::readStr(const bool checkError = true);
	 * @brief receives a buffer & adds '\0' at the end to convert to string
	 * @param checkError
	 * @return received string
	 */
	string readStr(const bool checkError = true);
	bool writeStr(const string TXT, const bool checkError = true);
	int readInt();
	long int readLongInt();
};
#endif
