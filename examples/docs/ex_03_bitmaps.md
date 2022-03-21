# Example 3 - Bitmaps

An example that shows drawing bitmaps using the  SparkFun Qwiic OLED Library.

**Key Demo Features**

* Understanding bitmap structure
* Bitmap objects
* Drawing Bitmap


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

The Example code supports all of the SparkFun Qwiic OLED boards. To select the board being used, uncomment the `#define` for the demo board. 

For this example, the Qwiic Micro OLED is used.

```C++
#define MICRO
//#define NARROW
//#define TRANSPARENT
``` 
Which results in myOLED being declared as:

```C++
QwiicMicroOLED myOLED;
```
## Initialization

In the ```setup()``` function of this sketch, like all of the SparkFun qwiic libraries, the device is initialized by calling the ```begin()``` method. This method returns a value of ```true``` on success, or ```false``` on failure. 

```C++
void setup()
{

    delay(500);   //Give display time to power on

    // Serial on!
    Serial.begin(115200);

    Serial.println("\n\r-----------------------------------");

    Serial.print("Running Example 01 on: ");
    Serial.println(String(deviceName));

    // Initalize the OLED device and related graphics system
    if(!myOLED.begin()){

        Serial.println(" - Device Begin Failed");
        while(1);
    }

    Serial.println("- Begin Success");

```
## Drawing Bitmaps

 

