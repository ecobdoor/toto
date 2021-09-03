#ifndef _000_WSFLOW_
#define _000_WSFLOW_
///////////////////////////////////////////////////////////////////////
/** @file */
//#include <functional>
#include <esp32_SYS_basic.h>
//#include <vector>
///////////////////////////////////////////////////////////////////////
/**
 * FifoQueriesChecker MAP structure
 */
typedef struct {
	uint64_t Tms;
	String Typ;
} s_QRflow;
/**
 * typedef std::map<uint32_t, s_QRflow> t_map_FLOW;
 */
typedef std::map<uint32_t, s_QRflow> t_map_FLOW;
#ifdef DEBUG_LVL_FLOW
void dumpFlow(t_map_FLOW & MAP, const String TITLE);
#endif
///////////////////////////////////////////////////////////////////////
/**
 * \class c_myFlow
 */
class c_myFlow: public Core {
private:
	t_map_FLOW QRwait;
	t_map_FLOW QRlost;
	public:
	int cntQUERIES = 0;
	c_myFlow(const String NAME, int8_t *DBGMAX);
	void clear();
	int tag(const uint64_t TMS, const String PROTOCOL);
	void ack(const uint32_t TIK);
	/**
	 * Threshold of waiting queries in the flow ??? in fifowait
	 */
	bool check();
	/**
	 * Number max of queued queries ??? from server
	 */
	bool over();
	uint16_t waitSize();
	uint16_t lostSize();
};
///////////////////////////////////////////////////////////////////////
#endif
///////////////////////////////////////////////////////////////////////

