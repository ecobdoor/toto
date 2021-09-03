#include "../UDP_log/wsClient.h"

#include <iostream>
#include <unistd.h>
#include <stdio.h>
//#include "../EXCEPT/except.h"
//#include "../../CORE/c_APP.h"
using namespace std;
//using namespace cv;

socketWSclient::socketWSclient(const std::string NAME, const string IP,
		const int PORT, uint16_t TIMEOUT,const int dbgLvlMax ) :
		socketWS(NAME, AF_INET, SOCK_STREAM, 0, TIMEOUT) {
	try {
	} catch (std::exception const& e) {
		throw;
		cerr << "ERREUR   : " << e.what() << endl;
	}
	setRecvBufSiz(DEF_UDP_SOCKET_BUFMAX);
//	memset(&_servaddr, 0, sizeof(_servaddr));
	connect2server( IP, PORT) ;
/*
	// Filling server information
	_servaddr.sin_family = AF_INET;
	_servaddr.sin_port = htons(PORT);
	_servaddr.sin_addr.s_addr = inet_addr(IP.c_str()); //INADDR_ANY;
	*/
//	__DEBUG _(4, "~", " :%s connect to %s:%i\n", _name.c_str(), IP.c_str(),PORT);
}
/*
void socketUDPclient::clear() {
	socketUDP::clear(_servaddr);
}
*/
