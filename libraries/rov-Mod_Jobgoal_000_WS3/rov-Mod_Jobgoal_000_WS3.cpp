#include "rov-Mod_Jobgoal_000_WS3.h"
//#include "rov-Mod_Motors_000_WS3.h"
#include "esp32_LOGS.h"
using namespace std;
///////////////////////////////////////////////////////////////////////
/**
 * \class Jobgoal
 */
Jobgoal::Jobgoal(Flow *FLOW, const s_JOBGOAL MOD_CFG, int8_t *DBGMAX)
:
	Module(FLOW, MOD_CFG.MOD, DBGMAX){
	map_OPjobgoal = {
		{ DEF_OP_OK, &Jobgoal::mOP_MisStatus },
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
bool Jobgoal::mOP_MisStatus(JsonObject &JOBJ){
	//JOBJ["KMD"]["RUN"] = _isRunning;
	JOBJ["KMD"][DEF_KF_EXE] = _isRunning;
	return true;
}
//---------------------------------------------------------------------
String Jobgoal::getNextLine()
{
	if (_qryFile.available()) {
		_qryLine++;
		return _qryFile.readStringUntil('\n');
	}
	return "";
}
//---------------------------------------------------------------------
bool Jobgoal::complete_MSG(JsonObject &KMD){
	KMD[DEF_KF_GET] = _qryLine; // Line number in jobgoal file
	return true;
}
//---------------------------------------------------------------------
/**
 * TMS is a relative time referring to an
 */
bool Jobgoal::splitQRYtxt(s_misQRY &STRQRY, const String CHN, const bool INITMS){
	static uint64_t previousTMS;
	if (INITMS)
		previousTMS = 0;
	int pos = CHN.indexOf('{');
	if ((pos < 1) || (CHN.length() == 0)) {
		STRQRY.ok = false;
		STRQRY.DTMS = 0;
		STRQRY.QRY = CHN;
		return false;
	}
	STRQRY.ok = true;
	uint64_t when = UI64S(CHN.substring(0, pos));
	STRQRY.DTMS = when - previousTMS;
	STRQRY.QRY = CHN.substring(pos);
	previousTMS = when;
	_SERIAL_5("\n%*s(%i,%llu,%s)", 35 + 11, "splitQRYtxt", STRQRY.ok, STRQRY.DTMS,
		STRQRY.QRY.c_str());
	return true;
}
//---------------------------------------------------------------------
/**
 * \fn bool Jobgoal::auto_MSG(const uint64_t DTMS)
 * \brief reads timestamped query from a jobgoals file
 * \n launch internally the query for the targeted module MID / operation OP
 * \n generates a DEF_OP_CHK OP (TIK=0)
 * \return bool true or false following  if an external query has to be sent
 */
bool Jobgoal::auto_MSG(const uint64_t DTMS){
	//	Jobgoal ended
	if (!_isRunning) {
		if (!_msgDone) {
			_SERIAL_3("\n===================================Jobgoal not running");
			_msgDone = true;
		}
		return false;
	}
	//	Jobgoal time-out
	if ((_isRunning) && (_milliPeriod != 0) && ((uint64_t)_milliPeriod < DTMS)) {
		if (!_msgDone) {
			_SERIAL_4("\n-----------------------------------Jobgoal time out");
			_msgDone = true;
		}
		return tmsout_MSG(DTMS);
	}
	//	Jobgoal next command not yet
	uint64_t now = milli_TS();
	uint64_t dt = now - _prev_TMS;
	if (dt < nxtQRY.DTMS) {
		if (!_msgDone) {
			_SERIAL_4(
				"\n-----------------------------------Jobgoal waiting %llu<%llu", dt,
				nxtQRY.DTMS);
			_msgDone = true;
		}
		return false;
	}
	//	Jobgoal has to execute something
	if (!_msgDone) {
		_SERIAL_3("\n===================================Jobgoal executing '%s'",
			_qryFileName.c_str());
		_msgDone = true;
	}
	try {
		String keeper = nxtQRY.QRY;
		DeserializationError error = deserializeJson(*_jsonDoc, nxtQRY.QRY.c_str()); // détruit la source which must stay available during _sonDoc used !!!
		if (error != 0)
			throw String("ERROR deserializeJson ????:" + nxtQRY.QRY);
		JsonObject root = (*_jsonDoc).as<JsonObject>();
		// Launch query to attached module
		const char *x = root["CTL"]["TYP"].as<string>().c_str();
		extern t_map_QSRC sourceMap;
		t_map_QSRC::iterator itDisps = sourceMap.find(x);
		if (itDisps == sourceMap.end())
			throw String("CTL.TYP not found: " + String(x));
		int nnn = itDisps->second.num;
		_SERIAL_7("\n TYP ok '%s' %i", x, nnn);
		// if it's a query, reply TIK=0 OP check because of jobgoal mode !!!
		if (itDisps->second.task(root)) {
			root["CTL"]["TIK"] = 0;
			root["KMD"]["OP"] = DEF_OP_CHK;
			String CHN;
			serializeJson(*_jsonDoc, CHN);
			_SERIAL_5("\n%sgenq %s [%s]",
				String("*************************\t").c_str(), _Mid.c_str(),
				CHN.c_str());
		}
	} catch (String const &chaine) {
		_DEBUG_ERR("\n#######->%s", chaine.c_str());
	}
	//	Jobgoal prepares next command
	_prev_TMS = now;
	if (!splitQRYtxt(nxtQRY, getNextLine())) {
		_isRunning = false;
		_msgDone = false;
	}
	return true;
}
//---------------------------------------------------------------------
bool Jobgoal::mOP_open_Jobgoal(JsonObject &JOBJ){
	_qryFileName = "/" + JOBJ["KMD"][DEF_KF_FIL].as<String>() + ".mis";
	_SERIAL_4("\n-----------------------------------Opening '%s'",
		_qryFileName.c_str());
	_msgDone = false;
	if (!SPIFFS.exists(_qryFileName)) {
		_DEBUG_ERR("\n-----------------------------------Error opening '%s'",
			_qryFileName.c_str());
		JOBJ["KMD"][DEF_KF_EXE] = false;
		JOBJ["KMD"][DEF_KF_FIL] = _qryFileName + " not found";
		_qryLine = -1;
		_isRunning = false;
		return true;
	} else {
		_qryFile = SPIFFS.open(_qryFileName, "r");
		_prev_TMS = 0;
		_qryLine = 0;
		_isRunning = splitQRYtxt(nxtQRY, getNextLine(), true); // load first line of jobgoal (if any)
		JOBJ["KMD"][DEF_KF_EXE] = _isRunning;
		/*
		 _SERIAL_3( "\n'{%p}'",_qryFile);
		 String CHN = getNextLine();
		 while (CHN != "") {
		 _SERIAL_3( "\n'%s'", CHN.c_str());
		 CHN = getNextLine();
		 */
	}
	return true;
}
//---------------------------------------------------------------------
bool Jobgoal::mOP_close_Jobgoal(JsonObject &JOBJ){
	_qryFile.close();
	JOBJ["KMD"][DEF_KF_EXE] = true;
	_isRunning = false;
	return true;
}
///////////////////////////////////////////////////////////////////////
