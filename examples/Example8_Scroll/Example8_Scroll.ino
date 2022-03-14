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

#include "res/qw_bmp_sparkfun.h"

QwI2C i2cBus;

int yoffset;

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
    
    yoffset = (myOLED.get_height() - myOLED.get_font()->height)/2;

    delay(1000);
}

// Our testing functions

void scroll_right(void){

    myOLED.scroll_stop();
    myOLED.scroll(SCROLL_RIGHT, 0, 7, SCROLL_INTERVAL_2_FRAMES); 
}

void scroll_right_vert(void){
    myOLED.scroll_stop();    
    myOLED.scroll(SCROLL_VERT_RIGHT, 0, 7, SCROLL_INTERVAL_3_FRAMES); 
}

void scroll_left(void){
    myOLED.scroll_stop();    
    myOLED.scroll(SCROLL_LEFT, 0, 7, SCROLL_INTERVAL_4_FRAMES);
}

void scroll_left_vert(void){
    myOLED.scroll_stop();    
    myOLED.scroll(SCROLL_VERT_LEFT, 0, 7, SCROLL_INTERVAL_5_FRAMES);
}

void scroll_stop(void){
    myOLED.scroll_stop();
}

void flip_horz(void){

    for(int i=0; i < 6; i++){
        myOLED.flip_horz(!(i & 0x01));
        delay(800);
    }
}
void flip_vert(void){
    for(int i=0; i < 6; i++){
        myOLED.flip_vert(!(i & 0x01));
        delay(800);
    }
}

void invert(void){
    for(int i=0; i < 6; i++){
        myOLED.invert(!(i & 0x01));
        delay(800);
    }    
}

// testing function table
typedef void (*testFn)(void);
typedef struct _testRoutines{
    void (*testFn)(void);
    const char *title;
}testRoutine;

static const testRoutine testFunctions[] = {
    {scroll_right, "Right>"},
    {scroll_right_vert, "^Right-Up>"},
    {scroll_left, "<Left"},
    {scroll_left_vert, "<Left-Up^"},
    {scroll_stop, "<STOP>"},
    {flip_horz, "-Flip-Horz-"},    
    {flip_vert, "|Flip-Vert|"},    
    {invert, "**INVERT**"}        
}; 

void loop()
{

    for(uint8_t i=0; i < sizeof(testFunctions)/sizeof(testFunctions[0]); i++){


        if(testFunctions[i].title){
            myOLED.erase();
            myOLED.text(3, yoffset, testFunctions[i].title);
            myOLED.display();
        }

        testFunctions[i].testFn();

        delay(3000);

    }
  
}
