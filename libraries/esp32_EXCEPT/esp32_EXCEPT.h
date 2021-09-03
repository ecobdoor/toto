#ifndef __esp32_EXCEPT__
#define __esp32_EXCEPT__
/** @file */
#include "Arduino.h"
#include "esp32_SYS_basic.h"
#include <esp_heap_caps.h>
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
	e_STR errCod() const; // !!! const because of c++ passing 'const EXCEPT' as 'this' argument discards qualifiers
};
inline e_STR EXCEPT::errCod() const{
	return _scratch->m_number;
}
Scratch* rtnThrow(const e_STR STR_COD, const char *FUNCTION, const int LINE, const char *FILE, ...);
//void Throw(const e_STR STR_COD ,const  char * FUNCTION,const int LINE,const  char * FILE,...);
/**
 * @fn ThrowErr(TaskHandle_t *TSKHDL, const char *FUNCTION, const int LINE, const char *FILE, const e_STR STR_COD, ...)
 * @brief Creates a new @ref Scratch & throw @ref EXCEPT
 *
 * @param TSKHDL Task (or function) handler
 * @param FUNCTION where throw was called
 * @param LINE where throw was called
 * @param FILE where throw was called
 * @param STR_COD string code
 */
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
/**
 * @fn #define THROWERR(STR_COD,...)
 * @brief call @ref ThrowErr (...)
 *
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
#define FIX_ERROR() \
	do{ \
		if(!On_Error(e.errCod())) { \
			Serial.printf("\nFIX_ERROR %s %i %s ",__FUNCTION__, __LINE__,__FILE__); \
			e.fixException(__FUNCTION__, __LINE__,__FILE__); \
		} \
	}while(0)
/**
 * @fn char* allocBuf(const size_t SIZE, int16_t *CNT = nullptr, const char *FUNCTION = __FUNCTION__);
 * @brief Allocateds a buffer on heap & set CNT buffer pointer to 0 (if defined)
 *
 * For MALLOC_CAP_8BIT, MALLOC_CAP_32BIT, MALLOC_CAP_DMA,MALLOC_CAP_IRAM_8BIT... CF.
 * https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/mem_alloc.html
 *
 * @param SIZE of buffer to be allocated
 * @param CNT index in buffer to be allocated. If (CNT) set to 0 else unchanged
 * @param FUNCTION calling function
 * @return buffer address or nullptr if no place in heap
 */
char* allocBuf(const size_t SIZE, int16_t *CNT = nullptr);
inline void freeBuf(char *BUFFER){
	free((void*)BUFFER);
	BUFFER = nullptr;
}
#endif //__000__EXCEPT__
///////////////////////////////////////////////////////////////////////
