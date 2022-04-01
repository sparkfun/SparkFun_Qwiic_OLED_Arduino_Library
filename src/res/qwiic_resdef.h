// qwiic_resdef.h
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
// Define resource objects/structures to manage fonts and bitmaps

//
#pragma once

// NOTE: The way I'm including the font/resource defines, some platforms have
//       include file issues

#include "qw_pgm_arduino.h"

#include <stdint.h>

////////////////////////////////////////////////////////////////////////
// Notes  Template singletons for managing resources
////////////////////////////////////////////////////////////////////////
//
// The common use/storage of bitmap and font data is creating a static
// array and placing it in a header file.
//
// This pattern is fine for simple uses, where the bitmap is only included
// in a single file. BUT if the bitmap header is included in multiple files,
// multiple copies of the bitmap are created.
//
// You could just make the bitmap array a const - but then multiple includes
// will lead to symbol collision at link time - confusing the user.
//
// These simple classes help place the bitmap data in a singleton class AND
// only create one copy/instance of the bitmap data no matter how many times a
// resource header file is included.
//
// The Plan:
//      - Define a base resource class
//          - attributes as public instance vars
//          - Define a constructor that init's the instance vars
//          - Resource data access via virtual accessor - make it const
//
//      - Create a template that subclasses the resource class, and defines a singleton
//          - It ensures only once of these classes is ever created.
//
//      - In a seperate header file - one for each resource, define the resource
//          - For example - for a bitmap - the data array, #defines for width and height
//          - The data array is declard as const static
//
//      - In another header file - create a subclass of our singleton
//          ex:
//            class QwBMPTruck final : public bitmapSingleton<QwBMPTruck> {
//
//          - Override the base class data accessor method
//          - In the body of this method, include the resource defined header file
//              - This defines the resource data as a static in this method. So only one copy is made
//                  ex:
//                      const uint8_t * bitmap(void){
//                  #include "bmp_truck.h"
//                          return bmp_truck_data;
//                      }
//
//          - After this resource access method, define the constructor of this class
//              - This is done after so it can use the attribute #defines of the included resource def header
//              - Have the constructor call it's superclass constructor, passing in resource attributes
//                  ex:
//                      QwBMPTruck(): bitmapSingleton<QwBMPTruck>(BMP_TRUCK_WIDTH, BMP_TRUCK_HEIGHT){}
//
//          - Lastly, define a macro that makes the syntax of access easy. This macro calls the instance
//              method of the singleton, getting access to the object that contains the data of the resource
//              ex:
//                  #define QW_BMP_TRUCK QwBMPTruck::instance()
//
//              To the user, they just have a bitmap, referenced as QW_BMP_TRUCK and can do the following
//                  ex:
//                      uint8_t width = QW_BMP_TRUCK.width;
//                      uint8_t height = QW_BMP_TRUCK.height;
//
//              And internal methods get the data of this bitmap object using the bitmap() method
//                  ex:
//                      const uint8_t * pData = QW_BMP_TRUCK.data();
//
//  It shoulds complicated - it isn't. Just look at examples in ths folder and copy when
//  adding new resources.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
// Simple Bitmap class definition

class QwBitmap {

public:
    uint8_t width;
    uint8_t height;
    virtual const uint8_t* data(void) { return nullptr; };

protected:
    QwBitmap(uint8_t w, uint8_t h)
        : width { w }
        , height { h }
    {
    }
};

// Template that creates a singleton for bitmaps.
template <typename T>
class bmpSingleton : public QwBitmap {
public:
    static T& instance(void)
    {
        static T instance;
        return instance;
    }

    bmpSingleton(const bmpSingleton&) = delete;
    bmpSingleton& operator=(const bmpSingleton) = delete;

protected:
    bmpSingleton() { }
    using QwBitmap::QwBitmap; // inherit contructor
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// Font things - class to hold font attributes

class QwFont {

public:
    uint8_t width;
    uint8_t height;
    uint8_t start;
    uint8_t n_chars;
    uint16_t map_width;
    const char* name;

    virtual const uint8_t* data(void) { return nullptr; };

protected:
    QwFont(uint8_t w, uint8_t h, uint8_t st_chr, uint8_t n_chr, uint16_t m_w, const char* f_name)
        : width { w }
        , height { h }
        , start { st_chr }
        , n_chars { n_chr }
        , map_width { m_w }
        , name { f_name }
    {
    }
};

// Template that creates a singleton for bitmaps.
template <typename T>
class fontSingleton : public QwFont {
public:
    static T& instance(void)
    {
        static T instance;
        return instance;
    }

    fontSingleton(const fontSingleton&) = delete;
    fontSingleton& operator=(const fontSingleton) = delete;

protected:
    fontSingleton() { }
    using QwFont::QwFont; // inherit constructor
};
