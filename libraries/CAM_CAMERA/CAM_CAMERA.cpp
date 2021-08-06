#include "CAM_CAMERA.h"
#include "pix_t.h"
//#include "CAM_FTPCLI.h"
//#include "esp32_SMS_client.h"
//#include "CAM_MOTION.h"
//#include <functional>
#include "esp_http_server.h"
#include "camera_pins.h"
///////////////////////////////////////////////////////////////////////
//
// WARNING!!! PSRAM IC required for UXGA resolution and high JPEG quality
//            Ensure ESP32 Wrover Module or other board with PSRAM is selected
//            Partial images will be transmitted if image exceeds buffer size
//
c_Camera cameraCtx(&CTX.CAMctx);
///////////////////////////////////////////////////////////////////////
void startCameraServer(const httpd_config_t *SRVCFG = nullptr); // automatically included from app_httpd.cpp !!!
//---------------------------------------------------------------------
	/*
	 *  frameSize   | 1BpP GRAY   | 2BpP rgb565 | 3BpP rgb888   bytes/pixel
	 * -------------+-------------+-------------+----------
	 * 1280 x 1024	| 1.310720 OK | 2.621440-no | 3.932160-no
	 * 1024 x  768	|  .786432 OK | 1.572864 OK | 2.359296-no
	 *  800 x  600	|  .480000 OK |  .960000 OK | 1.440000 OK
	 *
	 0  *  PIXEL_FORMAT_RGB565,    // 2BPP/RGB565
	 1     PIXEL_FORMAT_YUV422,    // 2BPP/YUV422
	 2     PIXEL_FORMAT_GRAYSCALE, // 1BPP/GRAYSCALE
	 3     PIXEL_FORMAT_JPEG,      // JPEG/COMPRESSED
	 4  *  PIXEL_FORMAT_RGB888,    // 3BPP/RGB888
	 5     PIXEL_FORMAT_RAW,       // RAW
	 6     PIXEL_FORMAT_RGB444,    // 3BP2P/RGB444
	 7  *  PIXEL_FORMAT_RGB555,    // 3BP2P/RGB555
	 */
pixformat_t get_goodPixFormat(const pixformat_t pixFormat, const framesize_t framSize){
	if (pixFormat == PIXFORMAT_JPEG)
		return pixFormat;
	if (framSize >= FRAMESIZE_SXGA) // 1280x1024
		return PIXFORMAT_GRAYSCALE;
	else if (framSize >= FRAMESIZE_XGA) // {1024x768}
		if (pixFormat != PIXFORMAT_GRAYSCALE)
			return PIXFORMAT_RGB565;
	/*
	 else if (framSize >= FRAMESIZE_SVGA) // 800x600
	 if (pixFormat != PIXFORMAT_GRAYSCALE)
	 return PIXFORMAT_RGB565;
	 */
	return pixFormat;
}
//---------------------------------------------------------------------
bool c_Camera::initCamera(const framesize_t FRAME_SIZE, const pixformat_t PIXEL_FORMAT){
	esp_camera_deinit();

	camera_config_t config;
	config.ledc_channel = LEDC_CHANNEL_0;
	config.ledc_timer = LEDC_TIMER_0;
	config.pin_d0 = Y2_GPIO_NUM;
	config.pin_d1 = Y3_GPIO_NUM;
	config.pin_d2 = Y4_GPIO_NUM;
	config.pin_d3 = Y5_GPIO_NUM;
	config.pin_d4 = Y6_GPIO_NUM;
	config.pin_d5 = Y7_GPIO_NUM;
	config.pin_d6 = Y8_GPIO_NUM;
	config.pin_d7 = Y9_GPIO_NUM;
	config.pin_xclk = XCLK_GPIO_NUM;
	config.pin_pclk = PCLK_GPIO_NUM;
	config.pin_vsync = VSYNC_GPIO_NUM;
	config.pin_href = HREF_GPIO_NUM;
	config.pin_sscb_sda = SIOD_GPIO_NUM;
	config.pin_sscb_scl = SIOC_GPIO_NUM;
	config.pin_pwdn = PWDN_GPIO_NUM;
	config.pin_reset = RESET_GPIO_NUM;
	config.xclk_freq_hz = 20000000;
	config.frame_size = FRAME_SIZE;
	config.pixel_format = PIXEL_FORMAT; //Before it was PIXEL_FORMAT_JPEG;
	if (psramFound()) {
		config.jpeg_quality = 10;
//    .fb_count = 1 //if more than one, i2s runs in continuous mode. Use only with JPEG cf. esp-32-camera-master/README.md

		config.fb_count = 2;
	} else {
		config.jpeg_quality = 12;
		config.fb_count = 1;
	}
	Serial.printf(
		"\nTrying to initialize camera: %s %s, JPEG_QUALITY %i, BUFFER_COUNT %i",
		textFrameSize(FRAME_SIZE).c_str(), textPixFormat(PIXEL_FORMAT).c_str(),
		config.jpeg_quality, config.fb_count);
#if defined(CAMERA_MODEL_ESP_EYE)
		pinMode(13, INPUT_PULLUP);
		pinMode(14, INPUT_PULLUP);
#endif

// camera init
	esp_err_t err = esp_camera_init(&config);
	if (err != ESP_OK) {
		halt("\nCamera init failed with error 0x%x", err);
		return false;
	}

	sensor_t *s = esp_camera_sensor_get();
// initial sensors are flipped vertically and colors are a bit saturated
	if (s->id.PID == OV3660_PID) {
		s->set_vflip(s, 1); // flip it back
		s->set_brightness(s, 1); // up the brightness just a bit
		s->set_saturation(s, -2); // lower the saturation
	}
// drop down frame size for higher initial frame rate
//	s->set_framesize(s, FRAMESIZE_QVGA);

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
		s->set_vflip(s, 1);
		s->set_hmirror(s, 1);
#endif
//Get camera infos to be extern
	camera_fb_t *fb = esp_camera_fb_get();
	Serial.printf("\n%s\n", frame_Infos(fb, "\n#### CAMERA INITIALISEE").c_str());
	ctx._framSize = FRAME_SIZE;
	ctx._pixFormat = PIXEL_FORMAT;
	ctx._width = fb->width;
	ctx._height = fb->height;
	esp_camera_fb_return(fb); //		fb = nullptr;
	return true;

}
//---------------------------------------------------------------------
bool SETUP_CAMERA(const framesize_t FRAME_SIZE, const pixformat_t PIXEL_FORMAT){
	if (!cameraCtx.initCamera(FRAME_SIZE, PIXEL_FORMAT))
		return false;
	Serial.printf("\n#### SETUP_CAMERA ok.\n");

	TEST_MONO8();
	TEST_RGB888();
	TEST_RGB565();
	TEST_RGBcast();

	return true;
}
//---------------------------------------------------------------------
int8_t channels(const camera_fb_t *FRAME){
	if (FRAME->format == PIXFORMAT_JPEG)
		return 0;
	uint32_t surface = (uint32_t)FRAME->width * (uint32_t)FRAME->height;
	int8_t channels = FRAME->len / surface;
	if (FRAME->len == channels * surface)
		return channels;
	Serial.printf(" #### channels=-1!");
	return -1;
}
//---------------------------------------------------------------------
/*
 bool uploadIMGjpg(camera_fb_t *IMG, const uint8_t QUALITY){
 Serial.printf("\nuploadIMGjpg(IMG=%p[%i:%ix%i]%i)",
 IMG, IMG->len, IMG->width, IMG->height, IMG->format);
 bool ok = false;
 uint8_t *jpeg;
 size_t len;
 if (fmt2jpg(IMG->buf, IMG->len, IMG->width, IMG->height, IMG->format, QUALITY, &jpeg, &len)) {
 Serial.printf("\nIMG(%6i):", IMG->len);
 for (size_t i = 0; i < 16; i++)
 Serial.printf(" %02X", IMG->buf[i]);
 Serial.printf("\nJPG(%6i):", len);
 for (size_t i = 0; i < 16; i++)
 Serial.printf(" %02X", jpeg[i]);
 Serial.printf("\n");
 ok = upload_BIN_buffer(jpeg, len);
 }
 free(jpeg);
 return ok;
 }
 */
//---------------------------------------------------------------------
String frame_Infos(const camera_fb_t *FRAME, const char *COMMENT, const char *BUFNAM){
	String form;
	switch(FRAME->format){
		case PIXFORMAT_RGB565:
			form = "2BPP/RGB565.";
			break;
		case PIXFORMAT_YUV422:
			form = "2BPP/YUV422.";
			break;
		case PIXFORMAT_GRAYSCALE:
			form = "1BPP/GRAY...";
			break;
		case PIXFORMAT_JPEG:
			form = "JPEG/COMPRES";
			break;
		case PIXFORMAT_RGB888:
			form = "3BPP/RGB888.";
			break;
		case PIXFORMAT_RAW:
			form = "RAW.........";
			break;
		case PIXFORMAT_RGB444:
			form = "3BP2P/RGB444";
			break;
		case PIXFORMAT_RGB555:
			form = "3BP2P/RGB555";
			break;
		default:
			form = "UNKNOWN!!!!!";
			break;
	}
	char buffer[256];
	int8_t okchannels = channels(FRAME);
	int16_t cnt = 0;
	if (COMMENT)
		cnt += sprintf((char*)&buffer[cnt], "%s: ", COMMENT);
	cnt += sprintf((char*)&buffer[cnt], "%s %s, WxH(%4ix%-4i)^%i=",
		form.c_str(), (0 < okchannels) ? "OK" : (!okchannels ? "JPEG?" : "ERROR"),
		FRAME->width, FRAME->height, okchannels);
	cnt += sprintf((char*)&buffer[cnt], "%s", buf_Info(FRAME->buf, FRAME->len, BUFNAM).c_str());
	return String(buffer);
}
//---------------------------------------------------------------------
String jpeg_Infos(const uint8_t *JPGBUF, const size_t JPGLEN,
	const char *COMMENT, const char *BUFNAM){
	char buffer[150];
	int16_t cnt = 0;
	if (COMMENT)
		cnt += sprintf((char*)&buffer[cnt], "%s:                   ", COMMENT);
	cnt += sprintf((char*)&buffer[cnt], "%s", buf_Info(JPGBUF, JPGLEN, BUFNAM).c_str());
	return String(buffer);
}
//---------------------------------------------------------------------
bool convert_565to555(camera_fb_t *FRAME){
	if (FRAME->format != PIXFORMAT_RGB565)
		return false;
	RGB565_t *pB565 = (RGB565_t*)(FRAME->buf);
	RGB555_t *pB555 = (RGB555_t*)(FRAME->buf);
	for (size_t x = 0; x < (uint32_t)FRAME->width * FRAME->height; x++)
		*pB555++ = rgb555(*pB565++);
	FRAME->format = PIXFORMAT_RGB555;
	return true;
}
//---------------------------------------------------------------------
bool convert_555to565(camera_fb_t *FRAME){
	if (FRAME->format != PIXFORMAT_RGB555)
		return false;
	RGB565_t *pB565 = (RGB565_t*)(FRAME->buf);
	RGB555_t *pB555 = (RGB555_t*)(FRAME->buf);
	for (size_t x = 0; x < (uint32_t)FRAME->width * FRAME->height; x++)
		*pB565++ = rgb565(*pB555++);
	FRAME->format = PIXFORMAT_RGB565;
	return true;
}
///////////////////////////////////////////////////////////////////////
