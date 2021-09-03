#ifndef __esp32_EXCEPT__
#define __esp32_EXCEPT__
/** @file */
#include "esp32_SYS_basic.h"
#include <exception>
///////////////////////////////////////////////////////////////////////
/**
 * \class Scratch: public exception
 * \brief Manages the try{...throw...}catch{...} statements exceptions
 */
class Scratch {
public:
	/**
	 * Error levels mapErrLVL
	 * The documentation block cannot be put after the enum!
	 */
	typedef enum {
		_INFOS, //=0
		_RECOV, //=1
		_FATAL, //>=2
	} erLVL_t;
	e_STR m_number; //Error number
	char buffer[512]; // initialized by constructor !!! Error description
	erLVL_t m_level; //Error level
	Scratch(const e_STR ERRCOD, const char *FUNCTION, const int LINE, const char *FILE,
		va_list *ARGS);
	void fixError(const char *FUNCTION, const int LINE, const char *FILE);
};
class EXCEPT: public exception {
private:
	Scratch *_scratch = nullptr;
	public:
	EXCEPT(Scratch *SCRATCH) throw () :
		_scratch(SCRATCH){
	}
	virtual const char* what() const throw (){
		return (char*)&_scratch->buffer;
	}
	/**
	 * @fn void fixException()const throw ()
	 * @brief Just calls _scratch->@ref fixError() to treat the error
	 */
	void fixException(const char *FUNCTION, const int LINE, const char *FILE) const throw (){
		_scratch->fixError(FUNCTION, LINE, FILE);
	}
	/*
	 erLVL_t getLevel() const throw () {
	 return _scratch->m_level;
	 }
	 e_STR getNumber() const throw () {
	 return _scratch->m_number;
	 }
	 */
	virtual ~EXCEPT() throw (){
	}
};
Scratch* rtnThrow(const e_STR STR_COD, const char *FUNCTION, const int LINE, const char *FILE, ...);
//void Throw(const e_STR STR_COD ,const  char * FUNCTION,const int LINE,const  char * FILE,...);
void ThrowErr(TaskHandle_t *TSKHDL, const char *FUNCTION, const int LINE, const char *FILE,
	const e_STR STR_COD, ...);
/*
 #define THROW(STR_COD,...) \
	do { \
		Throw(STR_COD,__FUNCTION__, __LINE__,__FILE__,## __VA_ARGS__); \
	}while(0)
 //		if(e.getLevel()!=NOTIFY)
 #define THROW(STR_COD,...) \
	do { \
		Throw(STR_COD,__FUNCTION__, __LINE__,__FILE__,## __VA_ARGS__); \
	}while(0)
 */
#define THROWERR(STR_COD,...) \
	do { \
		Serial.printf("\nTHROWERR %s %i %s ",__FUNCTION__, __LINE__,__FILE__); \
		ThrowErr((TaskHandle_t*)xTaskGetCurrentTaskHandle(),__FUNCTION__, __LINE__,__FILE__,STR_COD,## __VA_ARGS__); \
	}while(0)
//		if(e.getLevel()!=NOTIFY)
#define THROW_UP() \
	do{ \
		Serial.printf("\nTHROW_UP %s %i %s ",__FUNCTION__, __LINE__,__FILE__); \
		throw; \
	}while(0)
// Possible to use __PRETTY_FUNCTION__
/**
 * @brief FIX_ERROR(), vs @ref fixException(, )is a macro because it gets __FUNCTION__, __LINE__ & __FILE__ from where
 * the call is done without passing these arguments each time.
 * That gives the source line where the exception is caught
 * Following m_level rover is reset, or error display until cnt==lvl
 *
 */
void FIX_ERRORS();
#define FIX_ERROR() \
	do{ \
		Serial.printf("\nFIX_ERROR %s %i %s ",__FUNCTION__, __LINE__,__FILE__); \
		e.fixException(__FUNCTION__, __LINE__,__FILE__); \
	}while(0)
char* charAlloc(const size_t SIZE);
inline void charFree(char *BUFFER){
	free((void*)BUFFER);
	BUFFER = nullptr;
}
#endif //__000__EXCEPT__
///////////////////////////////////////////////////////////////////////
