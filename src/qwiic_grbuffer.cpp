
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
// draw_pixel() method, which the graphics routines in this class use to render graphics.
//
// This class also performs runtime object vtable inspection to determine the methods
// to call for drawing, and set fallbacks as needed. The vtable is mapped to a draw
// interface structure, which is used to call the desired graphics operations. Besides
// providing flexiblity, this implementation might provide a slight performance gain
// for virtual method dispatch.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qwiic_grbuffer.h"

// default font
#include "res/qw_fnt_5x7.h"

// Handy helper
const uint8_t byte_bits[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

////////////////////////////////////////////////////////////////////////////////////
// vTable/IDraw Interface Routines
////////////////////////////////////////////////////////////////////////////////////
// Support for our drawing interface/vtable inspection
//
// function to get the vTable from an object reference

_QwIDraw_vtable *GetIDrawVTable(_QwIDraw *obj){

    _QwIDraw_vtable **vtable_ptr = (_QwIDraw_vtable **)obj;
    return *vtable_ptr;
}

////////////////////////////////////////////////////////////////////////////////////
// init_draw_functions()
//
// This method pulls out the vtable of this object and determines what methods are
// defined in the QwIDraw interface.
//
// Using what is defined, the functions in _idraw interface are set. This class uses
// the draw functions in this interface to skip call time vtable method lookup and call the
// target method directly.
//
// Ideally this pattern/implementation increase performace a slight bit. Also it
// allows runtime method implementation determination in this class, which can then
// setup fallbacks if need.
//

bool QwGrBufferDevice::init_draw_functions(void){

    // Get our vtable, and that of a null _QwIDraw object/interface.
    // Use these to determine if a method was overriden/implemented and
    // set defaults ... etc

    _QwIDraw null_obj_; // dummy object

    // copy in the vtable to our draw interface
    _idraw = *GetIDrawVTable(this);

    // Null vtable - has defaults setup
    _QwIDraw_vtable *null_vtable = GetIDrawVTable(&null_obj_);

    // Review implemented methods - take actions as needed

    // If we don't have a pixel set method, this is a fatal error
    if(_idraw.draw_pixel == null_vtable->draw_pixel){
        // ERROR - draw_pixel is required
        fprintf(stderr, "[ERROR][QwGrBufferDevice] Render class must implement draw_pixel method\n");
        return false;
    }

    // If the horz line function wasn't implemented (function pointer == null function) = set it to line

    if(_idraw.draw_line_horz == null_vtable->draw_line_horz)
        _idraw.draw_line_horz = _idraw.draw_line;

    // If the vert line function wasn't implemented (function pointer == null function) = set it to line
    if(_idraw.draw_line_vert == null_vtable->draw_line_vert)
        _idraw.draw_line_vert = _idraw.draw_line;

    // if rect_filled isn't implemented, just sub in rect - it' something ...
    if(_idraw.draw_rect_filled == null_vtable->draw_rect_filled)
        _idraw.draw_rect_filled = _idraw.draw_rect;

    // if circle_filled isn't implemented, just sub in circle - it' something ...
    if(_idraw.draw_circle_filled == null_vtable->draw_circle_filled)
        _idraw.draw_circle_filled = _idraw.draw_circle;

    return true;
}
////////////////////////////////////////////////////////////////////////////////////
// init()
//
// Intialize the class. User called as part of the startup process of the library.
//

bool QwGrBufferDevice::init(void){

    // Initialize our Font
    init_font();

    // setup the draw function table and return status

    return init_draw_functions();
}
////////////////////////////////////////////////////////////////////////////////////
// Font related things
////////////////////////////////////////////////////////////////////////////////////
// init_font()
//
// Set the default font if one hasn't been set yet

void QwGrBufferDevice::init_font(void){

    if(!_currFont)
        _currFont = &QW_FONT_5X7;
}
////////////////////////////////////////////////////////////////////////////////////
// get_font()
//
// Return the current font.

QwFont *QwGrBufferDevice::get_font(void){

    if(!_currFont) // device hasn't been init'd - maybe ? Go default
        init_font();

    return _currFont;
}

////////////////////////////////////////////////////////////////////////////////////////
// set_font()
//
// Object ref version

void QwGrBufferDevice::set_font(QwFont &font){

    // just call the pointer version of this method
    set_font(&font);
}
////////////////////////////////////////////////////////////////////////////////////////
// set_font()
//
// Object pointer version

void QwGrBufferDevice::set_font(const QwFont *font){

    if (font)
        _currFont = (QwFont *)font;
    else if(!_currFont) // null passed, we have no default set - init font.
        init_font();
}

////////////////////////////////////////////////////////////////////////////////////////
// get_string_size()
//
// Return the size (width, height) in pixels of the given string using the current font

bool QwGrBufferDevice::get_string_size(const char *text, uint16_t &width, uint16_t &height)
{

    if(!_currFont) // no font?
        init_font();

    // no font, no text - no dice
    if(!_currFont || !text)
        return false; 

    int sLen = strlen(text);

    width = sLen * _currFont->width;

    // The 5x7 font width is off by one - historical - was same in orig lib, which fonts
    // are used. 
    if(_currFont == &QW_FONT_5X7)
        width += sLen;

    height = (sLen  > 0 ? _currFont->height : 0 );

    return true;

}

////////////////////////////////////////////////////////////////////////////////////////
// Public Graphics Methods
////////////////////////////////////////////////////////////////////////////////////////
//
// pixel()
//
// Set a pixel on the screen

void QwGrBufferDevice::pixel(uint8_t x, uint8_t y, uint8_t clr){

    // in range?
    if(x >= _viewport.width || y >= _viewport.height)
        return;

    // Call the pixel draw method in the draw interface.
    (*_idraw.draw_pixel)(this, x, y, clr);
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

void QwGrBufferDevice::line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr){

    // if we have a vertical or horizonal line, call the optimized drawing functions

    if (x0 == x1)
        (*_idraw.draw_line_vert)(this, x0, y0, x1, y1, clr);
    else if (y0 == y1)
        (*_idraw.draw_line_horz)(this, x0, y0, x1, y1, clr);
    else
        (*_idraw.draw_line)(this, x0, y0, x1, y1, clr);
}

//////////////////////////////////////////////////////////////////////////////////////////
// draw_line()
//
// Core, internal line drawing method - performs a line rasterization algorithm

void QwGrBufferDevice::draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr){

    // The rasterization method uses an increment of 1 to walk the desired line
    // to determine which pixels are being set in the buffer device.
    //
    // To ensure a detailed rasterization, the dimention with the largest value is used.

    // deltas for each dimension
    uint8_t dx = abs(x1 - x0);
    uint8_t dy = abs(y1 - y0);

    // zero len line - turns out that's a point
    if (!dx && !dy){
        (*_idraw.draw_pixel)(this, x0, y0, clr);
        return;
    }
    // If this is a steep line, switch the axis to increment along
    bool isSteep = dy > dx;

    if (isSteep){
        swap_int(x0, y0);
        swap_int(x1, y1);
        swap_int(dx, dy);
    }

    // let's be positive about our increment - if negative, swap our vertex
    if (x0 > x1){
        swap_int(x0, x1);
        swap_int(y0, y1);
    }

    // value used to determine when to bump up Y as we walk the line
    int8_t err = dx / 2;
    int8_t ystep = y0 < y1 ? 1 : -1; // is y going up or down?

    // Crank over our x range of the line and draw our pixels
    for(; x0 <= x1; x0++){

        if (isSteep)
            (*_idraw.draw_pixel)(this, y0, x0, clr);
        else
            (*_idraw.draw_pixel)(this, x0, y0, clr);

        // do we need to bump up y? or if the slow is shallow, stay of
        // the same value?
        err -= dy;
        if(err < 0){
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
    if(width <= 1 || height <= 1){
        // this is a line
        line(x0, y0, x0 + width - 1, y0 + height - 1);
        return;
    }

    // bounds check
    if(x0 >= _viewport.width || y0 >= _viewport.height)
        return; // not on screen

    // Send to drawing routine.

    (*_idraw.draw_rect)(this, x0, y0, width, height, clr);
}

////////////////////////////////////////////////////////////////////////////////////////
// draw_rect()
//
// Does the actual drawing/logic

void QwGrBufferDevice::draw_rect(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, uint8_t clr){

    // A rect is really just a series of vert and horz lines

    uint8_t x1 = x0 + width - 1;
    uint8_t y1 = y0 + height - 1;

    (*_idraw.draw_line_horz)(this, x0, y0, x1, y0, clr);
    (*_idraw.draw_line_horz)(this, x0, y1, x1, y1, clr);

    // If the height is less than 2, no need for vert lines b/c the line itself is 1 pixel

    if(y1 - y0 < 3)
        return;

    // Vertical lines - note - do not draw over our horz lines b/c of potential xor turds
    (*_idraw.draw_line_vert)(this, x0, y0 + 1, x0, y1, clr);
    (*_idraw.draw_line_vert)(this, x1, y0 + 1, x1, y1, clr);
}
////////////////////////////////////////////////////////////////////////////////////////
// rectangle_fill()

void QwGrBufferDevice::rectangle_fill(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, uint8_t clr){

    // Is this a line?
    if(width <= 1 || height <= 1){
        // this is a line
        line(x0, y0, x0 + width - 1, y0 + height - 1);
        return;
    }
    // bounds check
    if(x0 >= _viewport.width || y0 >= _viewport.height)
        return; // not on screen

    // Send to drawing routine.

    (*_idraw.draw_rect_filled)(this, x0, y0, width, height, clr);
}
////////////////////////////////////////////////////////////////////////////////////////
// draw_rect_fill()
//
// Does the actual drawing/logic

void QwGrBufferDevice::draw_rect_filled(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, uint8_t clr){

    uint8_t x1 = x0 + width - 1;
    uint8_t y1 = y0 + height - 1;

    // Just draw vertical lines
    for(int i = x0; i <= x1; i++)
        (*_idraw.draw_line_vert)(this, i, y0, i, y1, clr);
}
////////////////////////////////////////////////////////////////////////////////////////
// circle()
//

void QwGrBufferDevice::circle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t clr){

    // Anything visible on screen?
    if(!radius || x0 - (int8_t)radius >= _viewport.width || y0 - (int8_t)radius >= _viewport.height)
        return;

    // Is this just a pixel
    if(radius == 1){
        (*_idraw.draw_pixel)(this, x0, y0, clr);
        return;
    }

    (*_idraw.draw_circle)(this, x0, y0, radius, clr);
}
////////////////////////////////////////////////////////////////////////////////////////
// draw_circle()
//
// Draw a circle

void QwGrBufferDevice::draw_circle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t clr){

    int8_t f = 1 - radius;
    int8_t ddF_x = 1;
    int8_t ddF_y = -2 * radius;
    int8_t x = 0;
    int8_t y = radius;

    (*_idraw.draw_pixel)(this, x0, y0 + radius, clr);
    (*_idraw.draw_pixel)(this, x0, y0 - radius, clr);
    (*_idraw.draw_pixel)(this, x0 + radius, y0, clr);
    (*_idraw.draw_pixel)(this, x0 - radius, y0, clr);

    while(x < y){

        if(f >= 0){
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        (*_idraw.draw_pixel)(this, x0 + x, y0 + y, clr);
        (*_idraw.draw_pixel)(this, x0 - x, y0 + y, clr);
        (*_idraw.draw_pixel)(this, x0 + x, y0 - y, clr);
        (*_idraw.draw_pixel)(this, x0 - x, y0 - y, clr);

        (*_idraw.draw_pixel)(this, x0 + y, y0 + x, clr);
        (*_idraw.draw_pixel)(this, x0 - y, y0 + x, clr);
        (*_idraw.draw_pixel)(this, x0 + y, y0 - x, clr);
        (*_idraw.draw_pixel)(this, x0 - y, y0 - x, clr);
    }
}
////////////////////////////////////////////////////////////////////////////////////////
// circle_fill()
//
// Draw a filled circle

void QwGrBufferDevice::circle_fill(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t clr){

    // Anything visible on screen?
    if(!radius || x0 - (int8_t)radius >= _viewport.width || y0 - (int8_t)radius >= _viewport.height)
        return;

    if(radius == 1){
        (*_idraw.draw_pixel)(this, x0, y0, clr);
        return;
    }

    (*_idraw.draw_circle_filled)(this, x0, y0, radius, clr);
}

////////////////////////////////////////////////////////////////////////////////////////
// draw_circle_filled()
//
void QwGrBufferDevice::draw_circle_filled(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t clr)
{

    int8_t f = 1 - radius;
    int8_t ddF_x = 1;
    int8_t ddF_y = -2 * radius;
    int8_t x = 0;
    int8_t y = radius;

    (*_idraw.draw_line_vert)(this, x0, y0 - radius, x0, y0 + radius, clr);

    while (x < y){

        if (f >= 0){
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        (*_idraw.draw_line_vert)(this, x0 + x, y0 - y, x0 + x, y0 + y, clr);
        (*_idraw.draw_line_vert)(this, x0 - x, y0 - y, x0 - x, y0 + y, clr);

        (*_idraw.draw_line_vert)(this, x0 + y, y0 - x, x0 + y, y0 + x, clr);
        (*_idraw.draw_line_vert)(this, x0 - y, y0 - x, x0 - y, y0 + x, clr);
    }
}
////////////////////////////////////////////////////////////////////////////////////////
// bitmap()
//
// Draw a bitmap on the screen

void QwGrBufferDevice::bitmap(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,
                              uint8_t *pBitmap, uint8_t bmp_width, uint8_t bmp_height){

    (*_idraw.draw_bitmap)(this, x0, y0, x1, y1, pBitmap, bmp_width, bmp_height);
}

////////////////////////////////////////////////////////////////////////////////////////
// bitmap()
//
// Draw a bitmap on the screen

void QwGrBufferDevice::bitmap(uint8_t x0, uint8_t y0, uint8_t *pBitmap, uint8_t bmp_width, uint8_t bmp_height ){

    (*_idraw.draw_bitmap)(this, x0, y0, bmp_width, bmp_height, pBitmap, bmp_width, bmp_height);
}

////////////////////////////////////////////////////////////////////////////////////////
// bitmap() - use a bitmap object
//
void QwGrBufferDevice::bitmap(uint8_t x0, uint8_t y0, QwBitmap &theBMP){

    // just pass to graphics device
    (*_idraw.draw_bitmap)(this, x0, y0, theBMP.width, theBMP.height,
                          (uint8_t *)theBMP.data(), theBMP.width, theBMP.height);
}
////////////////////////////////////////////////////////////////////////////////////////
// text()
//
// Draw a string on the screen using the current font

void QwGrBufferDevice::text(uint8_t x0, uint8_t y0, const char *text, uint8_t clr){

    if(!text || x0 >= _viewport.width || y0 >= _viewport.height)
        return;

    (_idraw.draw_text)(this, x0, y0, text, clr);
}
////////////////////////////////////////////////////////////////////////////////////////
// draw_test()
//
// Draw text - one pixel at a time. Based on the algorithm in the Micro OLED
// Arduino library
//
void QwGrBufferDevice::draw_text(uint8_t x0, uint8_t y0, const char *text, uint8_t clr){

    // check things
    if(!_currFont || !text)
        return;

    int slen = strlen(text);

    if(!slen) // empty string?
        return;

    uint8_t nRows = _currFont->height / kByteNBits;
    if(!nRows)
        nRows = 1;

    // 5x7 font is special - need to add a margin
    uint8_t margin5x7 = (nRows == 1); // For the 5x7 font

    // used in loop
    uint16_t nRowLen = _currFont->map_width / _currFont->width; // how long is a
    uint16_t rowBytes = _currFont->map_width * nRows;

    // get font data
    const uint8_t *pFont = _currFont->data();

    // vars for the loops ...
    uint16_t charOffset, fontIndex;
    uint8_t rowOffset, currChar, i, j, row;

    // walk the string ... note: doing all loop incs here - to handle continue statement below

    for(int k = 0; k < slen; k++, text++, x0 += _currFont->width + margin5x7){

        // index into the font data - takes into account font size, rows span ... etc
        charOffset = *text - _currFont->start;

        // does the char location exceed the number of chars in the font?
        if(charOffset >= _currFont->n_chars)
            continue; // neeeexxxtt!

        // offset into the font data array - start location
        fontIndex = (charOffset / nRowLen * rowBytes) + ((charOffset % nRowLen) * _currFont->width);

        // Now walk the rows of this font entry (it can span bytes)
        for(row = 0; row < nRows; row++){

            rowOffset = row * kByteNBits; // y offset for multi row fonts - used in pixel draw

            // walk the width of the font
            for(i = 0; i < _currFont->width + margin5x7; i++){

                // this is done in a weird way because the 5x7 font has no margin
                if(margin5x7 && i == _currFont->width)
                    continue; // skip to next

                // finally - data!
                currChar = pgm_read_byte(pFont + fontIndex + i + (row * _currFont->map_width));

                // draw bits
                for(j = 0; j < kByteNBits; j++)
                    if(currChar & byte_bits[j])
                        (*_idraw.draw_pixel)(this, x0 + i, y0 + j + rowOffset, clr);

            } // walk font width

        } // row loop

    } // string loop
}
