// Example-08_Multi.ino
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
// Updated from example writtin by Paul Clark @ SparkFun Electronics
// Original Creation Date: December 11th, 2020
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
// Example 8 for the SparkFun Qwiic OLED Arduino Library
//
// >> Overview <<
//
// This demo performs multiple examples
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


///////////////////////////////////////////////////////////////////////////////////////////////
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


    randomSeed(analogRead(A0) + analogRead(A1));
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
void pixelExample()
{
    //printTitle("Pixels", 1);
    myOLED.erase();
    for(int i = 0; i < 512; i++){
        myOLED.pixel(random(width), random(height));
        myOLED.display();
        delay(10);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////

void lineExample()
{
    int middleX = width / 2;
    int middleY = height/ 2;
    int xEnd, yEnd;
    int lineWidth = min(middleX, middleY);

    //printTitle("Lines!", 1);
    myOLED.erase();
    int deg;

    for (int i = 0; i < 3; i++){

        for (deg = 0; deg < 360; deg += 15){

            xEnd = lineWidth * cos(deg * PI / 180.0);
            yEnd = lineWidth * sin(deg * PI / 180.0);

            myOLED.line(middleX, middleY, middleX + xEnd, middleY + yEnd);
            myOLED.display();
            delay(10);
        }
    
        for(deg = 0; deg < 360; deg += 15){

            xEnd = lineWidth * cos(deg * PI / 180.0);
            yEnd = lineWidth * sin(deg * PI / 180.0);

            myOLED.line(middleX, middleY, middleX + xEnd, middleY + yEnd, 0);
            myOLED.display();
        delay(10);
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////
void shapeExample()
{
    //printTitle("Shapes!", 0);

    // Silly pong demo. It takes a lot of work to fake pong...
    int paddleW = 3;  // Paddle width
    int paddleH = 15; // Paddle height

    // Paddle 0 (left) position coordinates
    int paddle0_Y = (height / 2) - (paddleH / 2);
    int paddle0_X = 2;
    
    // Paddle 1 (right) position coordinates
    int paddle1_Y = ( height / 2) - (paddleH / 2);
    int paddle1_X = width - 3 - paddleW;
    int ball_rad = 2; // Ball radius
    
    // Ball position coordinates
    int ball_X = paddle0_X + paddleW + ball_rad;
    int ball_Y = random(1 + ball_rad, height - ball_rad); //paddle0_Y + ball_rad;
    int ballVelocityX = 1;                                             // Ball left/right velocity
    int ballVelocityY = 1;                                             // Ball up/down velocity
    int paddle0Velocity = -1;                                          // Paddle 0 velocity
    int paddle1Velocity = 1;                                           // Paddle 1 velocity

    //while(ball_X >= paddle0_X + paddleW - 1)
    while((ball_X - ball_rad > 1) && (ball_X + ball_rad <  width - 2)){

        // Increment ball's position
        ball_X += ballVelocityX;
        ball_Y += ballVelocityY;

        // Check if the ball is colliding with the left paddle
        if (ball_X - ball_rad < paddle0_X + paddleW){
            
            // Check if ball is within paddle's height
            if((ball_Y > paddle0_Y) && (ball_Y < paddle0_Y + paddleH)){

                ball_X++;                       // Move ball over one to the right
                ballVelocityX = -ballVelocityX; // Change velocity
            }
        }

        // Check if the ball hit the right paddle
        if(ball_X + ball_rad > paddle1_X){

            // Check if ball is within paddle's height
            if((ball_Y > paddle1_Y) && (ball_Y < paddle1_Y + paddleH)){

                ball_X--;                       // Move ball over one to the left
                ballVelocityX = -ballVelocityX; // change velocity
            }
        }
    
        // Check if the ball hit the top or bottom
        if((ball_Y <= ball_rad) || (ball_Y >= (height - ball_rad - 1))){

            // Change up/down velocity direction
            ballVelocityY = -ballVelocityY;
        }

        // Move the paddles up and down
        paddle0_Y += paddle0Velocity;
        paddle1_Y += paddle1Velocity;
    
        // Change paddle 0's direction if it hit top/bottom
        if((paddle0_Y <= 1) || (paddle0_Y > height - 2 - paddleH))
            paddle0Velocity = -paddle0Velocity;
    
        // Change paddle 1's direction if it hit top/bottom
        if((paddle1_Y <= 1) || (paddle1_Y > height - 2 - paddleH))
            paddle1Velocity = -paddle1Velocity;

        // Draw the Pong Field
        myOLED.erase();
    
        // Draw an outline of the screen:
        myOLED.rectangle(0, 0, width-1,  height-1);
    
        // Draw the center line
        myOLED.rectangleFill( width/ 2 - 1, 0, width/2 + 1, height-1);

        // Draw the Paddles:
        myOLED.rectangleFill(paddle0_X, paddle0_Y, paddle0_X+paddleW, paddle0_Y+paddleH);
        myOLED.rectangleFill(paddle1_X, paddle1_Y, paddle1_X+paddleW, paddle1_Y+paddleH);

        // Draw the ball:
        myOLED.circle(ball_X, ball_Y, ball_rad);

        // Actually draw everything on the screen:
        myOLED.display();

        delay(25); // Delay for visibility
    }
    delay(1000);
}
///////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
  pixelExample();  // Run the pixel example function
  lineExample();  // Then the line example function
  shapeExample(); // Then the shape example
}

