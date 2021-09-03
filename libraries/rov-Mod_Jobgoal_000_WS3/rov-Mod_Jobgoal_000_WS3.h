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
	uint64_t DTMS = 0;
	String QRY;
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
	int16_t _qryLine;
	bool mOP_MisStatus(JsonObject &JOBJ);
	bool mOP_open_Jobgoal(JsonObject &JOBJ);
	bool mOP_close_Jobgoal(JsonObject &JOBJ);
	bool splitQRYtxt(s_misQRY &STRQRY, const String CHN, const bool INITMS = false);
	String getNextLine();
	uint64_t _prev_TMS;
	public:
	s_misQRY nxtQRY;
	Jobgoal(Flow *FLOW, const s_JOBGOAL MOD_CFG, int8_t *DBGMAX);
	bool auto_MSG(const uint64_t DTMS);
	/**
	 * \fn bool complete_MSG(JsonObject & KMD);
	 * \param JsonObject & KMD default tmsout_MSG command
	 * \brief adds the file line number in the DEF_KF_GET field
	 */
	bool complete_MSG(JsonObject &KMD);
	bool reply2pilotQ_OP(JsonObject &JOBJ);
};
///////////////////////////////////////////////////////////////////////
#endif
