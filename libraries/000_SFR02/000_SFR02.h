/* 06/16/2020 Copyright Scadis
 *  
 *  Created by Eric COsnard
 *  
 Demonstrate basic SFR02 functionality including parameterizing the register addresses, initializing the sensor, 
 
 Library may be used freely and without limit with attribution.
 
*/
  
#ifndef __000_SFR02__
#define __000_SFR02__

#include "Arduino.h"

// See also MPU-9250 Register Map and Descriptions, Revision 4.0, RM-MPU-9250A-00, Rev. 1.4, 9/9/2013 for registers not listed in 
// above document; the MPU9250 and MPU9150 are virtually identical but the latter has a different register map
//
//Magnetometer Registers
#define SFR02_ADDRESS     0x70 //0xE0 // from 0xE0 to 0xFE (step 2 ... I²C on 7 bits)
#define CMD_0             0x00 // Write Command register / Read  Software revision or 0xFF while ranging (useful)
#define REG_1             0x01 //       Unused returns 0x80
#define REG_2             0x02 // Read only  Range high byte
#define REG_3             0x03 // Read only  Range low byte
#define REG_4             0x04 // Read only  Auto tune Minimum high byte
#define REG_5             0x05 // Read only  Auto tune Minimum low byte

#define REAL_INCH         0x50 // Real ranging mode (inches)
#define REAL_CM           0x51 // Real ranging mode (centimeters)
#define REAL_MICS         0x52 // Real ranging mode (microseconds)

#define FAKE_INCH         0x56 // Fake ranging mode (inches)
#define FAKE_CM           0x57 // Fake ranging mode (centimeters)
#define FAKE_MICS         0x58 // Fake ranging mode (microseconds)

#define BURST_8           0x5C // Transmit 8 cycle 40kHz burst (no ranging)

#define AUTO_TUNE         0x60 // Force autotune retart as power-up

#define ADRS_1            0xA0 // 1 st cmd to change address
#define ADRS_3            0xA5 // 3 rd cmd to change address
#define ADRS_2            0xAA // 2 nd cmd to change address




class SFR02{
  public: 
  SFR02();
  int16_t readRangeCM(uint8_t MPUnum);
  int16_t readTune(uint8_t MPUnum);
  void Burst();
};

#endif //__000_SFR02__
