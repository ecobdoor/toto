#ifndef _esp32_SYS_basic_
#define _esp32_SYS_basic_
#include <cassert>
#include <vector>
#include "Arduino.h"
#include <map>
#include <new>          // std::nothrow
//#include "esp32_GOODIES.h"
/*
 Dans .h pour espace de noms (C & pas C++)!!
 BDOOR_ protocol
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
uint64_t micro_TS();
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
//String datim(uint64_t TMS, const int SHIFTHOURS = 1);
inline String SBOOL(const bool BOOLEAN){
if (BOOLEAN)
	return "Yes";
else
	return "No";
}
/**
 * \brief      Converts an unsigned 64 bits integer to a String
 * \param    VAL unsigned long long int result
 * \return   String result
 */
String SUI64(const uint64_t VAL);
/**
 * @fn uint64_t UI64S(const String CHN)
 * \brief      Converts a String to a unsigned 64 bits integer
 * \details    REMARK no consistency check is performed on the input string
 * \param    CHN	String to convert
 * \return   unsigned long long int result
 */
uint64_t UI64S(const String CHN);
uint32_t UI32S(const String CHN);
///////////////////////////////////////////////////////////////////////
/**
 * \class Core
 */
class Core {
protected:
const char *class_Type;
int8_t &this_DebugLVL;

public:
Core(const char *classType, int8_t *dbgLvlMax);
void set_dbgLVL(const int8_t LVL);
/**
 * @fn virtual bool On_Error(const e_STR ERRCOD)
 * @brief To be overload by each Core inherited class
 * @param ERRCOD @ref Langage
 * @return true if the error is resolved, eslse false to default except management
 */
virtual bool On_Error(const e_STR ERRCOD){
	return false;
}
;
int8_t get_dbgLVL();
int8_t& adr_dbgLVL();
};
inline void Core::set_dbgLVL(const int8_t LVL){
this_DebugLVL = LVL;
}
inline int8_t Core::get_dbgLVL(){
return this_DebugLVL;
}
inline int8_t& Core::adr_dbgLVL(){
return this_DebugLVL;
}
///////////////////////////////////////////////////////////////////////
extern const char blackNum[10][4];
extern const char whiteNum[10][4];
//extern const char lowerAlpha[26][4];
//extern const char upperAlpha[26][4];
#endif
