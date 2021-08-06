#ifndef __000_FILSYS__
#define __000_FILSYS__
///////////////////////////////////////////////////////////////////////
//#include <Arduino.h>
//#include </home/bigboss/embedcpp-latest-released/eclipse/arduinoPlugin/packages/esp32/hardware/esp32/2.0.0-rc1/libraries/FS/src/FS.h>
#include <FS.h>
#include "SPIFFS.h" 
#include "SD_MMC.h" // psramFound()
///////////////////////////////////////////////////////////////////////
bool SETUP_FILSYS();
void testFileIO(fs::FS &fs, const char *path);
void deleteFile(fs::FS &fs, const char *path);
void renameFile(fs::FS &fs, const char *path1, const char *path2);
void appendFile(fs::FS &fs, const char *path, const char *message);
void writeFile(fs::FS &fs, const char *path, const char *message);
void readFile(fs::FS &fs, const char *path);
void removeDir(fs::FS &fs, const char *path);
void createDir(fs::FS &fs, const char *path);
void listDirSDC(const char *dirname, uint8_t depth = 255);
void listDirSPI(const char *dirname, uint8_t depth = 255);
/**
 * @fn void listDirJSON(char **buffer, const char *volpath, const uint8_t depth = 255)
 * @brief return in *buffer a text JSON HTML table list of files
 * @param buffer address of char buffer
 * @param volpath root from where list is done (/FFS/... or /SSD/...)
 * @param depth of folder scan
 */
void listDirHTML(char **buffer, const char *volpath, uint8_t depth= 255);
bool deleteFile( const char *volpath);
void volSizes(const String volName, uint64_t &totalBytes, uint64_t &usedBytes);
/**
 * @fn int64_t volFreeSize(const char * volPathFilename)
 * @brief bytes free on volume (-1 if volume is not mount)
 * @param volName can be a complete file name or /FFS /SSD minimum
 * @return free size on volume
 */
int64_t volFreeSize(const char * volPathFilename);
///////////////////////////////////////////////////////////////////////
#endif
