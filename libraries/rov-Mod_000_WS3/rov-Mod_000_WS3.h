#ifndef __000_MODULE___
#define __000_MODULE___
/** @file */
#include "rov-CfgH_000_WS3.h"
#include "000_WSFLOW.h"
#include "rov.CONTEXT.h"
#include <functional>
//#define ARDUINOJSON_USE_LONG_LONG 1
//#include "ArduinoJson.h"
#include "esp32_SYS_json.h"
///////////////////////////////////////////////////////////////////////
void clearWatchdog();
String dumpJSON();
bool createquery_PILOT(JsonObject &JOBJ);
bool reply2query_PILOT(JsonObject &JOBJ);
bool ackno2reply_PILOT(JsonObject &JOBJ); // return false nothing to do
bool createquery_BASIS(JsonObject &JOBJ);
bool reply2query_BASIS(JsonObject &JOBJ);
bool ackno2reply_BASIS(JsonObject &JOBJ);
///////////////////////////////////////////////////////////////////////
/**
 * \fn OP findOP(const MAP & M, const char * X)
 *
 * Launch method syntax: (this->*(findOP<OP_method>(map_OP...,X)))(JSON_object);
 * \brief Finds in map M the matching X key's OP method.
 * \tparam const MAP& M map where looking for.
 * \tparam const char * X searched key
 * \return OP of typedef bool ((Module_inherited)::*(OP_method)(JsonObject & JOBJ);
 */
template<typename OP, typename MAP>
OP findOP(const MAP &M, const char *X){
	auto it = M.find(X);
	assert(it != M.end());
	return (OP)(it->second);
}
///////////////////////////////////////////////////////////////////////
/**
 * \brief typedef std::function<bool(JsonObject &)> t_fct2;
 *
 * t_fct2 is the fundamental function executing a part of a module
 * theses functions return
 * @n true if a response has to be sent to client web socket
 * @n false if no response has to be sent to client web socket
 */
typedef std::function<bool(JsonObject&)> t_fct2;
/**
 * \fn t_fct2 findFCT(const char * CTL_TYP);
 * \brief Finds the function attached to the query|reply source
 * \param const char * CTL_TYP as "pQr", "rRb"...
 * \return @ref  t_fct2 function associated
 */
t_fct2 findFCT(const char *CTL_TYP);
/**
 * t_map_QSRC MAP structure
 */
typedef struct {
	t_fct2 task; //=[](JsonObject & ) -> void * {return nullptr;};
	void *data; //=nullptr;
	int num;
} s_xQ;
typedef std::map<const char*, s_xQ, strCmp> t_map_QSRC;
///////////////////////////////////////////////////////////////////////
/**
 * \class Module
 */
class Module: public Core {
private:
	public:
	bool _MODon = true;
	Flow &_Flow;
	bool _isRunning = false;
	String _protocol;
	String _name;
	String _Mid;
	/**
	 * \var uint32_t _milliPeriod time-out (ms) from which the task is considered in error
	 */
	uint32_t _milliPeriod;
	uint64_t _milliLastTS; //ms
	Module(Flow *FLOW, const s_MODULE MOD_CFG, int8_t *DBGMAX);
	bool tmsout_MSG(const uint64_t DTMS);
	/**
	 * \fn bool has_MSG();
	 * \brief Tests if a module has a query to send
	 *
	 * \Usually modules receive a request, execute something, and return a response. Except:
	 * \\n The WATCHDOG module sends a request when the engines are running and the rover no longer
	 * \\n receives control orders (it causes an emergency stop).
	 * \\n The JOBGOAL module executes queries from a file and can send queries via \ref auto_MSG ...
	 * \return true if the module has a query to send
	 */
	bool has_MSG();
	/**
	 * \fn virtual bool auto_MSG(const uint64_t DTMS)=0;
	 * \brief Pure virtual method launched by inherited modules
	 * \ If module is in time-out, launches a default \ref tmsout_MSG else \ref auto_MSG
	 * \param const uint64_t DTMS delay since the last module's query
	 * \return bool false if nothing to send else true
	 */
	virtual bool auto_MSG(const uint64_t DTMS)=0;
	/**
	 * \fn virtual bool complete_MSG(JsonObject & KMD)=0;
	 * \brief Pure virtual method instantiates by inherited modules
	 * \Completes the defaults \ref tmsout_MSG with additionnal module-specific informations
	 */
	virtual bool complete_MSG(JsonObject &KMD)=0;
	/**
	 * \fn virtual bool reply2pilotQ_OP(JsonObject & JOBJ)=0;
	 * \brief For children modules, finds in map_OPmodule the operation & launchs it
	 * \n if return true, JOBJ has to be modified to answer a new query
	 * \n else the message will be a reply with TIK==0
	 * \param JsonObject & JOBJ (map_OPmotors.find... first from JOBJ KMD.OP field)
	 * \return false if rover has no query to resend to client else true
	 */
	virtual bool reply2pilotQ_OP(JsonObject &JOBJ)=0;
	bool updateATreply(JsonObject &JOBJ);
	String dump();
};
String dumpAllMODULES();
/**
 * typedef std::map<const char*, Module *, strCmp> t_map_MOD;
 */
typedef std::map<const char*, Module*, strCmp> t_map_MOD;
/**
 * \fn  Module * findModule(const char * IDM)
 * \brief	LOCATE MODULE IN modulesMap
 * \param const char * IDM module TYP ID
 * \return Module * (or nullptr if not found)
 */
Module* findModule(const char *IDM);
Module* findModule0(const char IDM0);
///////////////////////////////////////////////////////////////////////
extern t_map_MOD modulesMap;
#endif
