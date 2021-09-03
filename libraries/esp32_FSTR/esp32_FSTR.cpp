#include <esp32_FSTR.h>
#include <esp32_LOGS.h>
#include "esp32_SPRINT.h"
#include "esp32_GOODIES.h"
Langage Lang;
///////////////////////////////////////////////////////////////////////
bool Langage::Begin(const String FILE_LANGUE){
	extern int &this_DebugLVL;
	_DBG_APP_BOOT("\n#### Building localized strings");
	if (!SPIFFS.exists(FILE_LANGUE)) {
		_DBG_APP_BOOT("FAILS, '%s' not found\n", FILE_LANGUE.c_str());
		return false;
	}
	File file = SPIFFS.open(FILE_LANGUE, FILE_READ);
	if (!file) {
		_DBG_APP_BOOT(" FAILS to open '%s'", FILE_LANGUE.c_str());
		return false;
	}
	char bufENUM[48];
	char bufTHR[16];
	char bufSTR[208];
	int16_t cnt = 0;
	int lenLevel;
	int len_Text;
	int lenLabel;
	char *cstrIdx = &buffer_CSTR[0];
	char *labelIdx = &buffer_LABEL[0];
	_DBG_APP_BOOT("\n\t**** IGNORED DATA ****************");
	/*
	 */
	while (file.available()) {
		len_Text = file.readBytesUntil('\n', bufSTR, sizeof(bufSTR));
		bufSTR[len_Text] = 0;
		if ((bufSTR[0] == '/') && (bufSTR[1] == '/'))
			_DBG_APP_BOOT("\n\t[%s]", bufSTR);
	}
	_DBG_APP_BOOT("\n");
	file.close();
	delay(1000);
	file = SPIFFS.open(FILE_LANGUE, FILE_READ);
	while (file.available()) {
		lenLabel = file.readBytesUntil(';', bufENUM, sizeof(bufENUM));
		lenLabel = lenLabel - 3; // supress ,//
		bufENUM[lenLabel] = 0;
		lenLevel = file.readBytesUntil(';', bufTHR, sizeof(bufTHR));
		bufTHR[lenLevel] = 0;
		int16_t cntThreshold = atoi((char*)bufTHR);
		len_Text = file.readBytesUntil('\n', bufSTR, sizeof(bufSTR));
		bufSTR[len_Text] = 0;
		/*
		 if (it != mapErrLVL.end())
		 cntThreshold = it->second;
		 else {
		 _DBG_APP_BOOT("\n%4i:%-4i %3i[%48s][%s(??)][%s]",
		 (ptrdiff_t)(labelIdx - &buffer_LABEL[0]),
		 (ptrdiff_t)(cstrIdx - &buffer_CSTR[0]), cnt, bufENUM, bufTHR, bufSTR);
		 inKey("\nProblem decoding error level! Release the csv file !!!");
		 }
		 */
		if ((bufENUM[0] == '/') && (bufENUM[1] == '/')) {
			_DBG_APP_BOOT("\n  IGNORED    [%48s](%s->%4i)[%s]",
				bufENUM, bufTHR, cntThreshold, bufSTR);
			continue;
		}

		_DBG_APP_BOOT("\n%4i:%-4i %3i[%48s](%s->%4i)[%s]",
			(ptrdiff_t )(labelIdx - &buffer_LABEL[0]), (ptrdiff_t )(cstrIdx - &buffer_CSTR[0]),
			cnt, bufENUM, bufTHR, cntThreshold, bufSTR);
		for (size_t i = 0; i < len_Text; i++) {
			if (bufSTR[i] == 0x7F)
				bufSTR[i] = '\n';
			/*
			 _SERIAL_0("'");
			 _SERIAL_0(bufSTR[i]);
			 _SERIAL_0("'");
			 */
		}
		if (DEF_CSTRSIZE < (strlen(bufSTR) + (ptrdiff_t)(cstrIdx - &buffer_CSTR[0])))
			halt("Overflow on string %i", DEF_CSTRSIZE);
		if (DEF_LABELSIZE < (strlen(bufENUM) + (ptrdiff_t)(labelIdx - &buffer_LABEL[0])))
			halt("Overflow on labels %i", DEF_LABELSIZE);
		strcpy(cstrIdx, &bufSTR[0]);
		strcpy(labelIdx, &bufENUM[0]);
		s_CSTR xxx;
		xxx.pCHAR = cstrIdx;
		xxx.pLABEL = labelIdx;
		xxx.sTHR = cntThreshold;
		xxx.sCNT = 0;
		vSTRs.push_back(xxx);
		cstrIdx += len_Text + 1;
		labelIdx += lenLabel + 1;
		cnt++;
	}
	file.close();
	ptrdiff_t diff = cstrIdx - &buffer_CSTR[0];
	ptrdiff_t diffLabel = labelIdx - &buffer_LABEL[0];
	_DBG_APP_BOOT("\n");
	_DBG_APP_BOOT(vSTRs[STR_SIZE].pCHAR, diff + 1, DEF_CSTRSIZE, diffLabel + 1, DEF_LABELSIZE);
	/*
	 * NO_ERROR______ is not first & RECOV_ERR_EXAMPLE is not last (csv is sorted!!!!
	 for(int s=(int)NO_ERROR______;s<=(int)RECOV_ERR_EXAMPLE;s++) {
	 _SERIAL_0("\n%4i [%40s] [%s]",s,vSTRs[(e_STR)s].pLABEL,vSTRs[(e_STR)s].pCHAR);
	 }
	 _SERIAL_0("");
	 */
	return true;
}
//---------------------------------------------------------------------
String Langage::listDir2download(fs::FS &fs, const char *dirname, uint8_t levels){
	typedef struct {
		string _path;
		string _name;
		uint32_t _size;
		int _level;
		string _ext;
	} pa2img_t;
	static vector<pa2img_t> infos;
	static char buffer[4096];
	static int16_t cnt = 0;
	if (levels == 0) {
		infos.clear();
		cnt = 0;
	}
	File root = fs.open(dirname);
	if (!root) {
		SPrintF(sizeof(buffer), cnt, buffer, "Failed to open directory '%s'\r\n", dirname);
		return String(buffer);
	}
	File file = root.openNextFile();
	int cnt_fil = 0;
	while (file) {
		if (file.isDirectory()) {
			SPrintF(sizeof(buffer), cnt, buffer, "  DIR : %s", file.name());
			if (levels) {
				listDir(fs, file.name(), levels - 1);
			}
		} else {
//			SPrintF(sizeof(buffer), cnt, buffer, "%3i %7i\t%s\n", cnt_fil, file.size(),file.name());
			pa2img_t record;
			SplitOnLast(file.name(), record._path, record._name, "/\\");
			// string s=file.name();
			record._level = count(record._path.begin(), record._path.end(), '/');
			record._size = file.size();
			infos.push_back(record);
		}
		file = root.openNextFile();
		cnt_fil++;
	}
	std::sort(infos.begin(), infos.end(),
		[](pa2img_t const &a, pa2img_t const &b) -> bool{
			String A = String(a._path.c_str()) + "/" + String(a._name.c_str());
			String B = String(b._path.c_str()) + "/" + String(b._name.c_str());
			return (A < B);
		});
	SPrintF(sizeof(buffer), cnt, buffer, "Listing directory '%s'</br>\n", dirname);
	SPrintF(sizeof(buffer), cnt, buffer, "<table>\n");
	for (size_t i = 0; i < infos.size(); i++) {
		string space = "";
		for (size_t j = 0; j < infos[i]._level; j++)
			space += "&nbsp;&nbsp;&nbsp;";
		SPrintF(sizeof(buffer), cnt, buffer, "<tr>");
		{
			SPrintF(sizeof(buffer), cnt, buffer,
				"<td align =\"right\">%3i</td><td align =\"right\">%3i</td><td align =\"right\"> %7i</td>",
				i, infos[i]._level, infos[i]._size);
			SPrintF(sizeof(buffer), cnt, buffer,
				"<td><a href='/?download=/%s'>%s/%s</a></td>",
				infos[i]._name.c_str(), space.c_str(), infos[i]._name.c_str());
			SPrintF(sizeof(buffer), cnt, buffer,
				"<td style=\"color:#ff0000\"><a href='/?remove=/%s' title=\"DELETE\">&nbsp;&nbsp;&nbsp;|X|</a></td>",
				infos[i]._name.c_str());
		}
		SPrintF(sizeof(buffer), cnt, buffer, "</tr>\n");
	}
	SPrintF(sizeof(buffer), cnt, buffer, "</table>\n");
	SPrintF(sizeof(buffer), cnt, buffer,
		"</br>--------------------------------------</br>----%7i Bytes disponibles\n",
		SPIFFS.totalBytes() - SPIFFS.usedBytes());
	return String(buffer);
}
//---------------------------------------------------------------------
/**
 *
 <select id="IDtim_DRIVER" class="mv2_select  classTimer">
 <option value="0" selected="selected">   Never</option>
 <option value="1"> 1 Shoot</option><option value="2">1/(hour)</option>
 <option value="3">1/(5 mn)</option><option value="4">1/(1 mn)</option>
 <option value="5">  0.1 Hz</option><option value="6">    1 Hz</option>
 </select>
 *
 *
 *
 * @param fs
 * @param dirname
 * @param levels
 * @return
 */
String Langage::listDir2select(fs::FS &fs, const char *dirname, uint8_t levels){
	typedef struct {
		string _path;
		string _name;
		uint32_t _size;
		int _level;
		string _ext;
	} pa2img_t;
	static vector<pa2img_t> infos;
	static char buffer[3048];
	static int16_t cnt = 0;
	if (levels == 0) {
		infos.clear();
		cnt = 0;
	}
	File root = fs.open(dirname);
	if (!root) {
		SPrintF(sizeof(buffer), cnt, buffer, "Failed to open directory '%s'\r\n", dirname);
		return String(buffer);
	}
	File file = root.openNextFile();
	int cnt_fil = 0;
	while (file) {
		if (file.isDirectory()) {
			SPrintF(sizeof(buffer), cnt, buffer, "  DIR : %s", file.name());
			if (levels) {
				listDir(fs, file.name(), levels - 1);
			}
		} else {
//			SPrintF(sizeof(buffer), cnt, buffer, "%3i %7i\t%s\n", cnt_fil, file.size(),file.name());
			pa2img_t record;
			SplitOnLast(file.name(), record._path, record._name, "/\\");
			SplitOnLast(record._name, record._name, record._ext, ".");
			// string s=file.name();
			record._level = count(record._path.begin(), record._path.end(), '/');
			record._size = file.size();
			infos.push_back(record);
		}
		file = root.openNextFile();
		cnt_fil++;
	}
	std::sort(infos.begin(), infos.end(),
		[](pa2img_t const &a, pa2img_t const &b) -> bool{
			String A = String(a._path.c_str()) + "/" + String(a._name.c_str());
			String B = String(b._path.c_str()) + "/" + String(b._name.c_str());
			return (A < B);
		});
	/*
	SPrintF(sizeof(buffer), cnt, buffer, "\n0\n1\n2\n3<br><select>");
	SPrintF(sizeof(buffer), cnt, buffer, "\n<option value=\"0\">Choose a file</option>");
	uint8_t val = 1;
	for (size_t i = 0; i < infos.size(); i++) {
		if (infos[i]._ext != "work") {
			Serial.printf("\nExcluding %s %s", infos[i]._name.c_str(), infos[i]._ext.c_str());
			continue;
		}
		SPrintF(sizeof(buffer), cnt, buffer, "\n<option value=\"%i\">", val);
		SPrintF(sizeof(buffer), cnt, buffer, "%s", infos[i]._name.c_str());
		SPrintF(sizeof(buffer), cnt, buffer, "</option>");
		val++;
	}
	SPrintF(sizeof(buffer), cnt, buffer, "\n</select>", dirname);
	*/
	/*
	'__field_timer':
{
	LISTEN: {
		opt: {
			sel: 1, show: 1, entry: "   Never", background: '#aaaaaa'
		},
		kmd: {
		 	OP: DEF_OP_GETTOUT, codeTMS: -1
		}
	},
		SHOOT: { opt: { sel: 0, show: 1, entry: " 1 Shoot", background: '#888888' }, kmd: { OP: DEF_OP_SETTOUT, codeTMS: 0 } },
		L_hour: { opt: { sel: 0, show: 1, entry: "1/(hour)", background: '#ffffff' }, kmd: { OP: DEF_OP_SETTOUT, codeTMS: 3600000 } },


{
	"CHOOSE":{
		"opt":{
			"sel":1,"show":1,"entry":"Choose a file","background":"#ff0000"
		},
		"timer": 0,
		"kmd":{
			"OP":"DEF_OP_CLOSE","soks":["wsR"]
		},
	},
	"CHOICE_1":{"opt":{"sel":0,"show":1,"entry":"Learn.work","background":"#ffff00"},"timer": 6,"kmd":{"OP":"DEF_OP_OPEN","soks":["wsR"]},},"CHOICE_2":{"opt":{"sel":0,"show":1,"entry":"toto.work","background":"#ffff00"},"timer": 6,"kmd":{"OP":"DEF_OP_OPEN","soks":["wsR"]},
	}
}

	 */
	SPrintF(sizeof(buffer), cnt, buffer, "{");
	SPrintF(sizeof(buffer), cnt, buffer, "\"CHOOSE\":{\"opt\":{\"sel\":1,\"show\":1,");
	SPrintF(sizeof(buffer), cnt, buffer, "\"entry\":\"Choose a file\",");
	SPrintF(sizeof(buffer), cnt, buffer, "\"background\":\"#ff0000\"},");

	SPrintF(sizeof(buffer), cnt, buffer, "\"timer\": 0,");
	SPrintF(sizeof(buffer), cnt, buffer, "\"kmd\":{\"OP\":\"%s\",\"soks\":[\"wsR\"]},",DEF_OP_CLOSE);

	cnt--;// last ","

	SPrintF(sizeof(buffer), cnt, buffer, "},");
	uint8_t val = 1;
	for (size_t i = 0; i < infos.size(); i++) {
		if (infos[i]._ext != "work") {
			Serial.printf("\nExcluding %s %s", infos[i]._name.c_str(), infos[i]._ext.c_str());
			continue;
		}
		Serial.printf("\nIncluding %s %s", infos[i]._name.c_str(), infos[i]._ext.c_str());
		SPrintF(sizeof(buffer), cnt, buffer, "\"CHOICE_%i\":{\"opt\":{\"sel\":0,\"show\":1,",val);
		SPrintF(sizeof(buffer), cnt, buffer, "\"entry\":\"%s\",",infos[i]._name.c_str());
		SPrintF(sizeof(buffer), cnt, buffer, "\"background\":\"#ffff00\"},");

		SPrintF(sizeof(buffer), cnt, buffer, "\"timer\": 6,");
		SPrintF(sizeof(buffer), cnt, buffer, "\"kmd\":{\"OP\":\"%s\",\"soks\":[\"wsR\"]},",DEF_OP_OPEN);

		cnt--;// last ","

		SPrintF(sizeof(buffer), cnt, buffer, "},");

		val++;
	}
	cnt--;// last ","
	SPrintF(sizeof(buffer), cnt, buffer, "}");
	return String(buffer);
}
//---------------------------------------------------------------------
String Langage::listDir(fs::FS &fs, const char *dirname, uint8_t levels){
	typedef struct {
		string _path;
		string _name;
		uint32_t _size;
		int _level;
		string _ext;
	} pa2img_t;
	static vector<pa2img_t> infos;
	static char buffer[3048];
	static int16_t cnt = 0;
	if (levels == 0) {
		infos.clear();
		cnt = 0;
	}
	File root = fs.open(dirname);
	if (!root) {
		SPrintF(sizeof(buffer), cnt, buffer, "Failed to open directory '%s'\r\n", dirname);
		return String(buffer);
	}
	File file = root.openNextFile();
	int cnt_fil = 0;
	while (file) {
		if (file.isDirectory()) {
			SPrintF(sizeof(buffer), cnt, buffer, "  DIR : %s", file.name());
			if (levels) {
				listDir(fs, file.name(), levels - 1);
			}
		} else {
//			SPrintF(sizeof(buffer), cnt, buffer, "%3i %7i\t%s\n", cnt_fil, file.size(),file.name());
			pa2img_t record;
			SplitOnLast(file.name(), record._path, record._name, "/\\");
			// string s=file.name();
			record._level = count(record._path.begin(), record._path.end(), '/');
			record._size = file.size();
			infos.push_back(record);
		}
		file = root.openNextFile();
		cnt_fil++;
	}
	std::sort(infos.begin(), infos.end(),
		[](pa2img_t const &a, pa2img_t const &b) -> bool{
			String A = String(a._path.c_str()) + "/" + String(a._name.c_str());
			String B = String(b._path.c_str()) + "/" + String(b._name.c_str());
			return (A < B);
		});
	SPrintF(sizeof(buffer), cnt, buffer, "Listing directory '%s'</br>\n", dirname);
	SPrintF(sizeof(buffer), cnt, buffer, "<table>\n");
	for (size_t i = 0; i < infos.size(); i++) {
		string space = "";
		for (size_t j = 0; j < infos[i]._level; j++)
			space += "&nbsp;&nbsp;&nbsp;";
		SPrintF(sizeof(buffer), cnt, buffer,
			"<tr><td>%3i %7i%3i</td><td><a href='/?p=%s/%s'>%s%s/%s</a></td></tr>\n",
			i, infos[i]._size, infos[i]._level, infos[i]._path.c_str(), infos[i]._name.c_str(),
			space.c_str(), infos[i]._path.c_str(), infos[i]._name.c_str());
		// <a href="manette.html">manette</a>
	}
	SPrintF(sizeof(buffer), cnt, buffer, "</table>\n");
	SPrintF(sizeof(buffer), cnt, buffer, "</br>----%7i Bytes disponibles\n",
		SPIFFS.totalBytes() - SPIFFS.usedBytes());
	;
	return String(buffer);
}
/*
 https://github.com/espressif/arduino-esp32/tree/master/libraries/SPIFFS
 */
void Langage::listDir_Old(fs::FS &fs, const char *dirname, uint8_t levels){
	extern int8_t &this_DebugLVL;
//_SERIAL_0("\nthis_DebugLVL'%i'",this_DebugLVL);
	File root = fs.open(dirname);
	if (!root) {
		_DBG_APP_BOOT(" FAILED to open this path");
		return;
	}
	if (!root.isDirectory()) {
		_DBG_APP_BOOT(" - not a directory");
		return;
	}

	File file = root.openNextFile();
	int cnt = 0;
	while (file) {
		if (file.isDirectory()) {
			_SERIAL_0("\n	DIR : %s", file.name());
			if (levels) {
				listDir_Old(fs, file.name(), levels - 1);
			}
		} else {

			_DBG_APP_BOOT("\n%3i %7i\t%s", cnt, file.size(), file.name());
		}
		file = root.openNextFile();
		cnt++;
	}
	_DBG_APP_BOOT("\n");
	_DBG_APP_BOOT("%i files, %7i Bytes disponibles.\n", cnt,
		SPIFFS.totalBytes() - SPIFFS.usedBytes());
	;
}
/*
 Status code 	Nom 	Description
 0–999 	  	Reservé et inutilisé.
 1000 		CLOSE_NORMAL 			Fermeture normale ; la connexion s'est terminée normalement.
 1001 		CLOSE_GOING_AWAY 		Reçu lorsque une erreur est apparue sur le serveur ou le navigateur quitte la page ayant ouvert la connexion.
 1002 		CLOSE_PROTOCOL_ERROR 	L'autre partie de la connexion terminte la connexion à cause d'une erreur de protocole.
 1003 		CLOSE_UNSUPPORTED 		La connexion est terminée car le serveur à reçu des données d'un type qu'il ne peut pas accepter (par exemple des données binaire plutôt que du texte).
 1004 	  	Réservé. 				Une utilisation pourrait être définie dans le futur.
 1005 		CLOSE_NO_STATUS 		Réservé. Indique qu'aucun code n'est donné bien qu'un code était attendu.
 1006 		CLOSE_ABNORMAL 			Réservé. Utilisé pour indiquer que la connexion a été terminée anormalement (sans paquet indiquant la fermeture).
 1007 			  					Le serveur termine la connexion car un message contenant des données incohérentes a été reçu (e.g., des données qui ne sont pas au format UTF-8 dans un message texte).
 1008 			  					Le serveur termine la connexion car un message ne respectant pas la politique du serveur est reçu. C'est un code d'état générique qui est utilisé lorsque les codes 1003 et 1009 ne correspondent pas à la situation.
 1009 		CLOSE_TOO_LARGE 		Le serveur termine la connexion car les données reçues sont trop grosses.
 1010 	  							Le client termine la connexion car il souhaitait négocier une ou plusieurs extensions mais le serveur ne l'a pas fait.
 1011 	  							Le serveur termine la connexion car il a rencontré un problème qui l'empêche de traiter la requête.
 1014 	  							Réservé pour une utilisation future par le standard WebSocket.
 1015 	  	Réservé. 				Indique que la connexion a été fermée à cause d'une erreur durant le handshake TLS (e.g., le certificat du serveur ne peut pas être vérifié).
 1016–1999 	  						Réservé pour une utilisation future par le standard WebSocket.
 2000–2999 	  						Réservé pour une utilisation future par le standard WebSocket.
 3000–3999 	  						Disponible pour être utilisé par des bibliothèques ou des frameworks. Ces codes ne doivent pas être utilisés par des applications.
 4000–4999 	  						Disponible pour être utilisés par des applications.
 */
