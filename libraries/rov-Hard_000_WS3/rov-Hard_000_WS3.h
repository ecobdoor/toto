#ifndef _HARD_ROVER_000_WS3_
#define _HARD_ROVER_000_WS3_
//#include "000_UTL_SYS_BASIC.h"
#include "rov-CfgH_000_WS3.h"
#include <esp32_SYS_basic.h>
////////////////////////////////////////////////////////////////
/**
 * \class _Aop
 */
class _AsaDriver {
private:
public:
	_AsaDriver();
	/**
	 * \fn t_AfloatAxes DIF(const t_AfloatAxes A,const t_AfloatAxes B);
	 * \brief Difference of two analogical signals arrays
	 * \param const t_AfloatAxes A
	 * \param const t_AfloatAxes B
	 * \return t_AfloatAxes A=A-B (minus of functionnal)
	 */
	t_AfloatAxes ADD(const t_AfloatAxes A, const t_AfloatAxes B);
	t_AfloatAxes MUL(const t_AfloatAxes A, const t_AfloatAxes B);
	t_AfloatAxes DIF(const t_AfloatAxes A, const t_AfloatAxes B);

};
///////////////////////////////////////////////////////////////////////
/**
 * \class H_Module
 */
class H_Module: public Core {
private:
	bool _MODon = true;
	public:
	/**
	 * \fn H_Module(const bool ON = true);
	 * \param const bool default ON = true
	 */
	H_Module(const String NAME,  int8_t * DBGMAX, const bool ON = true);
	void modON(const bool ON);
	bool isON();
	virtual void Reset()=0; //to permit private call from children by pointer
};
///////////////////////////////////////////////////////////////////////
#endif
