/** @file */
#ifndef __pix_RGB888__
#define __pix_RGB888__
#include "esp32_GOODIES.h"
///////////////////////////////////////////////////////////////////////
/**
 * @brief printf pattern for 16 bits binary output vs RGB888_t
 */
#define B888_PAT "{%c%c%c%c-%c%c%c%c:%c%c%c%c-%c%c%c%c:%c%c%c%c-%c%c%c%c}"
/**
 * @brief printf pattern for 16 bits hexadecimal output vs RGB888_t <= FF:FF:FF
 */
#define H888_PAT "[%02X:%02X:%02X]"

///////////////////////////////////////////////////////////////////////
// @formatter:off
/**
 * @class RGB888_t
 * @brief class  RGB888_t is based on an array of 3 uint8_t. So pointer++ runs on frame buffer!!!
 *
 * The arithmetical operators threshold values between 0 and the max (ie red:FF green:FF blue:FF)
 * * * uint8_t product is available from right side or left side
 * / uint8_t division runs on left side only (zero division is implemented causing a runtime error)
 * * Pointers operations are available: RGB888_t *Pxlptr = (RGB888_t *)&buffer[...];
 *
 * class RGB888_t is part of the pxl_t templates automatically switched according to pixels length & encoding.
 * template<typename FORMAT> FORMAT BpP € [1:MONO8_t, 2:RGB555_t | RGB565_t, 3:RGB888_t].
 * So you can write:
 *  * *pxl_t<FORMAT> *Pxlptr = (pxl_t<FORMAT>*)&buffer[...
 *  * *Pxlptr = Pxlptr->gray(); // monochrome in RGB565  format ie r=g=b
 *  * *Pxlptr = 2 * FORMAT(0, Pxlptr->blue(), 0); to keep & amplify the blue channel
 *  * *Pxlptr ^= *Pxlptr; binary operations: & | ^ (unary ~ also operates)
 *  * *(uint32_t)&buffer[...] = Pxlptr->bin(); don't runs (you scratches the next byte in the buffer)
 *  * *Pxlptr++ (of course...);
 * * Pay attention:
 *  * RGB888_t is a class, it returns an address! Cast or exotic constructors are forbidden.
 *  * Don't forget this type runs on little endian representation only
 *  * Converting RGB888_t to RGB555 or RGB565 is not implemented yet
 * * Members functions:
 *  * uint32_t bin() returns the coded RGB565 value of the pixel
 *  * uint8_t red(), green(), blue() return a uint8_t value directly writable in a byte frame buffer
 *  * uint8_t norm() of pixel is (r+g+b)/3 (vs Manhattan distance) on [0...255]
 *  * RGB888_t gray() is defined as RGB888_t(norm,norm,norm);
 *  * void sum(uint32_t & SUM) allows to sum the RGB888::norm() up to 16 mega pixels (4096x4096)
 * Others criteria could be developed
 */
class  RGB888_t {
private:
	/**
	 * @brief RGB 3 bytes array ordered BRG
	 */
	uint8_t pix[3];
	inline void _plus(const RGB888_t cst){
		int16_t r=(int16_t)red()+cst.red();if(255<r)r=255;
		int16_t g=(int16_t)green()+cst.green();if(255<g)g=255;
		int16_t b=(int16_t)blue()+cst.blue();if(255<b)b=255;
		format(r,g,b);
	}
	inline void _minus(const RGB888_t cst){
		int16_t r=(int16_t)red()-cst.red();if(r<0)r=0;
		int16_t g=(int16_t)green()-cst.green();if(g<0)g=0;
		int16_t b=(int16_t)blue()-cst.blue();if(b<0)b=0;
		format(r,g,b);
	}
	inline void _div(const uint8_t cst){
		format(red()/cst,green()/cst,blue()/cst);
	}
	inline void _and(const RGB888_t cst){pix[0]&=cst.pix[0]; pix[1]&=cst.pix[1];pix[2]&=cst.pix[2];}
	inline void _or (const RGB888_t cst){pix[0]|=cst.pix[0]; pix[1]|=cst.pix[1];pix[2]|=cst.pix[2];}
	inline void _xor(const RGB888_t cst){pix[0]^=cst.pix[0]; pix[1]^=cst.pix[1];pix[2]^=cst.pix[2];}
	void format(const uint8_t& r,const uint8_t& g,const uint8_t& b){pix[2]=r;pix[1]=g;pix[0]=b;}
public:
	inline void _prod(const uint8_t cst){
		int16_t r=red()*cst;if(255<r)r=255;
		int16_t g=green()*cst;if(255<g)g=255;
		int16_t b=blue()*cst;if(255<b)b=255;
		format(r,g,b);
	}
	String dump(const char *COMMENT=nullptr, const uint8_t *BUFFER = nullptr, const uint8_t BUFLEN = 8)const{
		uint16_t cnt=0;
		char buffer[256];
		cnt+=sprintf(&buffer[cnt],"%s",COMMENT?COMMENT:"");
		cnt+=sprintf(&buffer[cnt]," 888:h_%02x%02x%02x ║%3i║ "H888_PAT""B888_PAT"",
			pix[2],pix[1],pix[0],norm(),pix[2],pix[1],pix[0],BIN8(pix[2]),BIN8(pix[1]),BIN8(pix[0]));
		cnt+=sprintf(&buffer[cnt],"%s",BUFFER?(buf_Info(BUFFER, BUFLEN).c_str()):"");
		return String(buffer);
	}
	inline uint32_t bin()   const {return ((uint32_t)(pix[2])<<16|(uint16_t)(pix[1])<<8|pix[0]);}
	inline uint8_t red()   const {return pix[2];}
	inline uint8_t green() const {return pix[1];}
	inline uint8_t blue()  const {return pix[0];}
	inline RGB888_t gray()  const {uint8_t n=norm();return RGB888_t(n,n,n);}
	inline uint8_t  norm()  const {return pix[0]/3 + pix[1]/3 + pix[2]/3;}
	inline void     sum(uint32_t & SUM) const {SUM+=norm();}
	RGB888_t() {}
	inline RGB888_t(const RGB888_t& a) {pix[0]=a.pix[0];pix[1]=a.pix[1];pix[2]=a.pix[2];}
	inline RGB888_t(const uint32_t& a) {pix[0]=a&0xFF;pix[1]=(a>>8)&0xFF;pix[2]=(a>>16)&0xFF;}		// this case uint32_t is to directly set a color 0x00RrGgBb
	inline RGB888_t(const uint8_t& r,const uint8_t& g,const uint8_t& b) {format(r,g,b);}
//	inline RGB888_t(const int& r,const int& g,const int& b) {pix[2]=r;pix[1]=g;pix[0]=b;}
	inline operator uint32_t() { return ((uint32_t)(pix[2])<<16|(uint16_t)(pix[1])<<8|pix[0]); } 		//forbidden to avoid uint32_t(RGB888_t) or *(uint32_t*)&buffer[...] = RGB888_t(...)
	inline operator uint32_t() const { return ((uint32_t)(pix[2])<<16|(uint16_t)(pix[1])<<8|pix[0]); }	//forbidden to avoid uint32_t(RGB888_t) or *(uint32_t*)&buffer[...] = RGB888_t(...)
/*
	inline RGB888_t operator++() {++val; val&=0x00FFFFFF; return *this;}
	inline RGB888_t operator--() {--val; val&=0x00FFFFFF; return *this;}
	inline RGB888_t operator++(int) {RGB888_t temp(val); ++val; val&=0x00FFFFFF; return temp;}
	inline RGB888_t operator--(int) {RGB888_t temp(val); --val; val&=0x00FFFFFF; return temp;}
	inline RGB888_t operator&(const RGB888_t& a) {return RGB888_t(val&a.val);}
	inline RGB888_t& operator=(const RGB888_t& a) { val=a.val; return *this; }
	inline RGB888_t& operator+=(const RGB888_t& a) { val+=a.val; val&=0x00FFFFFF; return *this; }
	inline RGB888_t& operator-=(const RGB888_t& a) { val-=a.val; val&=0x00FFFFFF; return *this; }
*/
	inline bool operator==( const RGB888_t& right ) const {return (pix[0]==right.pix[0])&&(pix[1]==right.pix[1])&&(pix[2]==right.pix[2]);}
	inline bool operator!=( const RGB888_t& right ) const {return (pix[0]!=right.pix[0])||(pix[1]!=right.pix[1])||(pix[2]!=right.pix[2]);}
	inline bool operator >( const RGB888_t& right ) const {return norm()>right.norm();}
	inline bool operator <( const RGB888_t& right ) const {return norm()<right.norm();}
	inline const RGB888_t operator+( const RGB888_t &other ) const {RGB888_t tmp(*this);tmp._plus(other)/*_plus888(other.pix,tmp.pix)*/; return tmp;}
	inline const RGB888_t operator-( const RGB888_t &other ) const {RGB888_t tmp(*this);tmp._minus(other); return tmp;}
	inline const RGB888_t operator*( const uint8_t &fact ) const {RGB888_t tmp(*this);tmp._prod(fact); return tmp; }
	inline const RGB888_t operator/( const uint8_t &fact ) const {RGB888_t tmp(*this);tmp._div(fact); return tmp; }
	inline RGB888_t operator~ () {uint8_t * pxl=&pix[0]; *pxl++ =~*pxl; *pxl++ =~*pxl; *pxl =~*pxl;return *this;}
	inline RGB888_t operator& (const RGB888_t &other) {RGB888_t tmp(*this);tmp._and(other); return tmp;}
	inline RGB888_t operator| (const RGB888_t &other) {RGB888_t tmp(*this);tmp._or(other); return tmp;}
	inline RGB888_t operator^ (const RGB888_t &other) {RGB888_t tmp(*this);tmp._xor(other); return tmp;}
	inline RGB888_t operator&=(const RGB888_t &other) {_and(other); return *this;}
	inline RGB888_t operator|=(const RGB888_t &other) { _or(other); return *this;}
	inline RGB888_t operator^=(const RGB888_t &other) {_xor(other); return *this;}
/*
*/
//	inline RGB888_t& operator=(const uint32_t& a) { _32to24(a,pix); return *this; }
/*
	inline RGB888_t& operator+=(const uint32_t& a) { val+=a; val&=0x00FFFFFF; return *this; }
	inline RGB888_t& operator-=(const uint32_t& a) { val-=a; val&=0x00FFFFFF; return *this; }
*/
	inline bool operator==( const uint32_t& right ) const {return ((uint32_t)(pix[2])<<16|(uint16_t)(pix[1])<<8|pix[0])==(right&0x00FFFFFF);}
	inline bool operator!=( const uint32_t& right ) const {return ((uint32_t)(pix[2])<<16|(uint16_t)(pix[1])<<8|pix[0])!=(right&0x00FFFFFF);}
	inline bool operator >( const uint32_t& right ) const {return norm()>((right>>16)&0xFF)+((right>>8)&0xFF)+(right&0xFF);}
	inline bool operator <( const uint32_t& right ) const {return norm()<((right>>16)&0xFF)+((right>>8)&0xFF)+(right&0xFF);}
/*
	inline const RGB888_t operator+( const uint32_t &other ) const { return RGB888_t(val+other); }
	inline const RGB888_t operator-( const uint32_t &other ) const { return RGB888_t(val-other); }
	inline const RGB888_t operator/( const uint32_t &other ) const { return RGB888_t(val/other); }
	inline const RGB888_t operator*( const uint32_t &other ) const { return RGB888_t(val*other); }
*/
};
// @formatter:on
//---------------------------------------------------------------------
/**
 * Non-members function for left operators
 */
inline RGB888_t operator*(const uint8_t &left, const RGB888_t &other){
	RGB888_t tmp = other;
	tmp._prod(left);
	return tmp;
}
inline RGB888_t operator/(const uint8_t &left, const RGB888_t &other){
	RGB888_t tmp = other;
	halt("Left divide forbidden!");
	return tmp;
}
//inline uint32_t(const RGB888_t& a){return 0;}
///////////////////////////////////////////////////////////////////////
#endif
