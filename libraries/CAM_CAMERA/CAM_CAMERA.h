#ifndef __000_CAMERA__
#define __000_CAMERA__
/**
 * exif - display|modify EXIF informations in JPEG files
 * 		Cf. https://fr.wikipedia.org/wiki/JPEG
 */
//#include "Arduino.h"
#include "CAM_CONTEXT.h"
#include "define_camera.h"
///////////////////////////////////////////////////////////////////////
/**
 * @brief printf a binary 76543210 byte (if bit==1 '@' else '-') for font debugging or others...
 */
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '@' : '-'), \
  (byte & 0x40 ? '@' : '-'), \
  (byte & 0x20 ? '@' : '-'), \
  (byte & 0x10 ? '@' : '-'), \
  (byte & 0x08 ? '@' : '-'), \
  (byte & 0x04 ? '@' : '-'), \
  (byte & 0x02 ? '@' : '-'), \
  (byte & 0x01 ? '@' : '-')
///////////////////////////////////////////////////////////////////////
/**
 typedef struct {
 uint8_t  * buf;              !< Pointer to the pixel data
 size_t  len;                 !< Length of the buffer in bytes
 size_t  width;               !< Width of the buffer in pixels
 size_t  height;              !< Height of the buffer in pixels
 pixformat_t format;         !< Format of the pixel data
 struct timeval timestamp;   !< Timestamp since boot of the first DMA buffer of the frame
 } camera_fb_t;

 typedef enum {
 0   FRAMESIZE_96X96,    //    96x96
 1 * FRAMESIZE_QQVGA,    //  160x120
 2   FRAMESIZE_QCIF,     //  176x144
 3 * FRAMESIZE_HQVGA,    //  240x176
 4   FRAMESIZE_240X240,  //  240x240
 5   FRAMESIZE_QVGA,     //  320x240
 6 * FRAMESIZE_CIF,      //  400x296
 7   FRAMESIZE_HVGA,     //  480x320
 8 * FRAMESIZE_VGA,      //  640x480
 9 * FRAMESIZE_SVGA,     //  800x600
 10 * FRAMESIZE_XGA,      // 1024x768
 11   FRAMESIZE_HD,       // 1280x720
 12 * FRAMESIZE_SXGA,     //1280x1024
 13 * FRAMESIZE_UXGA,     //1600x1200
 // 3MP Sensors
 14   FRAMESIZE_FHD,      //1920x1080
 15   FRAMESIZE_P_HD,     // 720x1280
 16   FRAMESIZE_P_3MP,    // 864x1536
 17   FRAMESIZE_QXGA,     //2048x1536
 // 5MP Sensors
 18   FRAMESIZE_QHD,      //2560x1440
 19   FRAMESIZE_WQXGA,    //2560x1600
 20   FRAMESIZE_P_FHD,    //1080x1920
 21   FRAMESIZE_QSXGA,    //2560x1920
 22   FRAMESIZE_INVALID
 } framesize_t  ;
 *
 * Source buffer format is on of:
 * typedef enum {
 0     PIXEL_FORMAT_RGB565,    // 2BPP/RGB565
 1     PIXEL_FORMAT_YUV422,    // 2BPP/YUV422
 2     PIXEL_FORMAT_GRAYSCALE, // 1BPP/GRAYSCALE
 3     PIXEL_FORMAT_JPEG,      // JPEG/COMPRESSED
 4     PIXEL_FORMAT_RGB888,    // 3BPP/RGB888
 5     PIXEL_FORMAT_RAW,       // RAW
 6     PIXEL_FORMAT_RGB444,    // 3BP2P/RGB444
 7     PIXEL_FORMAT_RGB555,    // 3BP2P/RGB555
 * } pixformat_t;
 *
 * @brief Configuration structure for camera initialization
 *
 typedef struct {
 int pin_pwdn;                   /*!< GPIO pin for camera power down line
 int pin_reset;                  /*!< GPIO pin for camera reset line
 int pin_xclk;                   /*!< GPIO pin for camera XCLK line
 int pin_sscb_sda;               /*!< GPIO pin for camera SDA line
 int pin_sscb_scl;               /*!< GPIO pin for camera SCL line
 int pin_d7;                     /*!< GPIO pin for camera D7 line
 int pin_d6;                     /*!< GPIO pin for camera D6 line
 int pin_d5;                     /*!< Gc_Kamera * PIO pin for camera D5 line
 int pin_d4;                     /*!< GPIO pin for camera D4 line
 int pin_d3;                     /*!< GPIO pin for camera D3 line
 int pin_d2;                     /*!< GPIO pin for camera D2 line
 int pin_d1;                     /*!< GPIO pin for camera D1 line
 int pin_d0;                     /*!< GPIO pin for camera D0 line
 int pin_vsync;                  /*!< GPIO pin for camera VSYNC line
 int pin_href;                   /*!< GPIO pin for camera HREF line
 int pin_pclk;         65556          /*!< GPIO pin for camera PCLK line

 int xclk_freq_hz;               /*!< Frequency of XCLK signal, in Hz. Either 20KHz or 10KHz for OV2640 double FPS (Experimental)

 ledc_timer_t ledc_timer;        /*!< LEDC timer to be used for generating XCLK
 ledc_channel_t ledc_channel;    /*!< LEDC channel to be used for generating XCLK

 pixformat_t pixel_format;       /*!< Format of the pixel data: PIXEL_FORMAT_ + YUV422|GRAYSCALE|RGB565|JPEG
 framesize_t   frame_size;         /*!< Size of the output image: FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA

 int jpeg_quality;               /*!< Quality of JPEG output. 0-63 lower means higher quality
 size_t  fb_count;                /*!< Number of frame buffers to be allocated. If more than one, then each frame will be acquired (double speed)
 } camera_config_t;

 #define HTTPD_DEFAULT_CONFIG() {                        \
        .task_priority      = tskIDLE_PRIORITY+5,       \
        .stack_size         = 4096,                     \
        .core_id            = tskNO_AFFINITY,           \
        .server_port        = 80,                       \
        .ctrl_port          = 32768,                    \
        .max_open_sockets   = 7,                        \
        .max_uri_handlers   = 8,                        \
        .max_resp_headers   = 8,                        \
        .backlog_conn       = 5,        c_Kamera *                 \
        .lru_purge_enable   = false,                    \
        .recv_wait_timeout  = 5,                        \
        .send_wait_timeout  = 5,                        \
        .global_user_ctx = NULL,                        \
        .global_user_ctx_free_fn = NULL,                \
        .global_transport_ctx = NULL,                   \
        .global_transport_ctx_free_fn = NULL,           \
        .open_fn = NULL,                                \
        .close_fn = NULL,                               \
        .uri_match_fn = NULL                            \
}
 */
///////////////////////////////////////////////////////////////////////
/**
 * @fn bool convert_565to555(camera_fb_t *FRAME)
 * @brief Converts a rgb565 frame to rgb555 format (bit green 0 preserved bit 15)
 * @param FRAME
 * @return true if FRAME format is rgb565, false otherwise
 */
bool convert_565to555(camera_fb_t *FRAME);
/**
 * @fn bool convert_565to555(camera_fb_t *FRAME)
 * @brief Converts a rgb555 frame to rgb565 format (bit green 0 restored from bit 15)
 * @param FRAME
 * @return true if FRAME format is rgb555, false otherwise
 */
bool convert_555to565(camera_fb_t *FRAME);
bool initCamera(const framesize_t FRAME_SIZE, const pixformat_t PIXEL_FORMAT);
String frame_Infos(const camera_fb_t *FRAME, const char *COMMENT, const char *BUFNAM = nullptr);
/**
 * @fn pixformat_t get_goodPixFormat(const pixformat_t pixFormat, const framesize_t framSize)
 * @brief check if a pixel format is supported depending on frame size
 *
 * Memory limitation forces to reduce pixel format as image size grows
 * @param pixFormat purposed pixel format
 * @param framSize purposed frame size
 * @return pixFormat if supported, else the best supported pixel format for framSize
 */
pixformat_t get_goodPixFormat(const pixformat_t pixFormat, const framesize_t framSize);
///////////////////////////////////////////////////////////////////////
class c_Camera {
	/*
	 typedef struct {
	 int jpgQuality_;
	 framesize_t frameSize_;
	 pixformat_t pixFormat_;
	 } s_modeCam;
	 s_modeCam modKam[1] = {
	 { .jpgQuality_ = 1, .frameSize_ = FRAMESIZE_VGA, .pixFormat_ = PIXFORMAT_RGB565 }
	 };
	 */
private:
	s_CAMERActx &ctx;
	public:
	c_Camera(s_CAMERActx *CTX) :
		ctx(*CTX){
	}
	bool initCamera(const framesize_t FRAME_SIZE, const pixformat_t PIXEL_FORMAT);
	bool set_pixFormat(const pixformat_t val){
		ctx._pixFormat = val;
		initCamera(ctx._framSize, ctx._pixFormat);
		sensor_t *s = esp_camera_sensor_get();
		return s->set_pixformat(s, ctx._pixFormat);
	}
	pixformat_t pixFormat(){
		return ctx._pixFormat;
	}
	bool set_frameSize(const framesize_t val){
		ctx._framSize = val;
		initCamera(ctx._framSize, ctx._pixFormat);
		sensor_t *s = esp_camera_sensor_get();
		return s->set_framesize(s, ctx._framSize);
	}
	framesize_t framSize(){
		return ctx._framSize;
	}
	uint16_t width(){
		return ctx._width;
	}
	uint16_t height(){
		return ctx._height;
	}
};
///////////////////////////////////////////////////////////////////////
bool SETUP_CAMERA(const framesize_t FRAME_SIZE = FRAMESIZE_SVGA,
	const pixformat_t PIXEL_FORMAT = PIXFORMAT_JPEG);
/**
 * @fn bool uploadIMGjpg(camera_fb_t *IMG, const uint8_t QUALITY = 10)
 * @brief convert IMG frame in jpg format and upload it on ftp server
 * @param IMG image frame
 * @param QUALITY for compression
 * @return true if uploaded else false if not converted or not uploaded
 */
//bool uploadIMGjpg(camera_fb_t *IMG, const uint8_t QUALITY = 10);
String jpeg_Infos(const uint8_t *JPGBUF, const size_t JPGLEN, const char *COMMENT = nullptr,
	const char *BUFNAM = nullptr);
/**
 * @fn int8_t channels(const  camera_fb_t *FRAME)
 * @brief Number of bytes/pixel (0 for jpeg or <0 if FRAME->len is not a multiple of width*height)
 * @param FRAME
 * @return cnt of channels
 */
int8_t channels(const camera_fb_t *FRAME); ///////////////////////////////////////////////////////////////////////
extern c_Camera cameraCtx;
#endif
