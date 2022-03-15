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

#include <SparkFun_Qwiic_OLED.h>

#define MICRO
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

#include "res/qw_bmp_sparkfun.h"

int yoffset;

void setup()
{

    delay(100);   //Give display time to power on
    Serial.begin(115200);

    Serial.println("\n\r-----------------------------------");

    Serial.print("Running Scroll Test on: ");
    Serial.println(String(deviceName));

    if(!myOLED.begin()){

        Serial.println("Device Begin Failed");
        while(1);
    }

    Serial.println("- Init Success");
    
    yoffset = (myOLED.getHeight() - myOLED.getFont()->height)/2;

    delay(1000);
}

// Our testing functions

void scroll_right(void){

    myOLED.scrollStop();
    myOLED.scrollRight(0, 7, SCROLL_INTERVAL_2_FRAMES); 
}

void scroll_right_vert(void){
    myOLED.scrollStop();    
    myOLED.scrollVertRight(0, 7, SCROLL_INTERVAL_3_FRAMES); 
}

void scroll_left(void){
    myOLED.scrollStop();    
    myOLED.scrollLeft(0, 7, SCROLL_INTERVAL_4_FRAMES);
}

void scroll_left_vert(void){
    myOLED.scrollStop();    
    myOLED.scrollVertLeft(0, 7, SCROLL_INTERVAL_5_FRAMES);
}

void scroll_stop(void){
    myOLED.scrollStop();
}

void flip_horz(void){

    for(int i=0; i < 6; i++){
        myOLED.flipHorizontal(!(i & 0x01));
        delay(800);
    }
}
void flip_vert(void){
    for(int i=0; i < 6; i++){
        myOLED.flipVertical(!(i & 0x01));
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
