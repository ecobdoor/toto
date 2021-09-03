 #ifndef __000_I2C__
#define __000_I2C__
/** @file */
#include "Wire.h"
///////////////////////////////////////////////////////////////////////
/*
  void I2Cscan();
  void    writeByte(uint8_t address, uint8_t subAddress, uint8_t data);
  uint8_t readByte(uint8_t address, uint8_t subAddress);
  void    readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest);
*/


  uint8_t I2Cscan();
  void    writeByte(uint8_t address, uint8_t subAddress, uint8_t data);
  uint8_t readByte(uint8_t address, uint8_t subAddress);
  uint16_t read2BytesHL(uint8_t address, uint8_t subAddress);
  void    readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest);
///////////////////////////////////////////////////////////////////////
  #endif //__000_I2C__
///////////////////////////////////////////////////////////////////////
