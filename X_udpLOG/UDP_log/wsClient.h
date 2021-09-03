#ifndef __I_WSCLIENT__
#define __I_WSCLIENT__
/** @file */
#include <stdio.h>
#include <string>
//#include <string.h>
#include <fstream>

#include "../UDP_log/ws.h"
/**
 * \class socketWSclient
 */
class socketWSclient: public socketWS {
private:
	//struct sockaddr_in _servaddr;
public:
	socketWSclient(const std::string NAME, const string IP, const int PORT,
			uint16_t TIMEOUT, const int dbgLvlMax);
};
#endif
