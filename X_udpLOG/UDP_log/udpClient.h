#ifndef __I_UDPCLIENT__
#define __I_UDPCLIENT__
/** @file */
#include <stdio.h>
#include <string>
//#include <string.h>
#include <fstream>

#include "../UDP_log/udp.h"
/**
 * \class socketUDP
 */
class socketUDPclient: public socketUDP {
private:
	//socketUDP &udpSocket;
public:
	socketUDPclient(const std::string NAME, const string IP, const int PORT,
		uint16_t TIMEOUT, const int dbgLvlMax);
	~socketUDPclient(){
		//printf("\n DESTROY socketUDPclient");
	}
};
#endif
