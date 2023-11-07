/*

  Example-09_CustomOLED.ino

  This demo shows the basic setup of the OLED library, generating simple graphics and displaying
  the results on the target device.

   Micro OLED             https://www.sparkfun.com/products/14532
   Transparent OLED       https://www.sparkfun.com/products/15173
   "Narrow" OLED          https://www.sparkfun.com/products/17153

  Written by Kirk Benell @ SparkFun Electronics, March 2022

  Repository:
     https://github.com/sparkfun/SparkFun_Qwiic_OLED_Arduino_Library

  Documentation:
     https://sparkfun.github.io/SparkFun_Qwiic_OLED_Arduino_Library/

  SparkFun code, firmware, and software is released under the MIT License(http://opensource.org/licenses/MIT).
*/

#include <SparkFun_Qwiic_OLED.h> //http://librarymanager/All#SparkFun_Qwiic_OLED


// This demo shows how to use the QwiicCustomOLED class,
// allowing the display width, height etc. to be set manually.

QwiicCustomOLED myOLED;


void setup()
{
    delay(1000);
    
    Serial.begin(115200);
    Serial.println("Running OLED example");

    Wire.begin();

    // If desired, we can customize the OLED before we begin it.
    // Otherwise it will default to 128x64 (1.3" OLED).
    myOLED.m_device.xOffset = 0; // Set the active area X offset. For the Micro 64x48, set this to 2
    myOLED.m_device.yOffset = 0; // Set the active area Y offset
    myOLED.m_device.displayWidth = 128; // Set the active area width
    myOLED.m_device.displayHeight = 64; // Set the active area height
    myOLED.m_device.pinConfig = 0x12; // Set COM Pins Hardware Configuration (DAh)
    myOLED.m_device.preCharge = 0xF1; // Set Pre-charge Period (D9h)
    myOLED.m_device.vcomDeselect = 0x40; // Set VCOMH Deselect Level (DBh)
    myOLED.m_device.contrast = 0xCF; // Set Contrast Control for BANK0 (81h)

    // Initalize the OLED device and related graphics system
    if (myOLED.begin(Wire, 0x3D) == false) // The TwoWire port and I2C address are set here
    {
        Serial.println("Device begin failed. Freezing...");
        while (true)
            ;
    }
    Serial.println("Begin success");

    // Do a simple test - fill a rectangle on the screen and then print hello!

    // Fill a rectangle on the screen that has a 4 pixel board
    myOLED.rectangleFill(4, 4, myOLED.getWidth() - 8, myOLED.getHeight() - 8);

    String hello = "hello"; // our message

    // Center our message on the screen. Get the screen size of the "hello" string,
    // calling the getStringWidth() and getStringHeight() methods on the oled

    // starting x position - screen width minus string width  / 2
    int x0 = (myOLED.getWidth() - myOLED.getStringWidth(hello)) / 2;

    // starting y position - screen height minus string height / 2 
    int y0 = (myOLED.getHeight() - myOLED.getStringHeight(hello)) / 2;

    // Draw the text - color of black (0)
    myOLED.text(x0, y0, hello, 0);

    // There's nothing on the screen yet - Now send the graphics to the device
    myOLED.display();

    // That's it - HELLO!
}

void loop()
{
    delay(1000); // Do nothing
}
