/*
  Example-01_Hello.ino

  This demo shows the various methods available for drawing shapes using the OLED library

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

// Global variables - used to stash our screen size

int width;
int height;

void setup()
{
    Serial.begin(115200);
    Serial.println("Running OLED example");

    Wire.begin();

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
}

void loop()
{
    // Loop over our test functions
    for (int i = 0; i < 7; i++)
    {
        myOLED.erase();

        if (i == 0)
            lineTest1();
        else if (i == 1)
            lineTest2();
        else if (i == 2)
            lineTestVertical();
        else if (i == 3)
            rectangleTest();
        else if (i == 4)
            rectangleTestMove();
        else if (i == 5)
            rectangleFillTest();
        else if (i == 6)
            circleTest();

        myOLED.display();

        delay(1000);
    }
}

////////////////////////////////////////////////////////////////////////////////
// lineTest1()
//
// This test draws a horizontal line on the screen, moving it from the
// top of the screen to the bottom.
//
// This sequence repeats multiple times, starting with a small line and
// growing to the width of the screen.
//
// Since the library uses a "dirty range" methodology to minimize the amount of
// data sent to the device, as the lenght of the line increases, the rate
// of the display updates also decreases.  A longer line, results in more data being
// sent to the display.

void lineTest1(void)
{
    int x, y, i;

    int mid = width / 2;
    int delta = mid / 8;

    for (int j = 1; j < 8; j++)
    {

        x = delta * j;

        for (i = 0; i < height * 2; i++)
        {

            y = i % height;
            myOLED.erase();
            myOLED.line(mid - x, y, mid + x, y);
            myOLED.display();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// lineTest2()
//
// This test draws a series of lines bursting out from the corner of the display.
//
// It demostrates the abilty of the library to draw arbitray straight lines.

void lineTest2(void)
{
    for (int i = 0; i < width; i += 6)
    {
        myOLED.line(0, 0, i, height - 1);
        myOLED.display();
    }
    delay(200);
    myOLED.erase();
    for (int i = width - 1; i >= 0; i -= 6)
    {
        myOLED.line(width - 1, 0, i, height - 1);
        myOLED.display();
    }
}

////////////////////////////////////////////////////////////////////////////////
// lineTestVertIterative()
//
// Draws a series of vertical lines, some horz lines and two diag lines.
//
// Internally, this uses the standard line algorithm  (diag), fast horz and fast
// vertical lines functions.
//
// Iterator function - called to animate graphic

void lineTestVerticalIterative(uint8_t y0, uint8_t y1)
{
    for (int i = 0; i < width; i += 8)
        myOLED.line(i, y0, i, y1);

    // end off the vertical lines
    myOLED.line(width - 1, y0, width - 1, y1);

    // End lines and cross lines
    myOLED.line(0, y0, width - 1, y0);
    myOLED.line(0, y1, width - 1, y1);
    myOLED.line(0, y0, width - 1, y1);
    myOLED.line(0, y1, width - 1, y0);
}

// Entry point for test
void lineTestVertical(void)
{
    int mid = height / 2;

    for (int i = 0; i < height; i += 4)
    {

        myOLED.erase();
        lineTestVerticalIterative(mid - i / 2, mid + i / 2);
        myOLED.display();
        delay(10);
    }
}

////////////////////////////////////////////////////////////////////////////////
// rect_test()
//
// Simple - draw a rectangle test

void rectangleTest(void)
{
    myOLED.rectangle(4, 4, width - 8, height - 8);
}

////////////////////////////////////////////////////////////////////////////////
// rectangleTestMove()
//
// Draws a rectangle, then moves it across the screen
//
void rectangleTestMove(void)
{
    float steps = height;
    float xinc = width / steps;
    float yinc = height / steps;
    int side = 10;
    float x = 0;
    float y = 0;

    for (int i = 0; i < steps; i++)
    {
        // Draw the rectangle and send it to device
        myOLED.rectangle(x, y, side, side);
        myOLED.display(); // sends erased rect and new rect pixels to device

        // Erase the that rect, increment and loop
        myOLED.rectangle(x, y, side, side, 0);

        x += xinc;
        y += yinc;
    }
}

////////////////////////////////////////////////////////////////////////////////
// rectangleFillTest()
//
// Draws two filled rectangles, switches to XOR draw mode and overwrites them
// with another rectangle

void rectangleFillTest(void)
{
    myOLED.rectangleFill(4, 4, width / 2 - 8, height - 8);

    myOLED.rectangleFill(width / 2 + 4, 4, width / 2 - 8, height - 8);

    myOLED.setDrawMode(grROPXOR); // xor
    myOLED.rectangleFill(width / 4, 8, width / 2, height - 16);
    myOLED.setDrawMode(grROPCopy); // back to copy op (default)
}

////////////////////////////////////////////////////////////////////////////////
// circleTest()
//
// Draw a series of circles - filled and not filled
void circleTest(void)
{
    // Let's draw some circles that fit on the device
    myOLED.circle(width / 4, height / 2, height / 3);

    myOLED.circleFill(width - width / 4, height / 2, height / 3);

    myOLED.circle(4, height / 2, height / 3);

    myOLED.circleFill(width - width / 2, height / 2, height / 4);
}