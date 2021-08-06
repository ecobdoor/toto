#include "CAM_FILSYS.h"
#include "esp32_GOODIES.h"
#include <vector>
#include <string>
#include<unistd.h> // unlink
bool ffsON = false;
bool ssdON = false;
///////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
bool SETUP_FILSYS(){
	if (!SPIFFS.begin(false, "/FFS")) {
		Serial.print("\nNo SPIFFS !!!");
		return false;
	}
	ffsON = true;
	Serial.printf("\nSPIFFS started");
	Serial.printf("\n==========================");
	listDirSPI("/", 2);
	Serial.printf("\n==========================");
	listDirSPI("/titi/tutu", 3);
	Serial.printf("\n==========================");
	readFile(SPIFFS, "/ca_cert.pem");
	return true;

	if (!SD_MMC.begin("/sdcard", false)) {
		//if (!SD_MMC.begin("/sdcard", false, true)) { // FORMAT ????
		Serial.print("\nNo SD_MMC !!!");
		return false;
	}
	ssdON = true;
	Serial.printf("\nSDCARD started");
	Serial.printf("\n\tcardType   %i", SD_MMC.cardType());
	Serial.printf("\n\tcardSize   %10.3f Mo", SD_MMC.cardSize() / 1000000.0);
	Serial.printf("\n\ttotalBytes %10.3f Mo", SD_MMC.totalBytes() / 1000000.0);
	Serial.printf("\n\tusedBytes  %10.3f Mo", SD_MMC.usedBytes() / 1000000.0);
	Serial.printf("\n--------------------------");
	listDirSDC("/");
	Serial.printf("\n--------------------------");
	listDirSDC("/", 1);
	Serial.printf("\n--------------------------");
	listDirSDC("/toto", 2);
	Serial.printf("\n--------------------------");
	readFile(SD_MMC, "/ca_cert.pem");
	return true;
}
/*
 https://github.com/espressif/arduino-esp32/tree/master/libraries/SPIFFS
 */
void SplitOnLast(const std::string str, std::string &first, std::string &second,
	const char *charPatterns){
	std::size_t found = str.find_last_of(charPatterns);
	first = str.substr(0, found + 1);
	second = str.substr(found + 1);
}
//---------------------------------------------------------------------
void dirHTML(char **buffer, fs::FS &fs,
	const char *volName, const char *dirname, const uint8_t depth, const bool init,
	const uint64_t totalBytes = 0, const uint64_t usedBytes = 0){
	if (!depth)
		return;
	typedef struct {
		std::string _path;
		std::string _name;
		uint32_t _size;
		int _level;
		std::string _ext;
	} pa2img_t;
	static std::vector<pa2img_t> infos;
	if (init) {
		infos.clear();
	}
	Serial.printf("\n==== init(%s) cnt_fil %3i, depth %3i, path '%s'",
		(init) ? "YES" : "NO ", infos.size(), depth, dirname);
	File root = fs.open(dirname);
	if (!root) {
		*buffer += sprintf(*buffer, "Failed to open directory '%s'\\n", dirname);
		return;
	}
	File file = root.openNextFile();
	while (file) {
		Serial.printf("\n file '%s' '%s'", file.path(), file.name());
		if (file.isDirectory()) {
			if (depth) {
				dirHTML(buffer, fs, volName, file.path(), depth - 1, false);
			}
		} else {
//			SPrintF(sizeof(buffer), cnt, buffer, "%3i %7i\t%s\n", cnt_fil, file.size(),file.name());
			pa2img_t record;
			SplitOnLast(file.path(), record._path, record._name, "/\\");
			// string s=file.name();
			record._level = count(record._path.begin(), record._path.end(), '/');
			record._size = file.size();
			infos.push_back(record);
			Serial.printf("\n%03i) level %-3i, path '%s', name '%s', size '%-6i'", infos.size(),
				record._level, record._path.c_str(), record._name.c_str(), record._size);
		}
		file = root.openNextFile();
	}
	if (init) {
		std::sort(infos.begin(), infos.end(),
			[](pa2img_t const &a, pa2img_t const &b) -> bool{
				String A = String(a._path.c_str()) + "/" + String(a._name.c_str());
				String B = String(b._path.c_str()) + "/" + String(b._name.c_str());
//			return (a.record._level < b.record._level)&&()&&();
				return (A < B);
			});
		*buffer += sprintf(*buffer, "Directory '%s' (%7llu Bytes free)</br>\\n",
			dirname, totalBytes - usedBytes);
		*buffer +=
			sprintf(*buffer,
				"<table border=1> <tr><th>N°</th><th>Size</th><th>lvl</th><th>Folder</th><th>File</th></tr>\\n");
		for (size_t i = 0; i < infos.size(); i++) {
			std::string space = "";
			for (size_t j = 0; j < infos[i]._level; j++)
				space += "&nbsp;&nbsp;&nbsp;";
			*buffer +=
				sprintf(*buffer,
					"<tr><td>%3i</td><td align ='right'>%7i</td><td>%3i</td><td>%s</td>\\n",
					i, infos[i]._size, infos[i]._level, infos[i]._path.c_str());
			*buffer +=
				sprintf(*buffer,
					"<td>%s<a href='#' onClick=\\\"OTA_download('%s%s%s');\\\">%s</a></td>\\n",
					space.c_str(), volName, infos[i]._path.c_str(), infos[i]._name.c_str(),
					infos[i]._name.c_str());
			*buffer +=
				sprintf(*buffer,
					"<td><a href='#' onClick=\\\"OTA_delete('%s%s%s');\\\">X</a></td></tr>\\n",
					volName, infos[i]._path.c_str(), infos[i]._name.c_str());
		}
		*buffer += sprintf(*buffer, "</table>\\n");
	}
}
//---------------------------------------------------------------------
#define DEF_VOLEN 4

int64_t volFreeSize(const char * volPathFilename){
	char volBase[DEF_VOLEN + 1];
	strncpy(volBase, volPathFilename, DEF_VOLEN);
	volBase[DEF_VOLEN + 1] = 0;
	if (0 == strcmp("/FFS", volBase)) {
		return SPIFFS.totalBytes() - SPIFFS.usedBytes();
	}
	else if (0 == strcmp("/SSD", volBase)) {
		return SD_MMC.totalBytes() - SD_MMC.usedBytes();
	}
	return -1;
}
void volSizes(const String volName, uint64_t &totalBytes, uint64_t &usedBytes){
	if (String("/FFS") == volName) {
		totalBytes = SPIFFS.totalBytes();
		usedBytes = SPIFFS.usedBytes();
	}
	else if (String("/SSD") == volName) {
		totalBytes = SD_MMC.totalBytes();
		usedBytes = SD_MMC.usedBytes();
	}
	else {
		totalBytes = 0;
		usedBytes = 0;
	}
}
fs::FS* volFS(String &volName, const char *volpath){
	volName = String(volpath).substring(0, DEF_VOLEN);
	_DEBUG_Others_(3, "\nvstr='%s'", volName.c_str());
	if (String("/FFS") == volName)
		return &SPIFFS;
	if (String("/SSD") == volName)
		return &SD_MMC;
	return nullptr;
}
//---------------------------------------------------------------------
bool deleteFile(const char *volpath){
	String volName;
	fs::FS *fs = volFS(volName, volpath);
	Serial.printf("\nTry deleting %s on fs %p", volpath, fs);
	if (!fs)
		return false;
	return fs->remove(volpath + DEF_VOLEN);
}
//---------------------------------------------------------------------
void listDirHTML(char **buffer, const char *volpath, uint8_t depth){
	String volName;
	fs::FS *fs = volFS(volName, volpath);
	if (fs) {
		uint64_t totalBytes;
		uint64_t usedBytes;
		volSizes(volName, totalBytes, usedBytes);
		dirHTML(buffer, *fs, volName.c_str(), volpath + DEF_VOLEN,
			depth, true, totalBytes, usedBytes);
	}
}
//---------------------------------------------------------------------
void listDir(fs::FS &fs, const char *dirname, uint8_t depth, bool SPIFFS){
	static uint8_t level = 0;
	Serial.printf("\nListing directory: '%s' depth %i", dirname, depth);

	File root = fs.open(dirname);
	if (!root) {
		Serial.printf("\n%i Failed to open directory %s", depth, dirname);
		return;
	}
	if (!root.isDirectory()) {
		Serial.println("Not a directory");
		return;
	}
	File file = root.openNextFile();
	while (file) {
		if (!file.isDirectory()) {
			Serial.printf("\n%i %12i ", depth, file.size());
			if (SPIFFS) {
				Serial.printf(" %*s", 32, file.path());
			} else {
				Serial.printf(" %*s%*s ", 4 * level, "", 24, file.name());
			}
		}
		file = root.openNextFile();
	}
	root = fs.open(dirname);
	file = root.openNextFile();
	while (file) {
		if (file.isDirectory()) {
			//Serial.printf("\n%i  DIR SDcards: '%s'",depth,file.name());
			if (depth) {
				String dir = String(dirname);
				if (dir.length() > 1) // for "/" alone...
					dir += "/";
//		    	listDir(fs, (dir+String(file.name())).c_str(), depth -1,SPIFFS);
				level++;
				listDir(fs, file.path(), depth - 1, SPIFFS);
				level--;
			}
		}
		file = root.openNextFile();
	}
}
//---------------------------------------------------------------------
void listDirSPI(const char *dirname, uint8_t depth){
	listDir(SPIFFS, dirname, depth, true);
}
//---------------------------------------------------------------------
void listDirSDC(const char *dirname, uint8_t depth){
	listDir(SD_MMC, dirname, depth, false);
}
//---------------------------------------------------------------------
void readFile(fs::FS &fs, const char *path){
	Serial.printf("\nReading file:'%s'\n", path);

	File file = fs.open(path);
	if (!file) {
		Serial.println("Failed to open file for reading");
		return;
	}

	while (file.available()) {
		Serial.write(file.read());
	}
	file.close();
	Serial.print("\nEnd of file");
}
//---------------------------------------------------------------------
void createDir(fs::FS &fs, const char *path){
	Serial.printf("Creating Dir: %s\n", path);
	if (fs.mkdir(path)) {
		Serial.println("Dir created");
	} else {
		Serial.println("mkdir failed");
	}
}
//---------------------------------------------------------------------
void removeDir(fs::FS &fs, const char *path){
	Serial.printf("Removing Dir: %s\n", path);
	if (fs.rmdir(path)) {
		Serial.println("Dir removed");
	} else {
		Serial.println("rmdir failed");
	}
}
//---------------------------------------------------------------------
void writeFile(fs::FS &fs, const char *path, const char *message){
	Serial.printf("Writing file: %s\n", path);

	File file = fs.open(path, FILE_WRITE);
	if (!file) {
		Serial.println("Failed to open file for writing");
		return;
	}
	if (file.print(message)) {
		Serial.println("File written");
	} else {
		Serial.println("Write failed");
	}
	file.close();
}
//---------------------------------------------------------------------
void appendFile(fs::FS &fs, const char *path, const char *message){
	Serial.printf("Appending to file: %s\n", path);

	File file = fs.open(path, FILE_APPEND);
	if (!file) {
		Serial.println("Failed to open file for appending");
		return;
	}
	if (file.print(message)) {
		Serial.println("Message appended");
	} else {
		Serial.println("Append failed");
	}
	file.close();
}
//---------------------------------------------------------------------
void renameFile(fs::FS &fs, const char *path1, const char *path2){
	Serial.printf("Renaming file %s to %s\n", path1, path2);
	if (fs.rename(path1, path2)) {
		Serial.println("File renamed");
	} else {
		Serial.println("Rename failed");
	}
}
//---------------------------------------------------------------------
void deleteFile(fs::FS &fs, const char *path){
	Serial.printf("Deleting file: %s\n", path);
	if (fs.remove(path)) {
		Serial.println("File deleted");
	} else {
		Serial.println("Delete failed");
	}
}
//---------------------------------------------------------------------
void testFileIO(fs::FS &fs, const char *path){
	File file = fs.open(path);
	static uint8_t buf[512];
	size_t len = 0;
	uint32_t start = millis();
	uint32_t end = start;
	if (file) {
		len = file.size();
		size_t flen = len;
		start = millis();
		while (len) {
			size_t toRead = len;
			if (toRead > 512) {
				toRead = 512;
			}
			file.read(buf, toRead);
			len -= toRead;
		}
		end = millis() - start;
		Serial.printf("%u bytes read for %u ms\n", flen, end);
		file.close();
	} else {
		Serial.println("Failed to open file for reading");
	}

	file = fs.open(path, FILE_WRITE);
	if (!file) {
		Serial.println("Failed to open file for writing");
		return;
	}

	size_t i;
	start = millis();
	for (i = 0; i < 2048; i++) {
		file.write(buf, 512);
	}
	end = millis() - start;
	Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
	file.close();
}
///////////////////////////////////////////////////////////////////////
//SDMMCFS SD_MMC = SDMMCFS(FSImplPtr(new VFSImpl()));
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
/*
 typedef enum {
 CARD_NONE,
 CARD_MMC,
 CARD_SD,
 CARD_SDHC,
 CARD_UNKNOWN
 } sdcard_type_t;
 */
