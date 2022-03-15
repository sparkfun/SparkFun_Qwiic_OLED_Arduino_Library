/*
  MicroOLED_Hello.ino
  SFE_MicroOLED Hello World Demo
  Jim Lindblom @ SparkFun Electronics
  Original Creation Date: October 26, 2014
  
  This sketch lights up a familiar pattern on the MicroOLED
  Breakout. It's a great way to prove you've connected everything
  correctly and that your display is in working order.
  
  Hardware Connections:
    This example assumes you are using Qwiic. See the SPI examples for
    a detailed breakdown of connection info.
  
  Note: The display on the MicroOLED is a 1.8V-3.3V device only.
  Don't try to connect a 5V Arduino directly to it! Use level
  shifters in between the data signals if you have to resort to
  a 5V board.
  
  This code is beerware; if you see me (or any other SparkFun 
  employee) at the local, and you've found our code helpful, 
  please buy us a round!
  
  Distributed as-is; no warranty is given.
*/

#include <stdint.h>

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


//#include "res/qw_fnt_5x7.h"
#include "res/qw_fnt_8x16.h"
//#include "res/qw_fnt_7segment.h"
//#include "res/qw_fnt_31x48.h"
//#include "res/qw_fnt_largenum.h"


int width;
int height;


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

void line_test_vert_iter(uint8_t y0, uint8_t y1){

    for(int i=0; i < width; i += 8)
        myOLED.line( i, y0, i, y1);

    myOLED.line( width-1, y0, width-1, y1);        

    myOLED.line(0, y0, width-1, y0);
    myOLED.line(0, y1, width-1, y1);
    myOLED.line(0, y0, width-1, y1);
    myOLED.line(0, y1, width-1, y0);    
}
void line_test_vert(void){

    int mid = height/2;

    for(int i=0; i < height; i+=4){

        myOLED.erase();
        line_test_vert_iter( mid - i/2, mid + i/2 );
        myOLED.display();
        delay(10);
    }
}

void rect_test(void){

    myOLED.rectangle(4, 4, width-4, height-4);

}

void rect_test_move(void){    

    float steps = height;
    float xinc = width/steps;
    float yinc = height/steps;
    int side = 10;
    float x = 0;
    float y = 0;
    for(int i = 0; i < steps; i++){

        myOLED.rectangle(x, y, x+side, y+side);
        myOLED.display();
        //delay(100);

        // Erase the that rect
        myOLED.rectangle(x, y, x+side, y+side, 0);

        x += xinc;
        y += yinc;       
    }

}

void rect_fill_test(void){

    myOLED.rectangleFill(4, 4, width/2-4, height-4);

    myOLED.rectangleFill(width/2+4, 4, width-4, height-4);

    myOLED.setDrawMode(grROPXOR);
    myOLED.rectangleFill(width/4, 8, width - width/4, height-8);
    myOLED.setDrawMode(grROPCopy);        
}

void circle_test(void){

    myOLED.circle(width/4, height/2, height/3);

    myOLED.circleFill(width - width/4, height/2, height/3);    

    myOLED.circle(4, height/2, height/3);

    myOLED.circleFill(width - width/4 , height/2, height/3);    

    myOLED.setDrawMode(grROPXOR);
    myOLED.circleFill(width - width/4 - height/4, height/2, height/3); // bug in here
    myOLED.setDrawMode(grROPCopy);    
}

void text_hello(void){

    myOLED.text(10, 10 , "Hello World");

    myOLED.setDrawMode(grROPXOR);
    myOLED.rectangleFill(8, 8, width/2, height/2);
    myOLED.setDrawMode(grROPCopy);    
}
// testing function table
typedef void (*testFn)(void);

static const testFn testFunctions[] = {
    line_test_1,
    line_test_2,
    line_test_vert,
    rect_test,
    rect_test_move,
    rect_fill_test,
    circle_test,
    text_hello
}; 

////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
    delay(100);   //Give display time to power on
    Serial.begin(115200);

    Serial.println("\n\r-----------------------------------");

    Serial.print("Running Test #1 on: ");
    Serial.println(String(deviceName));

    if(!myOLED.begin()){

        Serial.println("Device Begin Failed");
        while(1);
    }

    Serial.println("- Begin Success");

    width = myOLED.getWidth();
    height = myOLED.getHeight();

//    myOLED.set_font(QW_FONT_5X7);    // works
    myOLED.setFont(QW_FONT_8X16);   // works
//    myOLED.set_font(QW_FONT_31X48);   // works
 //   myOLED.set_font(QW_FONT_LARGENUM);   // works    

//    myOLED.set_font(QW_FONT_7SEGMENT);        // works

  
}


void loop(){

    for(uint8_t i=0; i < sizeof(testFunctions)/sizeof(testFunctions[0]); i++){

        myOLED.erase();
        testFunctions[i]();
        myOLED.display();
        delay(1000);

    }
    Serial.println(">>>>>> End Test Loop <<<<<<");
    delay(1000);
}