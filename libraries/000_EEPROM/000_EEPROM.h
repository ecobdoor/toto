#ifndef __000_EEPROM__
#define __000_EEPROM__
/** @file */
#include "EEPROM.h"
//#include "000_UTL_SYS_BASIC.h"
//#include "000_LOGS.h"
///////////////////////////////////////////////////////////////////////
template<typename T>
bool Ebegin( T & EP,char * ENAME) {
	return EP.begin(EP.length());
	/*
	if (ok)
		//_DEBUG_(0,0, Lang.CST(EEPROM_OK),ENAME);
	else
		//_DEBUG_(0,0, Lang.CST(EEPROM_FAIL),ENAME);
	return ok;
	*/
}
template<typename S>
bool  Esave(EEPROMClass & EP,S & DATA, const char CTX='\0') {
	DATA.TMS=esp_timer_get_time();
	if(CTX!='\0')
		DATA.Ename[5]=CTX;
	EP.put(0,DATA);
	EP.commit();
	return true;
	//_DEBUG_(0,0, Lang.CST(EEPROM_OK_SAVE),DATA.Ename,datim(DATA.TMS).c_str(),DATA.TMS);
}
template<typename S>
bool  Eload(EEPROMClass & EP,S & DATA) {
	EP.get(0,DATA);
	return true;
	//_DEBUG_(0,0, Lang.CST(EEPROM_OK_LOAD),DATA.Ename,datim(DATA.TMS).c_str(),DATA.TMS);
}
///////////////////////////////////////////////////////////////////////
  #endif //__000_EEPROM__
///////////////////////////////////////////////////////////////////////
