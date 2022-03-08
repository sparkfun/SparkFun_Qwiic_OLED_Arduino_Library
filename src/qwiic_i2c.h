//
//  TODO : HEADER
// 


// Header for I2C driver object


#pragma once 



// Simple object to encapsulate basic I2C operations.
//
// This is following a pattern for future implementations
//
// This class is focused on Aurduino..

#include <Wire.h>



class QwI2C {


public: 

	QwI2C(void);  

	bool init(void);
	bool init(TwoWire &wirePort);

	// see if a device exists
	bool ping(uint8_t address);


	bool writeRegisterByte(uint8_t address, uint8_t offset, uint8_t data);

	// Write a block of bytes to the device -- 
	int writeRegisterRegion(uint8_t address, uint8_t offset, uint8_t *data, uint16_t length); 


private:

	TwoWire  *_i2cPort;

};