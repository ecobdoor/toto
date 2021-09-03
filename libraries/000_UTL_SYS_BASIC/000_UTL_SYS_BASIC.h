#ifndef _UTL_SYS_BASIC_
#define _UTL_SYS_BASIC_
#include <assert.h>
#include <vector>
#include "Arduino.h"
#include <map>
#include <new>          // std::nothrow
#define ARDUINOJSON_USE_LONG_LONG 1
#include "ArduinoJson.h"
#include "esp32_GOODIES.h"
///////////////////////////////////////////////////////////////////////
// Approximately 32 bytes by field
typedef StaticJsonDocument<JSON_OBJECT_SIZE(50) + 60> t_jsonDoc;
/*
 Dans .h pour espace de noms (C & pas C++)!!
 BDOOR_protocol
 */
using namespace std;
#define pi  3.141592653589793238462643383279502884f
//          3,141592653589793
#define USE_SERIAL Serial
#define DEF_DBGMSG_BUFSIZE 2048 //3072=3*1024
///////////////////////////////////////////////////////////////////////
/**
 * \var errOK OK
 * \brief extern errOK class including the global atomic flag error for multi-threading exceptions management
 */
///////////////////////////////////////////////////////////////////////
typedef enum /*class e_STR*/{
#include "rov-Sen_000_WS3(french).csv"
} e_STR;
///////////////////////////////////////////////////////////////////////////////
/**
 * Struct sorter to manage map key of const char * type
 */
struct strCmp {
	bool operator()(const char *s1, const char *s2) const{
		return strcmp(s1, s2) < 0;
	}
};
///////////////////////////////////////////////////////////////////////
uint64_t milli_TS();
//uint64_t IRAM_ATTR micro_TS();
char inKey(const String CHN = "");
String inString(const char *PROMPT = nullptr);
uint64_t Uint64(const char *str);
int16_t Int16(const char *str);
int64_t Int64(const char *str);
int Int(const char *str);
float Float(const char *str);
inline bool isDigit(const char D){
	return (48 <= D) && (D <= 57);
}
inline uint8_t d09(const char D){
	return D - 48;
}
//=============================================================================
/**
 * \fn splitString(vector<string> & ARGS, const string & CHN,const char * SEPARATORS)
 * \brief SPLIT the string CHN considering SEPARATORS chars delimiters
 * \param vector<string> & ARGS address to return result of cut CHN
 * \param const string & CHN string to cut
 * \param char * SEPARATORS
 * @param const bool debug to display the result vector
 * \return first char of CHN command else '\0' if empty
 */
//-----------------------------------------------------------------------------
void SplitOnLast(const std::string str, std::string &first, std::string &second,
	const char *charPatterns);
char splitString(vector<String> &ARGS, const String &CHN, const char *SEPARATORS = ":,",
	const bool debug = false);
void SPrintF(const uint16_t SIZE, int16_t &CNT, char *BUFFER, const char *FORMAT, ...);
void VSPrintF(const uint16_t SIZE, int16_t &CNT, char *BUFFER, const char *FORMAT, va_list *ARGS);
//String datim(uint64_t TMS, const int SHIFTHOURS = 1);
inline String SBOOL(const bool BOOLEAN){
	if (BOOLEAN)
		return "Yes";
	else
		return "No";
}
void VSPRINTF_BUFFER(const int8_t LVL, const char *FORMAT,
	va_list *ARGS, int16_t SIZE, int16_t &CNT, char *BUFFER);
/**
 * \brief      Converts an unsigned 64 bits integer to a String
 * \param    VAL unsigned long long int result
 * \return   String result
 */
String SUI64(const uint64_t VAL);
/**
 * \brief      Converts a String to a unsigned 64 bits integer
 * \details    REMARK no consistency check is performed on the input string
 * \param    CHN	String to convert
 * \return   unsigned long long int result
 */
uint64_t UI64S(const String CHN);
/**
 * \class DbgLog
 */
class Core {
protected:
	const char *class_Type;
	int8_t &this_DebugLVL;
	public:
	Core(const char *classType, int8_t *dbgLvlMax);
	void set_dbgLVL(const int8_t LVL);
	int8_t get_dbgLVL();
};
extern const char blackNum[10][4];
extern const char whiteNum[10][4];
#endif
