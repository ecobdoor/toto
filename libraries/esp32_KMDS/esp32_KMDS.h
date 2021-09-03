#ifndef __esp32_KMDS__
#define __esp32_KMDS__
/** @file */
#include "Arduino.h"
//bool DATAGreceive ( const String KMD );// const Sting from AsynUdp packet
bool KMDS( char *KMD = nullptr); // for loop serial loop = Serial server
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
#endif //__000_KMDS__
