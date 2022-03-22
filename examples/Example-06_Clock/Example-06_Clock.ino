/*
  Example-06_Clock.ino

  Draws a clock face on the OLED display. This is a port of the demo for the original Micro OLED library.

  This library configures and draws graphics to OLED boards that use the
  SSD1306 display hardware. The library only supports I2C.

  SparkFun sells these at its website: www.sparkfun.com

  Do you like this library? Help support SparkFun. Buy a board!

   Micro OLED             https://www.sparkfun.com/products/14532
   Transparent OLED       https://www.sparkfun.com/products/15173
   "Narrow" OLED          https://www.sparkfun.com/products/17153

  Written by
      Jim Lindblom @ SparkFun Electronics
      Original Creation Date: October 27, 2014

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

// Use these variables to set the initial time
int hours = 11;
int minutes = 50;
int seconds = 30;

// How fast do you want the clock to spin? Set this to 1 for fun.
// Set this to 1000 to get _about_ 1 second timing.
const int CLOCK_SPEED = 1000;

// Global variables to help draw the clock face:
const int MIDDLE_Y = myOLED.getHeight() / 2;
const int MIDDLE_X = myOLED.getWidth() / 2;

int CLOCK_RADIUS;
int POS_12_X, POS_12_Y;
int POS_3_X, POS_3_Y;
int POS_6_X, POS_6_Y;
int POS_9_X, POS_9_Y;
int S_LENGTH;
int M_LENGTH;
int H_LENGTH;

unsigned long lastDraw = 0;

QwiicFont *pFont;

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

    pFont = myOLED.getFont();

    initClockVariables();

    drawFace();
    drawArms(hours, minutes, seconds);
    myOLED.display(); // display the memory buffer drawn
}

void loop()
{
    // Check if we need to update seconds, minutes, hours:
    if (lastDraw + CLOCK_SPEED < millis())
    {
        lastDraw += CLOCK_SPEED;

        // Add a second, update minutes/hours if necessary:
        updateTime();

        // Draw the clock:
        myOLED.erase();

        drawFace();                        // Draw the face to the buffer
        drawArms(hours, minutes, seconds); // Draw arms to the buffer

        myOLED.display(); // Draw the memory buffer
    }
}

void initClockVariables()
{
    // Calculate constants for clock face component positions:

    CLOCK_RADIUS = min(MIDDLE_X, MIDDLE_Y) - 1;
    POS_12_X = MIDDLE_X - pFont->width;
    POS_12_Y = MIDDLE_Y - CLOCK_RADIUS + 2;
    POS_3_X = MIDDLE_X + CLOCK_RADIUS - pFont->width - 1;
    POS_3_Y = MIDDLE_Y - pFont->height / 2;
    POS_6_X = MIDDLE_X - pFont->width / 2;
    POS_6_Y = MIDDLE_Y + CLOCK_RADIUS - pFont->height - 1;
    POS_9_X = MIDDLE_X - CLOCK_RADIUS + pFont->width - 2;
    POS_9_Y = MIDDLE_Y - pFont->height / 2;

    // Calculate clock arm lengths
    S_LENGTH = CLOCK_RADIUS - 2;
    M_LENGTH = S_LENGTH * 0.7;
    H_LENGTH = S_LENGTH * 0.5;
}

// Simple function to increment seconds and then increment minutes
// and hours if necessary.
void updateTime()
{
    seconds++; // Increment seconds
    if (seconds >= 60)
    { // If seconds overflows (>=60)

        seconds = 0; // Set seconds back to 0
        minutes++;   // Increment minutes

        if (minutes >= 60)
        { // If minutes overflows (>=60)

            minutes = 0; // Set minutes back to 0
            hours++;     // Increment hours

            if (hours >= 12) // If hours overflows (>=12)
                hours = 0;   // Set hours back to 0
        }
    }
}

// Draw the clock's three arms: seconds, minutes, hours.
void drawArms(int h, int m, int s)
{
    double midHours; // this will be used to slightly adjust the hour hand
    static int hx, hy, mx, my, sx, sy;

    // Adjust time to shift display 90 degrees ccw
    // this will turn the clock the same direction as text:
    h -= 3;
    m -= 15;
    s -= 15;

    if (h <= 0)
        h += 12;

    if (m < 0)
        m += 60;

    if (s < 0)
        s += 60;

    // Calculate and draw new lines:
    s = map(s, 0, 60, 0, 360);                  // map the 0-60, to "360 degrees"
    sx = S_LENGTH * cos(PI * ((float)s) / 180); // woo trig!
    sy = S_LENGTH * sin(PI * ((float)s) / 180); // woo trig!

    // draw the second hand:
    myOLED.line(MIDDLE_X, MIDDLE_Y, MIDDLE_X + sx, MIDDLE_Y + sy);

    m = map(m, 0, 60, 0, 360);                  // map the 0-60, to "360 degrees"
    mx = M_LENGTH * cos(PI * ((float)m) / 180); // woo trig!
    my = M_LENGTH * sin(PI * ((float)m) / 180); // woo trig!

    // draw the minute hand
    myOLED.line(MIDDLE_X, MIDDLE_Y, MIDDLE_X + mx, MIDDLE_Y + my);

    midHours = minutes / 12;                    // midHours is used to set the hours hand to middling levels between whole hours
    h *= 5;                                     // Get hours and midhours to the same scale
    h += midHours;                              // add hours and midhours
    h = map(h, 0, 60, 0, 360);                  // map the 0-60, to "360 degrees"
    hx = H_LENGTH * cos(PI * ((float)h) / 180); // woo trig!
    hy = H_LENGTH * sin(PI * ((float)h) / 180); // woo trig!

    // draw the hour hand:
    myOLED.line(MIDDLE_X, MIDDLE_Y, MIDDLE_X + hx, MIDDLE_Y + hy);
}

// Draw an analog clock face
void drawFace()
{
    // Draw the clock border
    myOLED.circle(MIDDLE_X, MIDDLE_Y, CLOCK_RADIUS);

    // Draw the clock numbers

    myOLED.setCursor(POS_12_X, POS_12_Y); // points cursor to x=27 y=0
    myOLED.print(12);
    myOLED.setCursor(POS_6_X, POS_6_Y);
    myOLED.print(6);
    myOLED.setCursor(POS_9_X, POS_9_Y);
    myOLED.print(9);
    myOLED.setCursor(POS_3_X, POS_3_Y);
    myOLED.print(3);
}