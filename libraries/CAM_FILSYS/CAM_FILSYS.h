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
/**
 * @fn bool split_Up_Before(const uint16_t cnt,const char *chn,
	std::string &first, std::string &second, const char *charPatterns = "/\\")
 * @brief split chn in 2 strings from start upto end on the nth pattern
 *
 * Warning: make a copy of *chn on the stack!
 *
 * @param nth count of recognized patterns to browse
 * @param chn to be cut
 * @param first part of chn excluding the nth pattern found at its end
 * @param second part of chn including the nth pattern found at its beginning
 * @param charPatterns array of char to search
 * @return true if the nth pattern char is found else false
 */
bool split_Up_Before(const uint16_t nth,const char *chn,
	std::string &first, std::string &second, const char *charPatterns = "/\\");
void testFileIO(fs::FS &fs, const char *path);
void deleteFile(fs::FS &fs, const char *path);
void renameFile(fs::FS &fs, const char *path1, const char *path2);
void appendFile(fs::FS &fs, const char *path, const char *message);
void writeFile(fs::FS &fs, const char *path, const char *message);
void readFile(fs::FS &fs, const char *path);
void removeDir(fs::FS &fs, const char *path);
void createDir(fs::FS &fs, const char *path);
/**
 * @fn void listDirJSON(char **buffer, const char *volpath, const uint8_t depth = 255)
 * @brief return in *buffer a text JSON HTML table list of files
 * @param format for output: console:"TEXT", table:"HTML"
 * @param buffer address of char buffer
 * @param volName volume from where list is done (/FFS/... or /SSD/...)
 * @param dirname root from where list is done (/...)
 * @param depth of folder scan
 */
void listDir(const char *format, char **buffer, const char *volName,
	const char *dirname = "/", const uint8_t depth = 255);
bool deleteFile(const char *volpath);
void volSizes(const char *volName, uint64_t &totalBytes, uint64_t &usedBytes);
/**
 * @fn int64_t volFreeSize(const char * volPathFilename)
 * @brief bytes free on volume (-1 if volume is not mount)
 * @param volName can be a complete file name or /FFS /SSD minimum
 * @return free size on volume
 */
int64_t volFreeSize(const char *volPathFilename);
///////////////////////////////////////////////////////////////////////
#endif
