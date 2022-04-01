// qwiic_oledmicro.h
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

// Implementation for the Qwiic Micro OLED Device

#pragma once

#include "qwiic_grssd1306.h"

//////////////////////////////////////////////////////////////////
// Set the defaults for the SparkFun Qwiic MicroOLED

#define kOLEDMicroWidth 64
#define kOLEDMicroHeight 48

// The viewport x is off by 2 on this device - starts at value 2
#define kOLEDMicroXOffset 2
#define kOLEDMicroYOffset 0

// Parameters for this device
#define kOLEDMicroPinConfig 0x12
#define kOLEDMicroPreCharge 0xF1
#define kOLEDMicroVCOM 0x40

#define kOLEDMicroDefaultAddress 0x3D
#define kOLEDMicroAltAddress 0x3C

class QwOLEDMicro : public QwGrSSD1306 {

public:
    // Constructor - setup the viewport and default address for this device.
    QwOLEDMicro()
        : QwGrSSD1306(kOLEDMicroXOffset, kOLEDMicroYOffset, kOLEDMicroWidth, kOLEDMicroHeight)
    {
        default_address = kOLEDMicroDefaultAddress;
    };

    // set up the specific device settings
    bool init(void)
    {
        setBuffer(m_graphicsBuffer); // The buffer to use

        setCommPins(kOLEDMicroPinConfig);
        setPreCharge(kOLEDMicroPreCharge);
        setVcomDeselect(kOLEDMicroVCOM);

        // Call the super class to do all the work
        return this->QwGrSSD1306::init();
    };

private:
    // Graphics buffer for this device.
    uint8_t m_graphicsBuffer[kOLEDMicroWidth * kOLEDMicroHeight / 8];
};