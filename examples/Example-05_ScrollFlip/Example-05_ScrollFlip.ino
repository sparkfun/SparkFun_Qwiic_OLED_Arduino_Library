// Example-05_ScrollFlip.ino
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
//////////////////////////////////////////////////////////////////////////////////////////
// Example 5 for the SparkFun Qwiic OLED Arduino Library
//
// >> Overview <<
//
// This demo shows the various display options - scrolling, flipping, invert.
//
// NOTE: Scrolling isn't supported on the Transparent OLED
//
//////////////////////////////////////////////////////////////////////////////////////////
//
// >>> SELECT THE CONNECTED DEVICE FOR THIS EXAMPLE <<<
//
// The Library supports three different types of SparkFun boards. The demo uses the following
// defines to determine which device is being used. Uncomment the device being used for this demo.
//
// The default is Micro OLED

#define MICRO
//#define NARROW
//#define TRANSPARENT

//////////////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>

// Include the SparkFun qwiic OLED Library
#include <SparkFun_Qwiic_OLED.h>

// What device is being used in this demo

#if defined(TRANSPARENT)
QwiicTransparentOLED myOLED;
const char *deviceName = "Transparent OLED";

#elif defined(NARROW)
QwiicNarrowOLED myOLED;
const char *deviceName = "Narrow OLED";

#else
QwiicMicroOLED myOLED;
const char *deviceName = "Micro OLED";

#endif

int yoffset;

////////////////////////////////////////////////////////////////////////////////////////////////
// setup()
//
// Standard Arduino setup routine

void setup()
{
    delay(500); // Give display time to power on
    Serial.begin(115200);

    Serial.println("\n\r-----------------------------------");

    Serial.print("Running Test #5 on: ");
    Serial.println(String(deviceName));

    if (!myOLED.begin())
    {

        Serial.println("- Device Begin Failed");
        while (1)
            ;
    }

    yoffset = (myOLED.getHeight() - myOLED.getFont()->height) / 2;

    delay(1000);
}

// Our testing functions

void scroll_right(void)
{

    myOLED.scrollStop();
    myOLED.scrollRight(0, 7, SCROLL_INTERVAL_2_FRAMES);
}

void scroll_right_vert(void)
{
    myOLED.scrollStop();
    myOLED.scrollVertRight(0, 7, SCROLL_INTERVAL_3_FRAMES);
}

void scroll_left(void)
{
    myOLED.scrollStop();
    myOLED.scrollLeft(0, 7, SCROLL_INTERVAL_4_FRAMES);
}

void scroll_left_vert(void)
{
    myOLED.scrollStop();
    myOLED.scrollVertLeft(0, 7, SCROLL_INTERVAL_5_FRAMES);
}

void scroll_stop(void)
{
    myOLED.scrollStop();
}

void flip_horz(void)
{

    for (int i = 0; i < 6; i++)
    {
        myOLED.flipHorizontal(!(i & 0x01));
        delay(800);
    }
}

void flip_vert(void)
{
    for (int i = 0; i < 6; i++)
    {
        myOLED.flipVertical(!(i & 0x01));
        delay(800);
    }
}

void invert(void)
{
    for (int i = 0; i < 6; i++)
    {
        myOLED.invert(!(i & 0x01));
        delay(800);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Use an array of testing functions, with a title, to run the tests

typedef void (*testFn)(void);
typedef struct _testRoutines
{
    void (*testFn)(void);
    const char *title;
} testRoutine;

static const testRoutine testFunctions[] = {
    {scroll_right, "Right>"},
    {scroll_right_vert, "^Right-Up>"},
    {scroll_left, "<Left"},
    {scroll_left_vert, "<Left-Up^"},
    {scroll_stop, "<STOP>"},
    {flip_horz, "-Flip-Horz-"},
    {flip_vert, "|Flip-Vert|"},
    {invert, "**INVERT**"}};

////////////////////////////////////////////////////////////////////////////////////////////////
// loop()
//
// Standard Arduino loop routine
void loop()
{

    // loop over the test table entries, write title and run test.
    for (uint8_t i = 0; i < sizeof(testFunctions) / sizeof(testFunctions[0]); i++)
    {

        if (testFunctions[i].title)
        {
            myOLED.erase();
            myOLED.text(3, yoffset, testFunctions[i].title);
            myOLED.display();
        }
        testFunctions[i].testFn(); // run test

        delay(3000);
    }
}
