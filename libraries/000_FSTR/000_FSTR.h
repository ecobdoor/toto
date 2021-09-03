#ifndef __000_FSTR__
#define __000_FSTR__
/** @file */
#include "000_UTL_SYS_BASIC.h"
//#include <000_EXCEPT.h>
#include "SPIFFS.h"
#include <iostream>
#include <stdio.h>
//
using namespace std;
///////////////////////////////////////////////////////////////////////
/*
static e_STR& operator ++(e_STR& t) {
	if (t == e_STR::End) {
		throw std::out_of_range("for e_STR& operator ++ (e_STR&)");
	}
	t = e_STR(static_cast<std::underlying_type<e_STR>::type>(t) + 1);
	return t;
 }
*/
/**
 *  \struct s_CSTR
 *  \brief enum e_STR language localized strings (infos, errors...)
 *  @var pCHAR ...Summarizes each error count
 *  @var pCHAR ...Contains the level of error (fatal, recoverable...)
 *  @var pLABEL ...Contains the string label of error
 */
typedef struct{
	char * pCHAR;/**< FATAL ERROR */
    /** \brief Callback invoked upon loss of connection */
    	char * pLABEL;
	int16_t sTHR;/**< ljdljldjljd */
	int16_t sCNT=0;// to count the number of usage of string unuseful !!!!
} s_CSTR;
#define DEF_CSTRSIZE 3072
#define DEF_LABELSIZE 1400
/**
 * @class Langage
 * @brief Manages multi langage message strings  for information and error exceptions
 *
 * Each message is a char* vs format for printf
 * sLvl count throw error from which the error becomes fatal
 * sCNT count number of invocations of this message
 *
 */
class Langage {
private:
	std::vector<s_CSTR> vSTRs;
	char buffer_CSTR[DEF_CSTRSIZE];
	char buffer_LABEL[DEF_LABELSIZE];
public:
	void listDir_Old(fs::FS &fs, const char * dirname, uint8_t levels);
	String listDir(fs::FS &fs, const char *dirname, uint8_t levels = 0);
	/**
	 * \fn void store(const int ERRCOD)
	 * \brief Sets \ref ok to ERRCOD
	 * \param ERRCOD thrown error code
	 * */
	bool Begin(const String FILE_LANGUE) ;
	const char * CST(const e_STR ENUM);
	const char * LABEL(const e_STR ENUM);
	int16_t thr(const e_STR ENUM);
	int16_t cnt(const e_STR ENUM);
	void inc(const e_STR ENUM);
};
inline const char * Langage::CST(const e_STR ENUM){
	return vSTRs[ENUM].pCHAR;
}
inline const char * Langage::LABEL(const e_STR ENUM){
	return vSTRs[ENUM].pLABEL;
}
inline int16_t Langage::thr(const e_STR ENUM){
	return vSTRs[ENUM].sTHR;
}
inline int16_t Langage::cnt(const e_STR ENUM){
	return vSTRs[ENUM].sCNT;
}
inline void Langage::inc(const e_STR ENUM) {
	vSTRs[ENUM].sCNT++;
}
///////////////////////////////////////////////////////////////////////
extern Langage Lang;
#endif //__000_FSTR__
