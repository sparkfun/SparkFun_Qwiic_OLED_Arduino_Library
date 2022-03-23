
// Example-03_Bitmap.ino
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
// Written by Nathan Seidle @ SparkFun Electronics
// Original Creation Date: November 15, 2020
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
// Example 3 for the SparkFun Qwiic OLED Arduino Library
//
// >> Overview <<
//
// This demo shows how to write a simple bitmap to an OLED screen
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

// Let's draw a truck - use our built in bitmap
#include "res/qw_bmp_truck.h"
#include "res/qw_bmp_sparkfun.h"

int width;
int height;

// For simple frame rate calculations
uint32_t draw_total_time;
uint32_t n_draws;

////////////////////////////////////////////////////////////////////////////////////////////////
//  show_splash()
//
// Show SFE spash screen

void show_splash()
{

  int x0 = (width - QW_BMP_SPARKFUN.width) / 2;
  if (x0 < 0)
    x0 = 0;

  int y0 = (height - QW_BMP_SPARKFUN.height) / 2;
  if (y0 < 0)
    y0 = 0;

  myOLED.erase();
  myOLED.bitmap(x0, y0, QW_BMP_SPARKFUN);
  myOLED.display();
  delay(2000);
}
////////////////////////////////////////////////////////////////////////////////////////////////
// setup()
//
// Standard Arduino setup routine

void setup()
{

  delay(500); // Give display time to power on
  Serial.begin(115200);

  Serial.println("\n\r-----------------------------------");

  Serial.print("Running Test #2 on: ");
  Serial.println(String(deviceName));

  if (!myOLED.begin())
  {

    Serial.println("- Device Begin Failed");
    while (1)
      ;
  }

  Serial.println("- Begin Successful");

  // save device dims for the test routines
  width = myOLED.getWidth();
  height = myOLED.getHeight();

  show_splash();

  draw_total_time = 0;
  n_draws = 0;

  // set a template for our framerate display
  Serial.println("- Frame Rate");
}

int iconX = 8;
int iconXChangeAmount = 1;
int iconY = 8;
int iconYChangeAmount = 1;

////////////////////////////////////////////////////////////////////////////////////////////////
// loop()
//
// Standard Arduino loop routine

void loop()
{

  // Calculate draw time...
  uint32_t milStart = millis();

  myOLED.bitmap(iconX, iconY, QW_BMP_TRUCK);
  myOLED.display();

  // Move the icon
  iconX += iconXChangeAmount;
  iconY += iconYChangeAmount;

  if (iconX + QW_BMP_TRUCK.width >= width)
    iconXChangeAmount *= -1; // Change direction

  if (iconX == 0)
    iconXChangeAmount *= -1; // Change direction

  if (iconY + QW_BMP_TRUCK.height >= height)
    iconYChangeAmount *= -1; // Change direction

  if (iconY == 0)
    iconYChangeAmount *= -1; // Change direction

  draw_total_time += millis() - milStart;
  n_draws++;

  // output framerate?
  if (n_draws % 120 == 0)
  {

    Serial.println(((float)draw_total_time) / n_draws);

    if (n_draws > 1000)
    { // reset after a bit...
      n_draws = 0;
      draw_total_time = 0;
    }
  }
}
