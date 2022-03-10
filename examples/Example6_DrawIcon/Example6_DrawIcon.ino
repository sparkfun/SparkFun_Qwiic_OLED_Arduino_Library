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

#define MICRO
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

QwI2C i2cBus;


int width;
int height;


//A 20 x 17 pixel image of a truck in a box
//Use http://en.radzio.dxp.pl/bitmap_converter/ to generate output
//Make sure the bitmap is n*8 pixels tall (pad white pixels to lower area as needed)
//Otherwise the bitmap bitmap_converter will compress some of the bytes together
uint8_t truck[] = {
    0xFF,
    0x01,
    0xC1,
    0x41,
    0x41,
    0x41,
    0x71,
    0x11,
    0x11,
    0x11,
    0x11,
    0x11,
    0x71,
    0x41,
    0x41,
    0xC1,
    0x81,
    0x01,
    0xFF,
    0xFF,
    0x80,
    0x83,
    0x82,
    0x86,
    0x8F,
    0x8F,
    0x86,
    0x82,
    0x82,
    0x82,
    0x86,
    0x8F,
    0x8F,
    0x86,
    0x83,
    0x81,
    0x80,
    0xFF,
};

int iconHeight = 16;
int iconWidth = 19;


void setup(){

    delay(100);   //Give display time to power on
    Serial.begin(115200);

    Serial.println("\n\r-----------------------------------");

    Serial.print("Bitmap test on: ");
    Serial.println(String(deviceName));


    i2cBus.init();
    myOLED.set_comm_bus(i2cBus, myOLED.default_address);
    if(!myOLED.init()){

        Serial.println("Init Failed");
        while(1);
    }

    Serial.println("- Init Success");

    width = myOLED.get_width();
    height = myOLED.get_height();

    myOLED.bitmap(8, 8, iconWidth, iconHeight, truck, iconWidth, iconHeight);
    myOLED.line(8,1,8+iconWidth, 1);
    myOLED.line(35,8, 35, 8+iconHeight);
    myOLED.display();
  
}

void loop(){
  
    delay(5000);
}

