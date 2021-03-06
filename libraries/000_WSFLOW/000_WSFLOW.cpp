#include "000_WSFLOW.h"
#include "esp32_LOGS.h"
using namespace std;
///////////////////////////////////////////////////////////////////////
c_myFlow::c_myFlow(const String NAME, int8_t *DBGMAX) :
	Core(NAME.c_str(), DBGMAX){
}
//---------------------------------------------------------------------
int c_myFlow::tag(const uint64_t TMS, const String PROTOCOL){
	cntQUERIES++;
	s_QRflow TTT;
	TTT.Tms = TMS;
	TTT.Typ = PROTOCOL;
	t_map_FLOW::iterator it = QRwait.begin();
	QRwait.insert(std::pair<int32_t, s_QRflow>(cntQUERIES, TTT));
#ifdef DEBUG_LVL_FLOW
	dumpFlow(QRwait,
			"********************INSERT TIK=" + String(cntQUERIES) + ":(" + PROTOCOL + ","
			+ SUI64(TMS) + ")\nQRwait...");
	dumpFlow(QRlost, "QRlost...");
#endif
	return cntQUERIES;
}
//---------------------------------------------------------------------
void c_myFlow::ack(const uint32_t TIK){
#ifdef DEBUG_LVL_FLOW
	dumpFlow(QRwait, "****************ACHNOLEDGE TIK="+String(TIK)+"\nQRwait...");
	dumpFlow(QRlost, "QRlost...");
#endif
	t_map_FLOW::iterator it = QRwait.find(TIK);
	try {
		if (it == QRwait.end()) {
			QRlost.insert(*it);
			THROWERR(FLOW_ACKERR, TIK);
		}
		QRwait.erase(it);
	} catch (EXCEPT const &e) {
		FIX_ERROR();
	}
}
//---------------------------------------------------------------------
void c_myFlow::clear(){
	QRwait.clear();
	QRlost.clear();
}
//---------------------------------------------------------------------
bool c_myFlow::check(){
	return true;
}
//---------------------------------------------------------------------
bool c_myFlow::over(){
	return false;
}
//---------------------------------------------------------------------
uint16_t c_myFlow::waitSize(){
	return QRwait.size();
}
//---------------------------------------------------------------------
uint16_t c_myFlow::lostSize(){
	return QRwait.size();
}
//---------------------------------------------------------------------
#ifdef DEBUG_LVL_FLOW
void dumpFlow(t_map_FLOW & MAP, const String TITLE) {
	int i = 0;
	_SERIAL_4( "\n%s", TITLE.c_str());
	for (auto it = MAP.begin(); it != MAP.end(); ++it) {
		s_QRflow qrf = it->second;
		_SERIAL_4( "\n %2i %5i '%3s' %llu", i, it->first, qrf.Typ.c_str(), qrf.Tms);
		i++;
	}
}
#endif
///////////////////////////////////////////////////////////////////////
