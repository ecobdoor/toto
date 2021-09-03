#include "esp32_LOGS.h"
#include "rov-Hard_000_WS3.h"
//#include "00_MODhardware.h"
#include <algorithm> // transform for_each
#include <functional> // minus
using namespace std;
////////////////////////////////////////////////////////////////
//--------------------------------------------------------------
_AsaDriver::_AsaDriver() {
}
//--------------------------------------------------------------
t_AfloatAxes _AsaDriver::MUL(const t_AfloatAxes A, const t_AfloatAxes B) {
	t_AfloatAxes C;
	transform(A.begin(), A.end(), B.begin(), C.begin(), multiplies<float>());
	return C;
}
//--------------------------------------------------------------
t_AfloatAxes _AsaDriver::DIF(const t_AfloatAxes A, const t_AfloatAxes B) {
	t_AfloatAxes C;
	transform(A.begin(), A.end(), B.begin(), C.begin(), minus<float>());
	return C;
}
//--------------------------------------------------------------
t_AfloatAxes _AsaDriver::ADD(const t_AfloatAxes A, const t_AfloatAxes B) {
	t_AfloatAxes C;
	transform(A.begin(), A.end(), B.begin(), C.begin(), plus<float>());
	return C;
}
////////////////////////////////////////////////////////////////
H_Module::H_Module(const String NAME,  int8_t * DBGMAX, const bool ON) :
		Core(NAME.c_str(), DBGMAX) {
	_MODon = ON;
}
//--------------------------------------------------------------
void H_Module::modON(const bool ON) {
	_MODon = ON;
}
//--------------------------------------------------------------
bool H_Module::isON() {
	return _MODon;
}
///////////////////////////////////////////////////////////////////////
