#include "CAM_MOTION.h"
#include "CAM_FTPCLI.h"
#include "esp32_SMS_client.h"
#include <FS.h>
#include <SPIFFS.h>
#include "CAM_GFONT.h"
///////////////////////////////////////////////////////////////////////
/// // WARNING at this moment the context is not initialized yet !!!!
c_Motion motionCtx(&CTX.MOVctx);
camera_fb_t *frame;
bool SETUP_MOTION(){
	return motionCtx.Begin();
}
//---------------------------------------------------------------------
bool c_Motion::Begin(){ // At this moment the context is  initialized !!!!
//	Serial.begin(115200);
//	Serial.println("HELLO");
	Serial.printf("\n%s", CTX.dumpMOVctx().c_str());
	Serial.printf("\n###########################################################################");
//	Serial.printf("\nmot %i", ctx._gridLen);
	_prv0 = new uint8_t[ctx._gridLen];
	_cur0 = new uint8_t[ctx._gridLen];
//	Serial.printf("\nmot %p %p", _prv0, _cur0);

	uint8_t *_prv = prvGrid();
	uint8_t *_cur = curGrid();
	for (size_t i = 0; i < ctx._gridLen; i++) {
		*_prv++ = 0;
		*_cur++ = 0;
	}
	return true;
}
//---------------------------------------------------------------------
template<typename PIXL>
void legend(c_Image<PIXL> &IMG, c_gTerminal<PIXL> &gtxt){
	IMG.fillRectangle(uint16_t{4}, uint16_t{20}, uint16_t{50}, uint16_t{100}, PIXL(0xff, 0xff, 0xff));
	for (size_t i = 0; i < 6; i++) {
		gtxt.setTo(5, 25 + 15 * i, IMG.palette(i));
		gtxt.gprintf("\e");
		gtxt.setTo(20, 25 + 15 * i);
		gtxt.gprintf("%i", i);
	}
	IMG.drawCircle(200, 200, 150, PIXL(0xff, 0xff, 0xff));
	IMG.drawCircle(200, 200, 140, PIXL(255,255,255), -PI / 4, -3 * PI / 4);
}
//---------------------------------------------------------------------
template<typename PIXL>
void run(camera_fb_t *FRAME, uint16_t &changes, float &ratio, const char *COMMENT){
	uint8_t mediumBrightness;
	c_gTerminal<PIXL> gtxt(FRAME->width, FRAME->height, FRAME->buf);
	c_Image<PIXL> image(FRAME->width, FRAME->height, FRAME->buf);
	MoveWatcher<PIXL> movement(&image, &motionCtx);
//	printStackLeft(__FUNCTION__);
//	Serial.printf("\nrun %p %p", motionCtx.prvGrid(), motionCtx.curGrid());

	if (motionCtx.dbgLevel() > 0) {
		timeit("Decode", mediumBrightness = movement.Decode());
//		return; // false;
		timeit("Detect", movement.Detect());
	} else {
		mediumBrightness = movement.Decode();
//		return; // false;
		movement.Detect();
	}
	if (motionCtx.dbgLevel() > 0)
		legend<PIXL>(image, gtxt);
	ratio = movement.ratio();
	uint16_t abschanges = movement.absChanges();
	uint16_t relchanges = movement.relChanges();
	changes = movement.changes();
	uint16_t cnt = 0;
	char buffer[128];
	bool alertCount = motionCtx.countMax() < changes;
	bool alertRatio = motionCtx.ratioMax() < ratio;
	bool alert = alertCount || alertRatio;
//	cnt += sprintf(&buffer[cnt], "%s cnt:abs+rel=%i+%i=%i%s%i, ratio=%.2f%%%s%.2f%%.",
	cnt +=
		sprintf(&buffer[cnt],
			"%s count: abs(%7i) + rel(%7i) =%7i %s %-5i, ratio=%6.2f%% %s %6.2f%%. brightness %3i errors(%i/%i) ",
			alert ? "ALERT!!" : "Changes",
			abschanges,
			relchanges,
			changes,
			alertCount ? ">" : "<",
			motionCtx.countMax(),
			100 * ratio,
			alertRatio ? ">" : "<",
			100 * motionCtx.ratioMax(),
			mediumBrightness,
			motionCtx.cnterror,
			motionCtx.cntloop
			);
	_DEBUG_Motion_(0, "\n%s", buffer);
	if (motionCtx.dbgLevel() > 2) {
		gtxt.setTo(5, 5, PIXL(0xff, 0xff, 0xff), true);
		gtxt.gprintf("%s", buffer);
	}
	if (motionCtx.dbgLevel() > 3)
		image.pattern();
}
//---------------------------------------------------------------------
void motionDetect(camera_fb_t *FRAME, const char *COMMENT){
//	printStackLeft(__FUNCTION__);
	typedef struct {

	} s_report;
	_DEBUG_Motion_(1, "\n----------------------------------------------------------- %s", COMMENT);
	_DEBUG_Others_(8, "\n%s", CTX.dumpBOOTctx().c_str());
	_DEBUG_Others_(8, "\n%s", CTX.dumpCAMctx().c_str());
	_DEBUG_Others_(8, "\n%s", CTX.dumpMOVctx().c_str());

	_DEBUG_Motion_(3, " (%ix%i)^%i=>(%ix%i), grid (step,area)=(%i,%i)", FRAME->width, FRAME->height,
		channels(FRAME),
		FRAME->width / motionCtx.gridStep(), FRAME->height / motionCtx.gridStep(),
		motionCtx.gridStep(),
		(FRAME->width / motionCtx.gridStep()) * (FRAME->height / motionCtx.gridStep()));
	_DEBUG_Motion_(6, "\n%s", frame_Infos(FRAME, "#### MOTION input ").c_str());
	motionCtx.cntloop++;
	if (channels(FRAME) < 1) {
		motionCtx.cnterror++;
		return;
	}
	float ratio = 0.0; // to permit return of run
	uint16_t changes = 0;
	switch(FRAME->format){
		case PIXFORMAT_RGB565:
			run<RGB565_t>(FRAME, changes, ratio, COMMENT);
			break;
		case PIXFORMAT_GRAYSCALE:
			run<MONO8_t>(FRAME, changes, ratio, COMMENT);
			break;
		case PIXFORMAT_RGB888:
			run<RGB888_t>(FRAME, changes, ratio, COMMENT);
			break;
		default:
			return;
	}
	_DEBUG_Motion_(6, "\n%s", frame_Infos(FRAME, "#### MOTION output").c_str());
	if ((motionCtx.ratioMax() < ratio) || (motionCtx.countMax() < changes)) {
		if (motionCtx.ftp()) {
			if (FRAME->format == PIXFORMAT_RGB565) {
				convert_565to555(FRAME);
				upload_BMP_frame(FRAME);
				convert_555to565(FRAME);
			}
			else
				upload_BMP_frame(FRAME);
		}
		if (motionCtx.sms())
			SEND_SMS("Alerte...\rÇa bouge à Barbe d'Or?");
	}

}
///////////////////////////////////////////////////////////////////////
