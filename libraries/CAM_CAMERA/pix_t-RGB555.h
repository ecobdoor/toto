/** @file */
#ifndef __pix_RGB555__
#define __pix_RGB555__
#include "esp32_GOODIES.h"
///////////////////////////////////////////////////////////////////////
/**
 * @brief printf a binary x:bbbbb:gg|ggg:rrrrr RBG555 16 bits pixel
 */
#define B555_PAT "{%c-%c%c%c%c%c:%c%c%c%c%c:%c%c%c%c%c}"
/**
 * @brief printf a hexadecimal <= 1~1F:1F:1F max RBG555 16 bits pixel
 */
#define H555_PAT "[%c~%02X:%02X:%02X]"
#define H555(word)  \
  ( word & 0b1000000000000000)?'1':'0', \
  ( word & 0b0111110000000000)>> 10, \
  ( word & 0b0000001111100000)>>  5, \
  ( word & 0b0000000000011111)
///////////////////////////////////////////////////////////////////////
class RGB565_t;
// @formatter:off
/**
 * class RGB555_t
 * @brief class RGB555_t is based on a single uint16_t identical to the value of the image frame
 *
 * The arithmetical operators threshold values between 0 and the max (x:1 red:1F green:1F blue:1F)
 * * * uint8_t product is available from righ color valuet side or left side
 * * / uint8_t division runs on left side only (zero division is implemented causing a runtime error)
 * * Pointers operations are available: RGB555_t *Pxlptr = (RGB555_t *)&buffer[...];
 *
 * class RGB555_t is part of the pxl_t <FORMAT> model auomatically switched according to pixels encoding
 * template<typename FORMAT> FORMAT BpP € [1:MONO8_t, 2:RGB555_t | RGB565_t, 3:RGB888_t].
 * So you can write:
 * * *pxl_t<FORMAT> *Pxlptr = (pxl_t<FORMAT>*)&buffer[...
 * * *Pxlptr = Pxlptr->gray(); // monochrome in RGB555 format ie r=g=b
 * * *Pxlptr = 2 * pxl_t<FORMAT>(Pxlptr->blue()); keep amplified blue channel
 * * *Pxlptr ^= *Pxlptr; binary operations: & | ^ (and unary ~)
 * * *(uint16_t)&buffer[...] = Pxlptr->bin();
 * * *Pxlptr++ (of course...);
 * * Pay attention:
 *  * RGB555_t is a class, it returns an address! Cast or exotic constructors are forbidden.
 *  * Don't forget this type runs on little endian representation only
 *  * Converting RGB565 to RGB555, the lower green bit is preserved in unused bit 15 of RGB555
 * * Members functions:
 *  * uint16_t bin() returns the coded RGB555 value of the pixel
 *  * uint8_t red(), green(), blue() return a uint16_t value directly writable in a frame buffer
 *  * uint8_t norm() of pixel is formatted on [0...255] equivalent to a monochrome pixel value
 *  * RGB555_t gray() ensures that the rgb channels are the most significant bits of norm() each
 *  *void sum(uint32_t & SUM) allows to sum the RGB555::norm() up to 16 mega pixels (4096x4096)
 * Others criteria could be developed
 */
class RGB555_t {
private:
	/**
	 * @brief 2 bytes unsigned word representing the RGB555 encoded pixel color value
	 */
	uint16_t pix;
public:
	RGB555_t() {}
	inline RGB555_t(const uint16_t& a) {pix=a;}
	inline RGB555_t(const RGB555_t& a) {pix=a.pix;}
	inline RGB555_t(const uint8_t& R,const uint8_t& G,const uint8_t& B) {format(R,G,B);}
	friend class RGB565_t;
	/**
	 * @brief FORBIDDEN default compiler casting from RGB565_t to uint16 can't run because bits are not in the same order!!!.
	 * Cause error message. ESP goes in deep sleep mode to avoid it
	 */
	inline RGB555_t(const RGB565_t& a) {halt("RGB555_t(RGB565_t) forbidden, use rgb555(...)!");}
	inline RGB555_t gray()  const {uint8_t n=norm();return RGB555_t(n,n,n);}
	inline uint16_t bin()   const {return   pix;}
	inline uint8_t  red()   const {return  (pix & 0b0111110000000000)>>7;}
	inline uint8_t  green() const {return ((pix & 0b0000001111100000)>>2)|((pix & 0x8000)?0b100:0);}
	inline uint8_t  blue()  const {return  (pix & 0b0000000000011111)<<3;}
	inline void format(const uint8_t& R,const uint8_t& G,const uint8_t& B) {// THIS IS RIGHT !!!! for BMP
		pix=(((uint16_t)R&0b11111000)<<7)|(((uint16_t)G&0b11111000)<<2)|((B&0b11111000)>>3)|(((uint16_t)G&0b100)<<13);}
	inline void sum(uint32_t & SUM) const {SUM+=norm();}
	inline uint8_t norm() const {
		uint8_t b =blue()/3;b+=b?3:0;
		uint8_t g =green()/3;g+=g?1:0;
		uint8_t r =red()/3;r+=r?3:0; // to be in[0...255]
		return b+g+r;
	}
	inline void _plus(const RGB555_t cst){
		int16_t r=(int16_t)red()+cst.red();if(255<r)r=255;
		int16_t g=(int16_t)green()+cst.green();if(255<g)g=255;
		int16_t b=(int16_t)blue()+cst.blue();if(255<b)b=255;
		format(r,g,b);
	}
	inline void _minus(const RGB555_t cst){
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
	String dump(const char *COMMENT=nullptr, const uint8_t *BUFFER = nullptr, const uint8_t BUFLEN = 8)const {
		uint16_t cnt=0;
		char buffer[128];
		cnt+=sprintf(&buffer[cnt] ,"%s",COMMENT ?COMMENT:"");
		cnt+=sprintf(&buffer[cnt] ," 555:hx%04x ║%3i║ "H555_PAT""B555_PAT"",pix,norm(), H555(pix), BIN16(pix));
		cnt+=sprintf(&buffer[cnt] ,"%s",BUFFER?(buf_Info(BUFFER, BUFLEN).c_str()):"");
		return String(buffer);
	}
	inline operator uint16_t() {return pix;}
	inline operator uint16_t() const {return pix;}
	/*
	 inline RGB555_t operator++() {++val; val&=0x00FFFFFF; return *this;}
	 inline RGB555_t operator--() {--val; val&=0x00FFFFFF; return *this;}
	 inline RGB555_t operator++(int) {RGB555_t temp(val); ++val; val&=0x00FFFFFF; return temp;}
	 inline RGB555_t operator--(int) {RGB555_t temp(val); --val; val&=0x00FFFFFF; return temp;}
	 inline RGB555_t operator&(const RGB555_t& a) {return RGB555_t(val&a.val);}
	 inline RGB555_t& operator=(const RGB555_t& a) { val=a.val; return *this; }
	 inline RGB555_t& operator+=(const RGB555_t& a) { val+=a.val; val&=0x00FFFFFF; return *this; }
	 inline RGB555_t& operator-=(const RGB555_t& a) { val-=a.val; val&=0x00FFFFFF; return *this; }
	 */
	inline bool operator== ( const RGB555_t& right ) const {return pix==right.pix;}
	inline bool operator!= ( const RGB555_t& right ) const {return pix!=right.pix;}
	inline bool operator > ( const RGB555_t& right ) const {return norm()>right.norm();}
	inline bool operator < ( const RGB555_t& right ) const {return norm()<right.norm();}
	inline const RGB555_t operator+( const RGB555_t &other ) const {RGB555_t tmp(*this);tmp._plus(other); return tmp;}
	inline const RGB555_t operator-( const RGB555_t &other ) const {RGB555_t tmp(*this);tmp._minus(other); return tmp;}
	inline const RGB555_t operator*( const uint8_t &right ) const {RGB555_t tmp(*this);tmp._prod(right); return tmp;}
	inline const RGB555_t operator/( const uint8_t &right ) const {RGB555_t tmp(*this);tmp._div(right); return tmp;}
	inline RGB555_t operator~ () {pix=~pix; return *this;}
	inline RGB555_t operator& (const RGB555_t &other) {RGB555_t tmp(*this);return other.pix & tmp.pix;}
	inline RGB555_t operator| (const RGB555_t &other) {RGB555_t tmp(*this);return other.pix | tmp.pix;}
	inline RGB555_t operator^ (const RGB555_t &other) {RGB555_t tmp(*this);return other.pix ^ tmp.pix;}
	inline RGB555_t operator&=(const RGB555_t &other) {pix &=other.pix;return *this;}
	inline RGB555_t operator|=(const RGB555_t &other) {pix |=other.pix;return *this;}
	inline RGB555_t operator^=(const RGB555_t &other) {pix ^=other.pix;return *this;}
	/*, the bit 15 of RGB555 is restore in the lower green bit
	inline RGB555_t& operator=(const uint32_t& a) { _32to24(a,pix); return *this; }
	inline RGB555_t& operator+=(const uint32_t& a) { val+=a; val&=0x00FFFFFF; return *this; }
	inline RGB555_t& operator-=(const uint32_t& a) { val-=a; val&=0x00FFFFFF; return *this; }
	*/
	inline bool operator==( const uint16_t& right ) const {return pix==right;}
	inline bool operator!=( const uint16_t& right ) const {return pix!=right;}
	inline bool operator >( const uint16_t& right ) const {return norm()>right;}
	inline bool operator <( const uint16_t& right ) const {return norm()<right;}
	/*
	 inline const RGB555_t operator+( const uint32_t &other ) const { return RGB555_t(val+other); }
	 inline const RGB555_t operator-( const uint32_t &other ) const { return RGB555_t(val-other); }
	 inline const RGB555_t operator/( const uint32_t &other ) const { return RGB555_t(val/other); }
	 inline const RGB555_t operator*( const uint32_t &other ) const { return RGB555_t(val*other); }
	 */
};
// @formatter:on
//---------------------------------------------------------------------
/**
 * Non-members function for left operators
 */
inline RGB555_t operator*(
	const uint8_t &left, const RGB555_t &other){
	RGB555_t tmp = other;
	tmp._prod(left);
	return tmp;
}
inline RGB555_t operator/(const uint8_t &left, const RGB555_t &other){
	RGB555_t tmp = other;
	halt("Left divide forbidden!");
	return tmp;
}
/*
 * also run but faster with pb of cross casting between class or incomplete or nto much nested include...
	inline RGB555_t(const RGB565_t& a) {// Convert 565 to 555 for BMP (preserve g0 in x)
		uint16_t C = a.pix;
		uint16_t r = (C & 0b0000000011111000) << 7;
		uint16_t g =((C & 0b1100000000000000) >> 9)|((C & 0b0000000000000111) << 7);
		uint16_t x = (C & 0b0010000000000000) ? 0x8000 : 0;
		uint16_t b = (C & 0b0001111100000000) >> 8;
		pix=x | b | g | r;
	}
*/
///////////////////////////////////////////////////////////////////////
#endif

