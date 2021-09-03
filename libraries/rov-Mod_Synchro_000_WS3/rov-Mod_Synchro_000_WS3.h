#ifndef _MOD_SYNCHRO_000_WS3_
#define _MOD_SYNCHRO_000_WS3_
/** @file */
#include "rov-Mod_000_WS3.h"
///////////////////////////////////////////////////////////////////////
class Synchro;
typedef bool (Synchro::*OPtms)(JsonObject &JOBJ);
typedef std::map<const char*, OPtms, strCmp> t_map_OPsynchro;
/**
 * \class Synchro :public Module
 */
class Synchro: public Module {
private:
	t_map_OPsynchro map_OPsynchro; // Mapping of available module's operations
	bool mOP_Synchro(JsonObject &JOBJ);
	public:
	Synchro(c_myFlow *FLOW, const s_TIMEMS MOD_CFG,  int8_t* DBGMAX);
	e_hasMsg auto_MSG(const uint64_t DTMS,JsonObject &KMD);
	e_hasMsg complete_MSG(JsonObject &KMD);
	bool reply2pilotQ_OP(JsonObject &JOBJ);
	bool updateATreply(JsonObject &JOBJ){
		return false;
	}
};
///////////////////////////////////////////////////////////////////////
#endif
