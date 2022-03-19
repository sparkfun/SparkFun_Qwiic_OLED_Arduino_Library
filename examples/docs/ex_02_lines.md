# Example 2 - Lines

A simple example to show the use of OLED Library. In this example, a series of lines are drawn that originate in a corner of the screen and span out to the other side of the display, incremented by six pixels.

## Setup

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

## Drawing Lines

In the ```setup()``` function of this sketch, like all of the SparkFun qwiic libraries, the device is initialized by calling the ```begin()``` method. This method returns a value of ```true``` on success, or ```false``` on failure. 
```C++
int width, height;  // global variables for use in the sketch
void setup()
{
    Serial.begin(115200);
    if(!myOLED.begin()){
        Serial.println("Device failed to initialize");
        while(1);  // halt execution
    }
    Serial.println("Device is initialized");
    
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
