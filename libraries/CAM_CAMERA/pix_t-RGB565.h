/** @file */
#ifndef __pix_RGB565__
#define __pix_RGB565__
#include "esp32_GOODIES.h"
///////////////////////////////////////////////////////////////////////
/**
 * @brief printf a binary rrrrr:ggg|ggg:bbbbb RBG565 16 bits pixel
 */
#define B565_PAT "{%c%c%c:%c%c%c%c%c:%c%c%c%c%c:%c%c%c}"
/**
 * @brief printf a hexadecimal <= 1F:3F:1F max RBG565 16 bits pixel
 */
#define H565_PAT "[%02X:%02X:%02X]"
#define H565(word)  \
  ( word & 0b0000000011111000)>> 3, \
  ((word & 0b0000000000000111)<<  3)|((word & 0b1110000000000000)>>  13), \
  ( word & 0b0001111100000000)>> 8

///////////////////////////////////////////////////////////////////////
class RGB555_t;
/**
 * @class  RGB565_t
 * @brief class RGB565_t is based on a single uint16_t identical to the value of the image frame
 *
 * The arithmetical operators threshold values between 0 and the max (red:1F green:3F blue:1F)
 * * * uint8_t product is available from right side or left side
 * * / uint8_t division runs on left side only (zero division is implemented causing a runtime error)
 * * Pointers operations are available: RGB565_t *Pxlptr = (RGB565_t *)&buffer[...];
 *
 * class RGB565_t is part of the pxl_t templates automatically switched according to pixels length & encoding
 * * template <typename FORMAT> FORMAT BpP € [1:RGB888_t, 2:RGB555_t | RGB565_t, 1:MONO8_t].
 * So you can write:
 *  * *pxl_t<FORMAT> *Pxlptr = (pxl_t<FORMAT>*)&buffer[...]
 *  * *Pxlptr = Pxlptr->gray(); // monochrome in RGB565  format ie r=g=b
 *  * *Pxlptr = 2 * pxl_t<FORMAT>(Pxlptr->blue()); keep amplified blue channel
 *  * *Pxlptr ^= *Pxlptr; binary operations: & | ^ (and unary ~)
 *  * *(uint16_t)&buffer[...] = Pxlptr->bin();
 *  * *Pxlptr++ (of course...);
 * * Pay attention: color value
 *  * RGB565_t is a class, it returns an address! Cast or exotic constructors are forbidden.
 *  For example use inline function \ref rgb565 to convert pixels from a BMP file.
 *  * Don't forget this type runs on little endian model only
 *  * Converting RGB555 to RGB565, the bit 15 of RGB555 is restore in the lower green bit
 * * Members functions:
 *  * uint16_t bin() returns the coded RGB565 value of the pixel
 *  * uint8_t red(), green(), blue() return a uint8_t value directly writable in a byte frame buffer
 *  * uint8_t norm() of pixel is formatted on [0...255] as Manhattan distance equivalent to a monochrome pixel value
 *  * RGB565_t gray() is defined as RGB565_t(norm,norm,norm), lowest bits are losted;
 *  * void sum(uint32_t & SUM) allows to sum the RGB565::norm() up to 16 mega pixels (4096x4096).
 * Others criteria could be developed.
 */
// @formatter:off
class  RGB565_t {
private:
	/**
	 * @brief 2 bytes unsigned word representing the RGB565 encoded pixel color value
	 */
	uint16_t pix;
public:
	RGB565_t() {}
	inline RGB565_t(const uint16_t& a) {pix=a;}
	inline RGB565_t(const RGB565_t& a) {pix=a.pix;}
	inline RGB565_t(const uint8_t& r,const uint8_t& g,const uint8_t& b) {format(r,g,b);}
	friend RGB555_t;
	/**
	 * @brief FORBIDDEN default compiler casting from RGB565_t to uint16 can't run because bits are not in the same order!!!.
	 * Cause error message. ESP goes in deep sleep mode to avoid it
	 */
	inline RGB565_t(const RGB555_t& a) {halt("RGB565_t(RGB555_t) forbidden, use rgb565(...)!");}
	inline RGB565_t gray()  const {uint8_t n=norm();return RGB565_t(n,n,n);}
	inline uint16_t bin()   const {return  pix;}
	inline uint8_t  red()   const {return  (pix & 0b0000000011111000);}
	inline uint8_t  green() const {return ((pix & 0b1110000000000000)>>11)|((pix & 0b111)<<5);}
	inline uint8_t  blue()  const {return  (pix & 0b0001111100000000)>>5;}
	inline void format(const uint8_t& r,const uint8_t& g,const uint8_t& b) {// THIS IS RIGHT !!!! for ESP
		pix =(r&0b11111000)|(((uint16_t)g&0b11100)<<11)|(g>>5)|(((uint16_t)b&0b11111000)<<5);}
	inline void sum(uint32_t & SUM) const {SUM+=norm();}
	inline uint8_t norm() const { // if 0, adds 3|1 to be in [0...3*85]
		uint8_t r = red()/3;r+=r?3:0;
		uint8_t g = green()/3;g+=g?1:0;
		uint8_t b = blue()/3;b+=b?3:0;
		return r+g+b;
	}
	inline void _plus(const RGB565_t cst){
		int16_t r=(int16_t)red()+cst.red();if(255<r)r=255;
		int16_t g=(int16_t)green()+cst.green();if(255<g)g=255;
		int16_t b=(int16_t)blue()+cst.blue();if(255<b)b=255;
		format(r,g,b);
	}
	inline void _minus(const RGB565_t cst){
		int16_t r=(int16_t)red()-cst.red();if(r<0)r=0;
		int16_t g=(int16_t)green()-cst.green();if(g<0)g=0;
		int16_t b=(int16_t)blue()-cst.blue();if(b<0)b=0;
		format(r,g,b);
	}
	inline void _prod(const uint8_t cst){
		int16_t r=red()*cst;if(255<r)r=255;
		int16_t g=green()*cst;if(255<g)g=255;
		int16_t b=blue()*cst;if(255<b)b=255;
		format(r,g,b);
	}
	inline void _div(const uint8_t cst){
		format(red()/cst,green()/cst,blue()/cst);
	}
	String dump(const char *COMMENT=nullptr, const uint8_t *BUFFER=nullptr,	const uint8_t BUFLEN=8)const{
		uint16_t cnt = 0;
		char buffer[256];
		cnt += sprintf(&buffer[cnt], "%s", COMMENT ? COMMENT : "");
		cnt += sprintf(&buffer[cnt], " 565:h_%04x ║%3i║ " H565_PAT "" B565_PAT "",pix,norm(), H565(pix), BIN16(pix));
		cnt += sprintf(&buffer[cnt], "%s", BUFFER ? (buf_Info(BUFFER, BUFLEN).c_str()) : "");
		return String(buffer);
	}
	inline operator uint16_t() {return pix;}
	inline operator uint16_t() const {return pix;}
	/*
	 inline RGB565_t operator++() {++val; val&=0x00FFFFFF; return *this;}
	 inline RGB565_t operator--() {--val; val&=0x00FFFFFF; return *this;}
	 inline RGB565_t operator++(int) {RGB565_t temp(val); ++val; val&=0x00FFFFFF; return temp;}
	 inline RGB565_t operator--(int) {RGB565_t temp(val); --val; val&=0x00FFFFFF; return temp;}
	 inline RGB565_t operator&(const RGB565_t& a) {return RGB565_t(val&a.val);}
	 inline RGB565_t& operator=(const RGB565_t& a) { val=a.val; return *this; }
	 inline RGB565_t& operator+=(const RGB565_t& a) { val+=a.val; val&=0x00FFFFFF; return *this; }
	 inline RGB565_t& operator-=(const RGB565_t& a) { val-=a.val; val&=0x00FFFFFF; return *this; }
	 */
	inline bool operator== ( const RGB565_t& right ) const {return pix==right.pix;}
	inline bool operator!= ( const RGB565_t& right ) const {return pix!=right.pix;}
	inline bool operator > ( const RGB565_t& right ) const {return norm()>right.norm();}
	inline bool operator < ( const RGB565_t& right ) const {return norm()<right.norm();}
	inline const RGB565_t operator+( const RGB565_t &other ) const {RGB565_t tmp(*this);tmp._plus(other); return tmp;}
	inline const RGB565_t operator-( const RGB565_t &other ) const {RGB565_t tmp(*this);tmp._minus(other); return tmp;}
	inline const RGB565_t operator*( const uint8_t &right ) const {RGB565_t tmp(*this);tmp._prod(right); return tmp;}
	inline const RGB565_t operator/( const uint8_t &right ) const {RGB565_t tmp(*this);tmp._div(right); return tmp;}
	inline RGB565_t operator~ () {pix=~pix; return *this;}
	inline RGB565_t operator& (const RGB565_t &other) {RGB565_t tmp(*this);return other.pix & tmp.pix;}
	inline RGB565_t operator| (const RGB565_t &other) {RGB565_t tmp(*this);return other.pix | tmp.pix;}
	inline RGB565_t operator^ (const RGB565_t &other) {RGB565_t tmp(*this);return other.pix ^ tmp.pix;}
	inline RGB565_t operator&=(const RGB565_t &other) {pix &=other.pix;return *this;}
	inline RGB565_t operator|=(const RGB565_t &other) {pix |=other.pix;return *this;}
	inline RGB565_t operator^=(const RGB565_t &other) {pix ^=other.pix;return *this;}
	/*
	inline RGB565_t& operator=(const uint32_t& a) { _32to24(a,pix); return *this; }
	inline RGB565_t& operator+=(const uint32_t& a) { val+=a; val&=0x00FFFFFF; return *this; }
	inline RGB565_t& operator-=(const uint32_t& a) { val-=a; val&=0x00FFFFFF; return *this; }
	 */
	inline bool operator==( const uint16_t& right ) const {return pix==right;}
	inline bool operator!=( const uint16_t& right ) const {return pix!=right;}
	inline bool operator >( const uint16_t& right ) const {return norm()>right;}
	inline bool operator <( const uint16_t& right ) const {return norm()<right;}
	/*
	 inline const RGB565_t operator+( const uint32_t &other ) const { return RGB565_t(val+other); }
	 inline const RGB565_t operator-( const uint32_t &other ) const { return RGB565_t(val-other); }
	 inline const RGB565_t operator/( const uint32_t &other ) const { return RGB565_t(val/other); }
	 inline const RGB565_t operator*( const uint32_t &other ) const { return RGB565_t(val*other); }
	 */
};
// @formatter:on
//---------------------------------------------------------------------
/**
 * Non-members function for left operators
 */
inline RGB565_t operator*(const uint8_t &left, const RGB565_t &other){
	RGB565_t tmp = other;
	tmp._prod(left);
	return tmp;
}
inline RGB565_t operator/(const uint8_t &left, const RGB565_t &other){
	RGB565_t tmp = other;
	halt("Left divide forbidden!");
	return tmp;
}
///////////////////////////////////////////////////////////////////////
#endif
