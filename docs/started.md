# Getting Started


## Installation

The SparkFun Qwiic OLED Arduino Library is available within in the Arduino library manager, which is launched via the `Sketch > Include Libraries > Manage Libraries …` menu option in the Arduino IDE. Just search for ***SparkFun Qwiic OLED Library***

The following resources available at [SparkFun](https://www.sparkfun.com) provide the details on setting up and configuring Arduino to use this library.

* [Installing the Arduino IDE](https://learn.sparkfun.com/tutorials/installing-arduino-ide)
* [ Installing Board Definitions in the Arduino IDE](https://learn.sparkfun.com/tutorials/installing-board-definitions-in-the-arduino-ide)
* [Installing an Arduino Library](https://learn.sparkfun.com/tutorials/installing-an-arduino-library)

An Initial Example - Drawing Lines
---------
A simple example to show the use of OLED Library. In this example, a series of lines are drawn that originate in a corner of the screen and span out to the other side of the display, incremented by six pixels.

After installing this library in your local Arduino environment, begin with a standard Arduino sketch, and include the header file for this library.
```C++
// Include the SparkFun qwiic OLED Library
#include <SparkFun_Qwiic_OLED.h>
```
The next step is to declare the object for the SparkFun qwiic OLED device used. Like most Arduino sketches, this is done at a global scope (after the include file declaration), not within the ```setup()``` or ```loop()``` functions. 

The user selects from one of the following classes:

| Class | Qwiic OLED Device |
| :--- | :--- |
| `QwiicMicroOLED` | [SparkFun Qwiic Micro OLED ]( https://www.sparkfun.com/products/14532)| 
| `QwiicNarrowOLED` | [SparkFun Qwiic OLED Display (128x32) ]( https://www.sparkfun.com/products/17153)| 
| `QwiicTransparentOLED` | [SparkFun Transparent Graphical OLED]( https://www.sparkfun.com/products/15173)| 

For this example, the Qwiic Micro OLED is used.
```C++
QwiicMicroOLED myOLED;
```
In the ```setup()``` function of this sketch, like all of the SparkFun qwiic libraries, the device is initialized by calling the ```begin()``` method. This method returns a value of ```true``` on success, or ```false``` on failure. 
```C++
int width, height;  // global variables for use in the sketch
void setup()
{
    Serial.begin(115200);
    if(!myOLED.begin()){
        Serial.println("Device failled to initialize");
        while(1);  // halt execution
    }
    Serial.println("Device is intitialized");
    
    // get the device dimensions
    width  = myOLED.getWidth();
    height = myOLED.getHeight();
}
```
Now that the library is initialized, in the ```loop()``` function of this sketch, the desired graphics are drawn. Here we erase the screen and draw simple series of lines that originate at the screen origin and fan out across the height of the display. 

```C++
void loop(){

    myOLED.erase();           // Erase the screen
    myOLED.display();         // Send erase to device
    
    delay(1000);    // Slight pause
    
    // Draw our lines from point (0,0) to (i, screen height)
    
    for(int i=0; i < width; i+= 6){
        myOLED.line(0, 0, i, height-1);    // draw the line
        myOLED.display();                  // Send the new line to the device for display
    }
}
```
A key point when using the OLED library, graphic/screen updates are only sent to the OLED device when the ```display()``` method is called. 

Library Provided Examples
--------
The following Examples are part of the library installation:

* [Example 1 - Hello](https://github.com/sparkfun/SparkFun_Qwiic_OLED_Arduino_Library/blob/main/examples/Example01_Hello/Example01_Hello.ino)
* [Example 4 - Cube](https://github.com/sparkfun/SparkFun_Qwiic_OLED_Arduino_Library/blob/main/examples/Example4_Cube/Example4_Cube.ino)
* [Example 6 - Draw Icon](https://github.com/sparkfun/SparkFun_Qwiic_OLED_Arduino_Library/blob/main/examples/Example6_DrawIcon/Example6_DrawIcon.ino)
* [Example 8 - Scrolling, flips and invert](https://github.com/sparkfun/SparkFun_Qwiic_OLED_Arduino_Library/blob/main/examples/Example8_Scroll/Example8_Scroll.ino)


