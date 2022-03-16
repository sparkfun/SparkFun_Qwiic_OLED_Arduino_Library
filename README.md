# SparkFun_Qwiic_OLED_Arduino_Library

A new Arduino library to support SparkFun's qwiic OLED boards

Currently in Development - Alpha

## Contents
* [Example Use](#Example)
* [Library  API Documentation](#API)

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


Example
---------
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

API 
------
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

### scrollStop()
If the device is in a scrolling mode, calling this method stops the scroll, and restores the device to normal display operation. This action is performed immediately.

```c++
void scrollStop(void)
```
| Parameter | Type | Description |
| :--- | :--- | :--- |
| NONE|  |  |



### scrollRight()
This method is called to start the device scrolling the displayed graphics to the right. This action is performed immediately.

The screen will scroll until the ```scrollStop()``` method is called.

```c++
void scrollRight(uint8_t start, uint8_t stop, uint8_t interval)
```
| Parameter | Type | Description |
| :--- | :--- | :--- |
| `start` | `uint8_t` | The start page address of the scroll - valid values are 0 thru 7|
| `stop` | `uint8_t` | The stop/end page address of the scroll - valid values are 0 thru 7|
| `interval` | `uint8_t` | The time interval between scroll step - values listed below |

Defined values for the `interval` parameter:
| Defined Symbol | Time Interval |
| :--- | :---|
|`SCROLL_INTERVAL_2_FRAMES`   |	2 |
|`SCROLL_INTERVAL_3_FRAMES`   | 3 |
|`SCROLL_INTERVAL_4_FRAMES`   | 4 |
|`SCROLL_INTERVAL_5_FRAMES`   | 5  |
|`SCROLL_INTERVAL_25_FRAMES`  | 25 |
|`SCROLL_INTERVAL_64_FRAMES`  | 64 |
|`SCROLL_INTERVAL_128_FRAMES` | 128 |
|`SCROLL_INTERVAL_256_FRAMES` | 256 |

### scrollVertRight()
This method is called to start the device scrolling the displayed graphics verticall and to the right. This action is performed immediately.

The screen will scroll until the ```scrollStop()``` method is called.

```c++
void scrolVertlRight(uint8_t start, uint8_t stop, uint8_t interval)
```
| Parameter | Type | Description |
| :--- | :--- | :--- |
| `start` | `uint8_t` | The start page address of the scroll - valid values are 0 thru 7|
| `stop` | `uint8_t` | The stop/end page address of the scroll - valid values are 0 thru 7|
| `interval` | `uint8_t` | The time interval between scroll step - values listed in ```scrollRight``` |

### scrollLeft()
This method is called start to the device scrolling the displayed graphics to the left. This action is performed immediately.

The screen will scroll until the ```scrollStop()``` method is called.

```c++
void scrollLeft(uint8_t start, uint8_t stop, uint8_t interval)
```
| Parameter | Type | Description |
| :--- | :--- | :--- |
| `start` | `uint8_t` | The start page address of the scroll - valid values are 0 thru 7|
| `stop` | `uint8_t` | The stop/end page address of the scroll - valid values are 0 thru 7|
| `interval` | `uint8_t` | The time interval between scroll step - values listed in ```scrollRight``` |

### scrollVertLeft()
This method is called to start the device scrolling the displayed graphics verticall and to the left. This action is performed immediately.

The screen will scroll until the ```scrollStop()``` method is called.

```c++
void scrolVertlLeft(uint8_t start, uint8_t stop, uint8_t interval)
```
| Parameter | Type | Description |
| :--- | :--- | :--- |
| `start` | `uint8_t` | The start page address of the scroll - valid values are 0 thru 7|
| `stop` | `uint8_t` | The stop/end page address of the scroll - valid values are 0 thru 7|
| `interval` | `uint8_t` | The time interval between scroll step - values listed in ```scrollRight``` |

### setFont()
This method is called to set the current font in the library. The current font is used when calling the ```text()``` method on this device. 

The default font for the device is `5x7`.

```c++
void setFont(QwiicFont& theFont)
```
| Parameter | Type | Description |
| :--- | :--- | :--- |
| `theFont` | `QwiicFont` | The font to set as current in the device|

For the library, fonts are added to your program by including them via include files which are part of this library. 

The following fonts are included:
| Font | Include File | Font Variable | Description|
| :--- | :--- | :--- | :--- |
| 5x7 | `<res/qw_fnt_5x7.h>` | `QW_FONT_5X7`| A full, 5 x 7 font|
| 31x48 | `<res/qw_fnt_31x48.h>` |`QW_FONT_31X48`| A full, 31 x 48 font|
| Seven Segment | `<res/qw_fnt_7segment.h>` |`QW_FONT_7SEGMENT`| Numbers only|
| 8x16 | `<res/qw_fnt_8x16.h>` | `QW_FONT_8X16`| A full, 8 x 16 font|
| Large Numbers | `<res/qw_fnt_largenum.h>` |`QW_FONT_LARGENUM`| Numbers only|

For each font, the font variables are objects with the following attributes:
| Attribute | Value |
| :--- | :--- | 
| `width` | The font width in pixels|
| `height` | The font height in pixels|
| `start` | The font start character offset|
| `n_chars` | The number of characters|
| `map_width` | The width of the font map|

Example use of a font object attribute:
```C++
#include <res/qw_fnt_31x48.h>
   
int myFontWidth = QW_FONT_31X48.width;
```
