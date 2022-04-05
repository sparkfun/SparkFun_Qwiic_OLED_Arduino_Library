// SparkFun_Qwiic_OLED.h
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

#pragma once

// Arduino wrapper for the OLED library. Provides an Arduino experience and platform support
// for the Qwiic OLED driver.

// include the underlying SDK implementation headers for the OLED devices
#include "qwiic_oledmicro.h"
#include "qwiic_olednarrow.h"
#include "qwiic_oledtransp.h"

#include <Arduino.h>
#include <Wire.h>

// Friendly typenames
typedef QwFont QwiicFont;
typedef QwBitmap QwiicBitmap;

#define COLOR_WHITE 1
#define COLOR_BLACK 0

// The Plan:
//
// For each supported device the following is needed
//
//      - A commmon Arduino interface/implementation. One impl, one area to maintain
//      - Classes that are specialized for each device
//      - No abstract methods that a subclass must implement
//
// The solution
//    A templated base class that device specific sub-classes derive from.
//

// flag - internal - to determine if an address is passed into begin or not

#define kNoAddressSet 0

// Define the template and fill in the interface methods in-line.

template <typename SSD1306DeviceType>
class QwiicOLEDBaseClass : public Print { // NOTE: implementing Arduino Print
private:
    // our device driver
    SSD1306DeviceType m_device;
    QwI2C m_i2cBus; // our i2c object

    // for the Aruduino print functionaliyt
    uint8_t m_cursorX;
    uint8_t m_cursorY;

    uint8_t m_color;

public:
    ///////////////////////////////////////////////////////////////////////
    // begin()
    //
    // This method is called to initialize the OLED library and connection to
    // the OLED device. This method must be called before calling any graphics methods.
    //
    // This method follows the standard startup pattern in SparkFun Arduino
    // libraries.
    //
    //  Parameter   Description
    //  ---------   ----------------------------
    //  wirePort    optional. The Wire port. If not provided, the default port is used
    //  address     optional. I2C Address. If not provided, the default address is used.
    //  retval      true on success, false on startup failure

    bool begin(TwoWire& wirePort = Wire, uint8_t address = kNoAddressSet)
    {

        // defaults for Arduino Print
        setCursor(0, 0);
        setColor(COLOR_WHITE);

        m_i2cBus.init(wirePort);

        m_device.setCommBus(m_i2cBus,
            (address == kNoAddressSet ? m_device.default_address : address));

        // call init on the device
        bool bStatus = m_device.init();

        // Want to start cursor at Y height of the current font, if we have a font.
        //
        // Get our font height ... a default font is set during init ...
        if (bStatus) {
            QwiicFont* pFont = m_device.font();
            if (pFont)
                m_cursorY = pFont->height;
        }

        return bStatus;
    }

    ///////////////////////////////////////////////////////////////////////
    // getWidth()
    //
    // This method returns the width, in pixels, of the connected OLED device
    //
    //  Parameter   Description
    //  ---------   -----------------------------
    //  retval      The width in pixels of the connected OLED device

    uint8_t getWidth(void)
    {
        return m_device.width();
    }

    ///////////////////////////////////////////////////////////////////////
    // getHeight()
    //
    // This method returns the height, in pixels, of the connected OLED device
    //
    //  Parameter   Description
    //  ---------   -----------------------------
    //  retval      The height in pixels of the connected OLED device

    uint8_t getHeight(void)
    {
        return m_device.height();
    }

    ///////////////////////////////////////////////////////////////////////
    // reset()
    //
    // When called, the system and OLED are reset back to an initial state
    //
    //  Parameter   Description
    //  ---------   -----------------------------
    //  clearDisplay true - clear the internal buffers during reset    
    //  retval      true on success, false on  failure

    bool reset(bool clearDisplay)
    {
        return m_device.reset(clearDisplay);
    }

    ///////////////////////////////////////////////////////////////////////
    // display()
    //
    // When called, any pending display updates are sent to the connected OLED
    // device. This includes drawn graphics and erase commands.
    //
    // To display any graphics, this method must be called.

    void display(void)
    {
        m_device.display();
    }

    ///////////////////////////////////////////////////////////////////////
    // erase()
    //
    // Erases all graphics on the device, placing the display in a blank state.
    // The erase update isn't sent to the device until the next display() call
    // on the device.

    void erase(void)
    {
        m_device.erase();
    }

    ///////////////////////////////////////////////////////////////////////
    // invert()
    //
    // This method inverts the current graphics on the display. This results
    // of this command happen immediatly.
    //
    // Parameter    Description
    // ---------    -----------------------------
    // bInvert      true - the screen is inverted. false - the screen is set to normal

    void invert(bool bInvert)
    {
        m_device.invert(bInvert);
    }

    ///////////////////////////////////////////////////////////////////////
    // flipVertical()
    //
    // When called, the screen contents are flipped vertically if the flip parameter
    // is true, or restored to normal display if the flip parameter is false.
    //
    // Parameter    Description
    // ---------    -----------------------------
    // bFlip        true - the screen is flipped vertically. false - the screen is set to normal

    void flipVertical(bool bFlip)
    {
        m_device.flipVert(bFlip);
    }

    ///////////////////////////////////////////////////////////////////////
    // flipHorizontal()
    //
    // When called, the screen contents are flipped horizontally if the flip parameter
    // is true, or restored to normal display if the flip parameter is false.
    //
    // Parameter    Description
    // ---------    -----------------------------
    // bFlip        true - the screen is flipped horizontally. false - the screen is set to normal

    void flipHorizontal(bool bFlip)
    {
        m_device.flipHorz(bFlip);
    }

    ///////////////////////////////////////////////////////////////////////
    // scrollStop()
    //
    // If the device is in a scrolling mode, calling this method stops the scroll,
    // and restores the device to normal display operation. This action is performed immediately.

    void scrollStop(void)
    {
        m_device.stopScroll();
    }

    ///////////////////////////////////////////////////////////////////////
    // scrollRight()
    //
    // This method is called to start the device scrolling the displayed graphics to the right.
    // This action is performed immediately.
    //
    // The screen will scroll until the scrollStop() method is called.
    //
    // Parameter    Description
    // ---------    -----------------------------
    // start        The start page address of the scroll - valid values are 0 thru 7
    // stop         The stop/end page address of the scroll - valid values are 0 thru 7
    // interval     The time interval between scroll step - values listed below
    //
    // Defined values for the interval parameter:
    //
    // Defined Symbol                   Time Interval Between Steps
    // ----------------------------     ---------------------------
    //  SCROLL_INTERVAL_2_FRAMES        2
    //  SCROLL_INTERVAL_3_FRAMES        3
    //  SCROLL_INTERVAL_4_FRAMES        4
    //  SCROLL_INTERVAL_5_FRAMES        5
    //  SCROLL_INTERVAL_25_FRAMES       25
    //  SCROLL_INTERVAL_64_FRAMES       64
    //  SCROLL_INTERVAL_128_FRAMES      128
    //  SCROLL_INTERVAL_256_FRAMES      256

    void scrollRight(uint8_t start, uint8_t stop, uint8_t interval)
    {
        m_device.scroll(SCROLL_RIGHT, start, stop, interval);
    }

    ///////////////////////////////////////////////////////////////////////
    // scrollVertRight()
    //
    // This method is called to start the device scrolling the displayed graphics verticall and to the right.
    // This action is performed immediately.
    //
    // The screen will scroll until the scrollStop() method is called.
    //
    // Parameter    Description
    // ---------    -----------------------------
    // start        The start page address of the scroll - valid values are 0 thru 7
    // stop         The stop/end page address of the scroll - valid values are 0 thru 7
    // interval     The time interval between scroll step - values listed in scrollRight()

    void scrollVertRight(uint8_t start, uint8_t stop, uint8_t interval)
    {
        m_device.scroll(SCROLL_VERT_RIGHT, start, stop, interval);
    }

    ///////////////////////////////////////////////////////////////////////
    // scrollLeft()
    //
    // This method is called start to the device scrolling the displayed graphics to the left.
    // This action is performed immediately.
    //
    // The screen will scroll until the scrollStop() method is called.
    //
    // Parameter    Description
    // ---------    -----------------------------
    // start        The start page address of the scroll - valid values are 0 thru 7
    // stop         The stop/end page address of the scroll - valid values are 0 thru 7
    // interval     The time interval between scroll step - values listed in scrollRight()

    void scrollLeft(uint8_t start, uint8_t stop, uint8_t interval)
    {
        m_device.scroll(SCROLL_LEFT, start, stop, interval);
    }

    ///////////////////////////////////////////////////////////////////////
    // scrollVertLeft()
    //
    // This method is called to start the device scrolling the displayed graphics verticall and to the left.
    // This action is performed immediately.
    //
    // The screen will scroll until the scrollStop() method is called.
    //
    // Parameter    Description
    // ---------    -----------------------------
    // start        The start page address of the scroll - valid values are 0 thru 7
    // stop         The stop/end page address of the scroll - valid values are 0 thru 7
    // interval     The time interval between scroll step - values listed in scrollRight()

    void scrollVertLeft(uint8_t start, uint8_t stop, uint8_t interval)
    {
        m_device.scroll(SCROLL_VERT_LEFT, start, stop, interval);
    }

    ///////////////////////////////////////////////////////////////////////
    // displayPower()
    //
    // Used to turn the OLED display on an off.
    //
    // Default value is on.
    //
    // Parameter    Description
    // ---------    -----------------------------
    // enable       Turn the display on or off - default is on

    void displayPower(bool enable = true)
    {

        m_device.displayPower(enable);
    }
    ///////////////////////////////////////////////////////////////////////
    // setFont()
    //
    // This method is called to set the current font in the library. The current font is used
    // when calling the text() method on this device.
    //
    // The default font for the device is 5x7.
    //
    // Parameter    Description
    // ---------    -----------------------------
    // theFont      The font to set as current in the device. A Font object or pointer is accepted
    //
    // For the library, fonts are added to your program by including them via include files which
    // are part of this library.
    //
    // The following fonts are included:
    //
    //  Font            Include File            Font Variable       Description
    //  -----------     ---------------------   ----------------    ---------------------
    //  5x7             <res/qw_fnt_5x7.h>      QW_FONT_5X7         A full, 5 x 7 font
    //  31x48           <res/qw_fnt_31x48.h>    QW_FONT_31X48       A full, 31 x 48 font
    //  Seven Segment   <res/qw_fnt_7segment.h> QW_FONT_7SEGMENT    Numbers only
    //  8x16            <res/qw_fnt_8x16.h>     QW_FONT_8X16        A full, 8 x 16 font
    //  Large Numbers   <res/qw_fnt_largenum.h> QW_FONT_LARGENUM    Numbers only
    //
    //  For each font, the font variables are objects with the following attributes:
    //
    //  Attribute   Value
    //  ----------  -------------------------------------
    //  width       The font width in pixels
    //  height      The font height in pixels
    //  start       The font start character offset
    //  n_chars     The number of characters
    //  map_width   The width of the font map
    //
    //  Example use of a font object attribute:
    //
    //     #include <res/qw_fnt_31x48.h>
    //
    //     int myFontWidth = QW_FONT_31X48.width;
    //

    void setFont(QwiicFont& theFont)
    {
        m_device.setFont(theFont);
    }
    void setFont(const QwiicFont* theFont)
    {
        m_device.setFont(theFont);
    }

    ///////////////////////////////////////////////////////////////////////
    // getFont()
    //
    // This method returns the current font for the device.
    //
    // Parameter    Description
    // ---------    -----------------------------
    //  retval      A pointer to the current font. See setFont() for font object details.

    QwiicFont* getFont(void)
    {
        return m_device.font();
    }

    ///////////////////////////////////////////////////////////////////////
    // getFontName()
    //
    // This method returns the name of the current font for the device.

    String getFontName(void)
    {
        QwiicFont* pFont = m_device.font();

        if (!pFont)
            return String("");

        return String(pFont->name);
    }
    ///////////////////////////////////////////////////////////////////////
    // getStringWidth()
    //
    // Returns the width of the provide string using the current font.
    //
    // Parameter    Description
    // ---------    -----------------------------
    //  text        The string used to determine width
    //  retval      The width of the provide string, as determined using the current font.

    unsigned int getStringWidth(String& text)
    {
        return getStringWidth(text.c_str());
    }

    unsigned int getStringWidth(const char* text)
    {

        uint16_t height, width;

        return (m_device.getStringSize(text, width, height) ? width : 0);
    }

    ///////////////////////////////////////////////////////////////////////
    // getStringHeight()
    //
    // Returns the height of the provide string using the current font.
    //
    // Parameter    Description
    // ---------    -----------------------------
    //  text        The string used to determine height
    //  retval      The height of the provide string, as determined using the current font.

    unsigned int getStringHeight(String& text)
    {
        return getStringHeight(text.c_str());
    }

    unsigned int getStringHeight(const char* text)
    {

        uint16_t height, width;

        return (m_device.getStringSize(text, width, height) ? height : 0);
    }
    ///////////////////////////////////////////////////////////////////////
    // setDrawMode()
    //
    // This method sets the current draw mode for the library. The draw mode
    // determines how pixels are set on the screen during drawing operations.
    //
    // Parameter    Description
    // ---------    -----------------------------
    // rop          The raster operation (ROP) to set the graphics system to.
    //
    // Raster operations device how source (pixels to draw) are represented on the
    // destination device. The available Raster Operation (ROP) codes are:
    //
    //  ROP Code        Description
    //  ---------       -------------------------------------
    //  grROPCopy       default Drawn pixel values are copied to the device screen
    //  grROPNotCopy    A not operation is applied to the source value before copying to screen
    //  grROPNot        A not operation is applied to the destination (screen) value
    //  grROPXOR        A XOR operation is performed between the source and destination values
    //  grROPBlack      A value of 0, or black is drawn to the destination
    //  grROPWhite      A value of 1, or black is drawn to the destination

    void setDrawMode(grRasterOp_t rop)
    {
        m_device.setRasterOp(rop);
    }

    ///////////////////////////////////////////////////////////////////////
    // setDrawMode()
    //
    // This method returns the current draw mode for the library. The draw mode
    // determines how pixels are set on the screen during drawing operations.
    //
    // Parameter    Description
    // ---------    -----------------------------
    //  retval      The current aster operation (ROP) of the graphics system.

    grRasterOp_t getDrawMode(void)
    {
        return m_device.rasterOp();
    }

    ///////////////////////////////////////////////////////////////////////
    // Drawing methods
    ///////////////////////////////////////////////////////////////////////
    //
    // pixel()
    //
    // Set the value of a pixel on the screen.
    //
    // Parameter    Description
    // ---------    -----------------------------
    //  x           The X coordinate of the pixel to set
    //  y           The Y coordinate of the pixel to set
    //  clr         optional The color value to set the pixel. This defaults to white (1).

    void pixel(uint8_t x, uint8_t y, uint8_t clr = COLOR_WHITE)
    {
        m_device.pixel(x, y, clr);
    }

    ///////////////////////////////////////////////////////////////////////
    // line()
    //
    // Draw a line on the screen.
    //
    // Note:    If a line is horizontal (y0 = y1) or vertical (x0 = x1), optimized
    //          draw algorithms are used by the library.
    //
    // Parameter    Description
    // ---------    -----------------------------
    // x0           The start X coordinate of the line
    // y0           The start Y coordinate of the line
    // x1           The end X coordinate of the line
    // y1           The end Y coordinate of the line
    // clr          optional The color value to draw the line. This defaults to white (1).

    void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr = COLOR_WHITE)
    {
        m_device.line(x0, y0, x1, y1, clr);
    }

    ///////////////////////////////////////////////////////////////////////
    // rectangle()
    //
    // Draw a rectangle on the screen.
    //
    // Parameter    Description
    // ---------    -----------------------------
    // x0           The start X coordinate of the rectangle - upper left corner
    // y0           The start Y coordinate of the rectangle - upper left corner
    // width        The width of the rectangle
    // height       The height of the rectangle
    // clr          optional The color value to draw the rectangle. This defaults to white (1).

    void rectangle(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, uint8_t clr = COLOR_WHITE)
    {
        m_device.rectangle(x0, y0, width, height, clr);
    }

    ///////////////////////////////////////////////////////////////////////
    // rectangleFill()
    //
    // Draw a filled rectangle on the screen.
    //
    // Parameter    Description
    // ---------    -----------------------------
    // x0           The start X coordinate of the rectangle - upper left corner
    // y0           The start Y coordinate of the rectangle - upper left corner
    // width        The width of the rectangle
    // height       The height of the rectangle
    // clr          optional The color value to draw the filled rectangle. This defaults to white (1).

    void rectangleFill(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, uint8_t clr = COLOR_WHITE)
    {
        m_device.rectangleFill(x0, y0, width, height, clr);
    }

    ///////////////////////////////////////////////////////////////////////
    // circle()
    //
    // Draw a circle on the screen.
    //
    // Parameter    Description
    // ---------    -----------------------------
    // x0           The X coordinate of the circle center
    // y0           The Y coordinate of the circle center
    // radius       The radius of the circle
    // clr          optional The color value to draw the circle. This defaults to white (1).

    void circle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t clr = COLOR_WHITE)
    {
        m_device.circle(x0, y0, radius, clr);
    }

    ///////////////////////////////////////////////////////////////////////
    // circleFill()
    //
    // Draw a circle on the screen.
    //
    // Parameter    Description
    // ---------    -----------------------------
    // x0           The X coordinate of the circle center
    // y0           The Y coordinate of the circle center
    // radius       The radius of the circle
    // clr          optional The color value to draw the circle. This defaults to white (1).

    void circleFill(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t clr = COLOR_WHITE)
    {
        m_device.circleFill(x0, y0, radius, clr);
    }

    ///////////////////////////////////////////////////////////////////////
    // bitmap()
    //
    // Draws a bitmap on the screen.
    //
    // Parameter    Description
    // ---------    -----------------------------
    // x0           The X coordinate to place the bitmap - upper left corner
    // y0           The Y coordinate to place the bitmap - upper left corner
    // x1           The end X coordinate of area to draw - lower right corner
    //              Range will not exceed bitmap width
    // y1           The end Y coordinate of area to draw - lower right corner
    //              Range will not exceed bitmap height
    // pBitmap      A pointer to the bitmap array
    // bmp_width    The width of the bitmap
    // bmp_height   The height of the bitmap

    void bitmap(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,
        uint8_t* pBitmap, uint8_t bmp_width, uint8_t bmp_height)
    {
        m_device.bitmap(x0, y0, x1, y1, pBitmap, bmp_width, bmp_height);
    }

    ///////////////////////////////////////////////////////////////////////
    // bitmap()
    //
    // Draws a bitmap on the screen.
    //
    // Parameter    Description
    // ---------    -----------------------------
    // x0           The X coordinate to place the bitmap - upper left corner
    // y0           The Y coordinate to place the bitmap - upper left corner
    // pBitmap      A pointer to the bitmap array
    // bmp_width    The width of the bitmap
    // bmp_height   The height of the bitmap

    void bitmap(uint8_t x0, uint8_t y0, uint8_t* pBitmap, uint8_t bmp_width, uint8_t bmp_height)
    {

        m_device.bitmap(x0, y0, pBitmap, bmp_width, bmp_height);
    }
    ///////////////////////////////////////////////////////////////////////
    // bitmap()
    //
    // Draws a bitmap object on the screen.
    //
    // Parameter    Description
    // ---------    -----------------------------
    // x0           The X coordinate to place the bitmap - upper left corner
    // y0           The Y coordinate to place the bitmap - upper left corner
    // bitmap       A bitmap object

    void bitmap(uint8_t x0, uint8_t y0, QwiicBitmap& bitmap)
    {
        m_device.bitmap(x0, y0, bitmap);
    }

    ///////////////////////////////////////////////////////////////////////
    // text()
    //
    // Draws a string using the current font on the screen.
    //
    // Parameter    Description
    // ---------    -----------------------------
    // x0           The X coordinate to start drawing the text
    // y0           The Y coordinate to start drawing the text
    // text         The string to draw on the screen
    // clr          optional The color value to draw the text. This defaults to white (1).

    void text(uint8_t x0, uint8_t y0, const char* text, uint8_t clr = COLOR_WHITE)
    {
        m_device.text(x0, y0, text, clr);
    }

    void text(uint8_t x0, uint8_t y0, String& text, uint8_t clr = COLOR_WHITE)
    {

        m_device.text(x0, y0, text.c_str(), clr);
    }

    ///////////////////////////////////////////////////////////////////////
    // Methods to support Arduino Print  capability
    ///////////////////////////////////////////////////////////////////////
    //
    // setCursor()
    //
    // This method is called set the "cursor" position in the device. The library
    // supports the Arduino Print interface, enabling the use of a print() and
    // println() methods. The set cursor position defines where to start text
    // output for this functionality.
    //
    // Parameter    Description
    // ---------    -----------------------------
    // x            The X coordinate of the cursor
    // y            The Y coordinate of the cursor

    void setCursor(uint8_t x, uint8_t y)
    {

        if (x < 0 || x >= m_device.width() || y < 0 || y >= m_device.height())
            return;

        m_cursorX = x;
        m_cursorY = y;
    }

    ///////////////////////////////////////////////////////////////////////
    // setColor()
    //
    // This method is called to set the current color of the system. This is
    // used by the Arduino Print interface functionality
    //
    // Parameter    Description
    // ---------    -----------------------------
    //  clr         The color to set. 0 = black, > 0 = white

    void setColor(uint8_t clr)
    {
        m_color = (clr > 0 ? COLOR_WHITE : COLOR_BLACK);
    }

    ///////////////////////////////////////////////////////////////////////
    // getColor()
    //
    // This method is called to get the current color of the system. This is
    // used by the Arduino Print interface functionality
    //
    // Parameter    Description
    // ---------    -----------------------------
    //  retval      The current color. 0 = black, > 0 = white

    uint8_t getColor(void)
    {
        return m_color;
    }

    ///////////////////////////////////////////////////////////////////////
    // write()
    //
    // For the Arduino Print interface
    //

    virtual size_t write(uint8_t theChar)
    {
        QwiicFont* pFont = m_device.font();

        if (!pFont) // no Font?! No dice
            return 0;

        switch (theChar) {
        case '\n': // Carriage return
            m_cursorX = 0;
            m_cursorY += pFont->height;
        case '\r': // Line feed - do nothing
            break;
        default:

            char buffer[2] = { theChar, '\0' }; // text() needs a c string
            m_device.text(m_cursorX, m_cursorY, buffer, m_color);

            m_cursorX += pFont->width + 1;

            if (m_cursorX > m_device.width() - pFont->width) { // overflow
                m_cursorX = 0;
                m_cursorY += pFont->height;
            }
            break;
        }
        if (m_cursorY >= m_device.height()) // check for overflow
            m_cursorY = 0;

        return 1;
    }
};

///////////////////////////////////////////////////////////////////////
// For our actual implementations - just subclass from the above Arduino template

class QwiicMicroOLED : public QwiicOLEDBaseClass<QwOLEDMicro> {
    // nothing here - see above
};

class QwiicNarrowOLED : public QwiicOLEDBaseClass<QwOLEDNarrow> {
    // nothing here - see above
};

class QwiicTransparentOLED : public QwiicOLEDBaseClass<QwOLEDTransparent> {
    // nothing here - see above
};
