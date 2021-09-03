/** @file */
#include "rov.CONTEXT.h"

#include "esp32_SYS_basic.h"
#include "esp32_GOODIES.h"
Context CTX(
	new EEPROMClass("eeprom0", sizeof(s_BOOTctx)),
	new EEPROMClass("eeprom1", sizeof(s_HARDctx)),
	new EEPROMClass("eeprom2", sizeof(s_HARDctx)),
	new EEPROMClass("eeprom3", sizeof(s_SOFTctx)),
	new EEPROMClass("eeprom4", sizeof(s_SOFTctx))
		);
Context::Context(
	EEPROMClass *E_B0,
	EEPROMClass *E_H1, EEPROMClass *E_H2,
	EEPROMClass *E_S1, EEPROMClass *E_S2)
:
	E_BOOT0(*E_B0), E_HARD1(*E_H1), E_HARD2(*E_H2), E_SOFT1(*E_S1), E_SOFT2(*E_S2){
}
void Context::LOAD(){
	Eload(E_BOOT0, BOOTctx);
	if (BOOTctx.SOFT == 1)
		Eload(E_SOFT1, SOFTctx);
	else if (BOOTctx.SOFT == 2)
		Eload(E_SOFT2, SOFTctx);
	else
	if (BOOTctx.HARD == 1)
		Eload(E_HARD1, HARDctx);
	else if (BOOTctx.HARD == 2)
		Eload(E_HARD2, HARDctx);
}
void Context::SAVE(){
	Esave(E_BOOT0, BOOTctx);
	Esave(E_SOFT1, SOFTctx);
	Esave(E_SOFT2, SOFTctx);
	Esave(E_HARD1, HARDctx);
	Esave(E_HARD2, HARDctx);
}
// @formatter:off
String Context::dumpBOOTctx(){
	char buffer[128];
	int16_t cnt = 0;
	SPrintF(sizeof(buffer), cnt, buffer, "%s %s", BOOTctx.Ename, datim(BOOTctx.TMS).c_str());
	SPrintF(sizeof(buffer), cnt, buffer, " Contexts SOFT(%i) HARD(%i)", BOOTctx.SOFT, BOOTctx.HARD);
	return String(buffer);
}
String Context::dumpHARDctx(){
	char buffer[512];
	int16_t cnt = 0;
	SPrintF(sizeof(buffer), cnt, buffer, "HARD context %s %s", HARDctx.Ename, datim(HARDctx.TMS).c_str());

	s_kineCTX &kineCTX = HARDctx.kineCTX;
	SPrintF(sizeof(buffer), cnt, buffer, "\n  kineCTX");
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t telemetry       =%s", SBOOL(kineCTX.telemetry).c_str());
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t dmpLvl_Attitude =%i", kineCTX.dmpLvl_Attitude);
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t dmpLvl_Keyframe =%i", kineCTX.dmpLvl_Keyframe);

	s_xpidCTX &xpidCTX = HARDctx.xpidCTX;
	SPrintF(sizeof(buffer), cnt, buffer, "\n  xpidCTX");
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t telemetry       =%s", SBOOL(xpidCTX.telemetry).c_str());
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t dmpLvl_Ringqueue=%i", xpidCTX.dmpLvl_Ringqueue);

	s_ximuCTX &ximuCTX = HARDctx.ximuCTX;
	SPrintF(sizeof(buffer), cnt, buffer, "\n  ximuCTX");
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t telemetry       =%s", SBOOL(ximuCTX.telemetry).c_str());
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t dmpLvl_Fusensor =%i", ximuCTX.dmpLvl_Fusensor);
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t gyroBias        =(%10.4f %10.4f %10.4f)", ximuCTX.gyroBias[0], ximuCTX.gyroBias[1], ximuCTX.gyroBias[2]);
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t accelBias       =(%10.4f %10.4f %10.4f)", ximuCTX.accelBias[0], ximuCTX.accelBias[1], ximuCTX.accelBias[2]);
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t magBias         =(%10.4f %10.4f %10.4f)", ximuCTX.magBias[0], ximuCTX.magBias[1], ximuCTX.magBias[2]);
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t magScale        =(%10.4f %10.4f %10.4f)", ximuCTX.magScale[0], ximuCTX.magScale[1], ximuCTX.magScale[2]);
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t beta            = %10.4f", ximuCTX.beta);
	return String(buffer);
}
String Context::dumpSOFTctx(){
	char buffer[1124];
	int16_t cnt = 0;
	SPrintF(sizeof(buffer), cnt, buffer, "SOFT context %s %s", SOFTctx.Ename, datim(SOFTctx.TMS).c_str());
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t csvActive       =%s", SBOOL(SOFTctx.csvActive).c_str());
	SPrintF(sizeof(buffer), cnt, buffer, "\t\t txtActive       =%s", SBOOL(SOFTctx.txtActive).c_str());
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t showWSX_event   =%s", SBOOL(SOFTctx.showWSX_event).c_str());
	SPrintF(sizeof(buffer), cnt, buffer, "\t\t showWSX_data    =%s", SBOOL(SOFTctx.showWSX_data).c_str());
	SPrintF(sizeof(buffer), cnt, buffer, "\t\t withCtrlBlock   =%s", SBOOL(SOFTctx.withCtrlBlock).c_str());
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t logWebsocket    =%s", SBOOL(SOFTctx.logWebsocket).c_str());
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t exportCSV       =%s", SBOOL(SOFTctx.exportCSV).c_str());
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t ALL_DBGMAX_TRUNC=%i", SOFTctx.ALL_DBGMAX_TRUNC);
	SPrintF(sizeof(buffer), cnt, buffer, "\t\t FCT_DBGMAX_SERV =%i", SOFTctx.FCT_DBGMAX_SERV);
	SPrintF(sizeof(buffer), cnt, buffer, "\t\t FCT_DBGMAX_MAIN =%i", SOFTctx.FCT_DBGMAX_MAIN);
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t FCT_DBGMAX_BOOT =%i", SOFTctx.FCT_DBGMAX_BOOT);
	SPrintF(sizeof(buffer), cnt, buffer, "\t\t FCT_DBGMAX_LOOP =%i", SOFTctx.FCT_DBGMAX_LOOP);
	SPrintF(sizeof(buffer), cnt, buffer, "\t\t FCT_DBGMAX_KMDS =%i", SOFTctx.FCT_DBGMAX_KMDS);
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t TSK_DBGMAX_CORE =%i", SOFTctx.TSK_DBGMAX_CORE);
	SPrintF(sizeof(buffer), cnt, buffer, "\t\t TSK_DBGMAX_SEND =%i", SOFTctx.TSK_DBGMAX_SEND);
	SPrintF(sizeof(buffer), cnt, buffer, "\t\t TSK_DBGMAX_UDPL =%i", SOFTctx.TSK_DBGMAX_UDPL);
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t TSK_DBGMAX_XIMU =%i", SOFTctx.TSK_DBGMAX_XIMU);
	SPrintF(sizeof(buffer), cnt, buffer, "\t\t TSK_DBGMAX_RECV =%i", SOFTctx.TSK_DBGMAX_RECV);
	SPrintF(sizeof(buffer), cnt, buffer, "\t\t TSK_DBGMAX_LOGS =%i", SOFTctx.TSK_DBGMAX_LOGS);
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t MOD_DBGMAX_DRV  =%i", SOFTctx.MOD_DBGMAX_DRV);
	SPrintF(sizeof(buffer), cnt, buffer, "\t\t MOD_DBGMAX_JOB  =%i", SOFTctx.MOD_DBGMAX_JOB);
	SPrintF(sizeof(buffer), cnt, buffer, "\t\t MOD_DBGMAX_MOT  =%i", SOFTctx.MOD_DBGMAX_MOT);
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t MOD_DBGMAX_TMS  =%i", SOFTctx.MOD_DBGMAX_TMS);
	SPrintF(sizeof(buffer), cnt, buffer, "\t\t MOD_DBGMAX_WAT  =%i", SOFTctx.MOD_DBGMAX_WAT);
	SPrintF(sizeof(buffer), cnt, buffer, "\t\t FLOW___DBGMAX_  =%i", SOFTctx.FLOW___DBGMAX_);
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t IMU____DBGMAX_  =%i", SOFTctx.IMU____DBGMAX_);
	SPrintF(sizeof(buffer), cnt, buffer, "\t\t JSTICK_DBGMAX_  =%i", SOFTctx.JSTICK_DBGMAX_);
	SPrintF(sizeof(buffer), cnt, buffer, "\t\t KINE___DBGMAX_  =%i", SOFTctx.KINE___DBGMAX_);
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t PID____DBGMAX_  =%i", SOFTctx.PID____DBGMAX_);
	SPrintF(sizeof(buffer), cnt, buffer, "\t\t PWM____DBGMAX_  =%i", SOFTctx.PWM____DBGMAX_);
	SPrintF(sizeof(buffer), cnt, buffer, "\t\t SPEED__DBGMAX_  =%i", SOFTctx.SPEED__DBGMAX_);
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t USD____DBGMAX_  =%i", SOFTctx.USD____DBGMAX_);

	s_WATctx &WATctx = SOFTctx.WATctx;
	SPrintF(sizeof(buffer), cnt, buffer, "\n  WATctx");
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t WATctx.on       =%s", SBOOL(WATctx.on).c_str());

	s_DRVctx &DRVctx = SOFTctx.DRVctx;
	SPrintF(sizeof(buffer), cnt, buffer, "\n  DRVctx");
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t clear_CineRover=%s", SBOOL(DRVctx.clear_CineRover).c_str());
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t simul_IMU      =%s", SBOOL(DRVctx.simul_IMU).c_str());
	SPrintF(sizeof(buffer), cnt, buffer, "\n\t kmdIdx         =%i", DRVctx.kmdIdx);
	for (size_t i = 0; i < 10; i++)
		SPrintF(sizeof(buffer), cnt, buffer, "\n\t kmdIdx[%i]      =%s", i, DRVctx.drvKmd[i].c_str());
	return String(buffer);
}
// @formatter:on
///////////////////////////////////////////////////////////////////////
