// qwiic_oled_custom.h
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
//   1.3" OLED              https://www.sparkfun.com/products/23453
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

// Implementation for the 1.3" OLED device

#pragma once

#include "qwiic_grssd1306.h"

//////////////////////////////////////////////////////////////////
// Set the defaults for the SparkFun Qwiic MicroOLED

#define kOLEDCustomDefaultWidth 128
#define kOLEDCustomDefaultHeight 64

#define kOLEDCustomDefaultXOffset 0
#define kOLEDCustomDefaultYOffset 0

// Parameters for this device
#define kOLEDCustomDefaultPinConfig 0x12
#define kOLEDCustomDefaultPreCharge 0xF1
#define kOLEDCustomDefaultVCOM 0x40
#define kOLEDCustomDefaultContrast 0xCF

#define kOLEDCustomDefaultDefaultAddress 0x3D
#define kOLEDCustomDefaultAltAddress 0x3C

class QwOLEDCustom : public QwGrSSD1306 {

public:
    // Constructor - setup the viewport and default address for this device.
    QwOLEDCustom()
        : QwGrSSD1306(kOLEDCustomDefaultXOffset, kOLEDCustomDefaultYOffset, kOLEDCustomDefaultWidth, kOLEDCustomDefaultHeight)
    {
        default_address = kOLEDCustomDefaultDefaultAddress;
    };

    ~QwOLEDCustom()
    {
        if (m_graphicsBuffer != nullptr)
            delete[] m_graphicsBuffer;
    };

    // set up the specific device settings
    bool init(void)
    {
        setViewport(xOffset, yOffset, displayWidth, displayHeight);

        setCommPins(pinConfig);
        setPreCharge(preCharge);
        setVcomDeselect(vcomDeselect);
        setContrast(contrast);

        if (m_graphicsBuffer != nullptr)
            delete[] m_graphicsBuffer;
        m_graphicsBuffer = new uint8_t[(uint16_t)displayWidth * (uint16_t)displayHeight / 8];
        setBuffer(m_graphicsBuffer); // The buffer to use

        // Call the super class to do all the work
        return this->QwGrSSD1306::init();
    };

    uint8_t xOffset = kOLEDCustomDefaultXOffset;
    uint8_t yOffset = kOLEDCustomDefaultYOffset;
    uint8_t displayWidth = kOLEDCustomDefaultWidth;
    uint8_t displayHeight = kOLEDCustomDefaultHeight;
    uint8_t pinConfig =  kOLEDCustomDefaultPinConfig;
    uint8_t preCharge = kOLEDCustomDefaultPreCharge;
    uint8_t vcomDeselect = kOLEDCustomDefaultVCOM;
    uint8_t contrast = kOLEDCustomDefaultContrast;

protected:
    // Graphics buffer for this device.
    uint8_t *m_graphicsBuffer = nullptr;
};