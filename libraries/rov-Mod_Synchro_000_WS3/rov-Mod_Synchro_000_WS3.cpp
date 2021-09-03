#include "rov-Mod_Synchro_000_WS3.h"
#include "esp32_LOGS.h"
using namespace std;
///////////////////////////////////////////////////////////////////////
/**
 * \class Synchro
 */
Synchro::Synchro(c_myFlow *FLOW, const s_TIMEMS MOD_CFG,  int8_t* DBGMAX) :
	Module(FLOW, MOD_CFG.MOD, DBGMAX){
	map_OPsynchro = { { DEF_OP_SYNC, &Synchro::mOP_Synchro } };
}
//---------------------------------------------------------------------
e_hasMsg Synchro::auto_MSG(const uint64_t DTMS,JsonObject &KMD){
	return e_hasMsg::No;
}
//---------------------------------------------------------------------
e_hasMsg Synchro::complete_MSG(JsonObject &KMD){
	return e_hasMsg::Full;
}
//---------------------------------------------------------------------
bool Synchro::reply2pilotQ_OP(JsonObject &JOBJ){
	const char *x = JOBJ["KMD"]["OP"].as<string>().c_str();
	(this->*(findOP < OPtms > (map_OPsynchro, x)))(JOBJ);
	return true;
}
//---------------------------------------------------------------------
bool Synchro::mOP_Synchro(JsonObject &JOBJ){
	int64_t pilotTMS = JOBJ["KMD"]["TMS"].as<int64_t>();
	int64_t oldTMS = (int64_t)milli_TS();
	int64_t dt = pilotTMS - oldTMS;
	extern uint64_t tmsOffset;
	//JOBJ["KMD"].remove("TMS");
	//tmsOffset += dt;
	int64_t newTMS = (int64_t)milli_TS();
	JOBJ["KMD"]["pilotTMS"] = pilotTMS;
	JOBJ["KMD"]["oldTMS"] = oldTMS;
	JOBJ["KMD"]["newTMS"] = newTMS;
	JOBJ["KMD"]["DT"] = dt;
	return true;
}
///////////////////////////////////////////////////////////////////////
