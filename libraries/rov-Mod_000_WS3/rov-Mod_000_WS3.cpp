/** @file */
#include "rov-Mod_000_WS3.h"
#include "esp32_LOGS.h"
#include "esp32_SPRINT.h"
/**
 * Mapping of modules
 */
t_map_MOD modulesMap;
///////////////////////////////////////////////////////////////////////
/**
 * \class Module
 */
Module::Module(c_myFlow *FLOW, const s_MODULE MOD_CFG, int8_t *DBGMAX)
:
	Core(MOD_CFG.Mid.c_str(), DBGMAX), _Flow(*FLOW){
	_name = MOD_CFG.Name;
	_Mid = MOD_CFG.Mid;
	_protocol = MOD_CFG.Qtype;
	_milliPeriod = MOD_CFG.loop_TMS;
	_milliLastTS = milli_TS();
	_SERIAL_0("\n     Creates module %s(%s)  \t '%s' %10i ms, HEAP: %s",
		_Mid.c_str(), _name.c_str(), _protocol, _milliPeriod, heapState().c_str());
}
//---------------------------------------------------------------------
bool Module::has_MSG(){
	if (!_MODon)
		return false;
	e_hasMsg has;
	//_SERIAL_6( "\n-has_MSG ");
	uint64_t milliTS = milli_TS();
	int64_t milliDT = (int64_t)(milliTS - _milliLastTS);
	try {
		if (milliDT < 0)
			THROWERR(MOD_DTMINUS, _Mid.c_str(), milliDT, milliTS, _milliLastTS);
		if ((_isRunning) && (_milliPeriod != 0) && ((uint64_t)_milliPeriod < milliDT)) {
			//_SERIAL_0( "\n-TimOut %s (%4llu/%4i ms)", _Mid.c_str(), milliDT, _milliPeriod);
			has = tmsout_MSG(milliDT);
		} else {
			//_SERIAL_0( "\n-Auto_Q %s(%4llu/%4i ms)", _Mid.c_str(), milliDT, _milliPeriod);
			has = popup_MSG(milliDT);
		}
//		_SERIAL_0( "\nhas_MSG %i %s [%s]", (has != e_hasMsg::No),ShasMsg(has).c_str(),dumpJSON().c_str());
		/*
		 */
		/*
		 JsonObject root = (*_jsonDoc).as<JsonObject>(); // keep _jsonDoc of course...
		 uint64_t tik = root["CTL"]["TIK"].as<uint64_t>();
		 _SERIAL_0(" %s %s [%s, %llu]",
		 _Mid.c_str(), ShasMsg(has).c_str(), _protocol.c_str(), tik);
		 */
	}
	catch (EXCEPT const &e) {
		THROW_UP();
	}
	return (has != e_hasMsg::No);
}
//---------------------------------------------------------------------
/**
 * \fn void Module::tmsout_MSG(const uint64_t DTMS)
 * \brief creates a default time-out json message completed by specific module informations
 *
 * Each time at the end of tmsout_MSG call, _milliLastTS is set to current time milli_TS()
 * to enable the next call comparing the delay with the module's _milliPeriod.
 * \if complete_MSG generates a QUERY inserted in _Flow control
 * \else generates a single info (TIK=0)
 * \param const uint64_t DTMS milliseconds since last send
 * \return a @ref e_hasMsg to cause sending or not
 */
e_hasMsg Module::tmsout_MSG(const uint64_t DTMS){

	_jsonDoc->clear();
	JsonObject root = (*_jsonDoc).to<JsonObject>();
	JsonObject CTL = root.createNestedObject("CTL");
	CTL["TYP"] = _protocol;
	JsonObject KMD = root.createNestedObject("KMD");
	KMD["MID"] = _Mid;
	KMD["DT"] = DTMS;
	//Suppose to be either a query or a simple additional information
	e_hasMsg has = complete_MSG(KMD);
	if (e_hasMsg::Ack == has) {
		CTL["TIK"] = 0;
		CTL["TYP"] = "rRp";
	} else if (e_hasMsg::Full == has)
		CTL["TIK"] = _Flow.tag(milli_TS(), CTL["TYP"].as<String>()); //after because Watchdog resets the flow !!!!
	//_SERIAL_0("\nTimout %s %s %s TIK(%llu)",_Mid.c_str(), ShasMsg(has).c_str(), _protocol.c_str(),CTL["TIK"].as<uint64_t>());
	_milliLastTS = milli_TS();
	return has;
}
//---------------------------------------------------------------------
/**
 * \fn void Module::popup_MSG(const uint64_t DTMS)
 * \brief creates a default auto json message auto-completed by specific module informations
 *
 * it is up to auto_MSG to manage or not the module's milliLastTS.
 * to avoid or not the following Time Out message
 * milliLastTS is only used by has_MSG() to cause a tmsout_MSG(milliDT)
 * if the module's _milliPeriod is non-zero.
 *
 * \if popup_MSG generates a QUERY inserted in _Flow control
 * \else generates a single info (TIK=0)
 * \param const uint64_t DTMS milliseconds since last send
 * \return a @ref e_hasMsg to cause sending or not
 */
e_hasMsg Module::popup_MSG(const uint64_t DTMS){
//	return e_hasMsg::No;

	_jsonDoc->clear();
	JsonObject root = (*_jsonDoc).to<JsonObject>();
	JsonObject CTL = root.createNestedObject("CTL");
	CTL["TYP"] = _protocol;
	JsonObject KMD = root.createNestedObject("KMD");
	KMD["MID"] = _Mid;
	KMD["DT"] = DTMS;

	e_hasMsg has = auto_MSG(DTMS,KMD);
	if (e_hasMsg::Ack == has) {
		CTL["TIK"] = 0;
		CTL["TYP"] = "rRp";
	} else if (e_hasMsg::Full == has)
		CTL["TIK"] = _Flow.tag(milli_TS(), CTL["TYP"].as<String>());
	//if (has != e_hasMsg::No) Serial.printf("\n popup_MSG  detected [%s]", dumpJSON().c_str());
	return has;
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
	SPrintF(sizeof(buffer), cnt, buffer, "\tMod dbg proto  T(s)     dt(s)   Address");
	for (auto itMod = modulesMap.begin(); itMod != modulesMap.end(); ++itMod)
		SPrintF(sizeof(buffer), cnt, buffer, "\n\t%s", itMod->second->dump().c_str());
	/*
	 SPrintF(sizeof(buffer), cnt, buffer, "\n=================");
	 */return String(buffer);
}
///////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
/**
 * @fn t_fct2 findFCT(const char * CTL_TYP)
 * \brief Retrieves a function to exec on *_jsonDoc
 * @param CTL_TYP € {"pQr","pRr"}
 * @return the needed function or @ref halt execution !!!
 */
t_fct2 findFCT(const char *CTL_TYP){
	extern t_map_QSRC sourceMap;
	auto itDisps = sourceMap.find(CTL_TYP);
	ASSERT(itDisps != sourceMap.end());
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
	ASSERT(itMod != modulesMap.end());
	return itMod->second;
} //e_roverMode roverMode = e_roverMode::idle; //roaming;//e_roverMode::

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
	Module *mod = findModule(idM);
	mod->_Flow.ack(JOBJ["CTL"]["TIK"].as<uint32_t>());

	return mod->updateATreply(JOBJ);
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
