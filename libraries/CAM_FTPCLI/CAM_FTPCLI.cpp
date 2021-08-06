/*
 * http://electroniqueamateur.blogspot.com/2020/05/enregistrement-sur-google-drive-des.html
 */

#include "CAM_FTPCLI.h"
#include "CAM_CAMERA.h"
#include "pix_t.h"
#include <ctime>
///////////////////////////////////////////////////////////////////////
ESP32_FTPClient *cliFTP;
///////////////////////////////////////////////////////////////////////
bool ftpConnect(){
	ESP32_FTPClient &ftp = *cliFTP;
	if (!ftp.isConnected(false)) {
		ftp.OpenConnection();
		delay(2000);
		if (!ftp.isConnected()) {
			Serial.printf("\nError connecting FTP");
			return false;
		}
	}
	Serial.printf("\nOK connecting FTP");
	return true;
}
//---------------------------------------------------------------------
String datimStr(){
	time_t rawtime = time(nullptr);
	struct tm *timeinfo;

//	time(&rawtime);
	timeinfo = localtime(&rawtime);

	char buffer[80];
	strftime(buffer, sizeof(buffer), "%Y%m%d-%H%M%S", timeinfo);
	//Serial.printf("\n{%s}", buffer);
	return String(buffer);
}
String randStr(){
	char buffer[9];
	sprintf(buffer, "%08X", esp_random()); // wifi must be on!!!
	return String(buffer);
}
//---------------------------------------------------------------------
bool SETUP_FTPCLI(const char *FTP_HOST, const uint16_t FTP_PORT,
	const char *FTP_LOGIN, const char *FTP_PSWD, const uint16_t TIMEOUT, const uint8_t VERBOSE){
	cliFTP = new ESP32_FTPClient(FTP_HOST, FTP_PORT, FTP_LOGIN, FTP_PSWD, TIMEOUT, VERBOSE);
	return true;
}
//---------------------------------------------------------------------
#include <vector>

bool ftpListdir(){
	Serial.printf("\n>>>>ftpListdir:\n");
	if (!ftpConnect())
		return false;
	ESP32_FTPClient &ftp = *cliFTP;
	if (!ftp.InitFile("A"))
		return false;
	if (!ftp.ChangeWorkDir("/"))
		return false;
	std::vector<String> list;
	if (!ftp.ContentList("", list))
		return false;
	for (int i = 0; i < list.size(); i++)
		Serial.printf("\n%3i %s", i, list[i].c_str());
	return true;
}
//---------------------------------------------------------------------
bool uploadBINFIL(fs::FS &fs, const char *localFullFilename, const char *remotePath,
	const char *remoteName){
	String remfil = randStr() + String("_") + String(remoteName);
	Serial.printf("\n>>>>upLoadBIN '%s' to '%s/%s':\n", localFullFilename, remotePath,
		remfil.c_str());
	if (!fs.exists(localFullFilename)) {
		Serial.printf("\n'%s' inexistant", localFullFilename);
		return false;
	}
	File src = fs.open(localFullFilename, "r");
	if (!ftpConnect())
		return false;
	ESP32_FTPClient &ftp = *cliFTP;
	if (!ftp.ChangeWorkDir(remotePath))
		return false;
	if (!ftp.InitFile("I"))
		return false;
	if (!ftp.NewFile(remfil.c_str()))
		return false;
	uint8_t buffer[2048];
	uint32_t remaining = src.size();
	Serial.printf("\nUploading FILE");
	while (remaining > 0) {
//		Serial.printf("\nRemaining '%10i'", remaining);
		Serial.printf(".");
		int32_t len2load;
		if (remaining <= sizeof(buffer))
			len2load = remaining;
		else
			len2load = sizeof(buffer);
		src.readBytes((char*)buffer, len2load);
		ftp.WriteData(buffer, len2load);
		remaining = remaining - len2load;
	}
//	Serial.printf("\nRemaining '%10i'", remaining);
	Serial.printf("ok.");
	ftp.CloseFile();
	return true;
}
//---------------------------------------------------------------------
bool upload_BIN_buffer(uint8_t *BUF, const size_t LEN, const char *remotePath,
	const char *remoteName){
	String remfil = randStr() + String("_") + String(remoteName);
	Serial.printf("\n>>>>upLoadBUF_JPG %p(%i) to '%s/%s':\n", BUF, LEN, remotePath, remfil.c_str());
	if (!ftpConnect())
		return false;
	ESP32_FTPClient &ftp = *cliFTP;
	if (!ftp.ChangeWorkDir(remotePath))
		return false;
	if (!ftp.InitFile("I"))
		return false;
	if (!ftp.NewFile(remfil.c_str()))
		return false;
	uint32_t remaining = LEN;
	Serial.printf("\nUploading JPEG");
	while (remaining > 0) {
//		Serial.printf("\nRemaining '%10i'", remaining);
		Serial.printf(".");
		int32_t len2load;
		if (remaining <= 2048) // or <LEN for one WriteData  only (maybe MTU problem or length of buffer some where in lwip or other...
			len2load = remaining;
		else
			len2load = 2048;
		ftp.WriteData(&BUF[LEN - remaining], len2load);
		remaining = remaining - len2load;
	}
//	Serial.printf("\nRemaining '%10i'", remaining);
	Serial.printf("ok.");
	ftp.CloseFile();
	return true;
}
//---------------------------------------------------------------------
bool upload_BMP_frame(const camera_fb_t *FRAME, const char *remotePath, const char *remoteName){
	/**
	 * \n Cf. https://web.maths.unsw.edu.au/~lafaye/CCM/video/format-bmp.htm
	 * \n Cf. https://web.archive.org/web/20080912171714/http://www.fortunecity.com/skyscraper/windows/364/bmpffrmt.html
	 */
#pragma pack(push, 1) // Else alignment char[2] is 4 bytes !!!
	typedef struct {
		char bfType[2] = { 'B', 'M' }; //   00*/
		uint32_t bfSize; //				+2 	02 3640 0000 -> 0000 4036=16438 (128*64*2=16384)+54 headers width 127=>+2bytes*/
		uint16_t bfReserved1 = 0; //	+4 	06 		0000 	must be 0*/
		uint16_t bfReserved2 = 0; //	+2 	08 		0000 	must be 0*/
		uint32_t bfOffBits; //			+2 	0A 3600 0000 -> 0000 0036=0x36=54d*/
	} BITMAPFILEHEADER;
	typedef struct {
		uint32_t biSize; //				+4 	0E 2800 0000 -> 0000 0028=0x28=40d BITMAPINFOHEADER bytes size
		uint32_t biWidth; //			+4 	12 7F00 0000 -> 0000 007F=127 (2 bytes will be added to divide by 4... ie file size*/
		uint32_t biHeight; //			+4 	16 4000 0000 -> 0000 0040=64d*/
		uint16_t biPlanes = 1; //		+4 	1A 		0100 -> 0001=1 plane*/
		uint16_t biBitCount; //			+4	1C 		1000 -> 0010=16d (16 bpP)*/
		uint32_t biCompression = 0; //	+4 	1E 0000 0000 	must be 0*/
		uint32_t biSizeImage = 0; //	+4 	22 0040 0000 -> 0000 4000=16384=128*64 image data bytes size. If no compression can be zero.
		uint32_t biXPelsPerMeter; //	+4 	26 130B 0000 -> 0000 0B13=2835 P/m =72,009 P/inch */
		uint32_t biYPelsPerMeter; //	+4 	2A 130B 0000 -> 0000 0B13=2835 P/m =72,009 P/inch */
		uint32_t biClrUsed = 0; //		+4 	2E 0000 0000 	must be 0*/
		uint32_t biClrImportant = 0; //	+4 	32 0000 0000 	must be 0*/
		//								+4 	36 sizeof(t_headerBmp)->54d*/
		// Here a palette could be inserted for 8 bits images...
		//
	} BITMAPINFOHEADER;
	typedef struct {
		BITMAPFILEHEADER file;
		BITMAPINFOHEADER data;
	} t_headerBmp;
	uint32_t RGBQUAD[256];
#pragma pack(pop)
	uint8_t cntBPP = channels(FRAME);
	t_headerBmp header;
	header.file.bfSize = FRAME->len + sizeof(header);
	header.file.bfOffBits = sizeof(header);
	if (cntBPP == 1)
		header.file.bfOffBits += 1024;
	header.data.biSize = sizeof(BITMAPINFOHEADER);
	header.data.biWidth = FRAME->width;
	header.data.biHeight = FRAME->height;
	header.data.biBitCount = cntBPP * 8;
	header.data.biXPelsPerMeter = FRAME->width / 0.16; //0.16m=21 -5 cm margins...
	header.data.biYPelsPerMeter = FRAME->height / 0.16;
	String remfil = randStr() + String("_") + String(remoteName);
	Serial.printf("\n>>>>upload_BMP_frame %p(%i) to '%s/%s':\n", FRAME->buf, FRAME->len, remotePath,
		remfil.c_str());
	if (!ftpConnect())
		return false;
	ESP32_FTPClient &ftp = *cliFTP;
	if (!ftp.ChangeWorkDir(remotePath))
		return false;
	if (!ftp.InitFile("I"))
		return false;
	if (!ftp.NewFile(remfil.c_str()))
		return false;
	ftp.WriteData((uint8_t*)&header, sizeof(header));// headers
	if (cntBPP == 1) {
		for (size_t i = 0; i < 256; i++) {
			RGBQUAD[i] = ((uint32_t)i << 16) + ((uint16_t)i << 8) + i; // Serial.printf("\n%3i %08X",i,RGBQUAD[i]);
		}
		ftp.WriteData((uint8_t*)&RGBQUAD[0], 1024);// palette
	}
	Serial.printf("\nUploading BMP...");
/*
	int written = ftp.WriteData(FRAME->buf, FRAME->len);// data but vertical flip !!!
	if (FRAME->len != written) {
		Serial.printf("ERROR %i written for %i expected!", written, FRAME->len);
		return false;
	}
*/
	uint32_t remaining = FRAME->len;
	uint16_t lineLength = FRAME->width * cntBPP;
	uint32_t start = FRAME->len - lineLength;
	uint16_t cnt = 0;
	Serial.printf("\nUploading BMP");
	while (remaining > 0) {
		if (cnt % 10 == 0)
			Serial.printf(".");
			ftp.WriteData(&FRAME->buf[start], lineLength);
		remaining -= lineLength;
		start -= lineLength;
		cnt++;
	}


	Serial.printf("ok.");
	return ftp.CloseFile();
}
//---------------------------------------------------------------------
bool upload_BMP_frameOLD(const camera_fb_t *FRAME, const char *remotePath,
	const char *remoteName){
	/**
	 * \n Cf. https://web.maths.unsw.edu.au/~lafaye/CCM/video/format-bmp.htm
	 * \n Cf. https://web.archive.org/web/20080912171714/http://www.fortunecity.com/skyscraper/windows/364/bmpffrmt.html
	 */
#pragma pack(push, 1) // Else alignment char[2] is 4 bytes !!!
	typedef struct {
		char bfType[2] = { 'B', 'M' }; //   00*/
		uint32_t bfSize; //				+2 	02 3640 0000 -> 0000 4036=16438 (128*64*2=16384)+54 headers width 127=>+2bytes*/
		uint16_t bfReserved1 = 0; //	+4 	06 		0000 	must be 0*/
		uint16_t bfReserved2 = 0; //	+2 	08 		0000 	must be 0*/
		uint32_t bfOffBits; //			+2 	0A 3600 0000 -> 0000 0036=0x36=54d*/
	} BITMAPFILEHEADER;
	typedef struct {
		uint32_t biSize; //				+4 	0E 2800 0000 -> 0000 0028=0x28=40d BITMAPINFOHEADER bytes size
		uint32_t biWidth; //			+4 	12 7F00 0000 -> 0000 007F=127 (2 bytes will be added to divide by 4... ie file size*/
		uint32_t biHeight; //			+4 	16 4000 0000 -> 0000 0040=64d*/
		uint16_t biPlanes = 1; //		+4 	1A 		0100 -> 0001=1 plane*/
		uint16_t biBitCount; //			+4	1C 		1000 -> 0010=16d (16 bpP)*/
		uint32_t biCompression = 0; //	+4 	1E 0000 0000 	must be 0*/
		uint32_t biSizeImage = 0; //	+4 	22 0040 0000 -> 0000 4000=16384=128*64 image data bytes size. If no compression can be zero.
		uint32_t biXPelsPerMeter; //	+4 	26 130B 0000 -> 0000 0B13=2835 P/m =72,009 P/inch */
		uint32_t biYPelsPerMeter; //	+4 	2A 130B 0000 -> 0000 0B13=2835 P/m =72,009 P/inch */
		uint32_t biClrUsed = 0; //		+4 	2E 0000 0000 	must be 0*/
		uint32_t biClrImportant = 0; //	+4 	32 0000 0000 	must be 0*/
		//								+4 	36 sizeof(t_headerBmp)->54d*/
		// Here a palette could be inserted for 8 bits images...
		//
	} BITMAPINFOHEADER;
	typedef struct {
		BITMAPFILEHEADER file;
		BITMAPINFOHEADER data;
	} t_headerBmp;
	uint32_t RGBQUAD[256];
#pragma pack(pop)
	uint8_t cntBPP = channels(FRAME);
	t_headerBmp header;
	header.file.bfSize = FRAME->len + sizeof(header);
	header.file.bfOffBits = sizeof(header);
	if (cntBPP == 1)
		header.file.bfOffBits += 1024;
	header.data.biSize = sizeof(BITMAPINFOHEADER);
	header.data.biWidth = FRAME->width;
	header.data.biHeight = FRAME->height;
	header.data.biBitCount = cntBPP * 8;
	header.data.biXPelsPerMeter = FRAME->width / 0.16; //0.16m=21 -5 cm margins...
	header.data.biYPelsPerMeter = FRAME->height / 0.16;
	String remfil = randStr() + String("_") + String(remoteName);
	Serial.printf("\n>>>>upload_BMP_frame %p(%i) to '%s/%s':\n", FRAME->buf, FRAME->len,
		remotePath,
		remfil.c_str());
	if (!ftpConnect())
		return false;
	ESP32_FTPClient &ftp = *cliFTP;
	if (!ftp.ChangeWorkDir(remotePath))
		return false;
	if (!ftp.InitFile("I"))
		return false;
	if (!ftp.NewFile(remfil.c_str()))
		return false;
	ftp.WriteData((uint8_t*)&header, sizeof(header));
	RGB555_t *BIN16;
	if (cntBPP == 1) {
		for (size_t i = 0; i < 256; i++) {
			RGBQUAD[i] = ((uint32_t)i << 16) + ((uint16_t)i << 8) + i;
//			Serial.printf("\n%3i %08X",i,RGBQUAD[i]);
		}
		ftp.WriteData((uint8_t*)&RGBQUAD[0], 1024);
	} else if (cntBPP == 2)
		BIN16 = new RGB555_t[FRAME->width];
	Serial.printf("ok.");
//	Serial.printf("\nRemaining '%10i'", remaining);
	if (cntBPP == 2)
		delete[] BIN16;
	return ftp.CloseFile();
}
///////////////////////////////////////////////////////////////////////
