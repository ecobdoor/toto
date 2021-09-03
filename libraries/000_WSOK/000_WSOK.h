/** @file */
#ifndef __000_WSOK__
#define __000_WSOK__
#include "esp32_DEBUG.h" 
#include <esp32_FSTR.h>
#include <esp32_TASKS.h>
#include <000_WSCHK.h>
using namespace std;
#define DEF_JEXPL_BUFSIZE 128
#define DEF_WSMSG_BUFSIZE 1020
typedef struct {
	uint32_t msgID = 99999999;
	s_wsChecker *pwsx = nullptr;
	char * outMSG=nullptr;
	uint16_t msgLEN = 0;
} s_OUTfifoItem;
//typedef s_logITM *pudpx; /** < @todo !!! pour aller avec classe public pdatagram avec aussi pwsx vers les fifos de régulation de 000_DATAG.h vs flow (datagramme) */
class c_myWSOK {
private:
	QueueHandle_t  _outWSOKqueue;
//	QueueHandle_t  _ackWSOKqueue;
public:
	bool Begin();
	uint8_t push(const uint32_t CALLS,  char *BUFFER, const int16_t CNT,s_wsChecker *pwsx);
	uint8_t pull(s_OUTfifoItem * ITEM);
	void sendJSON(s_wsChecker *pwsx, const String MID);
};
String Header(const String HEADER, const String REMOTE);
///////////////////////////////////////////////////////////////////////
extern c_myWSOK  myWSOK;
///////////////////////////////////////////////////////////////////////
#endif //__000_WSOK__
