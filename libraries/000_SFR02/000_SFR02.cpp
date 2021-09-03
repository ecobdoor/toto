/* 06/16/2020 Copyright Scadis
 *  
 *  Created by Eric COsnard
 *  
 Demonstrate basic SFR02 functionality including parameterizing the register addresses, initializing the sensor, 
 
 Library may be used freely and without limit with attribution.
 
*/

#include "000_SFR02.h"
#include "000_I2C.h"

SFR02::SFR02(){
}


int16_t SFR02::readRangeCM(uint8_t MPUnum)
{
   return read2BytesHL(SFR02_ADDRESS, REG_2);
}

  int16_t SFR02::readTune(uint8_t MPUnum)
{  
   return 0;
 }

