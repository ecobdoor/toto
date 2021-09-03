/** @file */
#ifndef __pix_MONO8__
#define __pix_MONO8__
#include "esp32_GOODIES.h"
///////////////////////////////////////////////////////////////////////
/**
 * @brief printf pattern 8 bits binary output vs MONO8_t
 */
#define BIN44_PAT "{%c%c%c%c-%c%c%c%c}"
///////////////////////////////////////////////////////////////////////
/**
 * @class MONO8_t
 * @brief class  MONO8_t is based on a single uint8_t, pointer++ runs on frame buffer!!!
 *
 * The arithmetical operators threshold values between 0 FF
 * * * uint8_t product is available from right side or left side
 * * / uint8_t division runs on left side only (zero division is implemented causing a runtime error)
 * * Pointers operations are available: MONO8_t *Pxlptr = (MONO8_t *)&buffer[...];
 *
 * class MONO8_t is part of the pxl_t templates automatically switched according to pixels length & encoding.
 * template<typename FORMAT> FORMAT BpP € [1:MONO8_t, 2:RGB555_t | RGB565_t, 3:RGB888_t].
 * So you can write:
 *  * *pxl_t<FORMAT> *Pxlptr = (pxl_t<FORMAT>*)&buffer[...]
 *  * *Pxlptr = Pxlptr->gray(); don't change the value...
 *  * *Pxlptr = 2 * *Pxlptr; to amplify the light
 *  * *Pxlptr ^= *Pxlptr; binary operations: & | ^ (unary ~ also operates)
 *  * *(uint8_t)&buffer[...] = Pxlptr->bin();
 *  * *Pxlptr++ (of course...);
 * * Pay attention:
 *  * MONO8_t is a class, it returns an address! Cast or exotic constructors are forbidden.
 *  * Converting MONO8_t to RGB888, RGB555 or RGB565 is not implemented yet
 * * Members functions:
 *  * uint8_t bin() returns the byte value of the pixel
 *  * for compatibility with color templates, uint8_t red(), green(), blue() are supported & returns the same uint8_t value directly writable in a byte frame buffer
 *  * uint8_t norm() returns the pixel value € [0...255]
 *  * MONO8_t gray() returns the pixel value € [0...255];
 *  * void sum(uint32_t & SUM) allows to sum the RGB888::norm() up to 16 mega pixels (4096x4096)
 * Others criteria could be developed
 */
class mono8_t {
protected:
	uint8_t pix = 0;
};
// @formatter:off
class  MONO8_t : public mono8_t {
private:
	/**
	 * @brief 1 unsigned byte representing a gray value (or a palette address)
	 */
	inline void _plus (const MONO8_t cst){int16_t PIX=(int16_t)pix+cst.pix;pix=(255<PIX)?255:PIX;}
	inline void _minus(const MONO8_t cst){int16_t PIX=(int16_t)pix-cst.pix;pix=(PIX<0)?0:PIX;}

public:
	inline void _prod (const uint8_t cst){int16_t PIX=(int16_t)pix*cst;pix=(255<PIX)?255:PIX;}
	inline void _div  (const uint8_t cst){pix=pix/cst;}
	String dump(const char *COMMENT=nullptr, const uint8_t *BUFFER = nullptr, const uint8_t BUFLEN = 8)const{
		uint16_t cnt=0;
		char buffer[128];
		cnt+=sprintf(&buffer[cnt],"%s",COMMENT    ?COMMENT:"");
		cnt+=sprintf(&buffer[cnt]," MONO:h_%02x ║%3i║ [%02X]" BIN44_PAT "",pix,norm(),pix,BIN8(pix));
		cnt+=sprintf(&buffer[cnt],"%s",BUFFER?(buf_Info(BUFFER,BUFLEN).c_str()):"");
		return String(buffer);
	}
	inline uint8_t bin()   const {return pix;}
	inline uint8_t red()   const {return pix;}
	inline uint8_t green() const {return pix;}
	inline uint8_t blue()  const {return pix;}
	inline MONO8_t gray()  const {return *this;}
	inline uint8_t norm() const {return pix;}
	inline void sum(uint32_t & SUM) const {SUM+=pix;}
	MONO8_t() {}
	inline MONO8_t(const MONO8_t& a) {pix=a.pix;}
//	inline MONO8_t(const uint8_t& a) {pix=a;}
//	inline MONO8_t(const int& a) {pix=a;}
//	inline MONO8_t(const uint16_t& a) {pix=a;}
//	inline MONO8_t(const int& a,const int& b=0,const int& c=0) {pix=a;}
	inline MONO8_t(const uint8_t& a,const uint8_t& b=0,const uint8_t& c=0) {pix=a;}
	// Because uint32_t() is forbidden to avoid uint32_t(RGB888_t) or *(uint32_t*)&buffer[...] = RGB888_t(...)
	// These 2 below are excluded also.  USE .bin() method;  ???
	inline operator uint8_t() { return pix; }
	inline operator uint8_t() const { return pix; }
/*
	inline MONO8_t operator++() {++val; val&=0x00FFFFFF; return *this;}
	inline MONO8_t operator--() {--val; val&=0x00FFFFFF; return *this;}
	inline MONO8_t operator++(int) {MONO8_t temp(val); ++val; val&=0x00FFFFFF; return temp;}
	inline MONO8_t operator--(int) {MONO8_t temp(val); --val; val&=0x00FFFFFF; return temp;}
	inline MONO8_t operator&(const MONO8_t& a) {return MONO8_t(val&a.val);}
	inline MONO8_t& operator=(const MONO8_t& a) { val=a.val; return *this; }
	inline MONO8_t& operator+=(const MONO8_t& a) { val+=a.val; val&=0x00FFFFFF; return *this; }
	inline MONO8_t& operator-=(const MONO8_t& a) { val-=a.val; val&=0x00FFFFFF; return *this; }
*/
	inline bool operator== ( const MONO8_t& right ) const {return pix==right.pix;}
	inline bool operator!= ( const MONO8_t& right ) const {return pix!=right.pix;}
	inline bool operator > ( const MONO8_t& right ) const {return pix>right.pix;}
	inline bool operator < ( const MONO8_t& right ) const {return pix<right.pix;}
	inline const MONO8_t operator+( const MONO8_t &other ) const {MONO8_t tmp(*this);tmp._plus(other); return tmp;}
	inline const MONO8_t operator-( const MONO8_t &other ) const {MONO8_t tmp(*this);tmp._minus(other); return tmp;}
	inline const MONO8_t operator*( const uint8_t &right ) const {MONO8_t tmp(*this);tmp._prod(right); return tmp; }
	inline const MONO8_t operator/( const uint8_t &right ) const {MONO8_t tmp(*this);tmp._div(right); return tmp; }
	inline MONO8_t operator& (const MONO8_t &other) {MONO8_t tmp(*this);tmp.pix &=other.pix; return tmp; }

//	inline RGB888_t operator& (const RGB888_t &other) {RGB888_t tmp(*this);_and(other.pix,tmp.pix); return tmp;}
//	inline RGB565_t operator& (const RGB565_t &other) {RGB565_t tmp(*this);return other.pix & tmp.pix;}



	inline MONO8_t operator| (const MONO8_t &other) {MONO8_t tmp(*this);tmp.pix |=other.pix; return tmp; }
	inline MONO8_t operator^ (const MONO8_t &other) {MONO8_t tmp(*this);tmp.pix ^=other.pix; return tmp; }
	inline MONO8_t operator&=(const MONO8_t &other) {pix &=other.pix;return *this;}
	inline MONO8_t operator|=(const MONO8_t &other) {pix |=other.pix;return *this;}
	inline MONO8_t operator^=(const MONO8_t &other) {pix ^=other.pix;return *this;}
/*
*/
//	inline MONO8_t& operator=(const uint32_t& a) { _32to24(a,pix); return *this; }
/*
	inline MONO8_t& operator+=(const uint32_t& a) { val+=a; val&=0x00FFFFFF; return *this; }
	inline MONO8_t& operator-=(const uint32_t& a) { val-=a; val&=0x00FFFFFF; return *this; }
*/
	inline bool operator==( const uint8_t& right ) const {return pix==right;}
	inline bool operator!=( const uint8_t& right ) const {return pix!=right;}
	inline bool operator >( const uint8_t& right ) const {return pix>right;}
	inline bool operator <( const uint8_t& right ) const {return pix<right;}
/*
	inline const MONO8_t operator+( const uint32_t &other ) const { return MONO8_t(val+other); }
	inline const MONO8_t operator-( const uint32_t &other ) const { return MONO8_t(val-other); }
	inline const MONO8_t operator/( const uint32_t &other ) const { return MONO8_t(val/other); }
	inline const MONO8_t operator*( const uint32_t &other ) const { return MONO8_t(val*other); }
*/
};
// @formatter:on
/**
 * Non-members function for left operators
 */
inline MONO8_t operator*(const uint8_t &left, const MONO8_t &other){
	MONO8_t tmp = other;
	tmp._prod(left);
	return tmp;
}
inline MONO8_t operator/(const uint8_t &left, const MONO8_t &other){
	MONO8_t tmp = other;
	tmp._div(0); // to make a running error !!!!
	return tmp;
}
///////////////////////////////////////////////////////////////////////
#endif
