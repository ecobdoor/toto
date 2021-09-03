// Client side implementation of UDP client-CLIENT model
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <chrono>
#include <termios.h>
#include <sys/ioctl.h> // For FIONREAD
#include <iomanip>      // std::setw
#include <fstream>
#include <chrono>
#include "/media/sf_media-host/DEV/ROB_000/setup/SET_000_WS3/rov_INC.c"
#include "UDP_log/udpClient.h"
#include "UDP_log/udp.h"
#include <stdarg.h>
using namespace std;
/**
 * A LIRE ABSOLUMENT:::::::::::::::::
 * https://github.com/eminfedar/async-sockets-cpp
 */
///////////////////////////////////////////////////////////////////////
FILE *pFile;
//bool csv = false;
void process_csv(const string &chn){
	string kmd = chn.substr(0, 7);
	cout << kmd.c_str() << flush;
	if (kmd == "@BEGIN:") {
		pFile = fopen("../../../GIT_Telemetry/telemetry.csv", "w");
//		csv = true;
	} else if (kmd == "@APPND:") {
		pFile = fopen("../../../GIT_Telemetry/telemetry.csv", "a");
//		csv = true;
	} else if (pFile) {
		if (kmd == "@CLOSE:")
//		csv = false;
			fclose(pFile);
		else
			fprintf(pFile, 7 + chn.c_str());
	}
}
//---------------------------------------------------------------------
void process_log(const string &chn){
	static FILE *pFile = nullptr;
	if (!pFile)
		pFile = fopen("../../../GIT_Telemetry/log.log", "w");
	fprintf(pFile, chn.c_str());
	fflush(pFile);
}
//---------------------------------------------------------------------
bool setSTDIN(){
	static struct termios old = { 0 };
	if (tcgetattr(0, &old) < 0)
		return false;
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old) < 0)
		return false;
	return true;
}
//---------------------------------------------------------------------
char getch(){
	char c = 0;
	int nbbytes;
	ioctl(0, FIONREAD, &nbbytes); // 0 is STDIN
	if (nbbytes) {
		if (read(0, &c, 1) < 0)
			perror("read()");
	}
	return c;
}
//---------------------------------------------------------------------
bool getstr(string &chn){
	char c = getch();
	if (c != 0) {
		if (c == '\n') {
			return chn.length() != 0;;
		} else {
			cout << "(" << c << ")" << flush;
			chn += c;
		}
	}
	return false;
}
//---------------------------------------------------------------------

int this_DebugLVL = 9;
bool checkError = false;
socketUDPclient *CLIENT = nullptr;
uint32_t cntCnx = 0;
uint32_t repSrv = 0;
uint32_t kmdCli = 0;
///////////////////////////////////////////////////////////////////////
const char *_foreBlack = "\033[30m";
const char *_foreRed = "\033[31m";
const char *_foreGreen = "\033[32m";
const char *_foreBrown = "\033[33m";
const char *_foreBlue = "\033[34m";
const char *_foreMagenta = "\033[35m";
const char *_foreCyan = "\033[36m";
const char *_foreWhite = "\033[37m";
const char *_foreDefault = "\033[39m";

const char *_foreHBlack = "\033[1;30m";
const char *_foreHRed = "\033[1;31m";
const char *_foreHGreen = "\033[1;32m";
const char *_foreHBrown = "\033[1;33m";
const char *_foreHBlue = "\033[1;34m";
const char *_foreHMagenta = "\033[1;35m";
const char *_foreHCyan = "\033[1;36m";
const char *_foreHWhite = "\033[1;37m";
const char *_foreHDefault = "\033[1;39m";

const char *_backBlack = "\033[40m";
const char *_backRed = "\033[41m";
const char *_backGreen = "\033[42m";
const char *_backBrown = "\033[43m";
const char *_backBlue = "\033[44m";
const char *_backMagenta = "\033[45m";
const char *_backCyan = "\033[46m";
const char *_backWhite = "\033[47m";
const char *_backDefault = "\033[49m";

const char *_backHBlack = "\033[1;40m";
const char *_backHRed = "\033[1;41m";
const char *_backHGreen = "\033[1;42m";
const char *_backHBrown = "\033[1;43m";
const char *_backHBlue = "\033[1;44m";
const char *_backHMagenta = "\033[1;45m";
const char *_backHCyan = "\033[1;46m";
const char *_backHWhite = "\033[1;47m";
const char *_backHDefault = "\033[1;49m";

const char *_line = "\033[K";
const char *_reset = "\033[0m";
const char *_bold = "\033[1m";
const char *_underline = "\033[4m";
const char *_underlineDouble = "\033[21m";
const char *_reverse = "\033[7m";

///////////////////////////////////////////////////////////////////////
/*
 OK connect to(192.168.4.1:2550)  CNX:144   CLOSE SOCKET 9999  RCV:0123456789 SND:01234 'm2dk'    ACK rcv/snd OK/OK   99999/99999
 OK connect to(192.168.252.222:32000)
 */
const size_t STA_SOK_connect = 0;
const size_t STA_CNX_count = 35;
const size_t STA_SOK_status = 45;
const size_t STA_SOK_rcv_snd = 64;
const size_t STA_SOK_sent = 79;
const size_t STA_SOK_kmd = 89;
const size_t STA_HC_status = 99;
const size_t STA_HC_dt = 119;
//---------------------------------------------------------------------
typedef struct {
	size_t uplrow;
	size_t uplcol;
	size_t heigh;
	size_t width;
	string pattern;
	string title;
	std::chrono::_V2::system_clock::time_point oldTMS;
} s_frame;
s_frame STA = { 1, 1, 2, 25, "", "STA" };
s_frame MOT = { 2, 1, 8, 37, "SETPTS", "MOTORS" };
s_frame PID = { 2, 40, 8, 125, "S--error--D", "PID ring queue" };
s_frame LOG = { 11, 02, 50, 25, "", "LOG" }; // scroll log area
s_frame ATT = { 11, 82, 13, 80, "ATTIT", "DRIVER" };
s_frame IMU = { 25, 82, 13, 80, "QUATER", "XIMU" };
//---------------------------------------------------------------------
inline string gotoRC(const s_frame &CFG, const size_t ROW = 0, const size_t COL = 0){
	stringstream ssValue;
	ssValue << "\033[" << CFG.uplrow + ROW << ";" << CFG.uplcol + COL << "H";
	return ssValue.str();
}
//---------------------------------------------------------------------
inline string shiftC(const size_t COL){
	stringstream ssValue;
	ssValue << "\033[" << COL << "C";
	return ssValue.str();
}
//---------------------------------------------------------------------
void updateStatus(const size_t COL, const char *FORMAT, ...){
	cout << gotoRC(STA, 0, COL) << "| " << flush;
	int16_t cnt = 0;
	char buffer[128];
	va_list args;
	va_start(args, FORMAT);
	vprintf(FORMAT, args);
	va_end(args);

}
//---------------------------------------------------------------------
inline string dTMS(s_frame &CFG){
	stringstream ssValue;
	std::chrono::_V2::system_clock::time_point now = std::chrono::high_resolution_clock::now();
	char buffer[50];
	long long duration =
		std::chrono::duration_cast<std::chrono::microseconds>(now - CFG.oldTMS).count();
	sprintf(buffer, "%10.3f ms)", duration / 1000.0);
	CFG.oldTMS = now;
	ssValue << gotoRC(CFG, 0, CFG.title.length() + 5) << "(" << buffer;
	return ssValue.str();
}
//---------------------------------------------------------------------
string
format(string &chn, const size_t COL){
	string format = "";
	for (size_t i = 0; i < chn.length(); i++) {
		format += chn[i];
		if ((chn[i] == '\n') && (COL)) {
			format += shiftC(COL);
		}
	}
	return format;
}
//---------------------------------------------------------------------
void drawFrame(s_frame &CFG){
	if (CFG.pattern.length()) {
		cout << gotoRC(CFG) << "╔══ " << CFG.title << " ";
		for (size_t i = 0; i < CFG.width - CFG.title.length() - 4; i++)
			cout << "═";
		cout << "╗";
		cout << gotoRC(CFG, CFG.heigh) << "╚";
		for (size_t i = 0; i < CFG.width; i++)
			cout << "═";
		cout << "╝";
		for (size_t i = 1; i < CFG.heigh; i++) {
			cout << gotoRC(CFG, i) << "║";
			cout << shiftC(CFG.width) << "║";
		}
	} else {
		cout << "\033[" << CFG.uplrow + 1 << ";" << CFG.uplrow + CFG.heigh << "r" << flush; //scroll area
		cout << _line << gotoRC(CFG) << _underlineDouble << _bold << _backGreen << _line
			<< CFG.title << _reset;
		cout << "\033[s"; //store
	}
	CFG.oldTMS = std::chrono::high_resolution_clock::now();
}
//---------------------------------------------------------------------
void initFrames(){
	drawFrame(PID);
	drawFrame(LOG);
	drawFrame(MOT);
	drawFrame(IMU);
	drawFrame(ATT);
}
//#define DEF_DBGHANSHAKE
//---------------------------------------------------------------------
bool handShake(string &RCV){
	static auto _last_clientReady = chrono::steady_clock::now();
	static auto _last_serverReady = chrono::steady_clock::now();
	auto now = chrono::steady_clock::now();
	uint32_t dtClient =
		chrono::duration_cast<chrono::milliseconds>(now - _last_clientReady).count();
	uint32_t dtServer =
		chrono::duration_cast<chrono::milliseconds>(now - _last_serverReady).count();
	updateStatus(STA_HC_dt, "ACK dt(%6i/%6i)ms", dtClient, dtServer);
//
// Check for OUT watchdog timeout
//
	if (1000 < dtClient) {
		_last_clientReady = now;
#ifdef DEF_DBGHANSHAKE
		cout << "\nC/S(" << setw(8) << dtClient / 1000.0 << "/" << setw(8) << dtServer / 1000.0
		<< ")s";
		cout << "-->>'" << DEF_UDP_CLIENT_HANDSHAKE << "'[" << cntCnx << ":" << kmdCli << "/"
		<< repSrv << "]" << flush;
#endif
		if (!CLIENT->writeStr(DEF_UDP_CLIENT_HANDSHAKE, checkError)) {
			updateStatus(STA_HC_status, "CLI NOT SENT");
			return false;
		}
	}
//
// Check for IN watchdog timeout
//
	RCV = CLIENT->readStr();
#ifdef DEF_DBGHANSHAKE
	cout << "\nC/S(" << setw(8) << dtClient / 1000.0 << "/" << setw(8) << dtServer / 1000.0 << ")s";
#endif
	if (0 == RCV.compare(DEF_UDP_SERVER_HANDSHAKE)) {
#ifdef DEF_DBGHANSHAKE
		cout << "<<--'" << RCV << "'[" << cntCnx << ":" << kmdCli << "/" << repSrv << "]"
		<< flush;
#endif
		_last_serverReady = chrono::steady_clock::now();
		if (dtServer < 3000) {
			updateStatus(STA_HC_status, "SRV OK      ");
#ifdef DEF_DBGHANSHAKE
			cout << "\n-------------------------------------" << flush;
#endif
			RCV = "";
			return true;
		} else { // time out
			updateStatus(STA_HC_status, "SRV TOO LATE");

#ifdef DEF_DBGHANSHAKE
			cout << "\n----HANDSHAKE RECV TOO LATE... " << flush;
#endif
			return false;
		}
	} else {
		if (3000 < dtServer) {
			_last_serverReady = chrono::steady_clock::now();
			updateStatus(STA_HC_status, "SRV TIMEOUT ");
#ifdef DEF_DBGHANSHAKE
			//RCV[0]='~';// to
			cout << "<-'" << RCV << "'[" << cntCnx << ":" << kmdCli << "/" << repSrv << "]"
			<< flush;
			cout << "\n----HANDSHAKE RECV TIMEOUT !!! " << flush;
#endif
			return false;
		}
	}
	static uint8_t zzz = 0;
	const char run[4] = { '|', '/', '-', '\\' };
	updateStatus(STA_HC_status, "%c           ", run[zzz]);
	zzz = (zzz + 1) % 4;
	return true; // single reception no _last_clientReady=now !!!!!
}
///////////////////////////////////////////////////////////////////////
//#define DEF_TSTKB
int main(int argc, char *argv[]){
	if (!setSTDIN())
		return 250;
	string kmd;
	ofstream myfile;
	myfile.open("example.txt");
	bool log = false;

#ifndef DEF_TSTKB
	string decorate = "";
	uint16_t lenMSG = 0; // Lenght of reassembled packets of message
	initFrames();
	do {
		cout << "\033[1;1H"; //row, colunm
		CLIENT = new socketUDPclient("CLIENT", "192.168.4.1", DEF_UDP_PORT, 900, this_DebugLVL);
		updateStatus(STA_CNX_count, "CNX:%-4i", cntCnx);
		updateStatus(STA_SOK_rcv_snd, "rcv/snd(%10i/%-4i)", repSrv, kmdCli);
		sleep(1);
		string recv;
		int cntFrame = 0;
		s_frame *cfg;
		while (handShake(recv)) {
			if (recv.length() > 0) {
				cntFrame++;
				if (cntFrame == 1) {
					log = false;
					decorate = "";
					updateStatus(STA_SOK_rcv_snd, "rcv/snd:%10i/%4i", repSrv, kmdCli);
					if (string::npos != recv.find(PID.pattern)) {
						cfg = &PID;
					} else if (string::npos != recv.find(MOT.pattern)) {
						cfg = &MOT;
					} else if (string::npos != recv.find(IMU.pattern)) {
						cfg = &IMU;
					} else if (string::npos != recv.find(ATT.pattern)) {
						cfg = &ATT;
					} else {
						log = true;
						cfg = &LOG; //LOGcol;
						decorate = string(_foreBlack) + string(_backBrown) + string(_line);
						if (recv[0] == '@')
							process_csv(recv);
					}
					cout << dTMS(*cfg) << gotoRC(*cfg); // update window timing (first packet
				}
				if (recv[recv.length() - 1] == '\x1e') // for MTU reassembled packets string
					recv.pop_back(); //suppress last char//recv[recv.length() - 1] = '|'; //
				else
					cntFrame = 0; // END of all packets of string !!!!!!!!!!!!!!!!!!!
				if (log) {
					process_log(recv);
					cout << "\033[u" << flush; // unstore cursor position
				}
				//cout << decorate.c_str() << recv << "(" << cntFrame << ":" << log << ")" << flush;
				cout << format(recv, cfg->uplcol) << _reset << flush;
				if (log)
					cout << "\033[s" << flush; // store cursor position

#ifdef DEF_DBGHANSHAKE
				cout << "\"" << recv << "\"[" << cntCnx << ":" << kmdCli << "/" << repSrv << "]" << flush;
#else
#endif

				repSrv++;
			}
			if ((cntFrame == 0) && (getstr(kmd))) { // end of complete udp paragraph && after !!getstr!!
				if (kmd == " ") { //clear console
					cout << "\033[H\033[2J" << flush;
					initFrames();
				} else {
					if (!CLIENT->writeStr(kmd, checkError))
						updateStatus(STA_SOK_kmd, "Err  \"%s\"", kmd.c_str());
					else
						updateStatus(STA_SOK_kmd, "Ok \"%s\"", kmd.c_str());
					kmdCli++;
				}
				kmd = "";
			}
		}
		updateStatus(STA_SOK_status, "CLOSE:%-4i", cntCnx);
		//if (!CLIENT->writeStr(DEF_UDP_CLIENT_HANDSHAKE, checkError)) {
		/*#define DEF_UDP_CLIENT_CLOSE "!CLOS"

		 if (CLIENT->writeStr(DEF_UDP_CLIENT_CLOSE, checkError)) {
		 sleep(1);
		 }
		 */delete CLIENT;
		cntCnx++;
	} while (1);
#else
	while (1) {
		if (getstr(kmd)) {
			cout << "'" << kmd << "'" << flush;
			kmd = "";
		}
	}
#endif
	return 0;
}
///////////////////////////////////////////////////////////////////////
/**
 *
 * CF. https://www.lihaoyi.com/post/BuildyourownCommandLinewithANSIescapecodes.html
 * 		http://manpages.ubuntu.com/manpages/bionic/man4/console_codes.4.html
 * 		https://qastack.fr/programming/4842424/list-of-ansi-color-escape-sequences
 *
 //					cout << "\033[H\033[2J"; goto upper left & clear !!!
 //					cout << "\033[H3;20\033[2J"; goto row 3;col 20 & clear all !!!
 //					cout << "\033[H3;20\033[1J"; goto row 3;col 20 & clear uppest part !!!
 //					cout << "\033[H3;20\033[J"; goto row 3;col 20 & clear downest part !!!

 //				myfile << recv << flush;
 */
