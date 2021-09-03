#include "CAM_FILSYS.h"
#include "esp32_GOODIES.h"
#include <vector>
#include <string>
#include<unistd.h> // unlink
bool ffsON = false;
bool ssdON = false;
///////////////////////////////////////////////////////////////////////
void testSplit(){
	uint16_t nth;
	const char *CHN;
	const char *patterns;
	std::string S1, S2;

	CHN = "/111/222/333/444";
	patterns = "/\\";
	nth = 3;
	if (split_Up_Before(nth, CHN, S1, S2, patterns))
		_DEBUG_Others_(3, "\nsplit_Up_Before(%i,%s,S1,S2,%s)=>S1='%s' S2='%s'.",
			nth, CHN, patterns, S1.c_str(), S2.c_str());
	else
		_DEBUG_Others_(3, "\nsplit_Up_Before(%i,%s,S1,S2,%s)=>ERROR.", nth, CHN, patterns);

	patterns = "2";
	nth = 2;
	if (split_Up_Before(nth, CHN, S1, S2, patterns))
		_DEBUG_Others_(3, "\nsplit_Up_Before(%i,%s,S1,S2,%s)=>S1='%s' S2='%s'.",
			nth, CHN, patterns, S1.c_str(), S2.c_str());
	else
		_DEBUG_Others_(3, "\nsplit_Up_Before(%i,%s,S1,S2,%s)=>ERROR.", nth, CHN, patterns);

	nth = 4;
	if (split_Up_Before(nth, CHN, S1, S2, patterns))
		_DEBUG_Others_(3, "\nsplit_Up_Before(%i,%s,S1,S2,%s)=>S1='%s' S2='%s'.",
			nth, CHN, patterns, S1.c_str(), S2.c_str());
	else
		_DEBUG_Others_(3, "\nsplit_Up_Before(%i,%s,S1,S2,%s)=>ERROR.", nth, CHN, patterns);

}
//---------------------------------------------------------------------
bool SETUP_FILSYS(){
	testSplit();

	char *bufDIR = new char[10000]; // On the heap !!!
	char *buffer;
	if (!SPIFFS.begin(false, "/FFS")) {
		Serial.print("\nNo SPIFFS !!!");
		delete[] bufDIR; // don't forget to free after finished using it
		return false;
	}
	ffsON = true;
	Serial.printf("\nSPIFFS started");
	Serial.printf("\n==========================");
	buffer = bufDIR; // !!! Before listDir
	listDir("TEXT", &buffer, "/FFS");
	Serial.printf("\n==========================\n%s", bufDIR);
	readFile(SPIFFS, "/ca_cert.pem");
//	return true;

	if (!SD_MMC.begin("/SSD", false)) {
		//if (!SD_MMC.begin("/sdcard", false, true)) { // FORMAT ????
		Serial.print("\nNo SD_MMC !!!");
		delete[] bufDIR; // don't forget to free after finished using it
		return false;
	}
	ssdON = true;
	Serial.printf("\nSDCARD started");
	Serial.printf("\n\tcardType   %i", SD_MMC.cardType());
	Serial.printf("\n\tcardSize   %10.3f Mo", SD_MMC.cardSize() / 1000000.0);
	Serial.printf("\n\ttotalBytes %10.3f Mo", SD_MMC.totalBytes() / 1000000.0);
	Serial.printf("\n\tusedBytes  %10.3f Mo", SD_MMC.usedBytes() / 1000000.0);
	buffer = bufDIR; // !!! Before listDir
	listDir("TEXT", &buffer, "/SSD");
	Serial.printf("\n==========================\n%s", bufDIR);
	readFile(SD_MMC, "/ca_cert.pem");
	delete[] bufDIR; // don't forget to free after finished using it
	return true;
}
//---------------------------------------------------------------------
bool splitAfterLast(const std::string str, std::string &first, std::string &second,
	const char *charPatterns){
	std::size_t found = str.find_last_of(charPatterns);
	if (found == std::string::npos)
		return false;
	first = str.substr(0, found + 1);
	second = str.substr(found + 1);
	return true;
}
//---------------------------------------------------------------------
size_t split_Up(const uint16_t nth, const std::string &str, const char *charPatterns){
	size_t found;
	size_t previous = 0;
	uint16_t i = 0;
	do {
		found = str.find_first_of(charPatterns, previous);
		if (found == std::string::npos)
			break;
		previous = 1 + found;
		i++;
	} while (i < nth);
	return found;
}
//---------------------------------------------------------------------
bool split_Up_Before(const uint16_t nth, const char *chn,
	std::string &first, std::string &second, const char *charPatterns){
	std::string str = std::string(chn);
	size_t found = split_Up(nth, str, charPatterns);
	if (found == std::string::npos)
		return false;
	first = str.substr(0, found);
	second = str.substr(found);
	return true;
}
//---------------------------------------------------------------------
bool splitAfterFirst(const std::string str, std::string &first, std::string &second,
	const char *charPatterns){
	std::size_t found = str.find_first_of(charPatterns);
	if (found == std::string::npos)
		return false;
	first = str.substr(0, found + 1);
	second = str.substr(found + 1);
	return true;
}
//---------------------------------------------------------------------
bool splitBeforeLast(const std::string str, std::string &first, std::string &second,
	const char *charPatterns){
	std::size_t found = str.find_last_of(charPatterns);
	if (found == std::string::npos)
		return false;
	first = str.substr(0, found);
	second = str.substr(found);
	return true;
}
//---------------------------------------------------------------------
#define DEF_VOLEN 4
fs::FS* volFS(const char *volpath){
	String volName = String(volpath).substring(0, DEF_VOLEN);
	_DEBUG_Others_(3, "\nvstr='%s'", volName.c_str());
	if (String("/FFS") == volName)
		return &SPIFFS;
	if (String("/SSD") == volName)
		return &SD_MMC;
	return nullptr;
}
//---------------------------------------------------------------------
typedef struct {
	std::string _path;
	std::string _name;
	uint32_t _size;
	int _level;
	std::string _ext;
} pa2img_t;
void dirTEXT(const char *format, char **buffer, const char *header,
	const char *volName, const char *dirname, std::vector<pa2img_t> &infos){
	//Serial.printf("\n*****dirTEXT buffer %p *buffer %p ", buffer, *buffer);
	//Serial.printf("\n*****dirTEXT buffer %p *buffer %p '%s'", buffer, *buffer, *buffer);
	std::sort(infos.begin(), infos.end(),
		[](pa2img_t const &a, pa2img_t const &b) -> bool{ // Needs first printable char separator (space is excluded from names!)
			String A = String(a._path.c_str()) + " " + String(a._name.c_str());
			String B = String(b._path.c_str()) + " " + String(b._name.c_str());
			return (A < B);
		});
	std::string prevPath = "";
	if (0 == strcmp("HTML", format)) { // Don't underline the a links
		*buffer += sprintf(*buffer, "<style type='text/css'>a {text-decoration: none;}</style>");
		*buffer += sprintf(*buffer, "%s</br>\\n", header);
		*buffer +=
			sprintf(*buffer,
				"<table cellspacing='0'><tr bgcolor='#cccccc'><th>N°</th><th>Lvl</th><th>Size</th><th>Folder -> file</th><th></th></tr>\\n");
		const char *color[2] = { "#ccffcc", "#ccccff" };
		for (size_t i = 0; i < infos.size(); i++) {
			if (prevPath != infos[i]._path) {
				*buffer +=
					sprintf(*buffer,
						"<tr bgcolor='#ffcccc'>\\n<td align ='right'>--</td><td align ='right'>%i</td><td align ='right'>dir:</td>\\n",
						infos[i]._level);
				*buffer +=
					sprintf(*buffer,
						"<td><a title='list dir' href='#' onClick=\\\"OTA_dirList('%s','%s%s');\\\"><b>%s</b></a></td>\\n",
						volName, volName, infos[i]._path.c_str(), infos[i]._path.c_str());
				*buffer += sprintf(*buffer, "<td></td>\\n</tr>\\n");
				prevPath = infos[i]._path;
			}
			*buffer +=
				sprintf(*buffer,
					"<tr bgcolor='%s'>\\n<td align ='right'>%i</td><td align ='right'>%i</td><td align ='right'>%i</td>\\n",
					color[i % 2], i, infos[i]._level, infos[i]._size);
			std::string space = "";
			for (size_t j = 0; j < infos[i]._level + 1; j++)
				space += "&nbsp;&nbsp;&nbsp;";
			if (((i + 1) == infos.size()) || (infos[i]._path != infos[i + 1]._path))
				space += "&#9492;"; //└
			else
				space += "&#9500;"; //├
			*buffer +=
				sprintf(*buffer,
					"<td>%s<a title='download' href='#' onClick=\\\"OTA_download('%s%s%s');\\\">%s</a></td>\\n",
					space.c_str(), volName, infos[i]._path.c_str(), infos[i]._name.c_str(),
					infos[i]._name.c_str());
			*buffer +=
				sprintf(*buffer,
					"<td><a title='delete' href='#' onClick=\\\"OTA_delete('%s%s%s');\\\">&#10060;</a></td>\\n</tr>\\n",
					volName, infos[i]._path.c_str(), infos[i]._name.c_str());
		}
		*buffer += sprintf(*buffer, "</table>\\n");
	} else if (0 == strcmp("TEXT", format)) {
		*buffer += sprintf(*buffer, "\n%s", header);
		*buffer += sprintf(*buffer, "\n N° Lvl    Size Folder               File name");
		for (size_t i = 0; i < infos.size(); i++) {
			if (prevPath != infos[i]._path) {
				*buffer += sprintf(*buffer, "\n %-3s%3i %-10s %-*s",
					"--", infos[i]._level, "dir:", 20, infos[i]._path.c_str());
				prevPath = infos[i]._path;
			}
			std::string space = " ";// Doesn't run with space = "" empty
			if (((i + 1) == infos.size()) || (infos[i]._path != infos[i + 1]._path))
				space += "└"; //└
			else
				space += "├"; //├
			*buffer += sprintf(*buffer, "\n%3i %3i %10i %*s%-*s",
				i, infos[i]._level, infos[i]._size,
				3 * (infos[i]._level + 1), space.c_str(), 20, infos[i]._name.c_str());
		}
	} else {
		*buffer += sprintf(*buffer, "%s, format '%s' unknown!", header, format);
	}
}
//---------------------------------------------------------------------
void dirVCT(fs::FS *fs, const char *dirname, char *header,
	std::vector<pa2img_t> &infos, const uint8_t depth){
	if (!depth)
		return;
	File root = fs->open(dirname);
	if (!root) {
		sprintf(header, "Failed to open directory '%s'\\n", dirname);
		return;
	}
	File file = root.openNextFile();
	while (file) {
		_DEBUG_Others_(9, "\n file '%s' '%s'", file.path(), file.name());
		if (file.isDirectory()) {
			if (depth) {
				dirVCT(fs, file.path(), header, infos, depth - 1);
			}
		} else {
//			SPrintF(sizeof(buffer), cnt, buffer, "%3i %7i\t%s\n", cnt_fil, file.size(),file.name());
			pa2img_t record;
			splitAfterLast(file.path(), record._path, record._name, "/\\");
			// string s=file.name();
			record._level = count(record._path.begin(), record._path.end(), '/') - 1;
			record._size = file.size();
			infos.push_back(record);
			_DEBUG_Others_(9, "\n%03i) level %-3i, path '%s', name '%s', size '%-6i'",
				infos.size(),
				record._level, record._path.c_str(), record._name.c_str(), record._size);
		}
		file = root.openNextFile();
	}
}
//---------------------------------------------------------------------
void listDir(const char *format, char **buffer, const char *volName,
	const char *dirname, const uint8_t depth){
	//Serial.printf("\n*****>listDir buffer %p *buffer %p '%s'", buffer, *buffer, *buffer);
	char header[128];
	header[0] = 0;
	fs::FS *fs = volFS(volName);
	if (!fs) {
		sprintf(header, "Volume '%s' is not mounted", volName);
		return;
	}
	uint64_t totalBytes;
	uint64_t usedBytes;
	volSizes(volName, totalBytes, usedBytes);
	sprintf(header, "Directory '%s%s' (%7llu Bytes free)",
		volName, dirname, totalBytes - usedBytes);
	std::vector<pa2img_t> *infos = new std::vector<pa2img_t>;
	//Serial.printf("\n*****<listDir buffer %p *buffer %p '%s'", buffer, *buffer, *buffer);
	std::string dirNAME = std::string(dirname);
	if (dirNAME.length() > 1)
		dirNAME = dirNAME.substr(0, dirNAME.length() - 1);
	dirVCT(fs, dirNAME.c_str(), header + strlen(header), *infos, depth);
	Serial.printf("\n==== cnt_fil %3i, depth %3i, path '%s'",
		infos->size(), depth, dirname);
	dirTEXT(format, buffer, header, volName, dirname, *infos);
	infos->clear();
}
//---------------------------------------------------------------------
int64_t volFreeSize(const char *volPathFilename){
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
void volSizes(const char *volName, uint64_t &totalBytes, uint64_t &usedBytes){
	if (0 == strcmp("/FFS", volName)) {
		totalBytes = SPIFFS.totalBytes();
		usedBytes = SPIFFS.usedBytes();
	}
	else if (0 == strcmp("/SSD", volName)) {
		totalBytes = SD_MMC.totalBytes();
		usedBytes = SD_MMC.usedBytes();
	}
	else {
		totalBytes = 0;
		usedBytes = 0;
	}
}
//---------------------------------------------------------------------
bool deleteFile(const char *volpath){
	fs::FS *fs = volFS(volpath);
	Serial.printf("\nTry deleting %s on fs %p", volpath, fs);
	if (!fs)
		return false;
	return fs->remove(volpath + DEF_VOLEN);
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
