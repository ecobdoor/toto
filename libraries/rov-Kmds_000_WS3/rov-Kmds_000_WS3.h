#pragma once
/** @file */
#include "Arduino.h"
//#include "esp32_SYS_basic.h"
#include "esp32_EXCEPT.h"
#include "esp32_DEBUG.h"
#include "esp32_GOODIES.h"
///////////////////////////////////////////////////////////////////////
#define DEF_KMDSMSGLEN 200
typedef struct {
	char kmdsMSG[DEF_KMDSMSGLEN];
} s_kmdITM;
//---------------------------------------------------------------------
class c_myKMDS: public Core {
private:
	QueueHandle_t _inKMDSqueue;

public:
	c_myKMDS(); // contructor for class_Type, &this_DebugLVL, virtual On_Error...
	/**
	 * @fn void KMDS(const char *  KMD = nullptr)
	 * @brief Launchs the command pointed by KMD. If !KMD scans serial keyboard & launchs the input command (if any)
	 *
	 * In non-interactive mode, commands are entered on the fly.
	 * - Some commands samples running from serial or udp input:
	 * 	- '?' toggles in verbose interactive dialog & display all commands syntax
	 * 	- '@' forces esp restart
	 * 	- 'D' reset kinematic data
	 * 	- '_' adjusts IMU bias
	 * 	- 'Bi', 'Ai', 'C' Begins, Appends, Closes IMU or DRIVER telemetric csv file
	 * 	- 'IA', 'ID' attaches or detaches IMU interrupts
	 * - Remote commands only from udp remote dashboard input:
	 * 	- ' ' clear udp remote dashboard (no action on application itself)
	 * - Internal commands only from application program:
	 * 	- 'Znnnn' delays next command for nnnn milliseconds (used to re-attach ISR after html pilot is load)
	 *
	 * @param KMD command
	 */
	bool KMDS(const char *KMD = nullptr); // for loop serial loop = Serial server
	void reset_ISR();
	/**
	 * @fn char* has_KMD()
	 * @brief get _inKMDSqueue command @ref KMDS(...)
	 *
	 * SPECIAL COMMANDS excluded from serial keyboard input:
	 * - ' ' local command from udp telemetry to clear the remote dashboard window
	 * - 'Znnnn' Delays the next command for nnnn milliseconds (ex: Z3000 IA)
	 * @return buffer pointer kept on stack in static s_kmdITM or nullptr if _inKMDSqueue is empty
	 */
	char* has_KMD();
	void push(char *KMD);
};
//---------------------------------------------------------------------
inline void c_myKMDS::push(char *KMD){
	ASSERT(strlen(KMD) < DEF_KMDSMSGLEN - 1);
	s_kmdITM item;
	strcpy(item.kmdsMSG, KMD);
	_SERIAL_0("\nKMDS %s", (char* )&item.kmdsMSG);
	try {
		if (pdPASS != xQueueSend(_inKMDSqueue, &item, 0))
			THROWERR(FIFO_OVERFLOW, 0, micro_TS());
	} catch (EXCEPT const &e) {
		THROW_UP();
	}
}
//bool KMDS(const char *KMD = nullptr); // for loop serial loop = Serial server
/**
 * DATAG::DATAG(jsonFlow)
 * DATAG::addHandler(Serial,spiffs,udp,ws,web)
 * DATAGreceive
 * 	Receive SPIFFS
 * 	Receive UDP
 * 	Receive WS
 * 	Receive SERIAL
 * DATAGrsend
 * 	Send SPIFFS
 * 	Send UDP
 * 	Send WS
 * 	Send SERIAL
 */
///////////////////////////////////////////////////////////////////////
extern c_myKMDS myKMDS;

