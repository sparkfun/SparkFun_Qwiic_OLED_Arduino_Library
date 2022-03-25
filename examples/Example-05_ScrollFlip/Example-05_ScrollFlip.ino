/*
  Example-05_ScrollFlip.ino

  This demo shows the various display options - scrolling, flipping, invert.

  NOTE: Scrolling isn't supported on the Transparent OLED

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

int yoffset;

// Our testing functions
void scrollRight(void)
{
    myOLED.scrollStop();
    myOLED.scrollRight(0, 7, SCROLL_INTERVAL_2_FRAMES);
}

void scrollRightVertical(void)
{
    myOLED.scrollStop();
    myOLED.scrollVertRight(0, 7, SCROLL_INTERVAL_3_FRAMES);
}

void scrollLeft(void)
{
    myOLED.scrollStop();
    myOLED.scrollLeft(0, 7, SCROLL_INTERVAL_4_FRAMES);
}

void scrollLeftVertical(void)
{
    myOLED.scrollStop();
    myOLED.scrollVertLeft(0, 7, SCROLL_INTERVAL_5_FRAMES);
}

void scrollStop(void)
{
    myOLED.scrollStop();
}

void flipHorizontal(void)
{
    for (int i = 0; i < 6; i++)
    {
        myOLED.flipHorizontal(!(i & 0x01));
        delay(800);
    }
}

void flipVertical(void)
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

// Use an array of testing functions, with a title, to run the tests
typedef void (*testFn)(void);
typedef struct _testRoutines
{
    void (*testFn)(void);
    const char *title;
} testRoutine;

static const testRoutine testFunctions[] = {
    {scrollRight, "Right>"},
    {scrollRightVertical, "^Right-Up>"},
    {scrollLeft, "<Left"},
    {scrollLeftVertical, "<Left-Up^"},
    {scrollStop, "<STOP>"},
    {flipHorizontal, "-Flip-Horz-"},
    {flipVertical, "|Flip-Vert|"},
    {invert, "**INVERT**"}};

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

    yoffset = (myOLED.getHeight() - myOLED.getStringHeight("a")) / 2;

    delay(1000);
}

void loop()
{
    // Loop over the test table entries, write title and run test.
    for (int i = 0; i < sizeof(testFunctions) / sizeof(testFunctions[0]); i++)
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
