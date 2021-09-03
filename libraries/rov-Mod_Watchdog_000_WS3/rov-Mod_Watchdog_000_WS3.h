#ifndef _MOD_WATCHDOG_000_WS3_
#define _MOD_WATCHDOG_000_WS3_
/** @file */
#include "rov-Mod_000_WS3.h"
///////////////////////////////////////////////////////////////////////
class Watchdog;
typedef bool (Watchdog::*OPdog)(JsonObject &JOBJ);
typedef std::map<const char*, OPdog, strCmp> t_map_OPwatchdog;
class Motors;
/**
 * \class Watchdog :public Module
 */
class Watchdog: public Module {
private:
	s_WATctx &_Ctx;
	t_map_OPwatchdog map_OPwatchdog; // Mapping of available module's operations
public:
	friend class Motors;
	Watchdog(c_myFlow *FLOW, const s_WATCHDOG MOD_CFG, s_WATctx *CONTEXT, int8_t *DBGMAX);
	e_hasMsg auto_MSG(const uint64_t DTMS,JsonObject &KMD);
	e_hasMsg complete_MSG(JsonObject &KMD);
	bool reply2pilotQ_OP(JsonObject &JOBJ);
	bool updateATreply(JsonObject &JOBJ){
		return false;
	}
};
///////////////////////////////////////////////////////////////////////
#endif
