#ifndef __I_WS__
#define __I_WS__
/** @file */
//#include "../CORE/c_APP.h"
//#include "../CORE/c_APP_Colors.h"
#if defined (WIN32)
    #include <winsock2.h>
    typedef int socklen_t;
#elif defined (linux)
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket(s) close(s)
    typedef int SOCKET;
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;
#endif
#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string>
#include <string.h>
//#include<opencv2/core/mat.hpp>
#include <sstream>
using namespace std;
#define DEF_UDP_DATA_BUFLEN 2000
#define DEF_UDP_SOCKET_BUFMAX 1000000

/**
 * \class socketWS
 */
class socketWS {
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
	socketWS(const std::string NAME, int domain, int type, int protocol,
			uint16_t TIMEOUT);
	void connect2server(const string IP, const int PORT);
	bool sendBuf(char * BUF, const uint16_t LEN, const bool checkError = true);
	int recvBuf(char * BUF, const uint16_t BUFLEN, const bool checkError = true);
	void setRecvBufSiz(const uint32_t Size);
	void setSendBufSiz(const uint32_t Size);
	void clear(const int TIMEOUT_MILLIS = 0);
	void end();
	void check();
	string readStr(const bool checkError = true);
	bool writeStr(const string TXT, const bool checkError = true);
	int readInt();
	long int readLongInt();
};
#endif
