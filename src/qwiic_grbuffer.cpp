
// qwiic_grbuffer.cpp
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

////////////////////////////////////////////////////////////////////////////////////
// Overview:
//
// Buffer graphics device implementation.
//
// The buffer device implements a base graphics class for this library, providing device setup
// and raster-based graphic commands that provide base functionality.
//
// A subclass is required to use this class. At a minimum, the subclass must implement a
// drawPixel() method, which the graphics routines in this class use to render graphics.
//
// This class also performs runtime object vtable inspection to determine the methods
// to call for drawing, and set fallbacks as needed. The vtable is mapped to a draw
// interface structure, which is used to call the desired graphics operations. Besides
// providing flexiblity, this implementation might provide a slight performance gain
// for virtual method dispatch.
//

#include "qwiic_grbuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// default font
#include "res/qw_fnt_5x7.h"

// Handy helper
const uint8_t byte_bits[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

////////////////////////////////////////////////////////////////////////////////////
// vTable/IDraw Interface Routines
////////////////////////////////////////////////////////////////////////////////////
// Support for our drawing interface/vtable inspection
//
// function to get the vTable from an object reference

_QwIDraw_vtable* GetIDrawVTable(_QwIDraw* obj)
{
    _QwIDraw_vtable** vtable_ptr = (_QwIDraw_vtable**)obj;
    return *vtable_ptr;
}

////////////////////////////////////////////////////////////////////////////////////
// initDrawFunctions()
//
// This method pulls out the vtable of this object and determines what methods are
// defined in the QwIDraw interface.
//
// Using what is defined, the functions in m_idraw interface are set. This class uses
// the draw functions in this interface to skip call time vtable method lookup and call the
// target method directly.
//
// Ideally this pattern/implementation increase performace a slight bit. Also it
// allows runtime method implementation determination in this class, which can then
// setup fallbacks if need.
//

bool QwGrBufferDevice::initDrawFunctions(void)
{
    // Get our vtable, and that of a null _QwIDraw object/interface.
    // Use these to determine if a method was overriden/implemented and
    // set defaults ... etc

    _QwIDraw null_obj_; // dummy object

    // copy in the vtable to our draw interface
    m_idraw = *GetIDrawVTable(this);

    // Null vtable - has defaults setup
    _QwIDraw_vtable* null_vtable = GetIDrawVTable(&null_obj_);

    // Review implemented methods - take actions as needed

    // If we don't have a pixel set method, this is a fatal error
    if (m_idraw.drawPixel == null_vtable->drawPixel) {
        // ERROR - drawPixel is required
        fprintf(stderr, "[ERROR][QwGrBufferDevice] Render class must implement drawPixel method\n");
        return false;
    }

    // If the horz line function wasn't implemented (function pointer == null function) = set it to line

    if (m_idraw.drawLineHorz == null_vtable->drawLineHorz)
        m_idraw.drawLineHorz = m_idraw.drawLine;

    // If the vert line function wasn't implemented (function pointer == null function) = set it to line
    if (m_idraw.drawLineVert == null_vtable->drawLineVert)
        m_idraw.drawLineVert = m_idraw.drawLine;

    // if rect_filled isn't implemented, just sub in rect - it' something ...
    if (m_idraw.drawRectFilled     == null_vtable->drawRectFilled)
        m_idraw.drawRectFilled = m_idraw.drawRect;

    // if circle_filled isn't implemented, just sub in circle - it' something ...
    if (m_idraw.drawCircleFilled == null_vtable->drawCircleFilled)
        m_idraw.drawCircleFilled = m_idraw.drawCircle;

    return true;
}
////////////////////////////////////////////////////////////////////////////////////
// init()
//
// Intialize the class. User called as part of the startup process of the library.
//

bool QwGrBufferDevice::init(void)
{
    // Initialize our Font
    initFont();

    // setup the draw function table and return status

    return initDrawFunctions();
}
////////////////////////////////////////////////////////////////////////////////////
// Font related things
////////////////////////////////////////////////////////////////////////////////////
// initFont()
//
// Set the default font if one hasn't been set yet

void QwGrBufferDevice::initFont(void)
{
    if (!m_currentFont)
        m_currentFont = &QW_FONT_5X7;
}
////////////////////////////////////////////////////////////////////////////////////
// font()
//
// Return the current font.

QwFont* QwGrBufferDevice::font(void)
{
    if (!m_currentFont) // device hasn't been init'd - maybe ? Go default
        initFont();

    return m_currentFont;
}

////////////////////////////////////////////////////////////////////////////////////////
// seFont()
//
// Object ref version

void QwGrBufferDevice::setFont(QwFont& font)
{
    // just call the pointer version of this method
    setFont(&font);
}
////////////////////////////////////////////////////////////////////////////////////////
// setFont()
//
// Object pointer version

void QwGrBufferDevice::setFont(const QwFont* font)
{
    if (font)
        m_currentFont = (QwFont*)font;
    else if (!m_currentFont) // null passed, we have no default set - init font.
        initFont();
}

////////////////////////////////////////////////////////////////////////////////////////
// get_string_size()
//
// Return the size (width, height) in pixels of the given string using the current font

bool QwGrBufferDevice::getStringSize(const char* text, uint16_t& width, uint16_t& height)
{
    if (!m_currentFont) // no font?
        initFont();

    // no font, no text - no dice
    if (!m_currentFont || !text)
        return false;

    int sLen = strlen(text);

    width = sLen * m_currentFont->width;

    // The 5x7 font width is off by one - historical - was same in orig lib, which fonts
    // are used.
    if (m_currentFont == &QW_FONT_5X7)
        width += sLen;

    height = (sLen > 0 ? m_currentFont->height : 0);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////
// Public Graphics Methods
////////////////////////////////////////////////////////////////////////////////////////
//
// pixel()
//
// Set a pixel on the screen

void QwGrBufferDevice::pixel(uint8_t x, uint8_t y, uint8_t clr)
{
    // in range?
    if (x >= m_viewport.width || y >= m_viewport.height)
        return;

    // Call the pixel draw method in the draw interface.
    (*m_idraw.drawPixel)(this, x, y, clr);
}
////////////////////////////////////////////////////////////////////////////////////////
// line()
//
// Method that rasterizes a line, setting pixels in the buffer device via
// the pixel command.
//
// Parameters
//
//      (x0, y0)   - Line origin
//      (x1, y1)   - Line end

void QwGrBufferDevice::line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr)
{
    // if we have a vertical or horizonal line, call the optimized drawing functions

    if (x0 == x1)
        (*m_idraw.drawLineVert)(this, x0, y0, x1, y1, clr);
    else if (y0 == y1)
        (*m_idraw.drawLineHorz)(this, x0, y0, x1, y1, clr);
    else
        (*m_idraw.drawLine)(this, x0, y0, x1, y1, clr);
}

//////////////////////////////////////////////////////////////////////////////////////////
// drawLine()
//
// Core, internal line drawing method - performs a line rasterization algorithm

void QwGrBufferDevice::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr)
{
    // The rasterization method uses an increment of 1 to walk the desired line
    // to determine which pixels are being set in the buffer device.
    //
    // To ensure a detailed rasterization, the dimention with the largest value is used.

    // deltas for each dimension
    uint8_t dx = abs(x1 - x0);
    uint8_t dy = abs(y1 - y0);

    // zero len line - turns out that's a point
    if (!dx && !dy) {
        (*m_idraw.drawPixel)(this, x0, y0, clr);
        return;
    }
    // If this is a steep line, switch the axis to increment along
    bool isSteep = dy > dx;

    if (isSteep) {
        swap_int(x0, y0);
        swap_int(x1, y1);
        swap_int(dx, dy);
    }

    // let's be positive about our increment - if negative, swap our vertex
    if (x0 > x1) {
        swap_int(x0, x1);
        swap_int(y0, y1);
    }

    // value used to determine when to bump up Y as we walk the line
    int8_t err = dx / 2;
    int8_t ystep = y0 < y1 ? 1 : -1; // is y going up or down?

    // Crank over our x range of the line and draw our pixels
    for (; x0 <= x1; x0++) {

        if (isSteep)
            (*m_idraw.drawPixel)(this, y0, x0, clr);
        else
            (*m_idraw.drawPixel)(this, x0, y0, clr);

        // do we need to bump up y? or if the slow is shallow, stay of
        // the same value?
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////
// rectangle()
//
// Draw a rectangle on screen.

void QwGrBufferDevice::rectangle(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, uint8_t clr)
{
    // Is this a line?
    if (width <= 1 || height <= 1) {
        // this is a line
        line(x0, y0, x0 + width - 1, y0 + height - 1);
        return;
    }

    // bounds check
    if (x0 >= m_viewport.width || y0 >= m_viewport.height)
        return; // not on screen

    // Send to drawing routine.

    (*m_idraw.drawRect)(this, x0, y0, width, height, clr);
}

////////////////////////////////////////////////////////////////////////////////////////
// drawRect()
//
// Does the actual drawing/logic

void QwGrBufferDevice::drawRect(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, uint8_t clr)
{
    // A rect is really just a series of vert and horz lines

    uint8_t x1 = x0 + width - 1;
    uint8_t y1 = y0 + height - 1;

    (*m_idraw.drawLineHorz)(this, x0, y0, x1, y0, clr);
    (*m_idraw.drawLineHorz)(this, x0, y1, x1, y1, clr);

    // If the height is less than 2, no need for vert lines b/c the line itself is 1 pixel

    if (y1 - y0 < 3)
        return;

    // Vertical lines - note - do not draw over our horz lines b/c of potential xor turds
    (*m_idraw.drawLineVert)(this, x0, y0 + 1, x0, y1, clr);
    (*m_idraw.drawLineVert)(this, x1, y0 + 1, x1, y1, clr);
}
////////////////////////////////////////////////////////////////////////////////////////
// rectangleFill()

void QwGrBufferDevice::rectangleFill(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, uint8_t clr)
{
    // Is this a line?
    if (width <= 1 || height <= 1) {
        // this is a line
        line(x0, y0, x0 + width - 1, y0 + height - 1);
        return;
    }
    // bounds check
    if (x0 >= m_viewport.width || y0 >= m_viewport.height)
        return; // not on screen

    // Send to drawing routine.

    (*m_idraw.drawRectFilled)(this, x0, y0, width, height, clr);
}
////////////////////////////////////////////////////////////////////////////////////////
// drawRectFill()
//
// Does the actual drawing/logic

void QwGrBufferDevice::drawRectFilled(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, uint8_t clr)
{
    uint8_t x1 = x0 + width - 1;
    uint8_t y1 = y0 + height - 1;

    // Just draw vertical lines
    for (int i = x0; i <= x1; i++)
        (*m_idraw.drawLineVert)(this, i, y0, i, y1, clr);
}
////////////////////////////////////////////////////////////////////////////////////////
// circle()
//

void QwGrBufferDevice::circle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t clr)
{
    // Anything visible on screen?
    if (!radius || x0 - (int8_t)radius >= m_viewport.width || y0 - (int8_t)radius >= m_viewport.height)
        return;

    // Is this just a pixel
    if (radius == 1) {
        (*m_idraw.drawPixel)(this, x0, y0, clr);
        return;
    }

    (*m_idraw.drawCircle)(this, x0, y0, radius, clr);
}
////////////////////////////////////////////////////////////////////////////////////////
// drawCircle()
//
// Draw a circle

void QwGrBufferDevice::drawCircle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t clr)
{
    int8_t f = 1 - radius;
    int8_t ddF_x = 1;
    int8_t ddF_y = -2 * radius;
    int8_t x = 0;
    int8_t y = radius;

    (*m_idraw.drawPixel)(this, x0, y0 + radius, clr);
    (*m_idraw.drawPixel)(this, x0, y0 - radius, clr);
    (*m_idraw.drawPixel)(this, x0 + radius, y0, clr);
    (*m_idraw.drawPixel)(this, x0 - radius, y0, clr);

    while (x < y) {

        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        (*m_idraw.drawPixel)(this, x0 + x, y0 + y, clr);
        (*m_idraw.drawPixel)(this, x0 - x, y0 + y, clr);
        (*m_idraw.drawPixel)(this, x0 + x, y0 - y, clr);
        (*m_idraw.drawPixel)(this, x0 - x, y0 - y, clr);

        (*m_idraw.drawPixel)(this, x0 + y, y0 + x, clr);
        (*m_idraw.drawPixel)(this, x0 - y, y0 + x, clr);
        (*m_idraw.drawPixel)(this, x0 + y, y0 - x, clr);
        (*m_idraw.drawPixel)(this, x0 - y, y0 - x, clr);
    }
}
////////////////////////////////////////////////////////////////////////////////////////
// circle_fill()
//
// Draw a filled circle

void QwGrBufferDevice::circleFill(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t clr)
{
    // Anything visible on screen?
    if (!radius || x0 - (int8_t)radius >= m_viewport.width || y0 - (int8_t)radius >= m_viewport.height)
        return;

    if (radius == 1) {
        (*m_idraw.drawPixel)(this, x0, y0, clr);
        return;
    }

    (*m_idraw.drawCircleFilled)(this, x0, y0, radius, clr);
}

////////////////////////////////////////////////////////////////////////////////////////
// drawCircleFilled()
//
void QwGrBufferDevice::drawCircleFilled(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t clr)
{
    int8_t f = 1 - radius;
    int8_t ddF_x = 1;
    int8_t ddF_y = -2 * radius;
    int8_t x = 0;
    int8_t y = radius;

    (*m_idraw.drawLineVert)(this, x0, y0 - radius, x0, y0 + radius, clr);

    while (x < y) {

        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        (*m_idraw.drawLineVert)(this, x0 + x, y0 - y, x0 + x, y0 + y, clr);
        (*m_idraw.drawLineVert)(this, x0 - x, y0 - y, x0 - x, y0 + y, clr);

        (*m_idraw.drawLineVert)(this, x0 + y, y0 - x, x0 + y, y0 + x, clr);
        (*m_idraw.drawLineVert)(this, x0 - y, y0 - x, x0 - y, y0 + x, clr);
    }
}
////////////////////////////////////////////////////////////////////////////////////////
// bitmap()
//
// Draw a bitmap on the screen

void QwGrBufferDevice::bitmap(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,
    uint8_t* pBitmap, uint8_t bmp_width, uint8_t bmp_height)
{
    (*m_idraw.drawBitmap)(this, x0, y0, x1, y1, pBitmap, bmp_width, bmp_height);
}

////////////////////////////////////////////////////////////////////////////////////////
// bitmap()
//
// Draw a bitmap on the screen

void QwGrBufferDevice::bitmap(uint8_t x0, uint8_t y0, uint8_t* pBitmap, uint8_t bmp_width, uint8_t bmp_height)
{
    (*m_idraw.drawBitmap)(this, x0, y0, bmp_width, bmp_height, pBitmap, bmp_width, bmp_height);
}

////////////////////////////////////////////////////////////////////////////////////////
// bitmap() - use a bitmap object
//
void QwGrBufferDevice::bitmap(uint8_t x0, uint8_t y0, QwBitmap& theBMP)
{
    // just pass to graphics device
    (*m_idraw.drawBitmap)(this, x0, y0, theBMP.width, theBMP.height,
        (uint8_t*)theBMP.data(), theBMP.width, theBMP.height);
}
////////////////////////////////////////////////////////////////////////////////////////
// text()
//
// Draw a string on the screen using the current font

void QwGrBufferDevice::text(uint8_t x0, uint8_t y0, const char* text, uint8_t clr)
{
    if (!text || x0 >= m_viewport.width || y0 >= m_viewport.height)
        return;

    (m_idraw.drawText)(this, x0, y0, text, clr);
}
////////////////////////////////////////////////////////////////////////////////////////
// draw_test()
//
// Draw text - one pixel at a time. Based on the algorithm in the Micro OLED
// Arduino library
//
void QwGrBufferDevice::drawText(uint8_t x0, uint8_t y0, const char* text, uint8_t clr)
{
    // check things
    if (!m_currentFont || !text)
        return;

    int slen = strlen(text);

    if (!slen) // empty string?
        return;

    uint8_t nRows = m_currentFont->height / kByteNBits;
    if (!nRows)
        nRows = 1;

    // 5x7 font is special - need to add a margin
    uint8_t margin5x7 = (nRows == 1); // For the 5x7 font

    // used in loop
    uint16_t nRowLen = m_currentFont->map_width / m_currentFont->width; // how long is a
    uint16_t rowBytes = m_currentFont->map_width * nRows;

    // get font data
    const uint8_t* pFont = m_currentFont->data();

    // vars for the loops ...
    uint16_t charOffset, fontIndex;
    uint8_t rowOffset, currChar, i, j, row;

    // walk the string ... note: doing all loop incs here - to handle continue statement below

    for (int k = 0; k < slen; k++, text++, x0 += m_currentFont->width + margin5x7) {

        // index into the font data - takes into account font size, rows span ... etc
        charOffset = *text - m_currentFont->start;

        // does the char location exceed the number of chars in the font?
        if (charOffset >= m_currentFont->n_chars)
            continue; // neeeexxxtt!

        // offset into the font data array - start location
        fontIndex = (charOffset / nRowLen * rowBytes) + ((charOffset % nRowLen) * m_currentFont->width);

        // Now walk the rows of this font entry (it can span bytes)
        for (row = 0; row < nRows; row++) {

            rowOffset = row * kByteNBits; // y offset for multi row fonts - used in pixel draw

            // walk the width of the font
            for (i = 0; i < m_currentFont->width + margin5x7; i++) {

                // this is done in a weird way because the 5x7 font has no margin
                if (margin5x7 && i == m_currentFont->width)
                    continue; // skip to next

                // finally - data!
                currChar = pgm_read_byte(pFont + fontIndex + i + (row * m_currentFont->map_width));

                // draw bits
                for (j = 0; j < kByteNBits; j++)
                    if (currChar & byte_bits[j])
                        (*m_idraw.drawPixel)(this, x0 + i, y0 + j + rowOffset, clr);

            } // walk font width

        } // row loop

    } // string loop
}
