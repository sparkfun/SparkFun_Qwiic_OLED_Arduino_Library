# SparkFun_Qwiic_OLED_Arduino_Library

A new Arduino library to support SparkFun's qwiic OLED boards

Currently in Development - Alpha
========================================

## Demos
The following demos work on validated hardware (see below)
* Example 1 - Hello
* Example 4 - Cube
* Example 6 - Draw Icon
* Example 8 - Scrolling, flips and invert

## Features
### Implemented
* Fast data transfers/updates to OLED device 
  * Only transfers needed bytes
  * 2x to 6x performance gains over current Micro OLED libray. Massive gains over "hyperdisplay"
* C++ / Platform neutral implementation
* Pixel set
* General lines
* Fast horizontal lines
* Fast vertical lines
* Rectangles (fast)
* Filled rectangles (fast)
* Circles
* Filled Circles
* Bitmap images (fast - 5x improvement on demo test)
* Fonts and Text rendering
* scrolling 
* flip - vert & horz
* invert 
* Raster OPs (ROPS) - XOR, Not, Copy, Not Copy, Black and White
* Arduino Object interface

### To Be Implemented
* Functionally Complete
### Known Issues
* Filled circles being clipped/not drawn based on positon 
* Narrow OLED - some pixels - max X are not getting erased at times 
  * See on nrf5280 and ESP32
* Fill Circle - XOR - some double draws so not a clean render ..
## Hardware
### Tested/Working
* Qwiic Micro OLED
* Qwiic "narrow OLED"
* Qwiic Transparent OLED
* Artemis
* SAMD51
* ESP32
* STM32
* SAMD21
* nrf5280
* Teensy (MicroMod + ATP carrier)
* Redboard Plus (works mostly. Cube demo w/ Transparent OLED fails - runs out of memory - not surprizing)
### To Be Tested/Supported
* RP2040 - Not working. Note - doesn't work with old Micro OLED library either


# Example
A simple example to show the use of OLED Library. In this example, a series of lines are drawn that originate in a corner of the screen and span out to the other side of the display, incremented by six pixels.

After installing this library in your local Arduino environment, begin with a standard Arduino sketch, and include the header file for this library.
```C++
// Include the SparkFun qwiic OLED Library
#include <SparkFun_Qwiic_OLED.h>
```
The next step is to declare the object for the SparkFun qwiic OLED device used. Like most Arduino sketches, this is done at a global scope (after the include file declaration), not with the ```setup()``` or ```loop()``` functions. 

The user selects from one of the following classes:
| Class | Device |
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

# API 
The device objects in the SparkFun qwiic OLED Library all implement the following methods.

### begin()
This method is called to initialize the OLED library and connection to the OLED device. This method must be called before calling any graphics methods. 

```c++
bool begin(TwoWire &wirePort, uint8_t address)
```
| Parameter | Type | Description |
| :--- | :--- | :--- |
| `wirePort` | `TwoWire` | **optional**. The Wire port to use for device communication. If not provided, the default port is used|
| `address` | `uint8_t` | **optional**. The I2C address for the device. If not provided, the default address is used.|
| return value | `bool` | ```true``` on success, ```false``` on startup failure |

### getWidth()
This method returns the width, in pixels, of the connected OLED device

```c++
uint8_t getWidth(void)
```
| Parameter | Type | Description |
| :--- | :--- | :--- |
| return value | `uint8_t` | The width in pixels of the connected OLED device |

### getHeight()
This method returns the height, in pixels, of the connected OLED device

```c++
uint8_t getHeight(void)
```
| Parameter | Type | Description |
| :--- | :--- | :--- |
| return value | `uint8_t` | The height in pixels of the connected OLED device |

### display()
When called, any pending display updates are sent to the connected OLED device. This includes drawn graphics and erase commands.

```c++
void display(void)
```
| Parameter | Type | Description |
| :--- | :--- | :--- |
| NONE|  |  |

### erase()
Erases all graphics on the device, placing the display in a blank state. The erase update isn't sent to the device until the next ```display()``` call on the device.

```c++
void erase(void)
```
| Parameter | Type | Description |
| :--- | :--- | :--- |
| NONE|  |  |


### invert()
This method inverts the current graphics on the display. This results of this command happen immediatly.

```c++
void invert(bool bInvert)
```
| Parameter | Type | Description |
| :--- | :--- | :--- |
| ```bInvert``` | `bool` | ```true``` - the screen is inverted. ```false``` - the screen is set to normal |

### flipVertical()
When called, the screen contents are flipped vertically if the flip parameter is true, or restored to normal display if the flip parameter is false. 

```c++
void flipVertical(bool bFlip)
```
| Parameter | Type | Description |
| :--- | :--- | :--- |
| ```bFlip``` | `bool` | ```true``` - the screen is flipped vertically. ```false``` - the screen is set to normal |

### flipHorizontal()
When called, the screen contents are flipped horizontally if the flip parameter is true, or restored to normal display if the flip parameter is false. 

```c++
void flipHorizontal(bool bFlip){
```
| Parameter | Type | Description |
| :--- | :--- | :--- |
| ```bFlip``` | `bool` | ```true``` - the screen is flipped horzontally. ```false``` - the screen is set to normal |
