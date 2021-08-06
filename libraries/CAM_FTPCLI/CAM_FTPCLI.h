#ifndef __000_FTPCLI__
#define __000_FTPCLI__
///////////////////////////////////////////////////////////////////////
#include <FS.h>
#include "esp32_FTP_client.h"
#include "esp_camera.h"
///////////////////////////////////////////////////////////////////////
/**
 * @fn bool SETUP_FTPCLI(const char *FTP_HOST, const uint16_t FTP_PORT, const char *FTP_LOGIN,
 const char *FTP_PSWD, const uint16_t TIMEOUT = 15000, const uint8_t VERBOSE = 2);
 * @brief Sets the extern ESP32_FTPClient *cliFTP variable
 * @param FTP_HOST ftp server dns name
 * @param FTP_PORT 21 (not tested for others values: SFTP...)
 * @param FTP_LOGIN login identifier
 * @param FTP_PSWD login password
 * @param TIMEOUT in milliseconds waiting for server response
 * @param VERBOSE 0:mute, 1:user comments, 2:deep client/server dialog
 * @return true
 */
bool SETUP_FTPCLI(const char *FTP_HOST, const uint16_t FTP_PORT, const char *FTP_LOGIN,
	const char *FTP_PSWD, const uint16_t TIMEOUT = 15000, const uint8_t VERBOSE = 2);
/**
 * @fn ftpConnect()
 * @brief Checks if FTP_HOST is connected else retry a new ftp server connection
 * @return true if success else false
 */
bool ftpConnect();
/**
 * @fn ftpListdir()
 * @brief Lists root files & dirs of ftp server
 * @return true if success else false
 */
bool ftpListdir();
/**
 * @fn uploadBINFIL(fs::FS &fs, const char *localFullFilename, const char *remotePath,
 const char *remoteName)
 * @brief upload a binary file to ftp server
 * @param fs file system (SPIFFS, SD, SDMMC...)
 * @param localFullFilename /path/file name
 * @param remotePath ftp folder
 * @param remoteName ftp file name
 * @return true if success else false
 */
bool uploadBINFIL(fs::FS &fs, const char *localFullFilename, const char *remotePath,
	const char *remoteName);
/**
 * @fn upload_BIN_buffer(uint8_t *BUF, const size_t LEN, const char *remotePath = "/!ESP32-CAM", const char *remoteName = "bdoor.jpg")
 * @brief Uploads a binary buffer as file to ftp server (directly usable to upload JPEG image buffers)
 *
 * Remote file name is prefixed with a 8 hexadecimal random identifier
 * @param BUF data buffer
 * @param LEN length of buffer
 * @param remotePath ftp folder
 * @param remoteName ftp file name
 * @return true if success else false
 */
bool upload_BIN_buffer(uint8_t *BUF, const size_t LEN,
	const char *remotePath = "/!ESP32-CAM", const char *remoteName = "bdoor.jpg");
/**
 * @fn upload_BMP_frame(const camera_fb_t *FRAME, const char *remotePath = "/!ESP32-CAM", const char *remoteName = "bdoor.bmp")
 * @brief Uploads a image frame as BMP file to ftp server (gray with 256 depth palette, rgb565 in rgb555 format, rgb888 as is)
 *
 * Remote file name is prefixed with a 8 hexadecimal random identifier
 * @param FRAME data image
 * @param remotePath ftp folder
 * @param remoteName ftp file name
 * @return true if success else false
 */
bool upload_BMP_frame(const camera_fb_t *FRAME,
	const char *remotePath = "/!ESP32-CAM", const char *remoteName = "bdoor.bmp");
///////////////////////////////////////////////////////////////////////
extern ESP32_FTPClient *cliFTP;
#endif
