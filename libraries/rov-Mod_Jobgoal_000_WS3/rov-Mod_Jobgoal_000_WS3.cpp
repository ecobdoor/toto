#include "rov-Mod_Jobgoal_000_WS3.h"
//#include "rov-Mod_Motors_000_WS3.h"
#include "esp32_LOGS.h"
using namespace std;
///////////////////////////////////////////////////////////////////////
/**
 * \class Jobgoal
 */
Jobgoal::Jobgoal(c_myFlow *FLOW, const s_JOBGOAL MOD_CFG, int8_t *DBGMAX)
:
	Module(FLOW, MOD_CFG.MOD, DBGMAX){
	map_OPjobgoal = {
		{ DEF_OP_OK, &Jobgoal::mOP_wrkReport },
		{ DEF_OP_OPEN, &Jobgoal::mOP_open_Jobgoal },
		{ DEF_OP_CLOSE, &Jobgoal::mOP_close_Jobgoal }
	};
//	nxtQRY.ok=false;
//	nxtQRY.TMS=0;
}
//---------------------------------------------------------------------
bool Jobgoal::reply2pilotQ_OP(JsonObject &JOBJ){
	const char *x = JOBJ["KMD"]["OP"].as<string>().c_str();
	(this->*(findOP<OPmis>(map_OPjobgoal, x)))(JOBJ);
	return true;
}
//---------------------------------------------------------------------
bool Jobgoal::mOP_wrkReport(JsonObject &JOBJ){
	//JOBJ["KMD"]["RUN"] = _isRunning;
	JOBJ["KMD"]["MID"] = "JOB";
	JOBJ["KMD"][DEF_KF_GET] = _qryLine;
	JOBJ["KMD"][DEF_KF_EXE] = _isRunning;
	JOBJ["KMD"]["Report"] = nxtQRY.QRY;
	JOBJ["KMD"]["OP"] = "Report";

	return true;
}
//---------------------------------------------------------------------
void trimEoS(String &CHN){
	int len = CHN.length();
	if (!len)
		return;
	len--;
	int pos = len;
	do {
		char c = CHN[pos];
		if ((c != '\n') || (c != '\r'))
			break;
		pos--;
	} while (pos);
	if (pos != len)
		CHN = CHN.substring(0, pos);
}
//---------------------------------------------------------------------
String trimLEN(const String &CHN, const int LEN){
	String space = "                                    ";
	int len = CHN.length();
	if (len < LEN)
		return CHN + space.substring(0, LEN - len);
	else
		return CHN;
}
//---------------------------------------------------------------------
bool Jobgoal::getNextLine(const bool INITMS){
	static uint64_t previousTMS;
	if (!_qryFile.available())
		return false;
	_qryLine++;
	nxtQRY.QRY = _qryFile.readStringUntil('\n');
	trimEoS(nxtQRY.QRY);
	uint64_t when = 0;
	int pos = nxtQRY.QRY.indexOf('{');
	if (0 < pos) {
		when = UI64S(nxtQRY.QRY.substring(0, pos));
		nxtQRY.QRY = nxtQRY.QRY.substring(pos);
		if (!_firstGoodLine) {
			if (when >= previousTMS)
				nxtQRY.DTMS = when - previousTMS;
			else {
				nxtQRY.DTMS = 0; // to execute asap !!!
				nxtQRY.QRY = "!!! TIMING ERREUR !!!";
				nxtQRY.ok = false;
				previousTMS = when; // continue despite the error
				return true;
			}
		} else if (INITMS)
			nxtQRY.DTMS = 0;
		else if (_firstGoodLine) {
			nxtQRY.DTMS = 0; // to execute asap !!!
			_firstGoodLine = false;
		}
		previousTMS = when;
		nxtQRY.ok = true;
	} else {
		nxtQRY.DTMS = 0; // to execute asap !!!
		nxtQRY.ok = false;
	}
	_SERIAL_0("\n%-8s %4i getNextLine(%s) dt:%5llu, prv:%8llu `%s`", CTX.strRoverMode(), _qryLine,
		(nxtQRY.ok) ? "ok" : "KO", nxtQRY.DTMS, previousTMS, nxtQRY.QRY.c_str());
//	_SERIAL_0("%x %x %x", nxtQRY.QRY[nxtQRY.QRY.length() - 3],nxtQRY.QRY[nxtQRY.QRY.length() - 2],nxtQRY.QRY[nxtQRY.QRY.length() - 1]);
	return true;
}
//---------------------------------------------------------------------
e_hasMsg Jobgoal::complete_MSG(JsonObject &KMD){
	KMD[DEF_KF_GET] = _qryLine; // Line number in jobgoal file
	KMD[DEF_KF_FIL] = _qryFileName;
	KMD["OP"] = DEF_KF_GET;
	return e_hasMsg::Ack;
}
//---------------------------------------------------------------------
e_hasMsg Jobgoal::auto_MSG(const uint64_t DTMS,JsonObject &KMD){
//	Jobgoal ended
	if (!_isRunning) {
		if (!_msgDone) {
			_SERIAL_0("\n%-8s ==== Jobgoal not running", CTX.strRoverMode());
			_msgDone = true;
		}
		return e_hasMsg::No;
	}
//	Jobgoal _countdown
	if (_countdown) {
		if ((milli_TS() - _rover_prevTMS) < _countdown) {
			if (!_msgDone) {
				_SERIAL_0("\n%-8s ---- Jobgoal _countdown %llu ms", CTX.strRoverMode(), _countdown);
				_msgDone = true;
			}
		} else {
			_countdown = 0;
			_rover_prevTMS = milli_TS();
		}
		return e_hasMsg::No;
	}
	/*
	 //	Jobgoal time-out
	 if ((_isRunning) && (_milliPeriod != 0) && ((uint64_t)_milliPeriod < DTMS)) {
	 if (!_msgDone) {
	 _SERIAL_0("\n%s ---- Jobgoal time out", CTX.strRoverMode());
	 _msgDone = true;
	 }
	 return tmsout_MSG(DTMS);
	 }
	 */
//	Jobgoal next command at time not yet
	uint64_t now = milli_TS();
	uint64_t dt = now - _rover_prevTMS;
	if (dt < nxtQRY.DTMS) {
		if (!_msgDone) {
			_SERIAL_0("\n%-8s ---- Jobgoal waiting %llu<%llu", CTX.strRoverMode(), dt, nxtQRY.DTMS);
			_msgDone = true;
		}
		return e_hasMsg::No;
	}
	if (!getNextLine()) {
		_isRunning = false;
		_msgDone = false;
		_qryLine = -_qryLine;
		_SERIAL_0("\n%-8s %04i Jobgoal ENDING  '%s'", CTX.strRoverMode(), _qryLine,
			_qryFileName.c_str());
		CTX.setRoverMode(e_roverMode::idle);
		JsonObject root = (*_jsonDoc).as<JsonObject>(); // Here _jsonDoc is still the JOB message built by popup_MSG from bool Module::has_MSG()
		mOP_close_Jobgoal(root);
		return e_hasMsg::Ack; // eof...
	}
//	Jobgoal has to execute something
	if (!_msgDone) {
		_SERIAL_0("\n%-8s ==== Jobgoal executing '%s'", CTX.strRoverMode(), _qryFileName.c_str());
		_msgDone = true;
	}
	try {
		if (nxtQRY.ok) { //command line *_jsonDoc mostly becomes a `MOT` query
			DeserializationError error = deserializeJson(*_jsonDoc, nxtQRY.QRY.c_str()); // détruit la source which must stay available during _sonDoc used !!!
			if (error != 0)
				throw String("ERROR deserializeJson ????:" + nxtQRY.QRY);
			JsonObject root = (*_jsonDoc).as<JsonObject>(); // the json file line (MID=MO or others)
			// Launch query to attached module
			const char *x = root["CTL"]["TYP"].as<string>().c_str(); // DEF_QUERY_PILOT_TO_ROVER, { reply2query_PILOT
			extern t_map_QSRC sourceMap;
			t_map_QSRC::iterator itDisps = sourceMap.find(x);
			if (itDisps == sourceMap.end())
				THROWERR(MOD_TYPE_ERR, x);
			//int nnn = itDisps->second.num;		_SERIAL_0("\n TYP ok '%s' %i", x, nnn);
			// if it's a query, reply TIK=0 OP check because of jobgoal mode !!!
			if (itDisps->second.task(root)) { // call the module OP
				root["CTL"]["TIK"] = 0;
				root["KMD"]["OP"] = DEF_OP_CHK;
				String CHN;
				serializeJson(*_jsonDoc, CHN);
				_SERIAL_0("\n%-8s %4i EXE WORKING(%s) dt:%5llu, PRV:%8llu `%s`",
					CTX.strRoverMode(), _qryLine, (nxtQRY.ok) ? "ok" : "KO",
					nxtQRY.DTMS, _rover_prevTMS, nxtQRY.QRY.c_str());
				_rover_prevTMS = now;
			}
		} else {
			JsonObject root = (*_jsonDoc).as<JsonObject>(); // Here _jsonDoc is still the JOB message built by popup_MSG from bool Module::has_MSG()
			mOP_wrkReport(root);
			return e_hasMsg::Ack; // comment...
		}
	} catch (EXCEPT const &e) {
		FIX_ERROR();
	}
	return e_hasMsg::No;
}
//---------------------------------------------------------------------
bool Jobgoal::mOP_close_Jobgoal(JsonObject &JOBJ){
	_SERIAL_0("\n%-8s %04i Jobgoal CLOSING '%s'", CTX.strRoverMode(), _qryLine,
		_qryFileName.c_str());
	_qryFile.close();
	JOBJ["KMD"]["OP"] = DEF_OP_CLOSE;
	JOBJ["KMD"][DEF_KF_EXE] = true;
	JOBJ["KMD"][DEF_KF_FIL] = _qryFileName;
	_qryFileName = "";
	_qryLine = -1;
	_isRunning = false;
	return true;
}
//---------------------------------------------------------------------
bool Jobgoal::mOP_open_Jobgoal(JsonObject &JOBJ){
	_qryFileName = "/" + JOBJ["KMD"][DEF_KF_FIL].as<String>() + ".work";
	_SERIAL_0("\n%-8s %04i Jobgoal OPENING '%s'", CTX.strRoverMode(), _qryLine,
		_qryFileName.c_str());
	_msgDone = false;
	try {
		if (!SPIFFS.exists(_qryFileName)) {
			JOBJ["KMD"][DEF_KF_EXE] = false;
			JOBJ["KMD"][DEF_KF_FIL] = _qryFileName + " not found";
			_qryLine = -1;
			_isRunning = false;
			_qryFileName = "";
			THROWERR(FILE_NOT_FOUND, _qryFileName.c_str());
			return false;
		} else {
			_qryFile = SPIFFS.open(_qryFileName, "r");
			_rover_prevTMS = 0;
			_qryLine = 0;
			_countdown = 2000;
			_firstGoodLine = true;
			_rover_prevTMS = milli_TS();
			_isRunning = true;
			CTX.setRoverMode(e_roverMode::working);
			JOBJ["KMD"][DEF_KF_EXE] = _isRunning; //CTX.strRoverMode();
			/*
			 _SERIAL_3( "\n'{%p}'",_qryFile);
			 String CHN = getNextLine();
			 while (CHN != "") {
			 _SERIAL_3( "\n'%s'", CHN.c_str());
			 CHN = getNextLine();
			 */
		}
	} catch (EXCEPT const &e) {
		FIX_ERROR();
	}
	return true;
}
///////////////////////////////////////////////////////////////////////
