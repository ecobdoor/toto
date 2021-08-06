#include <math.h>
//#include <CAM_IMAGE.h> // NO the .inc is included at the end  .h file
///////////////////////////////////////////////////////////////////////
template<typename PIXL>
c_Image<PIXL>::c_Image(const uint16_t sourceWidth, const uint16_t sourceHeight, void *buffer) :
	_fWidth(sourceWidth),
		_fHeight(sourceHeight){
	if (buffer) {
		_buffer = (PIXL*)buffer;
		_local = false;
	} else {
		_buffer = new PIXL[(uint32_t)_fWidth * _fHeight];
		_local = true;
	}
	if (sizeof(PIXL) == 1) {
		_palette[0] = PIXL(0); // black
		_palette[1] = PIXL(80); // dark gray
		_palette[2] = PIXL(120); // middle gray
		_palette[3] = PIXL(170); // middle gray
		_palette[4] = PIXL(220); // light gray
		_palette[5] = PIXL(255); // white
	} else if (sizeof(PIXL) == 2) {
		_palette[0] = PIXL(0xff, 0x00, 0xff); // indigo
		_palette[1] = PIXL(0x00, 0x00, 0xff); // blue
		_palette[2] = PIXL(0x00, 0xff, 0x00); // green
		_palette[3] = PIXL(0xff, 0xff, 0x00); // yellow
		_palette[4] = PIXL(0x00, 0xff, 0xff); // orange
		_palette[5] = PIXL(0xff, 0x00, 0x00); // red
		/*
		 _palette[0] = PIXL(0x94, 0x14, 0xcb); // indigo
		 _palette[1] = PIXL(0x00, 0x00, 0xff); // blue
		 _palette[2] = PIXL(0x00, 0xdd, 0x00); // green
		 _palette[3] = PIXL(0xee, 0xee, 0x00); // yellow
		 _palette[4] = PIXL(0xff, 0x9e, 0x00); // orange
		 _palette[5] = PIXL(0xff, 0x11, 0x11); // red

		 */
	} else if (sizeof(PIXL) == 3) {
		_palette[0] = PIXL(0x9414cb); // indigo
		_palette[1] = PIXL(0x0000ff); // blue
		_palette[2] = PIXL(0x00dd00); // green
		_palette[3] = PIXL(0xeeee00); // yellow
		_palette[4] = PIXL(0xff9e00); // orange
		_palette[5] = PIXL(0xff1111); // red
	} else
		halt("Palet problem with <typename PIXL>?");
}
//---------------------------------------------------------------------
template<typename PIXL>
inline PIXL c_Image<PIXL>::palette(const uint8_t i){
	return _palette[i];
}
//---------------------------------------------------------------------
template<typename PIXL>
inline PIXL* c_Image<PIXL>::pixl(const uint16_t X, const uint16_t Y){
	return _buffer + (uint32_t)Y * _fWidth + X;
}
//---------------------------------------------------------------------
template<typename PIXL>
inline void c_Image<PIXL>::drawPixel(const int16_t X, const int16_t Y, const PIXL COLOR){
	*pixl(X, Y) = COLOR;
}
//---------------------------------------------------------------------
template<typename PIXL>
inline void c_Image<PIXL>::drawPixel(PIXL *PIXEL, const uint8_t r, const uint8_t g,
	const uint8_t b){
//	Serial.printf("\n%s ", (*PIXEL).dump("*PX").c_str());
	PIXL pixel = PIXL(r, g, b);
	*PIXEL = pixel;
//	Serial.printf(" to rgb(%3i,%3i,%3i) %s", r, g, b, pixel.dump("px").c_str());
//	RGB555_t z = RGB555_t(pixel);
//	Serial.printf(" => %s", z.dump().c_str());
}
//---------------------------------------------------------------------
template<typename PIXL>
void c_Image<PIXL>::pattern(){
	uint16_t row = 50;
	uint16_t col = 100;
	uint16_t colStep = 3;
	uint16_t rowStep = 4;
	for (size_t r = 0; r < (256 / rowStep); r++) {
		PIXL *pp = (PIXL*)&_buffer[width() * (r + row) + col];
		for (size_t c = 0; c < 256; c += colStep)
			drawPixel(pp++, 255 - r * rowStep, c, c);
	}
	col += 256 / colStep;
	for (size_t r = 0; r < (256 / rowStep); r++) {
		PIXL *pp = (PIXL*)&_buffer[width() * (r + row) + col];
		for (size_t c = 0; c < 256; c += colStep)
			drawPixel(pp++, c, 255 - r * rowStep, c);
	}
	col += 256 / colStep;
	for (size_t r = 0; r < (256 / rowStep); r++) {
		PIXL *pp = (PIXL*)&_buffer[width() * (r + row) + col];
		for (size_t c = 0; c < 256; c += colStep)
			drawPixel(pp++, c, c, 255 - r * rowStep);
	}
	col += 256 / colStep;
	for (size_t r = 0; r < (256 / rowStep); r++) {
		PIXL *pp = (PIXL*)&_buffer[width() * (r + row) + col];
		for (size_t c = 0; c < 256; c += colStep)
			drawPixel(pp++, c, c, c);
	}
}
//---------------------------------------------------------------------
template<typename PIXL>
uint8_t c_Image<PIXL>::Decode(uint8_t *grid, const uint8_t gridStep, const bool marker){
//	printStackLeft(__FUNCTION__);
	uint16_t index = 0;
	uint32_t sumBrightness = 0;
	const uint16_t blockArea = (uint16_t)gridStep * gridStep; //up to  65536=255²
	for (uint16_t y = 0; y <= (int16_t)_fHeight - gridStep; y += gridStep) {
		uint16_t gridRow = y / gridStep;
		for (uint16_t x = 0; x <= (int16_t)_fWidth - gridStep; x += gridStep) {
			uint16_t gridCol = x / gridStep;
			uint32_t RGB = 0;
			for (uint8_t j = 0; j < gridStep; j++) {
				PIXL *Pxlptr = pixl(x, y + j); // buffer[x] x€[0..._fWidth*_fHeight]from 00
				for (uint8_t i = 0; i < gridStep; i++) {
					Pxlptr->sum(sumBrightness);
					Pxlptr->sum(RGB);
					if (marker) {
						// Draw points on down right zone corner
						if ((i == gridStep - 1) && (j == gridStep - 1)) {
							*Pxlptr = PIXL(0, 0, 0);
						}
						/*
						 // Draw points on 4 zone corners
						 if (((j == 0) && ((i == 0) || (i == gridStep - 1)))	|| ((i == 0) && ((j == 0) || (j == gridStep - 1)))) {
						 *Pxlptr = PIXL(0, 0, 0);
						 }
						 // Draw square around zone
						 if ((j == 0) || (j == gridStep - 1) || (i == 0) || (i == gridStep - 1)) {
						 *Pxlptr = PIXL(0, 0, 0);
						 }
						 */
						if (gridCol == 1) {
							if (gridRow == 1)
								*Pxlptr = PIXL(Pxlptr->red(), 0, 0);
							if (gridRow == 2)
								*Pxlptr = PIXL(0, Pxlptr->green(), 0);
							if (gridRow == 3)
								*Pxlptr = PIXL(0, 0, Pxlptr->blue());
						} else if (gridCol == 2) {
							if (gridRow == 1)
								*Pxlptr = 2 * PIXL(Pxlptr->red(), 0, 0);
							if (gridRow == 2)
								*Pxlptr = 2 * PIXL(0, Pxlptr->green(), 0);
							if (gridRow == 3)
								*Pxlptr = PIXL(0, 0, Pxlptr->blue()) * 2;
						} else if (gridCol == 3) {
							if (gridRow == 1)
								*Pxlptr = Pxlptr->gray();
							if (gridRow == 2)
								*Pxlptr |= 2 * *Pxlptr;
							if (gridRow == 3)
								*Pxlptr ^= *Pxlptr;
						}
					}
					Pxlptr++;
				}
			}
			grid[index++] = RGB / blockArea;
		}
	}
	return sumBrightness / _fHeight / _fWidth;
}
//---------------------------------------------------------------------
template<typename PIXL>
void c_Image<PIXL>::drawRectangle(const uint16_t X, const uint16_t Y,
	const uint16_t W, const uint16_t H, const PIXL COLOR){
	drawHorizontalLine(X, Y, W, COLOR);
	drawHorizontalLine(X, Y + H - 1, W, COLOR);
	drawVerticalLine(X, Y + 1, H - 2, COLOR);
	drawVerticalLine(X + W - 1, Y + 1, H - 2, COLOR);
}
//---------------------------------------------------------------------
template<typename PIXL>
inline void c_Image<PIXL>::drawVerticalLine(const uint16_t X, const uint16_t Y,
	const uint16_t Len, const PIXL COLOR){
	PIXL *Pxlptr = pixl(X, Y);
	for (size_t row = 0; row < Len; row++) {
		*Pxlptr = COLOR;
		Pxlptr += _fWidth;
	}
}
//---------------------------------------------------------------------
template<typename PIXL>
inline void c_Image<PIXL>::drawHorizontalLine(const uint16_t X, const uint16_t Y,
	const uint16_t Len, const PIXL COLOR){
	PIXL *Pxlptr = pixl(X, Y);
	for (size_t col = 0; col < Len; col++)
		*Pxlptr++ = COLOR;
}
//---------------------------------------------------------------------
template<typename PIXL>
inline void c_Image<PIXL>::fillRectangle(const uint16_t X, const uint16_t Y,
	const uint16_t W, const uint16_t H, const PIXL COLOR){
	//Serial.printf("\n%s", COLOR.dump().c_str());
	for (size_t col = 0; col < H; col++)
		drawHorizontalLine(X, Y + col, W, COLOR);
}
//---------------------------------------------------------------------
template<typename PIXL>
inline void c_Image<PIXL>::markRectangle(const uint8_t delta,
	const uint16_t X, const uint16_t Y, const uint8_t Xwidth, const uint8_t Yheight){
// idx € [0...255] idx/51 € [0...5]
	PIXL color;
	if (delta <= 255)
		color = _palette[delta / 51];
	else
		color = _palette[255 / 51];
	drawRectangle(X, Y, Xwidth, Yheight, color);
}
//---------------------------------------------------------------------
template<typename PIXL>
inline void c_Image<PIXL>::markCross(const uint8_t delta,
	const uint16_t X, const uint16_t Y, const uint8_t Xwidth, const uint8_t Yheight){
	;
// idx € [0...255] idx/51 € [0...5]
	PIXL color;
	if (delta <= 255)
		color = _palette[delta / 51];
	else
		color = _palette[255 / 51];
	drawLine(X, Y, X + Xwidth - 1, Y + Yheight - 1, color);
	drawLine(X, Y + Yheight - 1, X + Xwidth - 1, Y, color);
}
//---------------------------------------------------------------------
template<typename PIXL>
void c_Image<PIXL>::drawCircle(const uint16_t X, const uint16_t Y, const uint16_t R,
	const PIXL COLOR, const float A0, const float A1){
	float start;
	float end;
	if (A0 < A1) {
		start = A0;
		end = A1;
	} else {
		start = A1;
		end = A0;
	}
	float dA = 1.0 / R; // smallest angle from a pixel to the next
	float A = start;
	while (A <= end) {
		//Serial.printf("%10.3f",A);
		drawPixel(X + R * cos(A), Y + R * sin(A), COLOR);
		A += dA;
	}
}
//---------------------------------------------------------------------
#ifndef _swap_int16_t
#define _swap_int16_t(a, b){ \
	int16_t t = a; \
	a = b; \
	b = t; \
}
#endif

template<typename PIXL>
void c_Image<PIXL>::drawLine(const int16_t XA, const int16_t YA, const int16_t XB,
	const int16_t YB,
	const PIXL COLOR){
	int16_t x0 = XA;
	int16_t y0 = YA;
	int16_t x1 = XB;
	int16_t y1 = YB;
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		_swap_int16_t(x0, y0);
		_swap_int16_t(x1, y1);
	}
	if (x0 > x1) {
		_swap_int16_t(x0, x1);
		_swap_int16_t(y0, y1);
	}
	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);
	int16_t err = dx / 2;
	int16_t ystep;
	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}
	for (; x0 <= x1; x0++) {
		if (steep) {
			drawPixel(y0, x0, COLOR);
		} else {
			drawPixel(x0, y0, COLOR);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}
///////////////////////////////////////////////////////////////////////
