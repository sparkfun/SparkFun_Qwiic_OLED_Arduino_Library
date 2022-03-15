/*
  Micro OLED Display Icon
  Nathan Seidle @ SparkFun Electronics
  Original Creation Date: November 15, 2020

  Draw a variable sized icon anywhere on the display. This is helpful
  when you need a icon (GPS, battery, etc) on a certain part of the screen.

  This example assumes an I2C connection (Qwiic)

  This code is beerware; if you see me (or any other SparkFun
  employee) at the local, and you've found our code helpful,
  please buy us a round!

  Distributed as-is; no warranty is given.
*/

#include <SparkFun_Qwiic_OLED.h>

//#define MICRO
//#define NARROW

#if defined(MICRO)
QwiicMicroOLED myOLED;
const char * deviceName = "Micro OLED";

#elif defined(NARROW)
QwiccNarrowOLED myOLED;
const char * deviceName = "Narrow OLED";

#else
QwiicTransparentOLED myOLED;
const char * deviceName = "Transparent OLED";
#endif


// Let's draw a truck
#include "res/qw_bmp_truck.h"
//#include "res/qw_bmp_sparkfun.h"


int width;
int height;

uint32_t draw_total_time;
uint32_t n_draws;

void setup(){

    delay(100);   //Give display time to power on
    Serial.begin(115200);

    Serial.println("\n\r-----------------------------------");

    Serial.print("Bitmap test on: ");
    Serial.println(String(deviceName));

    if(!myOLED.begin()){

        Serial.println("Device Begin Failed");
        while(1);
    }

    Serial.println("- Begin Success");

    width = myOLED.getWidth();
    height = myOLED.getHeight();

    draw_total_time =0;
    n_draws=0;

    // set a template for our framerate display
    Serial.println("- Frame Rate"); 
}

int iconX = 8;
int iconXChangeAmount = 1;
int iconY = 8;
int iconYChangeAmount = 1;

void loop(){
  
    // Calculate draw time...
    uint32_t milStart = millis();

    myOLED.bitmap(iconX, iconY, QW_BMP_TRUCK);
    myOLED.display();
    //Move the icon
    iconX += iconXChangeAmount;
    iconY += iconYChangeAmount;

    if (iconX + QW_BMP_TRUCK.width >= width)    
      iconXChangeAmount *= -1; //Change direction
    if (iconX == 0)
      iconXChangeAmount *= -1; //Change direction


    if (iconY + QW_BMP_TRUCK.height >= height)    
      iconYChangeAmount *= -1; //Change direction
    if (iconY == 0)
      iconYChangeAmount *= -1; //Change direction

    draw_total_time += millis() - milStart;
    n_draws++;

    // output framerate?
    if(n_draws % 120 == 0){

        Serial.println(((float)draw_total_time)/n_draws);

        if(n_draws > 1000){ // reset after a bit...
            n_draws = 0;
            draw_total_time=0;
        }
    }
}

