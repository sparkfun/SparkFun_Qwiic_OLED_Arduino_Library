
//
//  TODO : HEADER
// 

#include <Arduino.h>
#include "qwiic_i2c.h"


// What is the max buffer size for this platform.

#if defined(SERIAL_BUFFER_SIZE)
#define kMaxTransferBuffer SERIAL_BUFFER_SIZE

#elif defined(I2C_BUFFER_LENGTH)
#define kMaxTransferBuffer I2C_BUFFER_LENGTH

#elif defined(BUFFER_LENGTH)
#define kMaxTransferBuffer BUFFER_LENGTH

#else  // just the standard Arduino value
#define kMaxTransferBuffer 32   

#endif


const static 	uint16_t kChunkSize = kMaxTransferBuffer - 1;

//////////////////////////////////////////////////////////////////////////////////////////////////
// Constructor

QwI2C::QwI2C(void){

	_i2cPort = nullptr;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
bool QwI2C::init(TwoWire &wirePort){

    // if we don't have a wire port already
    if(!_i2cPort){
    	_i2cPort = &wirePort;

    	_i2cPort->begin();
    }

    return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
bool QwI2C::init(void){

    // do we already have a wire port?
    if(!_i2cPort)
    	return init(Wire); // no wire, send in Wire

    return true;

}

//////////////////////////////////////////////////////////////////////////////////////////////////
bool QwI2C::ping(uint8_t i2c_address){

	_i2cPort->beginTransmission(i2c_address);
	return _i2cPort->endTransmission() == 0;	
}

//////////////////////////////////////////////////////////////////////////////////////////////////
bool QwI2C::writeRegisterByte(uint8_t i2c_address, uint8_t offset, uint8_t dataToWrite){

	_i2cPort->beginTransmission(i2c_address);
	_i2cPort->write(offset);
	_i2cPort->write(dataToWrite);
	return _i2cPort->endTransmission() == 0;	

}
//////////////////////////////////////////////////////////////////////////////////////////////////
// writeRegisterRegion()
//
//

int QwI2C::writeRegisterRegion(uint8_t i2c_address, uint8_t offset, uint8_t *data, uint16_t length){

    uint16_t nSent;
    uint16_t nRemaining=length;
    uint16_t nToWrite;

    while(nRemaining > 0){

        _i2cPort->beginTransmission(i2c_address);
        _i2cPort->write(offset);

        nToWrite = (nRemaining > kChunkSize ? kChunkSize : nRemaining);
        nSent = _i2cPort->write(data, nToWrite);

        nRemaining -= nToWrite;        // Note - use nToWrite, not nSent, or lock on esp32
        data += nSent; // move up to remaining data in buffer

        // only release bus if we've sent all data
        if( _i2cPort->endTransmission( nRemaining <= 0 ) )
            return -1; // the client didn't ACK
    }

    return length - nRemaining;
}