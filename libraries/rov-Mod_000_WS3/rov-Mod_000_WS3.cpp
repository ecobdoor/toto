/** @file */
#include "rov-Mod_000_WS3.h"
#include "esp32_LOGS.h"
/**
 * Mapping of modules
 */
t_map_MOD modulesMap;
///////////////////////////////////////////////////////////////////////
/**
 * \class Module
 */
Module::Module(Flow *FLOW, const s_MODULE MOD_CFG, int8_t *DBGMAX)
:
	Core(MOD_CFG.Mid.c_str(), DBGMAX), _Flow(*FLOW){
	_name = MOD_CFG.Name;
	_Mid = MOD_CFG.Mid;
	_protocol = MOD_CFG.Qtype;
	_milliPeriod = MOD_CFG.loop_TMS;
	_milliLastTS = milli_TS();
	_SERIAL_2("\n$%s", dump().c_str());
	_SERIAL_3("\n-Creates module %s\t '%s' (%i ms)", _name.c_str(), _protocol,
		_milliPeriod);
}
//---------------------------------------------------------------------
bool Module::has_MSG(){
	if (!_MODon)
		return false;
	bool ok;
	//_SERIAL_6( "\n-has_MSG ");
	uint64_t milliTS = milli_TS();
	int64_t milliDT = (int64_t)(milliTS - _milliLastTS);
	try {
		if (milliDT < 0)
			_DEBUG_ERR("\n%s (DT<0) %lli=(%llu-%llu) ?", _Mid.c_str(), milliDT, milliTS,
				_milliLastTS);
		if ((_isRunning) && (_milliPeriod != 0) && ((uint64_t)_milliPeriod < milliDT)) {
			//_SERIAL_6( "\n-TimOut %s (%4llu/%4i ms)", _Mid.c_str(), milliDT, _milliPeriod);
			ok = tmsout_MSG(milliDT);
		} else {
			//_SERIAL_6( "\n-Auto_Q %s(%4llu/%4i ms)", _Mid.c_str(), dt, _milliPeriod);
			ok = auto_MSG(milliDT);
		}
	}
	catch (EXCEPT const &e) {
		//FIX_ERROR(); //
		THROW_UP();
	}
	return ok;
}
//---------------------------------------------------------------------
/**
 * \fn void Module::tmsout_MSG(const uint64_t DTMS)
 * \brief creates a default time-out json message completed by specific module informations
 *
 * \if complete_MSG generates a QUERY inserted in _Flow control
 * \else generates a reply with TIK=0
 * \param const uint64_t DTMS milliseconds since last send
 * \return true to cause sending
 */
bool Module::tmsout_MSG(const uint64_t DTMS){
//	try {
		_jsonDoc->clear();
		JsonObject root = (*_jsonDoc).to<JsonObject>();
		JsonObject CTL = root.createNestedObject("CTL");
		/*DTMS
		 if (PROTOCOL == "")
		 CTL["TYP"] = _protocol;
		 else
		 CTL["TYP"] = PROTOCOL;
		 */
		CTL["TYP"] = _protocol;
		JsonObject KMD = root.createNestedObject("KMD");
		KMD["MID"] = _Mid;
		KMD["DT"] = DTMS;
		;
		//Suppose to be either a query a simple additional information
		if (complete_MSG(KMD))
			CTL["TIK"] = _Flow.tag(milli_TS(), CTL["TYP"].as<String>()); //after because Watchdog resets the flow !!!!
		else {
			CTL["TIK"] = 0;
			CTL["TYP"] = "rRp";
		}
		//_SERIAL_4( "\n%sgenq  [%s]", String("******_________**********\t").c_str(),dumpJSON().c_str());
		_milliLastTS = milli_TS();
/*
}
	catch (EXCEPT const &e) {
		//FIX_ERROR();//
		THROW_UP();
	}*/
	return true;
}
//---------------------------------------------------------------------
String Module::dump(){
	char buffer[92];
	int16_t cnt = 0;
	SPrintF(sizeof(buffer), cnt, buffer, "%s", _Mid.c_str());
	SPrintF(sizeof(buffer), cnt, buffer, "  %i", get_dbgLVL());
	SPrintF(sizeof(buffer), cnt, buffer, "  '%s'", _protocol.c_str());
	SPrintF(sizeof(buffer), cnt, buffer, " %6.3f", (1.0 * _milliPeriod) / 1000.0);
	SPrintF(sizeof(buffer), cnt, buffer, " %8.3f", (1.0 * (milli_TS() - _milliLastTS)) / 1000.0);
	SPrintF(sizeof(buffer), cnt, buffer, " %p", this);
	return String(buffer);
}
///////////////////////////////////////////////////////////////////////
String dumpAllMODULES(){
	char buffer[512];
	int16_t cnt = 0;
	SPrintF(sizeof(buffer), cnt, buffer, "#### Mod dbg proto  T(s)     dt(s)   Address");
	for (auto itMod = modulesMap.begin(); itMod != modulesMap.end(); ++itMod)
		SPrintF(sizeof(buffer), cnt, buffer, "\n\t%s", itMod->second->dump().c_str());
	SPrintF(sizeof(buffer), cnt, buffer, "\n=================");
	return String(buffer);
}
///////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
/**
 * @fn t_fct2 findFCT(const char * CTL_TYP)
 * \brief retrieves a function to exec on *_jsonDoc
 * @param CTL_TYP
 * @return
 */
t_fct2 findFCT(const char *CTL_TYP){
	extern t_map_QSRC sourceMap;
	auto itDisps = sourceMap.find(CTL_TYP);
	assert(itDisps != sourceMap.end());
	return itDisps->second.task;
}
///////////////////////////////////////////////////////////////////////
/**
 * \fn Module * findModule(const char * IDM)
 * \brief retrieves Module * in \ref modulesMap
 * \param const char * IDM module TYP ID
 * \return Module * found in app (or assert...)
 */
Module* findModule(const char *IDM){
	auto itMod = modulesMap.find(IDM);
	assert(itMod != modulesMap.end());
	return itMod->second;
}
///////////////////////////////////////////////////////////////////////
/**
 * \fn Module * findModule0(const char IDM0)
 * \brief retrieves Module from IDM0 in \ref modulesMap
 * \param const char IDM0 first char of module TYP ID
 * \return Module * found in app (or nullptr)
 */
Module* findModule0(const char IDM0){
	t_map_MOD::iterator itMod;
	for (itMod = modulesMap.begin(); itMod != modulesMap.end(); ++itMod)
		if (itMod->first[0] == IDM0)
			return itMod->second;
	return nullptr;
}
//---------------------------------------------------------------------
void clearWatchdog(){
	static Module *watchdog = findModule("WAT"); // one time is enough !!
	watchdog->_milliLastTS = milli_TS();
}
///////////////////////////////////////////////////////////////////////
/**
 * PILOT QUERY PROCESSING
 * \n receives the query & transmits it to the good mapped module
 */
bool reply2query_PILOT(JsonObject &JOBJ){
	//clearWatchdog(); 'est un soucis risque de dt<0 pour WAT dans has a query
	//	LOCATE MODULE IN modulesMap & launch it000
	const char *idM = JOBJ["KMD"]["MID"].as<string>().c_str();
	bool z = findModule(idM)->reply2pilotQ_OP(JOBJ);
	JOBJ["CTL"]["TYP"] = DEF_REPLY_ROVER_TO_PILOT;
	return z;
}
//---------------------------------------------------------------------
bool ackno2reply_PILOT(JsonObject &JOBJ){
	//	LOCATE MODULE IN modulesMap & launch it000
	const char *idM = JOBJ["KMD"]["MID"].as<string>().c_str();
	return findModule(idM)->updateATreply(JOBJ);
}
//---------------------------------------------------------------------
bool Module::updateATreply(JsonObject &JOBJ){
	_Flow.ack(JOBJ["CTL"]["TIK"].as<uint32_t>());
	return false;
}
//---------------------------------------------------------------------
bool reply2query_BASIS(JsonObject &JOBJ){
	//_SERIAL_9( "\n DEF_QUERY_BASIS_TO_ROVER");
	JOBJ["KMD"]["CHN"] = "ROVER reply to BASIS";
	JOBJ["CTL"]["TYP"] = DEF_REPLY_ROVER_TO_BASIS;
	//	_SERIAL_9(  "\n%s", JOBJ["KMD"]["CHN"].as<string>().c_str());
	return true;
}
//---------------------------------------------------------------------
bool ackno2reply_BASIS(JsonObject &JOBJ){
	return false;
}
///////////////////////////////////////////////////////////////////////
String dumpJSON(){
	char buffer[512];
	serializeJson(*_jsonDoc, &buffer, 511);
	return String(buffer);
}
///////////////////////////////////////////////////////////////////////
