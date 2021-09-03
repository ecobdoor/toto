#ifndef __000_GFONT__
#define __000_GFONT__
///////////////////////////////////////////////////////////////////////
#include "pix_t.h"
#include "CAM_GFONT.inc"
#include "stdarg.h"
#include "stdio.h"
///////////////////////////////////////////////////////////////////////
template<typename PIXL>
class c_gTerminal {
private:
	uint16_t _fWidth;
	uint16_t _fHeight;
	PIXL *_buffer;
	uint16_t _col = 2;
	uint32_t _row = 2;
	PIXL _color = PIXL(0);
	bool _shadow = false;

public:
	c_gTerminal(const uint16_t sourceWidth, const uint16_t sourceHeight, void *buffer) :
		_fWidth(sourceWidth), _fHeight(sourceHeight), _buffer((PIXL*)buffer){
	}
	void setTo(const uint16_t COL, const uint16_t ROW, const PIXL COLOR = 0, const bool SHADOW =
		false){
		_col = COL;
		_row = (uint32_t)ROW * _fWidth;
		_color = COLOR;
		_shadow = SHADOW;
	}
	void display(const char CAR);
	void gprintf(const char *FORMAT, ...);
};
//---------------------------------------------------------------------
template<typename PIXL>
void c_gTerminal<PIXL>::gprintf(const char *FORMAT, ...){
	char buffer[64];
	va_list args;
	va_start(args, FORMAT);
	vsnprintf(buffer, 64, FORMAT, args);
	va_end(args);
//	Serial.printf("\n[%s]", buffer);
	for (size_t i = 0; i < strlen(buffer); i++) {
		display(buffer[i] - 32);
//		Serial.printf("%c:%i:%i)", buffer[i], buffer[i], buffer[i] - 32);
	}
}
//#define GDBG
//---------------------------------------------------------------------
template<typename PIXL>
void c_gTerminal<PIXL>::display(const char CAR){
	if (DEF_FN <= CAR)
		return display(DEF_FN - 1);
	const uint8_t (&car)[DEF_FH] = gFont[CAR];
	uint8_t Wfont = car[0];
	if (_shadow) {
		uint32_t savRow = _row;
		uint16_t savCol = _col;
		for (uint8_t r = 1; r < DEF_FH; r++) {
			for (uint8_t c = 0; c < Wfont + 2; c++) {
				*(PIXL*)&_buffer[_row + _col] = *(PIXL*)&_buffer[_row + _col] / uint8_t{2};
				_col++;
			}
			_col = savCol;
			_row += _fWidth;
		}
		_row = savRow;
		_col = savCol;
	}
	/*
	 Serial.printf("\n==========%i",Wfont);
	 for (size_t i = 1; i < 12; i++)
	 Serial.printf("\n'"BYTE_TO_BINARY_PATTERN"'", BYTE_TO_BINARY(car[i]));
	 */
#ifdef GDBG
		Serial.printf("\n'01234567'%i", Wfont);
#endif
	uint32_t savRow = _row;
	uint16_t savCol = _col;
	for (uint8_t r = 1; r < DEF_FH; r++) {
		uint8_t linebits = car[r];
#ifdef GDBG
			Serial.printf("\n'"BYTE_TO_BINARY_PATTERN"'", BYTE_TO_BINARY(linebits));
#endif
		if (linebits) {
			for (uint8_t c = 0; c < Wfont; c++) {
				if (linebits & 0x80) {
#ifdef GDBG
						Serial.printf("%i", c);
#endif
					//if (_invert) *(PIXL*)&_buffer[_row + _col] =~*(PIXL*)&_buffer[_row + _col];	else
					*(PIXL*)&_buffer[_row + _col] = _color;
				}
				linebits <<= 1;
				_col++;
			}
			_col = savCol;
		}
		_row += _fWidth;
	}
#ifdef GDBG
		Serial.printf("\n==========");
#endif
	_row = savRow;
	_col = savCol + (Wfont + 2);
}
///////////////////////////////////////////////////////////////////////
#endif
