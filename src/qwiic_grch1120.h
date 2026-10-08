// qwiic_gr1120.h
//
// This is a library written for SparkFun Qwiic OLED boards that use the CH1120.
//
// Written by SparkFun Electronics, March 2025
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
//    Copyright (c) 2025 SparkFun Electronics
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


#pragma once

#include "qwiic_grbuffer.h"
#include "qwiic_i2c.h"
#include "res/qwiic_resdef.h"
#include "qwiic_grcommon.h"

#define kMaxPageNumberCH1120 20
#define kMaxCH1120Width 160

/////////////////////////////////////////////////////////////////////////////
// Buffer Management
/////////////////////////////////////////////////////////////////////////////
class QwGrCH1120 : public QwGrBufferDevice {
    private: 
        void setupDefaults(void);
        
    public: 
        QwGrCH1120() {
            setupDefaults(); // default constructor - always called
        }
        QwGrCH1120(uint8_t width, uint8_t height) : QwGrCH1120(0, 0, width, height) {};

        // call super class
        QwGrCH1120(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height) : QwGrBufferDevice(x0, y0, width, height) {
            setupDefaults();
        };

        // Public draw methods
        void display(void); // send graphics buffer to the devices screen buffer
        void erase(void);

        // Device setup
        virtual bool init(void);

        bool isInitialized(void) {
            return m_isInitialized;
        };

        bool reset(bool clearDisplay = true);

        // method to set the communication bus this object should use
        void setCommBus(QwI2C &theBus, uint8_t id_bus);

        // Set the current color/pixel write operation
        void setColor(uint8_t color);

        // Settings/operational methods
        void setContrast(uint8_t);

        // default address of the device - expect the sub to fill in.
        uint8_t default_address;

        void setRasterOp(grRasterOp_t rop) {
            m_rop = rop;
        }

        // screen control
        void invert(bool);
        void flipVert(bool);
        void flipHorz(bool);

        // screen scrolling
        void stopScroll(void);
        void scroll(uint16_t scroll_type = SCROLL_RIGHT, uint8_t start = 0, uint8_t stop = 128, uint8_t interval = SCROLL_INTERVAL_2_FRAMES);

        void displayPower(bool enable = true);
    
        protected: 
            // Subclasses of this class define the specifics of the device, including size.
            // Subclass needs to define the graphics buffer array - stack based - and pass in
            void setBuffer(uint8_t *pBuffer);

            ///////////////////////////////////////////////////////////////////////////
            // Internal, fast draw routines - this are used in the overall
            // draw interface (_QwIDraw) for this object/device/system.
            //
            // >> Pixels <<
            void drawPixel(uint8_t x, uint8_t y, uint8_t clr);
            
            // >> Fast Lines <<
            void drawLineHorz(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr);
            void drawLineVert(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr);

            // fast rect fill
            void drawRectFilled(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, uint8_t clr);

            // >> Fast Bitmap <<
            void drawBitmap(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t *pBitmap, uint8_t bmp_width,
                            uint8_t bmp_height);
            
            //TODO: Are these needed for this driver?
            /////////////////////////////////////////////
            // configuration methods for sub-classes. Settings unique to a device
            void setCommPins(uint8_t);
            void setPreCharge(uint8_t);
            void setVcomDeselect(uint8_t);

    private:
        // Internal buffer management methods
        bool setScreenBufferAddress(uint8_t page, uint8_t column);
        void initBuffers(void); // clear graphics and screen buffer
        void clearScreenBuffer(void);
        void resendGraphics(void);
        void setupOLEDDevice(bool clearDisplay = true);

        // Added debug functions
        bool getPixel(uint8_t x, uint8_t y); // Gets the pixel value at (x, y)
        void printBuffer(void); // Prints all of the bits in the buffer as a 2D pixel array over serial
        void rawPrintBuffer(void); // Prints all of the bits in the buffer directly

        // device communication methods
        void sendDevCommand(uint8_t command);
        void sendDevCommand(uint8_t command, uint8_t value);
        void sendDevCommand(uint8_t *commands, uint8_t n);
        void sendDevData(uint8_t *pData, uint16_t nData);
        void sendDevCommand(uint8_t command, uint8_t start, uint8_t stop);

        /////////////////////////////////////////////////////////////////////////////
        // instance vars

        // Buffer variables
        uint8_t *m_pBuffer;                      // Pointer to the graphics buffer
        uint8_t m_nPages;                        // number of pages for current device
        pageState_t m_pageState[kMaxPageNumberCH1120]; // page state descriptors
        pageState_t m_pageErase[kMaxPageNumberCH1120]; // keep track of erase boundaries
        bool m_pendingErase;

        // display variables
        uint8_t m_color;    // current color (really 0 or 1)
        grRasterOp_t m_rop; // current raster operation code

        // I2C  things
        QwI2C *m_i2cBus;      // pointer to our i2c bus object
        uint8_t m_i2cAddress = 0x3C; // address of the device (0x3D for closed)

        // Stash values for settings that are unique to each device.
        uint8_t m_initHWComPins;
        uint8_t m_initPreCharge;
        uint8_t m_initVCOMDeselect;
        uint8_t m_initContrast;

        bool m_isInitialized; // general init flag

        uint8_t horz_flip_offset  = 0; // Shift applied when horizontally flipped to account for viewport < max screen size
};
