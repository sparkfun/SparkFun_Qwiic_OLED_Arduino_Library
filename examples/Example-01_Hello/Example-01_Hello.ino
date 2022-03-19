
// Example-01_Hello.ino
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

// Example 01 for the SparkFun Qwiic OLED Arduino Library
//
// >> Overview <<
//
// This demo shows the basic setup of the OLED library, generating simple graphics and displaying
// the results on the target device. 
//
//////////////////////////////////////////////////////////////////////////////////////////
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
const char * deviceName = "Transparent OLED";

#elif defined(NARROW)
QwiicNarrowOLED myOLED;
const char * deviceName = "Narrow OLED";

#else
QwiicMicroOLED myOLED;
const char * deviceName = "Micro OLED";

#endif


////////////////////////////////////////////////////////////////////////////////////////////////
// setup()
// 
// Arduino setup() function. 
//
// Initialize our device and draw a simple graphic. That's all this example does.

void setup()
{
    delay(500);   //Give display time to power on

    // Serial on!
    Serial.begin(115200);

    Serial.println("\n\r-----------------------------------");

    Serial.print("Running Example 01 on: ");
    Serial.println(String(deviceName));

    // Initalize the OLED device and related graphics system
    if(!myOLED.begin()){

        Serial.println(" - Device Begin Failed");
        while(1);
    }

    Serial.println("- Begin Success");

    // Do a simple test - fill a rectangle on the screen and then print hello!

    // fill a rectangle on the screen that has a 4 pixel board

    myOLED.rectangleFill(4, 4, myOLED.getWidth()-4, myOLED.getHeight()-4);

    String hello = "hello"; // our message
    
    // Lets center our message on the screen. We need to current font. 

    QwiicFont * pFont = myOLED.getFont();

    // starting x position - width minus string length (font width * number of characters) / 2
    int x0 = (myOLED.getWidth() - pFont->width * hello.length())/2;

    int y0 = (myOLED.getHeight() - pFont->height)/2;

    // Draw the text - color of black (0)
    myOLED.text(x0, y0, hello, 0);

    // There's nothing on the screen yet - Now send the graphics to the device
    myOLED.display();

    // That's it - HELLO! 
  
}

// Standard Arduino loop function. 
void loop(){

    // All loop does in sleep. 
    delay(1000);
}