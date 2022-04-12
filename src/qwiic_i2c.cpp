// qwiic_i2c.cpp
//
// This is a library written for SparkFun Qwiic OLED boards that use the SSD1306.
//
// SparkFun sells these at its website: www.sparkfun.com
//
// Do you like this library? Help support SparkFun. Buy a board!
//
//   Micro OLED             https://www.sparkfun.com/products/14532
//   Transparent OLED       https://www.sparkfun.com/products/15173
//   "Narrow" OLED          https://www.sparkfun.com/products/17153
//
//
// Written by Kirk Benell @ SparkFun Electronics, March 2022
//
// This library configures and draws graphics to OLED boards that use the
// SSD1306 display hardware. The library only supports I2C.
//
// Repository:
//     https://github.com/sparkfun/SparkFun_Qwiic_OLED_Arduino_Library
//
// Documentation:
//     https://sparkfun.github.io/SparkFun_Qwiic_OLED_Arduino_Library/
//
//
// SparkFun code, firmware, and software is released under the MIT License(http://opensource.org/licenses/MIT).
//
// SPDX-License-Identifier: MIT
//
//    The MIT License (MIT)
//
//    Copyright (c) 2022 SparkFun Electronics
//    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
//    associated documentation files (the "Software"), to deal in the Software without restriction,
//    including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
//    and/or sell copies of the Software, and to permit persons to whom the Software is furnished to
//    do so, subject to the following conditions:
//    The above copyright notice and this permission notice shall be included in all copies or substantial
//    portions of the Software.
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
//    NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// Class provide an abstract interface to the I2C device

#include "qwiic_i2c.h"
#include <Arduino.h>

// What is the max buffer size for this platform.

#if defined(SERIAL_BUFFER_SIZE)
#define kMaxTransferBuffer SERIAL_BUFFER_SIZE

#elif defined(I2C_BUFFER_LENGTH)
#define kMaxTransferBuffer I2C_BUFFER_LENGTH

#elif defined(BUFFER_LENGTH)
#define kMaxTransferBuffer BUFFER_LENGTH

#else // just the standard Arduino value
#define kMaxTransferBuffer 32

#endif

// What we use for transfer chunk size

const static uint16_t kChunkSize = kMaxTransferBuffer - 1;

//////////////////////////////////////////////////////////////////////////////////////////////////
// Constructor

QwI2C::QwI2C(void)
{
    m_i2cPort = nullptr;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
// init()
//
// Methods to init/setup this device. The caller can provide a Wire Port, or this class
// will use the default

bool QwI2C::init(TwoWire& wirePort)
{
    // if we don't have a wire port already
    if (!m_i2cPort)
        m_i2cPort = &wirePort;

    return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//

bool QwI2C::init(void)
{
    // do we already have a wire port?
    if (!m_i2cPort)
        return init(Wire); // no wire, send in Wire

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ping()
//
// Is a device connected?
bool QwI2C::ping(uint8_t i2c_address)
{
    m_i2cPort->beginTransmission(i2c_address);
    return m_i2cPort->endTransmission() == 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// writeRegisterByte()
//
// Write a byte to a register

bool QwI2C::writeRegisterByte(uint8_t i2c_address, uint8_t offset, uint8_t dataToWrite)
{
    m_i2cPort->beginTransmission(i2c_address);
    m_i2cPort->write(offset);
    m_i2cPort->write(dataToWrite);
    return m_i2cPort->endTransmission() == 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
// writeRegisterRegion()
//
// Write a block of data to a device. This routine will chunk over the data if needed

int QwI2C::writeRegisterRegion(uint8_t i2c_address, uint8_t offset, uint8_t* data, uint16_t length)
{
    uint16_t nSent;
    uint16_t nRemaining = length;
    uint16_t nToWrite;

    while (nRemaining > 0) {

        m_i2cPort->beginTransmission(i2c_address);
        m_i2cPort->write(offset);

        nToWrite = (nRemaining > kChunkSize ? kChunkSize : nRemaining);
        nSent = m_i2cPort->write(data, nToWrite);

        nRemaining -= nToWrite; // Note - use nToWrite, not nSent, or lock on esp32
        data += nSent; // move up to remaining data in buffer

#if defined(ARDUINO_ARCH_ESP32)
     // if we are on ESP32, release bus no matter what
     if (m_i2cPort->endTransmission())
#else
        // only release bus if we've sent all data
     if (m_i2cPort->endTransmission(nRemaining <= 0))
#endif
            return -1; // the client didn't ACK
    }

    return length - nRemaining;
}
