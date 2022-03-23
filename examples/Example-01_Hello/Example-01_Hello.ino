/*
 Example-01_Hello.ino

 This is a library written for SparkFun Qwiic OLED boards that use the SSD1306.

 SparkFun sells these at its website: www.sparkfun.com

 Do you like this library? Help support SparkFun. Buy a board!

   Micro OLED             https://www.sparkfun.com/products/14532
   Transparent OLED       https://www.sparkfun.com/products/15173
   "Narrow" OLED          https://www.sparkfun.com/products/17153


 Written by Kirk Benell @ SparkFun Electronics, March 2022

 This library configures and draws graphics to OLED boards that use the
 SSD1306 display hardware. The library only supports I2C.

 Repository:
     https://github.com/sparkfun/SparkFun_Qwiic_OLED_Arduino_Library

 Documentation:
     https://sparkfun.github.io/SparkFun_Qwiic_OLED_Arduino_Library/


 SparkFun code, firmware, and software is released under the MIT License(http://opensource.org/licenses/MIT).

 SPDX-License-Identifier: MIT

 Example 01 for the SparkFun Qwiic OLED Arduino Library

 >> Overview <<

 This demo shows the basic setup of the OLED library, generating simple graphics and displaying
 the results on the target device.
*/
////////////////////////////////////////////////////////////////////////////////////////
// >>> SELECT THE CONNECTED DEVICE FOR THIS EXAMPLE <<<
//
// The Library supports three different types of SparkFun boards. The demo uses the following
// defines to determine which device is being used. Uncomment the device being used for this demo.

// The default is Micro OLED

#define MICRO
//#define NARROW
//#define TRANSPARENT

//////////////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>

// Include the SparkFun qwiic OLED Library
#include <SparkFun_Qwiic_OLED.h>

// Add in our font descriptor -- so we can center the text on the scren
#include <res/qw_fnt_5x7.h>

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

////////////////////////////////////////////////////////////////////////////////////////////////
// setup()

void setup()
{
    delay(500); // Give display time to power on

    // Serial on!
    Serial.begin(115200);

    Serial.println("\n\r-----------------------------------");

    Serial.print("Running Example 01 on: ");
    Serial.println(String(deviceName));

    // Initalize the OLED device and related graphics system
    if (!myOLED.begin())
    {

        Serial.println(" - Device Begin Failed");
        while (1)
            ;
    }

    Serial.println("- Begin Success");

    // Do a simple test - fill a rectangle on the screen and then print hello!

    // fill a rectangle on the screen that has a 4 pixel board
    myOLED.rectangleFill(4, 4, myOLED.getWidth() - 8, myOLED.getHeight() - 8);

    String hello = "hello"; // our message

    // Center our message on the screen. Use our Font Descriptor: QW_FONT_5X7, the default
    // font of the system.

    // starting x position - width minus string length (font width * number of characters) / 2
    int x0 = (myOLED.getWidth() - QW_FONT_5X7.width * hello.length()) / 2;

    int y0 = (myOLED.getHeight() - QW_FONT_5X7.height) / 2;

    // Draw the text - color of black (0)
    myOLED.text(x0, y0, hello, 0);

    // There's nothing on the screen yet - Now send the graphics to the device
    myOLED.display();

    // That's it - HELLO!
}

// Standard Arduino loop function.
void loop()
{

    // All loop does in sleep.
    delay(1000);
}