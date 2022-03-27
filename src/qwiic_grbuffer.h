
// qwiic_grbuffer.h
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
//

// A Graphics System base class for qwiic display devices - simple bmp based devices.

#pragma once

// required on some platforms
#include <stdint.h>

// Include resource definitions
#include "res/qwiic_resdef.h"

// RECT!
struct QwRect{
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
};

//////////////////////////////////////////////////////////////////////////////////
// Utils
//
// Bit level tools/helpers
//
// Handy const = save some compute used bit shifting
extern const uint8_t byte_bits[8];

// something more readable
#define kByteNBits 8

// Mod 8 is really just the lower 3 bits of a value. This might be faster than
// standard mod operator - maybe
#define mod_byte(_value_) (_value_ & 0x07)

// Some rouintes need a int swap function. Many ways to do this- all the cool kids
// use bitwise ops  - namely the xor.
//
#define swap_int(_a_, _b_) (((_a_) ^= (_b_)), ((_b_) ^= (_a_)), ((_a_) ^= (_b_)))

///////////////////////////////////////////////////////////////////////////////////////////
// _QwIDraw
//
// IDraw interface for the graphics system. Defines methods used to draw 2D primatives.
//
// Seperated out to enable easy vtable access and method dispatch short circuiting

class _QwIDraw {

    // Pixel Methods
    virtual void draw_pixel(uint8_t x, uint8_t y, uint8_t clr) {} // A subclass must implement this

    // Line Methods
    virtual void draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr) {}
    virtual void draw_line_horz(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr) {}
    virtual void draw_line_vert(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr) {}

    // Rectangles
    virtual void draw_rect(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, uint8_t clr) {}
    virtual void draw_rect_filled(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, uint8_t clr) {}

    // Circles
    virtual void draw_circle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t clr) {}
    virtual void draw_circle_filled(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t clr) {}

    // Bitmaps
    virtual void draw_bitmap(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,
                             uint8_t *pBitmap, uint8_t bmp_width, uint8_t bmp_height) {}

    virtual void draw_text(uint8_t x0, uint8_t y0, const char *text, uint8_t clr) {}
};

// Drawing fuction typedefs
typedef void (*QwDrawPntFn)(void *, uint8_t, uint8_t, uint8_t);
typedef void (*QwDrawTwoPntFn)(void *, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
typedef void (*QwDrawCircleFn)(void *, uint8_t, uint8_t, uint8_t, uint8_t);
typedef void (*QwDrawBitmapFn)(void *, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t *, uint8_t, uint8_t);
typedef void (*QwDrawTextFn)(void *, uint8_t, uint8_t, const char *, uint8_t);

// Define the vtable struct for IDraw
struct _QwIDraw_vtable {
    QwDrawPntFn draw_pixel;
    QwDrawTwoPntFn draw_line;
    QwDrawTwoPntFn draw_line_horz;
    QwDrawTwoPntFn draw_line_vert;
    QwDrawTwoPntFn draw_rect;
    QwDrawTwoPntFn draw_rect_filled;
    QwDrawCircleFn draw_circle;
    QwDrawCircleFn draw_circle_filled;
    QwDrawBitmapFn draw_bitmap;
    QwDrawTextFn draw_text;
};

///////////////////////////////////////////////////////////////////////////////////////////
//  QwGrBufferDevice
//
//  Buffer class - defines basics for a memory buffer drawing class. Note it subclasses
//  from QwIDraw

class QwGrBufferDevice : protected _QwIDraw
{

public:
    // Constructors
    QwGrBufferDevice() : _currFont{nullptr} {};
    QwGrBufferDevice(uint8_t width, uint8_t height) : QwGrBufferDevice(0, 0, width, height){};
    QwGrBufferDevice(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height){
        set_viewport(x0, y0, width, height);
    };

    // Buffer location on the device
    void set_viewport(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height){
        _viewport.x = x0;
        _viewport.y = y0;
        _viewport.width = width;
        _viewport.height = height;
    };

    QwRect get_viewport(void) { return _viewport; };
    uint16_t get_origin_x(void) { return _viewport.x; };
    uint16_t get_origin_y(void) { return _viewport.y; };
    uint16_t get_width(void) { return _viewport.width; };
    uint16_t get_height(void) { return _viewport.height; };

    // Lifecycle
    virtual bool init(void);

    // Font methods
    void init_font(void);
    void set_font(QwFont &font);
    void set_font(const QwFont *font);
    QwFont *get_font(void);

    // Returns the size of a string - in pixels - using current font
    bool get_string_size(const char *text, uint16_t &width, uint16_t &height);

    // Public Interface - Graphics interface
    void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr = 1);

    void circle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t clr = 1);
    void circle_fill(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t clr = 1);

    void pixel(uint8_t x, uint8_t y, uint8_t clr = 1);

    void rectangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr = 1);
    void rectangle_fill(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr = 1);


    void bitmap(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, 
                uint8_t *pBitmap, uint8_t bmp_width, uint8_t bmp_height );

    // draw full bitmap
    void bitmap(uint8_t x0, uint8_t y0, uint8_t *pBitmap, uint8_t bmp_width, uint8_t bmp_height );

    // Bitmap draw - using a bitmap object
    void bitmap(uint8_t x0, uint8_t y0, QwBitmap &bitmap);

    void text(uint8_t x0, uint8_t y0, const char *text, uint8_t clr = 1);

    // subclass interface
    virtual void display(void) = 0;
    virtual void erase(void) = 0;

protected:
    QwRect _viewport;

    // Internal, fast draw routines - These implement QwIDraw

    // Pixels
    //    At a minimum, a sub-class must implement a pixel set function
    virtual void draw_pixel(uint8_t x, uint8_t y, uint8_t clr) = 0; // A subclass must implement this

    // Lines
    //     Sub-class implementation is optional
    virtual void draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr);

    // Rects
    virtual void draw_rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr);
    virtual void draw_rect_filled(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr);

    // Circle
    virtual void draw_circle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t clr);
    virtual void draw_circle_filled(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t clr);

    // Text
    virtual void draw_text(uint8_t x0, uint8_t y0, const char *text, uint8_t clr);

    // Our drawing interface - open to sub-classes ...
    _QwIDraw_vtable _idraw;

    // Current Font
    QwFont *_currFont;

private:
    bool init_draw_functions(void);
};