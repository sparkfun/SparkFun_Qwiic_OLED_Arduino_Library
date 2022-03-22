/*
  Example-03_Bitmap.ino

  This demo shows how to write a simple bitmap to an OLED screen

  This library configures and draws graphics to OLED boards that use the
  SSD1306 display hardware. The library only supports I2C.

  SparkFun sells these at its website: www.sparkfun.com

  Do you like this library? Help support SparkFun. Buy a board!

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

#include <SparkFun_Qwiic_OLED.h> //http://librarymanager/All#SparkFun_Qwiic_Graphic_OLED

// The Library supports three different types of SparkFun boards. The demo uses the following
// defines to determine which device is being used. Uncomment the device being used for this demo.

QwiicMicroOLED myOLED;
// QwiicTransparentOLED myOLED;
// QwiicNarrowOLED myOLED;

// Let's draw a truck - use our built in bitmap
#include "res/qw_bmp_truck.h"
#include "res/qw_bmp_sparkfun.h"

int width;
int height;

// For simple frame rate calculations
long drawTotalTime = 0;
int numberOfDraws = 0;

int iconX = 8;
int iconXChangeAmount = 1;
int iconY = 8;
int iconYChangeAmount = 1;

void setup()
{
  Serial.begin(115200);
  Serial.println("Running OLED example");

  Wire.begin();
  Wire.setClock(400000); // Optional increase I2C to 400kHz

  // Initalize the OLED device and related graphics system
  if (myOLED.begin() == false)
  {
    Serial.println("Device begin failed. Freezing...");
    while (true)
      ;
  }
  Serial.println("Begin success");

  // save device dims for the test routines
  width = myOLED.getWidth();
  height = myOLED.getHeight();

  showSplash();

  drawTotalTime = 0;
  numberOfDraws = 0;

  // set a template for our framerate display
  Serial.println("- Frame Rate");
}

void loop()
{
  // Calculate draw time
  long startTime = millis();

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

  numberOfDraws++;
  drawTotalTime += (millis() - startTime);

  // Output framerate once every 120 frames
  if (numberOfDraws % 120 == 0)
  {
    Serial.print(" Frame rate: ");
    Serial.println(numberOfDraws / (float)drawTotalTime * 1000.0);

    numberOfDraws = 0;
    drawTotalTime = 0;
  }
}

void showSplash()
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