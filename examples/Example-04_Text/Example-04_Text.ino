
// Example-04_Text.ino
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
// Example 4 for the SparkFun Qwiic OLED Arduino Library
//
// >> Overview <<
//
// This demo shows writing text to an OLED screen using various fonts
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

// Fonts
#include <res/qw_fnt_5x7.h>
#include <res/qw_fnt_8x16.h>
#include <res/qw_fnt_31x48.h>
#include <res/qw_fnt_7segment.h>
#include <res/qw_fnt_largenum.h>

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

// An array of fonts to loop over
QwiicFont *demoFonts[] = {
    &QW_FONT_5X7,
    &QW_FONT_8X16,
    &QW_FONT_31X48,
    &QW_FONT_LARGENUM,
    &QW_FONT_7SEGMENT};
uint8_t nFONTS = sizeof(demoFonts) / sizeof(demoFonts[0]);
uint8_t iFont = 0;

// Some vars for the title.
uint8_t xTitle, yTitle;
String strTitle = "<<Font>>";
QwiicFont *pFntTitle = &QW_FONT_5X7;

////////////////////////////////////////////////////////////////////////////////////
//
void setup()
{

    delay(500); // Give display time to power on
    Serial.begin(115200);

    Serial.println("\n\r-----------------------------------");
    Serial.print("Example #4 on: ");
    Serial.println(String(deviceName));

    if (!myOLED.begin())
    {

        Serial.println("- Device Begin Failed");
        while (1)
            ;
    }

    Serial.println("- Begin Successful");

    // Position to use for the time/banner displayed before each font

    // starting x position - width minus string length (font width * number of characters) / 2
    xTitle = (myOLED.getWidth() - (pFntTitle->width + 1) * strTitle.length()) / 2;

    yTitle = (myOLED.getHeight() - pFntTitle->height) / 2;
}

////////////////////////////////////////////////////////////////////////////////////
// writeFontChars()
//
// For the current font, write out all it's characters

void writeFontChars()
{

    // get the font
    QwiicFont *pFont = myOLED.getFont();

    // how many chars can a screen handle? (x * y)
    uint16_t screenChars = myOLED.getWidth() / (pFont->width + 1); // X
    uint8_t nY = myOLED.getHeight() / pFont->height;               // Y

    screenChars *= (nY == 0 ? 1 : nY); // need at least 1 row

    // Loop over the characters in the font.
    for (int i = 0; i < pFont->n_chars; i++)
    {

        if (i % screenChars == 0)
        { // next page
            delay(400);
            myOLED.erase();
            myOLED.setCursor(0, 0);
        }

        // if the character is a carriage return, send a blank - otherwise the
        // write routine will perform a CR and lead to a confusing display.
        myOLED.write((i + pFont->start != '\n') ? i + pFont->start : ' ');

        myOLED.display(); // show the added char

        delay(10);
    }
}
////////////////////////////////////////////////////////////////////////////////////
// write_title()
//
// Simple title for a font

void write_title()
{

    // Set title font font
    myOLED.setFont(pFntTitle);

    myOLED.erase();

    // Draw the text
    myOLED.text(xTitle, yTitle, strTitle);

    // There's nothing on the screen yet - Now send the graphics to the device
    myOLED.display();
}

////////////////////////////////////////////////////////////////////////////////////
// loop()

void loop()
{

    // Write out a title
    write_title();

    delay(1000);

    // next font for display
    iFont = (iFont + 1) % nFONTS;
    myOLED.setFont(demoFonts[iFont]);

    // Write out the full font char set
    writeFontChars();

    delay(2000);
}
