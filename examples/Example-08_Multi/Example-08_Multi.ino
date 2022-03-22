/*
  Example-08_Multi.ino

  This demo performs multiple examples

  This library configures and draws graphics to OLED boards that use the
  SSD1306 display hardware. The library only supports I2C.

  SparkFun sells these at its website: www.sparkfun.com

  Do you like this library? Help support SparkFun. Buy a board!

   Micro OLED             https://www.sparkfun.com/products/14532
   Transparent OLED       https://www.sparkfun.com/products/15173
   "Narrow" OLED          https://www.sparkfun.com/products/17153

  Updated from example writtin by Paul Clark @ SparkFun Electronics
  Original Creation Date: December 11th, 2020

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

    // Not all platforms have A0
#ifdef A0
    randomSeed(analogRead(A0));
#endif
}

void loop()
{
    pixelExample();
    lineExample();
    shapeExample();
}

void pixelExample()
{
    myOLED.erase();
    for (int i = 0; i < 512; i++)
    {
        myOLED.pixel(random(width), random(height));
        myOLED.display();
        delay(10);
    }
}

void lineExample()
{
    int middleX = width / 2;
    int middleY = height / 2;
    int xEnd, yEnd;
    int lineWidth = min(middleX, middleY);

    myOLED.erase();
    int deg;

    for (int i = 0; i < 3; i++)
    {

        for (deg = 0; deg < 360; deg += 15)
        {

            xEnd = lineWidth * cos(deg * PI / 180.0);
            yEnd = lineWidth * sin(deg * PI / 180.0);

            myOLED.line(middleX, middleY, middleX + xEnd, middleY + yEnd);
            myOLED.display();
            delay(10);
        }

        for (deg = 0; deg < 360; deg += 15)
        {

            xEnd = lineWidth * cos(deg * PI / 180.0);
            yEnd = lineWidth * sin(deg * PI / 180.0);

            myOLED.line(middleX, middleY, middleX + xEnd, middleY + yEnd, 0);
            myOLED.display();
            delay(10);
        }
    }
}

void shapeExample()
{
    // Silly pong demo. It takes a lot of work to fake pong...
    int paddleW = 3;  // Paddle width
    int paddleH = 15; // Paddle height

    // Paddle 0 (left) position coordinates
    int paddle0_Y = (height / 2) - (paddleH / 2);
    int paddle0_X = 2;

    // Paddle 1 (right) position coordinates
    int paddle1_Y = (height / 2) - (paddleH / 2);
    int paddle1_X = width - 3 - paddleW;
    int ball_rad = 4; // Ball radius

    // Ball position coordinates
    int ball_X = paddle0_X + paddleW + ball_rad;
    int ball_Y = random(1 + ball_rad, height - ball_rad); // paddle0_Y + ball_rad;
    int ballVelocityX = 1;                                // Ball left/right velocity
    int ballVelocityY = 1;                                // Ball up/down velocity
    int paddle0Velocity = -1;                             // Paddle 0 velocity
    int paddle1Velocity = 1;                              // Paddle 1 velocity

    // Draw the Pong Field
    myOLED.erase();

    // Draw an outline of the screen:
    myOLED.rectangle(0, 0, width - 1, height);

    // Draw the center line
    myOLED.rectangleFill(width / 2 - 1, 0, 2, height);

    bool firstLoop = true;

    while ((ball_X - ball_rad > 1) && (ball_X + ball_rad < width - 2))
    {

        if (!firstLoop)
        {

            // Erase the old ball. In XOR mode, so just draw old values again!
            // Draw the Paddles:
            myOLED.rectangleFill(paddle0_X, paddle0_Y, paddleW, paddleH);
            myOLED.rectangleFill(paddle1_X, paddle1_Y, paddleW, paddleH);
            // Draw the ball: - use rect - xor and circle fails b/c of circle algorithm overdraws
            myOLED.rectangleFill(ball_X, ball_Y, ball_rad, ball_rad);
        }
        // Increment ball's position
        ball_X += ballVelocityX;
        ball_Y += ballVelocityY;

        // Check if the ball is colliding with the left paddle
        if (ball_X - ball_rad < paddle0_X + paddleW)
        {

            // Check if ball is within paddle's height
            if ((ball_Y > paddle0_Y) && (ball_Y < paddle0_Y + paddleH))
            {

                ball_X++;                       // Move ball over one to the right
                ballVelocityX = -ballVelocityX; // Change velocity
            }
        }

        // Check if the ball hit the right paddle
        if (ball_X + ball_rad > paddle1_X)
        {

            // Check if ball is within paddle's height
            if ((ball_Y > paddle1_Y) && (ball_Y < paddle1_Y + paddleH))
            {

                ball_X--;                       // Move ball over one to the left
                ballVelocityX = -ballVelocityX; // change velocity
            }
        }

        // Check if the ball hit the top or bottom
        if ((ball_Y <= 1) || (ball_Y >= (height - ball_rad - 1)))
        {

            // Change up/down velocity direction
            ballVelocityY = -ballVelocityY;
        }

        // Move the paddles up and down
        paddle0_Y += paddle0Velocity;
        paddle1_Y += paddle1Velocity;

        // Change paddle 0's direction if it hit top/bottom
        if ((paddle0_Y <= 1) || (paddle0_Y > height - 2 - paddleH))
            paddle0Velocity = -paddle0Velocity;

        // Change paddle 1's direction if it hit top/bottom
        if ((paddle1_Y <= 1) || (paddle1_Y > height - 2 - paddleH))
            paddle1Velocity = -paddle1Velocity;

        // Draw the Paddles:
        myOLED.rectangleFill(paddle0_X, paddle0_Y, paddleW, paddleH);
        myOLED.rectangleFill(paddle1_X, paddle1_Y, paddleW, paddleH);

        // Draw the ball:
        myOLED.rectangleFill(ball_X, ball_Y, ball_rad, ball_rad);

        // Actually draw everything on the screen:
        myOLED.display();

        // Once the first loop is done, switch to XOR mode. So we just update our
        // moving parts
        if (firstLoop)
        {
            firstLoop = false;
            myOLED.setDrawMode(grROPXOR);
        }

        delay(25); // Delay for visibility
    }
    delay(1000);
    myOLED.setDrawMode(grROPCopy);
}