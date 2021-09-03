#ifndef _MOD_JOBGOAL_000_WS3_
#define _MOD_JOBGOAL_000_WS3_
/** @file */
#include "rov-Mod_000_WS3.h"
#include "SPIFFS.h" // File
///////////////////////////////////////////////////////////////////////
class Jobgoal;
typedef bool (Jobgoal::*OPmis)(JsonObject &JOBJ);
typedef std::map<const char*, OPmis, strCmp> t_map_OPjobgoal;
typedef struct {
	bool ok = false;
	uint64_t DTMS = 0;///< milli time of the line in the file
	String QRY;///< JSON text query
} s_misQRY;
/**
 * \class Jobgoal :public Module
 */
class Jobgoal: public Module {
private:
	bool _msgDone = false;
	t_map_OPjobgoal map_OPjobgoal; // Mapping of available module's operations
	String _qryFileName;
	File _qryFile;
	bool _firstGoodLine;
	bool mOP_wrkReport(JsonObject &JOBJ);
	bool mOP_open_Jobgoal(JsonObject &JOBJ);
	bool mOP_close_Jobgoal(JsonObject &JOBJ);
	/**
	 * TMS is a relative time referring to an
	 */
	/**
	 * @fn bool Jobgoal::getNextLine(const bool INITMS)
	 * @brief load a file line & build the matching @ref s_misQRY record to work.
	 *
	 * the line is splitted between the time (ms) when the command should be executed
	 * and the body of the json command. Please note that the time file is when the file
	 * was created and not the local time of the rover. The function is responsible for
	 * calculating the delta T in local time between the line read and the previous line
	 * (is equal to 0 for the first line).
	 * If the line starts with # or is malformed, it is considered a comment (s_misQRY.ok = false).
	 * It is simply sent to the pilot for information.

	 * @param INITMS flag to initialize the time offset between rover and working file when it is opened
	 * @return true if a line was available, false otherwise
	 */
	bool getNextLine(const bool INITMS=false);
	uint64_t _rover_prevTMS;///< true esp32 milli time of of previous file line execution (not time in file!!!)
	uint64_t _countdown=0;///< _countdown before really starting the job (for js to be sent)

public:
	int16_t _qryLine;///< line number in working file (0 at open ok, -1 if open error)
	s_misQRY nxtQRY;
	Jobgoal(c_myFlow *FLOW, const s_JOBGOAL MOD_CFG, int8_t *DBGMAX);
	/**
	 * \fn bool Jobgoal::auto_MSG(const uint64_t DTMS,JsonObject &KMD)
	 * \brief executes one line from a jobgoals file
	 *
	 * \n launch internally the query for the file line targeted module MID / operation OP
	 * \n generates a DEF_OP_CHK OP (TIK=0)
	 * \return a @ref e_hasMsg to cause sending or not
	 */
	e_hasMsg auto_MSG(const uint64_t DTMS,JsonObject &KMD);
	/**
	 * \fn bool complete_MSG(JsonObject & KMD);
	 * \param JsonObject & KMD default tmsout_MSG command
	 * \brief adds the file line number in the DEF_KF_GET field
	 * @return if(isRunning) e_hasMsg::Ack else e_hasMsg::No
	 */
	e_hasMsg complete_MSG(JsonObject &KMD);
	bool reply2pilotQ_OP(JsonObject &JOBJ);
	bool updateATreply(JsonObject &JOBJ){
		return false;
	}
	bool isWorking();
};
inline bool Jobgoal::isWorking(){
	return _isRunning;
}
///////////////////////////////////////////////////////////////////////
#endif
