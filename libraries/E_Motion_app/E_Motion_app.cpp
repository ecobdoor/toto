// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "esp_timer.h"
#include "img_converters.h"
#include "motion_index_html_gz.h"
#include "CAM_MOTION.h"
//#include "CAM_FTPCLI.h"
#include "E_Motion_app.h"
#include "esp32_HTTP_server.h"
///////////////////////////////////////////////////////////////////////
typedef struct {
	size_t size; //number of values used for filtering
	size_t index; //current value index
	size_t count; //value count
	int sum;
	int *values; //array to be filled with values
} ra_filter_t;

typedef struct {
	httpd_req_t *req;
	size_t len;
} jpg_chunking_t;
#define PART_BOUNDARY "123456789000000000000987654321"
static const char *_STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char *_STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char *_STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

static ra_filter_t ra_filter;
////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
static ra_filter_t* ra_filter_init(ra_filter_t *filter, size_t sample_size){
	memset(filter, 0, sizeof(ra_filter_t));

	filter->values = (int*)malloc(sample_size * sizeof(int));
	if (!filter->values) {
		return NULL;
	}
	memset(filter->values, 0, sample_size * sizeof(int));

	filter->size = sample_size;
	return filter;
}
//---------------------------------------------------------------------
static int ra_filter_run(ra_filter_t *filter, int value){
	if (!filter->values) {
		return value;
	}
	filter->sum -= filter->values[filter->index];
	filter->values[filter->index] = value;
	filter->sum += filter->values[filter->index];
	filter->index++;
	filter->index = filter->index % filter->size;
	if (filter->count < filter->size) {
		filter->count++;
	}
	return filter->sum / filter->count;
}
///////////////////////////////////////////////////////////////////////
/**
 * @fn static size_t jpg_encode_stream(void *arg, size_t index, const void *data, size_t len)
 * @brief Jpeg compression call back to send chunk response packets in response to the capture request from the client browser
 * @param arg jpg_chunking_t*
 * @param index number of packet
 * @param data to send
 * @param len length of packet to send
 * @return size_t length sent
 */
static size_t jpg_encode_stream(void *arg, size_t index, const void *data, size_t len){
	jpg_chunking_t *j = (jpg_chunking_t*)arg;
	if (!index) {
		j->len = 0;
		//ICB = imgCallback(jpegInfos, (uint8_t*)data, j->len + 10,			"@@@@ CONVERT from motion capture");
	}
	if (httpd_resp_send_chunk(j->req, (const char*)data, len) != ESP_OK) {
		return 0;
	}
	j->len += len;
	return len;
}
///////////////////////////////////////////////////////////////////////
/**
 * @fn static esp_err_t capture_handler(httpd_req_t *req)
 * @brief Capture handler in response to the capture request from the client browser
 * @param req request pointer
 * @return esp_err_t for httpd_resp_send(...)
 */
static esp_err_t capture_handler(httpd_req_t *req){
	browser.On();
	camera_fb_t *fb = NULL;
	esp_err_t res = ESP_OK;
	int64_t fr_start = esp_timer_get_time();

	fb = esp_camera_fb_get();
	if (!fb) {
		Serial.println("\nCamera capture failed");
		httpd_resp_send_500(req);
		browser.Off();
		return ESP_FAIL;
	}

	httpd_resp_set_type(req, "image/jpeg");
	httpd_resp_set_hdr(req, "Content-Disposition", "inline; filename=capture.jpg");
	httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");

	size_t fb_len = 0;
	if (fb->format == PIXFORMAT_JPEG) {
		fb_len = fb->len;
		//if (motionCtx.ftp())upload_BIN_buffer(fb->buf, fb->len);// to precise if no motion detect
		res = httpd_resp_send(req, (const char*)fb->buf, fb->len);
	} else {
		if (motionCtx.active())
			motionDetect(fb, "capture A frame motion");
		jpg_chunking_t jchunk = { req, 0 };
		res = frame2jpg_cb(fb, 80, jpg_encode_stream, &jchunk) ? ESP_OK : ESP_FAIL;
		httpd_resp_send_chunk(req, NULL, 0);
		fb_len = jchunk.len;
		//if (motionCtx.ftp())upload... (const char*)fb->buf, fb->len,"capture 1 frame2jpg_cb");// to precise if no motion detect
	}
	esp_camera_fb_return(fb); // delete the fb frame buffer
	int64_t fr_end = esp_timer_get_time();
	_DEBUG_Motion_(0, "\nJPG: %u Bytes %ums", (uint32_t)(fb_len),
		(uint32_t)((fr_end - fr_start) / 1000));
	browser.Off();
	return res;
}
///////////////////////////////////////////////////////////////////////
/**
 * @fn static esp_err_t stream_handler(httpd_req_t *req)
 * @brief Streaming handler in response to the stream request from the client browser
 * @param req request pointer
 * @return esp_err_t for httpd_resp_send(...)
 */
static esp_err_t stream_handler(httpd_req_t *req){
	browser.On();
	int jpgQuality = 80;
	camera_fb_t *fb = NULL;
	esp_err_t res = ESP_OK;
	size_t _jpg_buf_len = 0;
	size_t _jpg_buf_len_prev2 = 0;
	size_t _jpg_buf_len_prev1 = 0;
	uint8_t *_jpg_buf = NULL;
	char *part_buf[64];
	int64_t fr_start = 0;
	int64_t fr_ready = 0;
	int64_t fr_face = 0;
	int64_t fr_recognize = 0;
	int64_t fr_encode = 0;

	static int64_t last_frame = 0;
	if (!last_frame)
		last_frame = esp_timer_get_time();

	res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
	if (res != ESP_OK) {
		browser.Off();
		return res;
	}

	httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");

	while (true) {
		fb = esp_camera_fb_get();
		if (!fb) {
			Serial.println("Camera capture failed");
			res = ESP_FAIL;
		} else {
			fr_start = esp_timer_get_time();
			fr_ready = fr_start;
			fr_face = fr_start;
			fr_encode = fr_start;
			fr_recognize = fr_start;
			if (fb->format != PIXFORMAT_JPEG) {
				if (motionCtx.active())
					motionDetect(fb, "stream  A frame motion");
				/**
				 * 320x240 quality 80 ok for streaming
				 * 400x296 quality 80 ok for streaming
				 * 640x480 quality 30 ok for streaming image without decoder grid
				 */
				bool jpeg_converted;
				/*					jpeg_converted= frame2jpg(fb,
				 get_jpgQuality(cameraCtx.framSize(), cameraCtx.pixFormat()), &_jpg_buf,
				 &_jpg_buf_len); //80 Initial quality
				 */
				bool jpgOk;
				int jpgQualityMAX = 100;
				int jpgQualityMIN = 5;

				do {
					jpeg_converted = frame2jpg(fb, jpgQuality, &_jpg_buf, &_jpg_buf_len);
					_DEBUG_Motion_(2, "\n>>>> QUAL=%3i, LEN=%i, prv1=%i, prv2=%i",
						jpgQuality, _jpg_buf_len, _jpg_buf_len_prev1, _jpg_buf_len_prev2);
					if (!jpeg_converted) {
						Serial.println("JPEG compression failed");
						res = ESP_FAIL;
						break;
					}
					jpgOk = ((50000 <= _jpg_buf_len) && (_jpg_buf_len <= 65500));
					if (jpgOk)
						break;
					else {
						if ((_jpg_buf_len == _jpg_buf_len_prev2) && (_jpg_buf_len <= 65500))
							break; // finish in case of oscillation...
						if (_jpg_buf_len < 50000) {
							//jpgQuality += 5;
							jpgQualityMIN = jpgQuality + 1;
							jpgQuality = (jpgQuality + jpgQualityMAX) / 2;
						}
						if (65500 < _jpg_buf_len) {
							//jpgQuality -= 5;
							jpgQualityMAX = jpgQuality - 1;
							jpgQuality = (jpgQuality + jpgQualityMIN) / 2;
						}
						free(_jpg_buf);
						_jpg_buf = nullptr;
					}
					_jpg_buf_len_prev2 = _jpg_buf_len_prev1;
					_jpg_buf_len_prev1 = _jpg_buf_len;
				} while (!jpgOk);
				//ICB = imgCallback(frameInfos, fb, "stream  A frame motion");
				_DEBUG_Motion_(3, "\n%s",
					jpeg_Infos(_jpg_buf, _jpg_buf_len, "@@@@ CONVERT from motion stream ").c_str());
				esp_camera_fb_return(fb); // delete the fb frame buffer
				fb = NULL;
				//if (motionCtx.ftp()) upload_BIN_buffer(fb->buf, fb->len);// to precise if no motion detect
			} else { //with direct PIXFORMAT_JPEG never ftp!!!
				_jpg_buf_len = fb->len;
				_jpg_buf = fb->buf;
			}
		}
		//-------------------------------------------------------------
		// ENVOI DES REQU??TES DU STREAMING
		// Cf. https://tropratik.fr/camera-wi-fi-esp32-cam-avec-arduino/5
		if (res == ESP_OK) {
			// ENVOI DU SEPARATEUR CHUNCK "\r\n--" PART_BOUNDARY "\r\n"
			res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
		}
		if (res == ESP_OK) {
			// ENVOI DE LA LONGUEUR DU JPG BUFFER "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n"
			size_t hlen = snprintf((char*)part_buf, 64, _STREAM_PART, _jpg_buf_len);
			res = httpd_resp_send_chunk(req, (const char*)part_buf, hlen);
		}
		if (res == ESP_OK) {
			// ENVOI DU JPG BUFFER
			res = httpd_resp_send_chunk(req, (const char*)_jpg_buf, _jpg_buf_len);
		}
		//-------------------------------------------------------------
		// LIBERATION MEMOIRE
		if (fb) {
			esp_camera_fb_return(fb); // delete the fb frame buffer
			fb = NULL;
			_jpg_buf = NULL;
		} else if (_jpg_buf) {
			free(_jpg_buf);
			_jpg_buf = NULL;
		}
		//-------------------------------------------------------------
		// BREAK WHILE ON ERROR
		if (res != ESP_OK)
			break;
		//-------------------------------------------------------------
		// REPORT EXECUTION TIMING
		int64_t fr_end = esp_timer_get_time();

		int64_t ready_time = (fr_ready - fr_start) / 1000;
		int64_t face_time = (fr_face - fr_ready) / 1000;
		int64_t recognize_time = (fr_recognize - fr_face) / 1000;
		int64_t encode_time = (fr_encode - fr_recognize) / 1000;
		int64_t process_time = (fr_encode - fr_start) / 1000;

		int64_t frame_time = fr_end - last_frame;
		last_frame = fr_end;
		frame_time /= 1000;
		uint32_t avg_frame_time = ra_filter_run(&ra_filter, frame_time);
		_DEBUG_Motion_(1, "\n->MJPG(%u) %ums (%.1ffps), AVG: %ums (%.1ffps), %u+%u+%u+%u=%u",
			(uint32_t)(_jpg_buf_len),
			(uint32_t )frame_time, 1000.0 / (uint32_t )frame_time,
			avg_frame_time, 1000.0 / avg_frame_time,
			(uint32_t )ready_time, (uint32_t )face_time, (uint32_t )recognize_time,
			(uint32_t )encode_time, (uint32_t )process_time);
		//MJPG: 11167B 31ms (32.3fps), AVG: 40ms (25.0fps), 0+0+0+0=0 0
	}
	last_frame = 0;
	browser.Off();
	return res;
}

//---------------------------------------------------------------------
int setGriStep(const char *variable, const uint8_t val, char **ppjs = nullptr){
	uint8_t stepmin = motionCtx.get_minGridStep(cameraCtx.width(), cameraCtx.height());
	_DEBUG_Motion_(3, "\nsetGriStep(%i) & %i=get_minGridStep(%i,%i)",
		val, stepmin, cameraCtx.width(), cameraCtx.height());
	if (stepmin <= val)
		motionCtx.set_gridStep(val);
	else {
		motionCtx.set_gridStep(stepmin);
		if (*ppjs) {
			*ppjs += sprintf(*ppjs,
				"\"%s\":\"\\nFor a (%ix%i) frame size, grid step is limited to %u.\",",
				"MSGgridStep", cameraCtx.width(), cameraCtx.height(), stepmin);
			*ppjs += sprintf(*ppjs, "\"%s\":%u,", variable, stepmin);
		}
	}
	return 0;
}
//---------------------------------------------------------------------
int setPixFormat(const char *variable, const pixformat_t val, const framesize_t framSize,
	char **ppjs = nullptr){
	if ((val == PIXFORMAT_JPEG) && (val != cameraCtx.pixFormat())) {
		cameraCtx.set_pixFormat(val);
		motionCtx.set_active(0);
		if (*ppjs) {
			*ppjs += sprintf(*ppjs,
				"\"MSG_actM\":\"\\nIn JPG format, motion detector is stopped.\",");
			*ppjs += sprintf(*ppjs, "\"actM\":0,");
		}
		return 0;
	}
	pixformat_t goodpixf = get_goodPixFormat(val, framSize);
	_DEBUG_Motion_(3, "\nsetPixFormat(%s) & %s=get_goodPixFormat(%s,%s)",
		textPixFormat(val).c_str(), textPixFormat(goodpixf).c_str(),
		textPixFormat(val).c_str(), textFrameSize(framSize).c_str());
	if (val == goodpixf) {
		if (val == cameraCtx.pixFormat()) // because set_pixFormat always reinits the camera (for test & debug...)
			return 0;
		cameraCtx.set_pixFormat(val);
	} else {
		if (*ppjs) {
			*ppjs += sprintf(*ppjs,
				"\"MSG_pixM\":\"\\nFor this camera resolution, pixel format is limited to %s .\",",
				textPixFormat(goodpixf).c_str());
			*ppjs += sprintf(*ppjs, "\"%s\":%u,", variable, goodpixf);
		}
		if (goodpixf == cameraCtx.pixFormat()) // because set_pixFormat always reinits the camera (for test & debug...)
			return 0;
		cameraCtx.set_pixFormat(goodpixf);
	}
	return 0;
}
//---------------------------------------------------------------------
int setFrameSize(const char *variable, const framesize_t val, char **ppjs = nullptr){
	_DEBUG_Motion_(3, "\nsetFrameSize(%s)", textFrameSize(val).c_str());
	setPixFormat("pixM", cameraCtx.pixFormat(), val, ppjs); // before set_frameSize !!!
	cameraCtx.set_frameSize(val); // executed on change javascript only
	setGriStep("dimM", motionCtx.gridStep(), ppjs); // after set_frameSize !!!
	return 0;
}
///////////////////////////////////////////////////////////////////////
/**
 * @fn static esp_err_t cmd_handler(httpd_req_t *req)
 * @brief updates any server parameters in response to the control request from the client browser
 * @param req request pointer
 * @return esp_err_t for httpd_resp_send(...)
 */
static esp_err_t cmd_handler(httpd_req_t *req){
	bool browserWasOFF = !browser.Running();
	if (browserWasOFF)
		browser.On();
	char *buf;
	size_t buf_len;
	char variable[32] = { 0, };
	char value[32] = { 0, };

	buf_len = httpd_req_get_url_query_len(req) + 1;
	if (buf_len > 1) {
		buf = (char*)malloc(buf_len);
		if (!buf) {
			httpd_resp_send_500(req);
			if (browserWasOFF)
				browser.Off();
			return ESP_FAIL;
		}
		if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
			if (httpd_query_key_value(buf, "var", variable, sizeof(variable)) == ESP_OK &&
				httpd_query_key_value(buf, "val", value, sizeof(value)) == ESP_OK) {
			} else {
				free(buf);
				httpd_resp_send_404(req);
				if (browserWasOFF)
					browser.Off();
				return ESP_FAIL;
			}
		} else {
			free(buf);
			httpd_resp_send_404(req);
			if (browserWasOFF)
				browser.Off();
			return ESP_FAIL;
		}
		free(buf);
	} else {
		httpd_resp_send_404(req);
		if (browserWasOFF)
			browser.Off();
		return ESP_FAIL;
	}

	int val = atoi(value);
	sensor_t *s = esp_camera_sensor_get();
	int res = 0;
///////////////////////////////////////////////////////////////////////
	static char json_response[256];
	char *pjson = json_response;
	*pjson++ = '{';
	if (!strcmp(variable, "actM"))
		res = motionCtx.set_active(val);
	else if (!strcmp(variable, "tagM"))
		res = motionCtx.set_marker((t_tagMotion)val);
	else if (!strcmp(variable, "dbgM"))
		res = motionCtx.set_dbgLevel(val);
	else if (!strcmp(variable, "ftpA"))
		res = motionCtx.set_ftp(val);
	else if (!strcmp(variable, "smsA"))
		res = motionCtx.set_sms(val);
	else if (!strcmp(variable, "absM"))
		res = motionCtx.set_absDelta(val);
	else if (!strcmp(variable, "relM"))
		res = motionCtx.set_relDelta(val / 100.0);
	else if (!strcmp(variable, "smoM"))
		res = motionCtx.set_smooth(val / 100.0);
	else if (!strcmp(variable, "cntM"))
		res = motionCtx.set_countMax(val);
	else if (!strcmp(variable, "ratM"))
		res = motionCtx.set_ratioMax(val / 100.0);
	else if (!strcmp(variable, "dimM"))
		res = setGriStep((char*)&variable, val, &pjson);
	else if (!strcmp(variable, "pixM"))
		res = setPixFormat((char*)&variable, (pixformat_t)val, cameraCtx.framSize(), &pjson);
	else if (!strcmp(variable, "framesize"))
		res = setFrameSize((char*)&variable, (framesize_t)val, &pjson);
///////////////////////////////////////////////////////////////////////
	else if (!strcmp(variable, "quality"))
		res = s->set_quality(s, val);
	else if (!strcmp(variable, "contrast"))
		res = s->set_contrast(s, val);
	else if (!strcmp(variable, "brightness"))
		res = s->set_brightness(s, val);
	else if (!strcmp(variable, "saturation"))
		res = s->set_saturation(s, val);
	else if (!strcmp(variable, "gainceiling"))
		res = s->set_gainceiling(s, (gainceiling_t)val);
	else if (!strcmp(variable, "colorbar"))
		res = s->set_colorbar(s, val);
	else if (!strcmp(variable, "awb"))
		res = s->set_whitebal(s, val);
	else if (!strcmp(variable, "agc"))
		res = s->set_gain_ctrl(s, val);
	else if (!strcmp(variable, "aec"))
		res = s->set_exposure_ctrl(s, val);
	else if (!strcmp(variable, "hmirror"))
		res = s->set_hmirror(s, val);
	else if (!strcmp(variable, "vflip"))
		res = s->set_vflip(s, val);
	else if (!strcmp(variable, "awb_gain"))
		res = s->set_awb_gain(s, val);
	else if (!strcmp(variable, "agc_gain"))
		res = s->set_agc_gain(s, val);
	else if (!strcmp(variable, "aec_value"))
		res = s->set_aec_value(s, val);
	else if (!strcmp(variable, "aec2"))
		res = s->set_aec2(s, val);
	else if (!strcmp(variable, "dcw"))
		res = s->set_dcw(s, val);
	else if (!strcmp(variable, "bpc"))
		res = s->set_bpc(s, val);
	else if (!strcmp(variable, "wpc"))
		res = s->set_wpc(s, val);
	else if (!strcmp(variable, "raw_gma"))
		res = s->set_raw_gma(s, val);
	else if (!strcmp(variable, "lenc"))
		res = s->set_lenc(s, val);
	else if (!strcmp(variable, "special_effect"))
		res = s->set_special_effect(s, val);
	else if (!strcmp(variable, "wb_mode"))
		res = s->set_wb_mode(s, val);
	else if (!strcmp(variable, "ae_level"))
		res = s->set_ae_level(s, val);
	else
		res = -1;
	/*
	 if (res) {
	 return httpd_resp_send_500(req);
	 }

	 httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
	 return httpd_resp_send(req, NULL, 0);
	 */
//	p += sprintf(p, "\"%s\":%u",variable, val);
	if (*(pjson - 1) == ',') // suppress the last comma!!!
		pjson--;
	*pjson++ = '}';
	*pjson++ = 0;
	httpd_resp_set_type(req, "application/json");
	httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
	if (browserWasOFF)
		browser.Off();
	return httpd_resp_send(req, json_response, strlen(json_response));
}
///////////////////////////////////////////////////////////////////////
/**
 * @fn static esp_err_t status_handler(httpd_req_t *req)
 * @brief returns the configuration parameters in response to the status request from the client browser
 * @param req request pointer
 * @return esp_err_t for httpd_resp_send(...)
 */
static esp_err_t status_handler(httpd_req_t *req){
	static char json_response[1024];

	sensor_t *s = esp_camera_sensor_get();
	char *p = json_response;
	*p++ = '{';
///////////////////////////////////////////////////////////////////////
	p += sprintf(p, "\"pixM\":%u,", cameraCtx.pixFormat());
	p += sprintf(p, "\"actM\":%u,", motionCtx.active());
	p += sprintf(p, "\"dbgM\":%u,", motionCtx.dbgLevel());
	p += sprintf(p, "\"tagM\":%u,", motionCtx.marker());
	p += sprintf(p, "\"dimM\":%u,", motionCtx.gridStep());
	p += sprintf(p, "\"smoM\":%u,", (uint8_t)(motionCtx.smooth() * 100));
	p += sprintf(p, "\"absM\":%u,", motionCtx.absDelta());
	p += sprintf(p, "\"relM\":%u,", (uint8_t)(motionCtx.relDelta() * 100));
	p += sprintf(p, "\"cntM\":%u,", motionCtx.countMax());
	p += sprintf(p, "\"ratM\":%u,", (uint8_t)(motionCtx.ratioMax() * 100));
	p += sprintf(p, "\"ftpA\":%u,", motionCtx.ftp());
	p += sprintf(p, "\"smsA\":%u,", motionCtx.sms());
	p += sprintf(p, "\"framesize\":%u,", s->status.framesize);
///////////////////////////////////////////////////////////////////////
	p += sprintf(p, "\"quality\":%u,", s->status.quality);
	p += sprintf(p, "\"brightness\":%d,", s->status.brightness);
	p += sprintf(p, "\"contrast\":%d,", s->status.contrast);
	p += sprintf(p, "\"saturation\":%d,", s->status.saturation);
	p += sprintf(p, "\"sharpness\":%d,", s->status.sharpness);
	p += sprintf(p, "\"special_effect\":%u,", s->status.special_effect);
	p += sprintf(p, "\"wb_mode\":%u,", s->status.wb_mode);
	p += sprintf(p, "\"awb\":%u,", s->status.awb);
	p += sprintf(p, "\"awb_gain\":%u,", s->status.awb_gain);
	p += sprintf(p, "\"aec\":%u,", s->status.aec);
	p += sprintf(p, "\"aec2\":%u,", s->status.aec2);
	p += sprintf(p, "\"ae_level\":%d,", s->status.ae_level);
	p += sprintf(p, "\"aec_value\":%u,", s->status.aec_value);
	p += sprintf(p, "\"agc\":%u,", s->status.agc);
	p += sprintf(p, "\"agc_gain\":%u,", s->status.agc_gain);
	p += sprintf(p, "\"gainceiling\":%u,", s->status.gainceiling);
	p += sprintf(p, "\"bpc\":%u,", s->status.bpc);
	p += sprintf(p, "\"wpc\":%u,", s->status.wpc);
	p += sprintf(p, "\"raw_gma\":%u,", s->status.raw_gma);
	p += sprintf(p, "\"lenc\":%u,", s->status.lenc);
	p += sprintf(p, "\"vflip\":%u,", s->status.vflip);
	p += sprintf(p, "\"hmirror\":%u,", s->status.hmirror);
	p += sprintf(p, "\"dcw\":%u,", s->status.dcw);
	p += sprintf(p, "\"colorbar\":%u", s->status.colorbar);
	*p++ = '}';
	*p++ = 0;
	httpd_resp_set_type(req, "application/json");
	httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
	return httpd_resp_send(req, json_response, strlen(json_response));
}
///////////////////////////////////////////////////////////////////////
/**
 * @fn static esp_err_t index_handler(httpd_req_t *req)
 * @brief Index home page handler
 * @param req
 * @return esp_err_t for httpd_resp_send(...)
 */
static esp_err_t index_handler(httpd_req_t *req){
	browser.On();
	httpd_resp_set_type(req, "text/html");
	httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
	sensor_t *s = esp_camera_sensor_get();
	browser.Off();
	//if (s->id.PID == OV3660_PID)		return httpd_resp_send(req, (const char*)index_ov3660_html_gz, index_ov3660_html_gz_len);
	return httpd_resp_send(req, (const char*)motion_index_ov2640_html_gz,
	motion_index_ov2640_html_gz_len);
}
///////////////////////////////////////////////////////////////////////

httpd_uri_t APP_home = {
	.uri = "/",
	.method = HTTP_GET,
	.handler = index_handler,
	.user_ctx = NULL
};

httpd_uri_t APP_status = {
	.uri = "/status",
	.method = HTTP_GET,
	.handler = status_handler,
	.user_ctx = NULL
};

httpd_uri_t APP_cmd = {
	.uri = "/control",
	.method = HTTP_GET,
	.handler = cmd_handler,
	.user_ctx = NULL
};

httpd_uri_t APP_capture = {
	.uri = "/capture",
	.method = HTTP_GET,
	.handler = capture_handler,
	.user_ctx = NULL
};

httpd_uri_t APP_stream = {
	.uri = "/stream",
	.method = HTTP_GET,
	.handler = stream_handler,
	.user_ctx = NULL
};
/**
 * @fn void startCameraServer(const httpd_config_t *SRVCFG = nullptr)
 * @brief Starts the camera server
 * @param SRVCFG to custom parameters as stack...
 */
void start_MOTION_webservices(httpd_handle_t server_httpd, httpd_handle_t stream_httpd){

	ra_filter_init(&ra_filter, 20);

	Serial.printf("\n%7x APP_home", httpd_register_uri_handler(server_httpd, &APP_home));
	Serial.printf("\n%7x APP_cmd", httpd_register_uri_handler(server_httpd, &APP_cmd));
	Serial.printf("\n%7x APP_status", httpd_register_uri_handler(server_httpd, &APP_status));
	Serial.printf("\n%7x APP_capture", httpd_register_uri_handler(server_httpd, &APP_capture));

	Serial.printf("\n%7x APP_stream",httpd_register_uri_handler(stream_httpd, &APP_stream));
}
///////////////////////////////////////////////////////////////////////
