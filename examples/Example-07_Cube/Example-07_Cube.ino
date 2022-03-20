// Example-07_Cube.ino
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
// Written Jim Lindblom @ SparkFun Electronics
//  Original Creation Date: October 27, 2014
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
// Example 7 for the SparkFun Qwiic OLED Arduino Library
//
// >> Overview <<
//
// This demo draws a rotating 3D cube 
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
const char * deviceName = "Transparent OLED";

#elif defined(NARROW)
QwiicNarrowOLED myOLED;
const char * deviceName = "Narrow OLED";

#else
QwiicMicroOLED myOLED;
const char * deviceName = "Micro OLED";

#endif


int width;
int height;

uint32_t draw_total_time;
uint32_t n_draws;

float d = 3;
float px[] = { -d, d, d, -d, -d, d, d, -d};
float py[] = { -d, -d, d, d, -d, -d, d, d};
float pz[] = { -d, -d, -d, -d, d, d, d, d};

float p2x[8] = {0};
float p2y[8] = {0};
float r[3] = {0};

#define SHAPE_SIZE 400
// Define how fast the cube rotates. Smaller numbers are faster.
// This is the number of ms between draws.
#define ROTATION_SPEED 00

////////////////////////////////////////////////////////////////////////////////////////////////
// setup()
// 
// Standard Arduino setup routine

void setup()
{
    delay(500);   //Give display time to power on
    Serial.begin(115200);

    Serial.println("\n\r-----------------------------------");

    Serial.print("Running Test #2 on: ");
    Serial.println(String(deviceName));

    if(!myOLED.begin()){

        Serial.println("- Device Begin Failed");
        while(1);
    }

    Serial.println("- Begin Successful");

    width = myOLED.getWidth();
    height = myOLED.getHeight();

    // for frame rate calc
    draw_total_time =0;
    n_draws=0;

    // set a template for our framerate display
    Serial.print("- Frame Rate: 00.00"); 
}

////////////////////////////////////////////////////////////////////////////////////////////////
// loop

void loop()
{
    // just draw the cube - as fast as we can!
    drawCube();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// drawCube()
//

void drawCube()
{


    r[0] = r[0] + PI / 180.0; // Add a degree
    r[1] = r[1] + PI / 180.0; // Add a degree
    r[2] = r[2] + PI / 180.0; // Add a degree
    if(r[0] >= 360.0 * PI / 180.0)
        r[0] = 0;
    if(r[1] >= 360.0 * PI / 180.0)
        r[1] = 0;
    if(r[2] >= 360.0 * PI / 180.0)
        r[2] = 0;

    // This routine gets called often, so just make these statics
    static float px2, py2, pz2, px3, py3, pz3, ax, ay, az;

    for (int i = 0; i < 8; i++){
        
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

    // Calculate draw time...
    uint32_t milStart = millis();

    myOLED.erase();
    for (int i = 0; i < 3; i++){

        myOLED.line(p2x[i], p2y[i], p2x[i + 1], p2y[i + 1]);
        myOLED.line(p2x[i + 4], p2y[i + 4], p2x[i + 5], p2y[i + 5]);
        myOLED.line(p2x[i], p2y[i], p2x[i + 4], p2y[i + 4]);
    }

    myOLED.line(p2x[3], p2y[3], p2x[0], p2y[0]);
    myOLED.line(p2x[7], p2y[7], p2x[4], p2y[4]);
    myOLED.line(p2x[3], p2y[3], p2x[7], p2y[7]);
    myOLED.display();

    // Write out our frame rate
    draw_total_time += millis() - milStart;
    n_draws++;

    // output framerate?
    if(n_draws % 120 == 0){
        // backspace over old number 
        Serial.print("\b\b\b\b\b");  // backspace over old number
        Serial.print(((float)draw_total_time)/n_draws);

        if(n_draws > 1000){ // reset after a bit...
            n_draws = 0;
            draw_total_time=0;
        }
    }

}
