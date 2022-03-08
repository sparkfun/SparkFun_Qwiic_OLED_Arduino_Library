# SparkFun_Qwiic_OLED_Arduino_Library

A new Arduino library to support SparkFun's qwiic OLED boards

Currently in Development - Alpha Testing
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
## Hardware
### Tested/Working
* Qwiic Micro OLED
* Qwiic "narrow OLED"
* Artemis
* SAMD21
### To Be Tested/Supported
* Qwiic Transparent OLED
* ESP32
* STM32
* RP2040
* SAMD21
* Teensy
* Redboard
