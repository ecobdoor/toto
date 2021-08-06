/** @file */
#include "CAM_CONTEXT.h"
#include "esp32_GOODIES.h"
Context CTX(
	new EEPROMClass("eeprom0", sizeof(s_BOOTctx)),
	new EEPROMClass("eepromC1", sizeof(s_CAMERActx)),
	new EEPROMClass("eepromM1", sizeof(s_MOTIONctx)),
	new EEPROMClass("eepromC2", sizeof(s_CAMERActx)),
	new EEPROMClass("eepromM2", sizeof(s_MOTIONctx))
		);
///////////////////////////////////////////////////////////////////////
Context::Context(
	EEPROMClass *E_B0,
	EEPROMClass *E_C1, EEPROMClass *E_M1,
	EEPROMClass *E_C2, EEPROMClass *E_M2)
:
	E_BOOT(*E_B0), E_CAM1(*E_C1), E_MOV1(*E_M1), E_CAM2(*E_C2), E_MOV2(*E_M2)
{
}
//---------------------------------------------------------------------
String Context::dumpBOOTctx()
{
	char buffer[128];
	int16_t cnt = 0;
	cnt += sprintf(&buffer[cnt], "%s %s", BOOTctx.Ename, datim(BOOTctx.TMS).c_str());
	cnt += sprintf(&buffer[cnt], " Contexts CAMERA(%i) MOTION(%i)", BOOTctx.CAMERA, BOOTctx.MOTION);
	return String(buffer);
}
//---------------------------------------------------------------------
String Context::dumpCAMctx()
{
	char buffer[256];
	int16_t cnt = 0;
	cnt += sprintf(&buffer[cnt], "CAMERA context %s %s", CAMctx.Ename, datim(CAMctx.TMS).c_str());
	cnt += sprintf(&buffer[cnt], "\n _framSize     %s", textFrameSize(CAMctx._framSize).c_str());
	cnt += sprintf(&buffer[cnt], "\n _pixFormat    %s", textPixFormat(CAMctx._pixFormat).c_str());
	cnt += sprintf(&buffer[cnt], "\n _width        %i", CAMctx._width);
	cnt += sprintf(&buffer[cnt], "\n _height       %i", CAMctx._height);
	return String(buffer);
}
//---------------------------------------------------------------------
String Context::dumpMOVctx()
{
	char buffer[512];
	int16_t cnt = 0;
	cnt += sprintf(&buffer[cnt], "MOTION context %s %s", MOVctx.Ename, datim(MOVctx.TMS).c_str());
	cnt += sprintf(&buffer[cnt], "\n _active          %s", MOVctx._active ? "Yes" : "No");
	cnt += sprintf(&buffer[cnt], "\n _marker      %5i", (int)MOVctx._marker);
	cnt += sprintf(&buffer[cnt], "\n _dbgLevel    %5i", MOVctx._dbgLevel);
	cnt += sprintf(&buffer[cnt], "\n _gridStep    %5i", MOVctx._gridStep);
	cnt += sprintf(&buffer[cnt], "\n _gridLen     %5i", MOVctx._gridLen);
	cnt += sprintf(&buffer[cnt], "\n _smooth   %11.2f", MOVctx._smooth);
	cnt += sprintf(&buffer[cnt], "\n _absDelta    %5i", MOVctx._absDelta);
	cnt += sprintf(&buffer[cnt], "\n _relDelta  %10.2f", MOVctx._relDelta);
	cnt += sprintf(&buffer[cnt], "\n _ratioMax  %10.2f", MOVctx._ratioMax);
	cnt += sprintf(&buffer[cnt], "\n _countMax    %5i", MOVctx._countMax);
	cnt += sprintf(&buffer[cnt], "\n _sendFTP         %s", MOVctx._sendFTP ? "Yes" : "No");
	cnt += sprintf(&buffer[cnt], "\n _sendSMS         %s", MOVctx._sendSMS ? "Yes" : "No");
	return String(buffer);
}
//---------------------------------------------------------------------
String textFrameSize(const framesize_t val){
	if (val == FRAMESIZE_96X96)
		return "{96x96}";
	else if (val == FRAMESIZE_QQVGA)
		return "{160x120}";
	else if (val == FRAMESIZE_QCIF)
		return "{176x144}";
	else if (val == FRAMESIZE_HQVGA)
		return "{240x176}";
	else if (val == FRAMESIZE_240X240)
		return "{240x240}";
	else if (val == FRAMESIZE_QVGA)
		return "{320x240}";
	else if (val == FRAMESIZE_CIF)
		return "{400x296}";
	else if (val == FRAMESIZE_HVGA)
		return "{480x320}";
	else if (val == FRAMESIZE_VGA)
		return "{640x480}";
	else if (val == FRAMESIZE_SVGA)
		return "{800x600}";
	else if (val == FRAMESIZE_XGA)
		return "{1024x768}";
	else if (val == FRAMESIZE_HD)
		return "{1280x720}";
	else if (val == FRAMESIZE_SXGA)
		return "{1280x1024}";
	else if (val == FRAMESIZE_UXGA)
		return "{1600x1200}";
	else if (val == FRAMESIZE_FHD)
		return "{1920x1080}";
	else if (val == FRAMESIZE_P_HD)
		return "{720x1280}";
	else if (val == FRAMESIZE_P_3MP)
		return "{864x1536}";
	else if (val == FRAMESIZE_QXGA)
		return "{2048x1536}";
	else if (val == FRAMESIZE_QHD)
		return "{2560x1440}";
	else if (val == FRAMESIZE_WQXGA)
		return "{2560x1600}";
	else if (val == FRAMESIZE_P_FHD)
		return "{1080x1920}";
	else if (val == FRAMESIZE_QSXGA)
		return "{2560x1920}";
	else
		return "{???x???}";
}
//---------------------------------------------------------------------
String textPixFormat(const pixformat_t val){
	if (val == PIXFORMAT_GRAYSCALE)
		return "8 bits-GRAY";
	else if (val == PIXFORMAT_RGB565)
		return "16 bits-RGB565";
	else if (val == PIXFORMAT_RGB888)
		return "24 bits-RGB888";
	else
		return "?BPP/????";
}
//---------------------------------------------------------------------
bool SETUP_CONTEXT(const bool FACTORY_RESET){
	bool ee = true;
	ee = ee & Ebegin(CTX.E_BOOT, "BOOT ");
	ee = ee & Ebegin(CTX.E_CAM1, "CAM 1");
	ee = ee & Ebegin(CTX.E_MOV1, "MOV 1");
	ee = ee & Ebegin(CTX.E_CAM2, "CAM 2");
	ee = ee & Ebegin(CTX.E_MOV2, "MOV 2");
	if (!ee)
		return false;
	if (FACTORY_RESET) {
		Esave(CTX.E_BOOT, CTX.BOOTctx);
		Esave(CTX.E_CAM1, CTX.CAMctx);
		Esave(CTX.E_CAM2, CTX.MOVctx);
		Esave(CTX.E_MOV1, CTX.CAMctx);
		Esave(CTX.E_MOV2, CTX.MOVctx);
	} else {
		Eload(CTX.E_BOOT, CTX.BOOTctx);
		_DEBUG_Others_(0, "with EEPROM %s", CTX.dumpBOOTctx().c_str());
		if (CTX.BOOTctx.CAMERA == 1)
			Eload(CTX.E_CAM1, CTX.CAMctx);
		else if (CTX.BOOTctx.CAMERA == 2)
			Eload(CTX.E_CAM2, CTX.CAMctx);
		else
			_DEBUG_Others_(0, "\nKEEP DEFAULT CAMERA context");
		if (CTX.BOOTctx.MOTION == 1)
			Eload(CTX.E_MOV1, CTX.MOVctx);
		else if (CTX.BOOTctx.MOTION == 2)
			Eload(CTX.E_MOV2, CTX.MOVctx);
		else
			_DEBUG_Others_(0, "\nKEEP DEFAULT MOTION context");
	}
	_DEBUG_Others_(0, "\n%s", CTX.dumpBOOTctx().c_str());
	_DEBUG_Others_(0, "\n%s", CTX.dumpCAMctx().c_str());
	_DEBUG_Others_(0, "\n%s", CTX.dumpMOVctx().c_str());
	return true;
}
///////////////////////////////////////////////////////////////////////
