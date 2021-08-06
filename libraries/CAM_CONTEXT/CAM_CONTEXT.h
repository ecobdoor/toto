/** @file */
#ifndef __CAM_CONTEXT__
#define __CAM_CONTEXT__
#include "000_EEPROM.h"
#include "esp_camera.h"
///////////////////////////////////////////////////////////////////////

#define DEF_MOTION_GRIDLEN 12288 // 1024x768/4²=49152, 800x600/4²=30000, 640*480/4²=19200, 640*480/4²=12288
//#define DEF_MOTION_GRIDSTEP 30
#define DEF_MOTION_GRIDSTEP 5
#if 255 < DEF_MOTION_GRIDSTEP
#error ERROR DEF_MOTION_GRIDSTEP € [4...255] (cf. uint16_t area...)
#endif
#if DEF_MOTION_GRIDSTEP < 4
#error ERROR DEF_MOTION_GRIDSTEP € [4...255] (cf. uint16_t area...)
#endif
#define DEF_MOTION_ABS_DELTAMAX 50
#if 255 < DEF_MOTION_ABS_DELTAMAX
#error ERROR DEF_MOTION_ABS_DELTAMAX >255 Absolute threshold must € [1...255]
#endif
#if DEF_MOTION_ABS_DELTAMAX < 1
#error ERROR DEF_MOTION_ABS_DELTAMAX >255 Absolute threshold must € [1...255]
#endif
#define DEF_MOTION_REL_DELTAMAX 100 // percents !!!
#if (DEF_MOTION_REL_DELTAMAX<0)||(999 < DEF_MOTION_REL_DELTAMAX)
#error 0<=DEF_MOTION_REL_DELTAMAX<=999. Relative threshold must € [0%...999%]
#endif
#define DEF_MOTION_SMOOTH 33 // float value/100.0 !!!
#if (DEF_MOTION_SMOOTH < 0) || (100 < DEF_MOTION_SMOOTH)
#error 0<=DEF_MOTION_SMOOTH<=100. Smoothing factor must € [0...1]
#endif
#define  DEF_MOTION_COUNT_MAX 300 // absolute count of grid pixels up to 65536!!!
#define  DEF_MOTION_RATIO_MAX 10 // % of grid Area
#if (DEF_MOTION_RATIO_MAX < 0) || (100 < DEF_MOTION_RATIO_MAX)
#error 0<=DEF_MOTION_RATIO_MAX<=100. Ratio max must € [0...1]
#endif
typedef char t_char8[8];
///////////////////////////////////////////////////////////////////////
typedef struct {
	uint64_t TMS = 0;
	t_char8 Ename = "BOOT ! ";
	uint8_t CAMERA = 0;
	uint8_t MOTION = 0; // if int =>LoadStoreAlignment???
} s_BOOTctx;
///////////////////////////////////////////////////////////////////////
typedef enum {
	tag_no = 0,
	tag_detect,
	tag_all
} t_tagMotion;
typedef struct {
	uint64_t TMS = 0;
	uint16_t _gridLen = DEF_MOTION_GRIDLEN;
	uint16_t _countMax = DEF_MOTION_COUNT_MAX; // Absolute count of grid pixels up to 65536
	t_char8 Ename = "MOV ! ";
	bool _active = true;/** < indicator for web server to show motion behaviour */
	t_tagMotion _marker = tag_no;/** < indicator to mark or not motion detection in the camera frame */
	int8_t _dbgLevel = 0;
	uint8_t _gridStep = DEF_MOTION_GRIDSTEP;
	bool _sendFTP = false;
	bool _sendSMS = false;
	uint8_t _absDelta = DEF_MOTION_ABS_DELTAMAX; //...
	float _smooth = DEF_MOTION_SMOOTH / 100.0;
	float _relDelta = DEF_MOTION_REL_DELTAMAX / 100.0;
	float _ratioMax = DEF_MOTION_RATIO_MAX / 100.0; //  percent of grid Area
} s_MOTIONctx;
///////////////////////////////////////////////////////////////////////
typedef struct {
	uint64_t TMS = 0;
	t_char8 Ename = "CAM ! ";
	framesize_t _framSize = FRAMESIZE_VGA;
	pixformat_t _pixFormat = PIXFORMAT_RGB565;
	uint16_t _width;
	uint16_t _height;
} s_CAMERActx;
///////////////////////////////////////////////////////////////////////
String textPixFormat(const pixformat_t val);
String textFrameSize(const framesize_t val);
/**
 * https://github.com/espressif/arduino-esp32/blob/master/libraries/EEPROM/examples/eeprom_class/eeprom_class.ino
 * Context buttons
 * 		Restart
 * 		Boot 0 save boot on context 0 on next restart (default)
 * 		Boot 1 save boot on context 1 on next restart
 * 		Boot 2 save boot on context 2 on next restart
 * 		Save Boot 00
 * 		Save Boot 10
 * 		Save Boot 20
 * 		Save Boot 01
 * 		Save Boot 02
 * 		Save Boot 11
 * 		Save Boot 12
 * 		Save Boot 21
 * 		Save Boot 22
 * 		Save Camera 1 save current camera context in eeprom camera context 1
 * 		Save Camera 2 save current camera context in eeprom camera context 2
 * 		Save Motion 1 save current motion context in eeprom motion context 1
 * 		Save Motion 2 save current motion context in eeprom motion context 2
 *
 */
///////////////////////////////////////////////////////////////////////
bool SETUP_CONTEXT(const bool FACTORY_RESET = false);
class Context {
public:
	EEPROMClass &E_BOOT;
	EEPROMClass &E_CAM1;
	EEPROMClass &E_MOV1;
	EEPROMClass &E_CAM2;
	EEPROMClass &E_MOV2;

	 s_BOOTctx BOOTctx;
	 s_CAMERActx CAMctx;
	 s_MOTIONctx MOVctx;
	Context(
		EEPROMClass *E_B0,
		EEPROMClass *E_C1,
		EEPROMClass *E_M1,
		EEPROMClass *E_C2,
		EEPROMClass *E_M2
		);
	String dumpCAMctx();
	String dumpMOVctx();
	String dumpBOOTctx();
//	bool init(const uint8_t CTX);
};
///////////////////////////////////////////////////////////////////////
extern Context CTX;
#endif //__000_CONTEXT__
