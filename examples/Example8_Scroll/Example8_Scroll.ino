/*
  SFE_MicroOLED Library Demo
  Paul Clark @ SparkFun Electronics
  Original Creation Date: December 10th, 2020

  This sketch uses the MicroOLED library to show all the functionality built into the library

  Hardware Connections:
    This example assumes you are using Qwiic. See the SPI examples for
    a detailed breakdown of connection info.

  This code is beerware; if you see me (or any other SparkFun employee) at the
  local, and you've found our code helpful, please buy us a round!
  
  Distributed as-is; no warranty is given.
*/


//#define MICRO
//#define NARROW


#if defined(MICRO)
#include <qwiic_oledmicro.h> 
QwOLEDMicro myOLED;
const char * deviceName = "Micro OLED";

#elif defined(NARROW)
#include <qwiic_olednarrow.h> 
QwOLEDNarrow myOLED;
const char * deviceName = "Narrow OLED";

#else
#include <qwiic_oledtransp.h>
QwOLEDTransparent myOLED;
const char * deviceName = "Transparent OLED";
#endif

#include "res/qw_fnt_8x16.h"

QwI2C i2cBus;

void setup()
{

    delay(100);   //Give display time to power on
    Serial.begin(115200);

    Serial.println("\n\r-----------------------------------");

    Serial.print("Running Scroll Test on: ");
    Serial.println(String(deviceName));


    i2cBus.init();
    myOLED.set_comm_bus(i2cBus, myOLED.default_address);
    if(!myOLED.init()){

        Serial.println("Init Failed");
        while(1);
    }

    Serial.println("- Init Success");

    myOLED.set_font(QW_FONT_8X16);
    char text[] = "Scrolling";
    myOLED.text(5, 5 , text);
    myOLED.display();
}

void loop()
{
  delay(2000);
  myOLED.scroll(SCROLL_RIGHT, 0, 7, SCROLL_INTERVAL_2_FRAMES); // Scroll all pages right

  delay(2000);
  myOLED.scroll(SCROLL_VERT_RIGHT, 0, 7, SCROLL_INTERVAL_3_FRAMES); // Scroll all pages vertical right, slower
  
  delay(2000);
  myOLED.scroll(SCROLL_LEFT, 0, 7, SCROLL_INTERVAL_4_FRAMES); // Scroll all pages left, slower again
  
  delay(2000);
  myOLED.scroll(SCROLL_VERT_LEFT, 0, 7, SCROLL_INTERVAL_5_FRAMES); // Scroll all pages vertical left, slower still...
}
