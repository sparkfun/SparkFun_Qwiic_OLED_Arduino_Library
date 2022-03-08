# SparkFun_Qwiic_OLED_Arduino_Library

A new Arduino library to support SparkFun's qwiic OLED boards

Currently in Development - Alpha
========================================

## Demos
The following demos work on validated hardware (see below)
* Example 1 - Hello
* Example 4 - Cube
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
### To Be Implemented
* Fonts
* Bitmap images
* Arduino Objects - right now mostly C++
### Known Issues
* Filled circles being clipped/not drawn based on positon 
* ESP32 compbiles, but fails to run - Looks like an I2C issue
## Hardware
### Tested/Working
* Qwiic Micro OLED
* Qwiic "narrow OLED"
* Artemis
* SAMD51
### To Be Tested/Supported
* Qwiic Transparent OLED
* ESP32
* STM32
* RP2040
* SAMD21
* Teensy
* Redboard
* nrf52840
