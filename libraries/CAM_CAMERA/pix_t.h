/** @file */
#pragma once
#include "Arduino.h"
///////////////////////////////////////////////////////////////////////
/**
 * @brief printf pattern for 16 bits binary output
 */
#define BIN16_PAT "{%c%c%c%c-%c%c%c%c|%c%c%c%c-%c%c%c%c}"
/**
 * @fn #define BIN16(word)
 * @brief Generates output to printf a word in binary format pattern
 */
#define BIN16(word)  \
	(word & 0x8000 ? '1' : 'o'), \
	(word & 0x4000 ? '1' : 'o'), \
	(word & 0x2000 ? '1' : 'o'), \
	(word & 0x1000 ? '1' : 'o'), \
	(word & 0x0800 ? '1' : 'o'), \
	(word & 0x0400 ? '1' : 'o'), \
	(word & 0x0200 ? '1' : 'o'), \
	(word & 0x0100 ? '1' : 'o'), \
	(word & 0x0080 ? '1' : 'o'), \
	(word & 0x0040 ? '1' : 'o'), \
	(word & 0x0020 ? '1' : 'o'), \
	(word & 0x0010 ? '1' : 'o'), \
	(word & 0x0008 ? '1' : 'o'), \
	(word & 0x0004 ? '1' : 'o'), \
	(word & 0x0002 ? '1' : 'o'), \
	(word & 0x0001 ? '1' : 'o')
/**
 * @fn #define BIN8(byte)
 * @brief Generates output to printf a byte in binary format pattern
 */
#define BIN8(byte)  \
	(byte & 0x80 ? '1' : 'o'), \
	(byte & 0x40 ? '1' : 'o'), \
	(byte & 0x20 ? '1' : 'o'), \
	(byte & 0x10 ? '1' : 'o'), \
	(byte & 0x08 ? '1' : 'o'), \
	(byte & 0x04 ? '1' : 'o'), \
	(byte & 0x02 ? '1' : 'o'), \
	(byte & 0x01 ? '1' : 'o')
//---------------------------------------------------------------------
/**
 * @fn String buf_Info(const uint8_t *BUFFER, const size_t COUNT, const char *LABEL = nullptr)
 * @brief Dumps COUNT bytes of a BUFFER prefixed with BUFNAM
 * @param BUFFER to dump
 * @param COUNT number of bytes to dump (truncated to 16 max)
 * @param LABEL prefixed label
 * @return String
 */
String buf_Info(const uint8_t *BUFFER, const size_t COUNT, const char *LABEL = nullptr);
///////////////////////////////////////////////////////////////////////
#include "pix_t-MONO8.h"
#include "pix_t-RGB555.h"
#include "pix_t-RGB565.h"
#include "pix_t-RGB888.h"
/*
 inline uint8_t swapBits(
 const uint8_t b){
 uint8_t x = 0;
 x |= (b & 0b00000001) << 7;
 x |= (b & 0b00000010) << 5;
 x |= (b & 0b00000100) << 3;
 x |= (b & 0b00001000) << 1;
 x |= (b & 0b00010000) >> 1;
 x |= (b & 0b00100000) >> 3;
 x |= (b & 0b01000000) >> 5;
 x |= (b & 0b10000000) >> 7;
 return x;
 }
 */
/**
 * @fn inline RGB565_t rgb565(const RGB555_t &a)
 * @brief Converts a RGB555_t pixel to RGB565_t format
 * @param a RGB555_t encoded pixel
 * @return RGB565_t encoded pixel
 */
inline RGB565_t rgb565(const RGB555_t &a){
	return RGB565_t(a.red(), a.green(), a.blue());
}
/**
 * @fn inline rgb555_t rgb555(const RGB565_t_t &a)
 * @brief Converts a RGB565_t pixel to RGB555_t format
 * @param a RGB565_t encoded pixel
 * @return RGB555_t_t encoded pixel
 */
inline RGB555_t rgb555(const RGB565_t &a){
	return RGB555_t(a.red(), a.green(), a.blue());
}
//#define DEF_TEST_PIXL_T
void TEST_MONO8();
void TEST_RGB565();
void TEST_RGB888();
void TEST_RGBcast();
///////////////////////////////////////////////////////////////////////
