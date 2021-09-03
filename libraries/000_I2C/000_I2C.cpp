#include "000_I2C.h"
#include "esp32_DEBUG.h"
#include "Arduino.h"
//#include "Wire.h"
// simple function to scan for I2C devices on the bus
uint8_t I2Cscan() 
{
  // scan for i2c devices
  uint8_t error, address;
  int nDevices;

  _SERIAL_0("\n\tBus scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
      

    if (error == 0)    {
      _SERIAL_0("\n\tI2C device found at address 0x");
      if (address<16) 
        _SERIAL_0("0");
      _SERIAL_0("%X  !",address);
      nDevices++;
    }
    else if (error==4)     {
      _SERIAL_0("\n\tUnknown error at address 0x");
      if (address<16) 
        _SERIAL_0("0");
      _SERIAL_0("%X  !",address);
    }    
  }
  if (nDevices == 0)
    _SERIAL_0("\n\tNo I2C devices found");
  else
    _SERIAL_0("\n\tdone");
  return nDevices;
}


// I2C read/write functions for the MPU9250 sensors

  void writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
  Wire.beginTransmission(address);  // Initialize the Tx buffer
  Wire.write(subAddress);           // Put slave register address in Tx buffer
  Wire.write(data);                 // Put data in Tx buffer
  Wire.endTransmission();           // Send the Tx buffer
}

  uint8_t readByte(uint8_t address, uint8_t subAddress)
{
  uint8_t data = 0;                        // `data` will store the register data   
  Wire.beginTransmission(address);         // Initialize the Tx buffer
  Wire.write(subAddress);                  // Put slave register address in Tx buffer
  Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
  Wire.requestFrom(address, 1);            // Read two bytes from slave register address on MPU9250 
  data = Wire.read();                      // Fill Rx buffer with result
  return data;                             // Return data read from slave register
}

  void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest)
{  
  Wire.beginTransmission(address);   // Initialize the Tx buffer
  Wire.write(subAddress);            // Put slave register address in Tx buffer
  Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
  uint8_t i = 0;
  Wire.requestFrom(address, count);  // Read bytes from slave register address 
  while (Wire.available()) {
        dest[i++] = Wire.read(); }         // Put read results in the Rx buffer
}
  uint16_t read2BytesHL(uint8_t address, uint8_t subAddress)
{
  uint16_t data = 0;                        // `data` will store the register data   
  Wire.beginTransmission(address);         // Initialize the Tx buffer
  Wire.write(subAddress);                  // Put slave register address in Tx buffer
  Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
  Wire.requestFrom(address, 2);            // Read two bytes from slave register address on MPU9250 
  data = ((uint16_t)Wire.read())<< 8;      // Fill Rx data high byte
  data |= Wire.read();                   // Fill Rx data low byte
  return data;                             // Return data read from slave register
}
