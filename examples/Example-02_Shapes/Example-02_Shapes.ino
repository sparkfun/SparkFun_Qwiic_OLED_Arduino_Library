
// Example-02_Shapes.ino
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
//
//////////////////////////////////////////////////////////////////////////////////////////
// Example 2 for the SparkFun Qwiic OLED Arduino Library
//
// >> Overview <<
//
// This demo shows the various methods available for drawing shapes using the OLED library
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

// Global variables - used to stash our screen size

int width;
int height;

////////////////////////////////////////////////////////////////////////////////
// line_test_1() 
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

void line_test_1(void){

    int x, y, i;

    int mid = width/2;
    int delta = mid/8;
    
    for(int j =1; j < 8; j++ ){

        x = delta*j;

        for(i=0; i < height*2; i++){

            y = i % height;
            myOLED.erase();
            myOLED.line(mid-x, y, mid+x, y);
            myOLED.display();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// line_test_2() 
//
// This test draws a series of lines bursting out from the corner of the display. 
//
// It demostrates the abilty of the library to draw arbitray straight lines.

void line_test_2(void){

    for(int i=0; i < width; i +=6){
        myOLED.line(0, 0, i, height-1);
        myOLED.display();
    }
    delay(200);
    myOLED.erase();
    for(int i=width-1; i >= 0; i -=6){
        myOLED.line(width-1, 0, i, height-1);
        myOLED.display();
    }    
}

////////////////////////////////////////////////////////////////////////////////
// line_test_vert() 
//
// Draws a series of vertical lines, some horz lines and two diag lines.
//
// Internally, this uses the standard line algorithm  (diag), fast horz and fast
// vertical lines functions. 
//
// Iterator function - called to animate graphic
void line_test_vert_iter(uint8_t y0, uint8_t y1){

    for(int i=0; i < width; i += 8)
        myOLED.line( i, y0, i, y1);

    // end off the vertical lines
    myOLED.line( width-1, y0, width-1, y1);        

    // End lines and cross lines
    myOLED.line(0, y0, width-1, y0);
    myOLED.line(0, y1, width-1, y1);
    myOLED.line(0, y0, width-1, y1);
    myOLED.line(0, y1, width-1, y0);    
}

// Entry point for test
void line_test_vert(void){

    int mid = height/2;

    for(int i=0; i < height; i+=4){

        myOLED.erase();
        line_test_vert_iter( mid - i/2, mid + i/2 );
        myOLED.display();
        delay(10);
    }
}

////////////////////////////////////////////////////////////////////////////////
// rect_test() 
//
// Simple - draw a rectangle test
void rect_test(void){

    myOLED.rectangle(4, 4, width-4, height-4);

}

////////////////////////////////////////////////////////////////////////////////
// rect_test_move() 
//
// Draws a rectangle, then moves it across the screen
//
void rect_test_move(void){    

    float steps = height;
    float xinc = width/steps;
    float yinc = height/steps;
    int side = 10;
    float x = 0;
    float y = 0;

    for(int i = 0; i < steps; i++){

        // Draw the rectangle and send it to device
        myOLED.rectangle(x, y, x+side, y+side);
        myOLED.display(); // sends erased rect and new rect pixels to device

        // Erase the that rect, increment and loop
        myOLED.rectangle(x, y, x+side, y+side, 0);

        x += xinc;
        y += yinc;       
    }

}

////////////////////////////////////////////////////////////////////////////////
// rect_fill_test() 
//
// Draws two filled rectangles, switches to XOR draw mode and overwrites them 
// with another rectangle

void rect_fill_test(void){

    myOLED.rectangleFill(4, 4, width/2-4, height-4);

    myOLED.rectangleFill(width/2+4, 4, width-4, height-4);

    myOLED.setDrawMode(grROPXOR);   // xor
    myOLED.rectangleFill(width/4, 8, width - width/4, height-8);
    myOLED.setDrawMode(grROPCopy);  // back to copy op (default)
}

////////////////////////////////////////////////////////////////////////////////
// circle_test() 
//
// Draw a series of circles - filled and not filled
void circle_test(void){

    // Lets draw some circles that fit on the device
    myOLED.circle(width/4, height/2, height/3);

    myOLED.circleFill(width - width/4, height/2, height/3);    

    myOLED.circle(4, height/2, height/3);

    myOLED.circleFill(width - width/2 , height/2, height/4);    

}


// Make an array of function pointers - used to hold the list of tests to run.
// 
// Makes the loop call to each test function simple.
//
// testing function table
typedef void (*testFn)(void);

static const testFn testFunctions[] = {
    line_test_1,
    line_test_2,
    line_test_vert,
    rect_test,
    rect_test_move,
    rect_fill_test,
    circle_test
}; 

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

    // save device dims for the test routines
    width = myOLED.getWidth();
    height = myOLED.getHeight();
  
}

////////////////////////////////////////////////////////////////////////////////////////////////
// loop()
//
// Standard Arduino loop function.

void loop(){

    // Just loop over our test functions
    for(uint8_t i=0; i < sizeof(testFunctions)/sizeof(testFunctions[0]); i++){

        myOLED.erase();

        testFunctions[i](); // next function
        
        myOLED.display();

        delay(1000);

    }

}