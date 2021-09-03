/** @file */
#pragma once
#include "esp32_DEBUG.h" 
#include <esp32_FSTR.h>
#include <esp32_TASKS.h>
#include <000_WSCHK.h>
using namespace std;
#define DEF_JEXPL_BUFSIZE 128
#define DEF_WSMSG_BUFSIZE 1020
#define inMSGqueueSIZE  10

typedef struct {
	uint32_t msgID = 99999999;
	bool showWSdump = true;
	protoType protout = "***";
	char *outMSG = nullptr;
	uint16_t msgLEN = 0;
} s_WS_sendMSG;
#define LENINWS 200
typedef struct {
	uint16_t msgLEN = 0;
	char inMSG[LENINWS];
} s_WS_recvMSG;
//typedef s_logITM *pudpx; /** < @todo !!! pour aller avec classe public pdatagram avec aussi pwsx vers les fifos de régulation de 000_DATAG.h vs flow (datagramme) */
class c_myWS: public Core {
private:
	QueueHandle_t WS_sendMSG_queue;
	QueueHandle_t WS_recvMSG_queue;
	s_WS_recvMSG rcvItm;
	void inAppend(uint8_t *data, const size_t len);

public:
	c_myWS();
	bool Begin();
	QueueHandle_t recv_queue();
	/**
	 * @fn uint8_t push_WS(const char *PROTOCOL, const uint32_t CALLS,
	 char *BUFFER, const int16_t CNT, const bool SHOW_DUMP = true)
	 * @brief pushes a json object to the web-socket output fifo
	 * @param PROTOCOL used to find the matching web socket
	 * @param CALLS number
	 * @param BUFFER to send
	 * @param CNT length of buffer
	 * @param SHOW_DUMP flag to optionally dump the output
	 * @return number of messages waiting in the output fifo after done push
	 */
	uint8_t push_WS(const char *PROTOCOL, const uint32_t CALLS,
		char *BUFFER, const int16_t CNT, const bool SHOW_DUMP = true);
	/**
	 * @fn uint8_t pull_WS(s_WS_sendMSG *ITEM)
	 * @brief pulls a json object from the web-socket output fifo
	 * @param ITEM pointer where to pull s_WS_sendMSG
	 * @return number of messages waiting in the output fifo before pull is done (0 if empty)
	 */
	uint8_t pull_WS(s_WS_sendMSG *ITEM);
	uint8_t peek_WS(s_WS_sendMSG *ITEM);
	uint8_t waitingRecv();
	uint8_t waitingSend();
	//	uint8_t peek(s_WS_sendMSG *ITEM);
	void sendJSON(const String MID, const char *PROTOCOL, const bool FORCEDUMP = false);
	void wsnd_websock_MSG();
	void ws_recvMSG(AsyncWebSocket *server, AsyncWebSocketClient *client,
		AwsEventType type, void *arg, uint8_t *data, size_t len);
};
inline QueueHandle_t c_myWS::recv_queue(){
	return WS_recvMSG_queue;
}
inline uint8_t c_myWS::waitingRecv(){
	return uxQueueMessagesWaiting(WS_recvMSG_queue);
}
inline uint8_t c_myWS::waitingSend(){
	return uxQueueMessagesWaiting(WS_sendMSG_queue);
}

void onWSMessage(AsyncWebSocket *server, AsyncWebSocketClient *client,
	AwsEventType type, void *arg, uint8_t *data, size_t len);
String natureMSG(const String &TYP, const String &TIK);
///////////////////////////////////////////////////////////////////////
extern c_myWS myWS;
///////////////////////////////////////////////////////////////////////
