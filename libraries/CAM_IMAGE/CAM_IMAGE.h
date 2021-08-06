/** @file */
#pragma once
#ifndef __000_IMAGE__
#define __000_IMAGE__
#include "pix_t.h"
///////////////////////////////////////////////////////////////////////

/**
 *
 * @tparam PIXL
 */
template<typename PIXL>
/**
 * @class c_Image
 * @brief Includes to frame pixels buffer drawing utilities
 * cf. Adafruit_GFX.h pour utilitaires graphiques
 */
class c_Image {
private:
	uint16_t _fWidth;
	uint16_t _fHeight;
	bool _local;
	/*static*/
	PIXL _palette[6];

public:
	PIXL *_buffer;
	uint16_t width(){
		return _fWidth;
	}
	uint16_t height(){
		return _fHeight;
	}
	/**
	 * @fn c_Image(const uint16_t sourceWidth, const uint16_t sourceHeight, void *buffer = nullptr)
	 * @brief Constructor. If buffer is NULL, creates a local public buffer on the heap (deleted by ~c_Image)
	 *
	 * Also a color palette of 6 values is created to display graphic criteria from 0 to 255 stepped 51
	 * @param sourceWidth in pixels units
	 * @param sourceHeight in pixels units
	 * @param buffer in <typename PIXL> units
	 */
	c_Image(const uint16_t sourceWidth, const uint16_t sourceHeight, void *buffer = nullptr);
	~c_Image(){
		if (_local)
			delete[] _buffer;
	}
	/**
	 * @fn PIXL* pixl(const uint16_t X, const uint16_t Y);
	 * @brief Gives the frame pointer of a (X,Y) pixel
	 * @param X
	 * @param Y
	 * @return buffer address of pixel
	 */
	PIXL* pixl(const uint16_t X, const uint16_t Y);
	/**
	 * @fn void drawPixel(const int16_t X, const int16_t Y, const PIXL COLOR)
	 * @brief Modifies pixel(X,Y)
	 * @param X  pixel abscisssa
	 * @param Y  pixel ordinate
	 * @param COLOR to draw
	 */
	void drawPixel(const int16_t X, const int16_t Y, const PIXL COLOR);
	/**
	 * @fn drawPixel(PIXL *PIXEL, const uint8_t r, const uint8_t g, const uint8_t b)
	 * @brief Modifies a pixel
	 * @param PIXEL target to be modified
	 * @param r red component
	 * @param g green component
	 * @param b blue component
	 */
	void drawPixel(PIXL *PIXEL, const uint8_t r, const uint8_t g, const uint8_t b);
	/**
	 * @fn void drawVerticalLine(const uint16_t X, const uint16_t Y, const uint16_t Len, const PIXL COLOR = PIXL(0xFF, 0, 0))
	 * @brief Draws a vertical line
	 * @param X start pixel abscisssa
	 * @param Y start pixel ordinate
	 * @param Len length of line
	 * @param COLOR to draw
	 */
	void drawVerticalLine(const uint16_t X, const uint16_t Y, const uint16_t Len,
		const PIXL COLOR = PIXL(0xFF, 0, 0));
	/**
	 * @fn drawHorizontalLine(const uint16_t X, const uint16_t Y, const uint16_t Len, const PIXL COLOR = PIXL(0xFF, 0, 0))
	 * @brief Draws a horizontal line
	 * @param X start pixel abscisssa
	 * @param Y start pixel ordinate
	 * @param Len length of line
	 * @param COLOR to draw
	 */
	void drawHorizontalLine(const uint16_t X, const uint16_t Y, const uint16_t Len,
		const PIXL COLOR = PIXL(0xFF, 0, 0));
	/**
	 * @fn drawRectangle(const uint16_t X, const uint16_t Y, const uint16_t W, const uint16_t H,
	 const PIXL COLOR = PIXL(0xFF, 0, 0))
	 * @brief Draws a rectangle
	 * @param X pixel abscisssa upper left
	 * @param Y pixel ordinate upper left
	 * @param W rectangle width
	 * @param H rectangle height
	 * @param COLOR to draw
	 */
	void drawRectangle(const uint16_t X, const uint16_t Y, const uint16_t W, const uint16_t H,
		const PIXL COLOR = PIXL(0xFF, 0, 0));
	/**
	 * @fn void drawLine(const int16_t XA, const int16_t YA, const int16_t XB, const int16_t YB,const PIXL COLOR)
	 * @brief Draws a line
	 * @param XA start pixel abscisssa
	 * @param YA start pixel ordinate
	 * @param XB end pixel abscisssa
	 * @param YB end pixel ordinate
	 * @param COLOR to draw
	 */
	void drawLine(const int16_t XA, const int16_t YA, const int16_t XB, const int16_t YB,
		const PIXL COLOR);
	/**
	 * @fn void drawCircle(const uint16_t X, const uint16_t Y, const uint16_t R,
	 const PIXL COLOR = PIXL(0xFF, 0, 0), const float A0 = 0, const float A1 = 2 * PI)
	 * @brief Draws a arc of circle
	 * @param X pixel abscisssa center
	 * @param Y pixel ordinate center
	 * @param R circle ray
	 * @param COLOR to draw
	 * @param A0 start angle
	 * @param A1 end angle
	 */
	void drawCircle(const uint16_t X, const uint16_t Y, const uint16_t R,
		const PIXL COLOR = PIXL(0xFF, 0, 0), const float A0 = 0, const float A1 = 2 * PI);
	/**
	 * @fn fillRectangle(const uint16_t X, const uint16_t Y, const uint16_t W, const uint16_t H,
	 const PIXL COLOR = PIXL(0xFF, 0, 0))
	 * @brief Draws a filled rectangle
	 * @param X pixel abscisssa upper left
	 * @param Y pixel ordinate upper left
	 * @param W rectangle width
	 * @param H rectangle height
	 * @param COLOR to fill
	 */
	void fillRectangle(const uint16_t X, const uint16_t Y, const uint16_t W, const uint16_t H,
		const PIXL COLOR = PIXL(0xFF, 0, 0));
	/**
	 * @fn void markRectangle(const uint8_t delta, const uint16_t X, const uint16_t Y,
	 const uint8_t Xwidth, const uint8_t Yheight);
	 * @param delta € [0...255] defines the color palette stepped 51
	 * @param X pixel abscisssa upper left
	 * @param Y pixel ordinate upper left
	 * @param Xwidth rectangle width
	 * @param Yheight rectangle height
	 */
	void markRectangle(const uint8_t delta, const uint16_t X, const uint16_t Y,
		const uint8_t Xwidth, const uint8_t Yheight);
	/**
	 * @fn void markCross(const uint8_t delta, const uint16_t X, const uint16_t Y,
	 const uint8_t Xwidth, const uint8_t Yheight);
	 * @param delta € [0...255] defines the color palette stepped 51
	 * @param X pixel abscisssa upper left
	 * @param Y pixel ordinate upper left
	 * @param Xwidth cross width
	 * @param Yheight cross height
	 */
	void markCross(const uint8_t delta, const uint16_t X, const uint16_t Y, const uint8_t Xwidth,
		const uint8_t Yheight);
	/**
	 * @fn
	 * @brief Gives the color matching i index € [0..5]
	 * @param i palette index
	 * @return the needed color
	 */
	PIXL palette(const uint8_t i);
	/**
	 * @fn void Decode( uint8_t *grid, const uint8_t gridStep)
	 * @brief Downsamples the image in a grid array according to a norm criterion
	 * @param grid must be large enough to support the requested step in relation to the size of the image
	 * @param gridStep pixels downsampling step
	 * @param marker to draw pixels grid corners in the frame for debug (slows down jpeg compression)
	 * @return image medium brightness
	 */
	uint8_t Decode(uint8_t *grid, const uint8_t gridStep, const bool marker);
	/**
	 * @fn void pattern()
	 * @brief Generates a pattern rgb, composite & gray colors
	 */
	void pattern();
};
///////////////////////////////////////////////////////////////////////
#include "CAM_IMAGE.inc"
#endif
