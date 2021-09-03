#include "rov-Mod_Watchdog_000_WS3.h"
#include "esp32_LOGS.h"
///////////////////////////////////////////////////////////////////////
Watchdog::Watchdog(Flow *FLOW, const s_WATCHDOG MOD_CFG, s_WATctx *CONTEXT, int8_t *DBGMAX)
:
	Module(FLOW, MOD_CFG.MOD, DBGMAX),
		_Ctx(*CONTEXT){
	_isRunning = true; // ALWAYS !!!
	_MODon = _Ctx.on;
}
//---------------------------------------------------------------------
/**
 * Executed before time out
 */
bool Watchdog::auto_MSG(const uint64_t DTMS){
	return false;
}
//---------------------------------------------------------------------
/**
 * Executed at (after) time out
 */
/**
 * \fn bool Watchdog::complete_MSG(JsonObject & KMD)
 * \brief signals a time-out of queries recieved
 *
 * Reset the motor outputs
 * Reset the pid ringqueue
 * Clears the flow of queries
 *
 */
bool Watchdog::complete_MSG(JsonObject &KMD){
	_DBG_MOD_WHAT("\n-----------------------------------EMERGENCY time out/_milliPeriod=(%llu/%i)",
		milli_TS() - _milliLastTS, _milliPeriod);
	// Emergency stop rover
	Motors *motors = reinterpret_cast<Motors*>(findModule("MOT"));
//	((H_Module &)(motors->_pwmMot)).Reset();// to force _pwmMot private call via H_Module public virtual
//	((H_Module &)(motors->_pidMot)).Reset();
	_Flow.clear();
	if (!_Flow.check()) {
		KMD[DEF_KF_EXE] = false;
	} else if (_Flow.over()) {
		KMD[DEF_KF_EXE] = false;
	} else { //simple time out of ws reception n!!!
		KMD[DEF_KF_EXE] = false;
	}
	return true;
}
//---------------------------------------------------------------------
bool Watchdog::reply2pilotQ_OP(JsonObject &JOBJ){
	return true;
}
///////////////////////////////////////////////////////////////////////
