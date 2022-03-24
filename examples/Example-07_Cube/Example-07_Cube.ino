/*
  Example-07_Cube.ino

  This demo draws a rotating 3D cube

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

int width;
int height;

// For simple frame rate calculations
long drawTotalTime = 0;
int numberOfDraws = 0;

float d = 3;
float px[] = {-d, d, d, -d, -d, d, d, -d};
float py[] = {-d, -d, d, d, -d, -d, d, d};
float pz[] = {-d, -d, -d, -d, d, d, d, d};

float p2x[8] = {0};
float p2y[8] = {0};
float r[3] = {0};

#define SHAPE_SIZE 400
// Define how fast the cube rotates. Smaller numbers are faster.
// This is the number of ms between draws.
#define ROTATION_SPEED 00

void setup()
{
    Serial.begin(115200);
    Serial.println("Running OLED example");

    Wire.begin();
    // Wire.setClock(400000); //Optionally increase I2C bus speed to 400kHz

    // Initalize the OLED device and related graphics system
    if (myOLED.begin() == false)
    {
        Serial.println("Device begin failed. Freezing...");
        while (true)
            ;
    }
    Serial.println("Begin success");

    width = myOLED.getWidth();
    height = myOLED.getHeight();

    // For frame rate calc
    drawTotalTime = 0;
    numberOfDraws = 0;

    // Set a template for our framerate display
    Serial.print("- Frame Rate: 00.00");
}

void loop()
{
    // Draw the cube - as fast as we can!
    drawCube();
}

void drawCube()
{
    r[0] = r[0] + PI / 180.0; // Add a degree
    r[1] = r[1] + PI / 180.0; // Add a degree
    r[2] = r[2] + PI / 180.0; // Add a degree
    if (r[0] >= 360.0 * PI / 180.0)
        r[0] = 0;
    if (r[1] >= 360.0 * PI / 180.0)
        r[1] = 0;
    if (r[2] >= 360.0 * PI / 180.0)
        r[2] = 0;

    // This routine gets called often, so just make these statics
    static float px2, py2, pz2, px3, py3, pz3, ax, ay, az;

    for (int i = 0; i < 8; i++)
    {
        px2 = px[i];
        py2 = cos(r[0]) * py[i] - sin(r[0]) * pz[i];
        pz2 = sin(r[0]) * py[i] + cos(r[0]) * pz[i];

        px3 = cos(r[1]) * px2 + sin(r[1]) * pz2;
        py3 = py2;
        pz3 = -sin(r[1]) * px2 + cos(r[1]) * pz2;

        ax = cos(r[2]) * px3 - sin(r[2]) * py3;
        ay = sin(r[2]) * px3 + cos(r[2]) * py3;
        az = pz3 - 150;

        p2x[i] = width / 2 + ax * SHAPE_SIZE / az;
        p2y[i] = height / 2 + ay * SHAPE_SIZE / az;
    }

    // Calculate draw time
    uint32_t startTime = millis();

    myOLED.erase();
    for (int i = 0; i < 3; i++)
    {
        myOLED.line(p2x[i], p2y[i], p2x[i + 1], p2y[i + 1]);
        myOLED.line(p2x[i + 4], p2y[i + 4], p2x[i + 5], p2y[i + 5]);
        myOLED.line(p2x[i], p2y[i], p2x[i + 4], p2y[i + 4]);
    }

    myOLED.line(p2x[3], p2y[3], p2x[0], p2y[0]);
    myOLED.line(p2x[7], p2y[7], p2x[4], p2y[4]);
    myOLED.line(p2x[3], p2y[3], p2x[7], p2y[7]);
    myOLED.display();

    // Write out our frame rate
    drawTotalTime += millis() - startTime;
    numberOfDraws++;

    // Output framerate once every 120 frames
    if (numberOfDraws % 120 == 0)
    {
        Serial.print(" Frame rate: ");
        Serial.println(numberOfDraws / (float)drawTotalTime * 1000.0);

        numberOfDraws = 0;
        drawTotalTime = 0;
    }
}