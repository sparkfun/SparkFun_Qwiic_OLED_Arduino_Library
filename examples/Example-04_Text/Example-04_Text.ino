/*
  Example-04_Text.ino

  This demo shows writing text to an OLED screen using various fonts

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

// Fonts
#include <res/qw_fnt_5x7.h>
#include <res/qw_fnt_8x16.h>
#include <res/qw_fnt_31x48.h>
#include <res/qw_fnt_7segment.h>
#include <res/qw_fnt_largenum.h>

// An array of fonts to loop over
QwiicFont *demoFonts[] = {
    &QW_FONT_5X7,
    &QW_FONT_8X16,
    &QW_FONT_31X48,
    &QW_FONT_LARGENUM,
    &QW_FONT_7SEGMENT};
int nFONTS = sizeof(demoFonts) / sizeof(demoFonts[0]);
int iFont = 0;

// Some vars for the title.
String strTitle = "<<Font>>";
QwiicFont *pFntTitle = &QW_FONT_5X7;

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

}

void loop()
{
    // next font for display
    iFont = (iFont + 1) % nFONTS;
    myOLED.setFont(demoFonts[iFont]);

    // Write font name to screen
    writeTitle();

    delay(1000);

    // Write out the full font char set
    writeFontChars();

    delay(2000);
}

// For the current font, write out all its characters
void writeFontChars()
{
    // get the font
    QwiicFont * currFont = myOLED.getFont();

    // how many chars can a screen handle? (x * y)
    uint16_t screenChars = myOLED.getWidth() / (currFont->width + 1); // X
    uint8_t nY = myOLED.getHeight() / currFont->height;               // Y

    screenChars *= (nY == 0 ? 1 : nY); // need at least 1 row

    // Loop over the characters in the font.
    for (int i = 0; i < currFont->n_chars; i++)
    {

        if (i % screenChars == 0)
        { // next page
            delay(400);
            myOLED.erase();
            myOLED.setCursor(0, 0);
        }

        // if the character is a carriage return, send a blank - otherwise the
        // write routine will perform a CR and lead to a confusing display.
        myOLED.write((i + currFont->start != '\n') ? i + currFont->start : ' ');

        myOLED.display(); // show the added char

        delay(10);
    }
}

// Simple title for a font
void writeTitle()
{

    // Get the current font name
    String strTitle = myOLED.getFontName();

    // save our current font, then switch to font for title
    QwiicFont * currFont = myOLED.getFont();
   
    // Set title font font
    myOLED.setFont(pFntTitle);

    // Position to use for the time/banner displayed before each font

    // Get the width of the title in screen size
    int width = myOLED.getStringWidth(strTitle);

    // if the string is wider than the screen, set x at 0, otherwise center text

    int xTitle = (width >= myOLED.getWidth() ? 0 : (myOLED.getWidth() - width) / 2);

    // starting y position - width minus string height / 2. No need to chech height, 5x7 font fits everything
    int yTitle = (myOLED.getHeight() - myOLED.getStringHeight(strTitle)) / 2;

    myOLED.erase();

    // Draw the text
    myOLED.text(xTitle, yTitle, strTitle);

    // There's nothing on the screen yet - Now send the graphics to the device
    myOLED.display();

    myOLED.setFont(currFont);
}
