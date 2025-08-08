// qwiic_oled_1in3.h
//
// This is a library written for SparkFun Qwiic OLED boards that use the CH1120.
//
// SparkFun sells these at its website: www.sparkfun.com
//
// Do you like this library? Help support SparkFun. Buy a board!
//
// Written by SparkFun Electronics, August 2025
//
// This library configures and draws graphics to OLED boards that use the
// CH1120 display hardware. The library only supports I2C.
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

// Implementation for the 1.5" OLED device

#pragma once 

#include "qwiic_grch1120.h"

//////////////////////////////////////////////////////////////////
// Set the defaults for the SparkFun Qwiic 1.5" OLED

#define kOLED1in5Width 128
#define kOLED1in5Height 128

#define kOLED1in5XOffset 0
#define kOLED1in5YOffset 0

// Parameters for this device
#define kOLED1in5PinConfig 0x12
#define kOLED1in5PreCharge 0xF1
#define kOLED1in5VCOM 0x40
#define kOLED1in5Contrast 0xCF

// 0x3C = Open, 0x3D = Closed
#define kOLED1in5DefaultAddress 0x3C
#define kOLED1in5AltAddress 0x3D

class QwOLED1in5 : public QwGrCH1120 {
    public:
    // Constructor - setup the viewport and default address for this device.

    QwOLED1in5() : QwGrCH1120(kOLED1in5XOffset, kOLED1in5YOffset, kOLED1in5Width, kOLED1in5Height) {
        default_address = kOLED1in5DefaultAddress;
    }

    // set up the specific device settings
    bool init(void)
    {
        setBuffer(m_graphicsBuffer); // The buffer to use

        setCommPins(kOLED1in5PinConfig);
        setPreCharge(kOLED1in5PreCharge);
        setVcomDeselect(kOLED1in5VCOM);
        setContrast(kOLED1in5Contrast);

        // Call the super class to do all the work
        return this->QwGrCH1120::init();
    }

    private:
        // Graphics buffer for this device
        uint8_t m_graphicsBuffer[kOLED1in5Width * kOLED1in5Height / 8];

};