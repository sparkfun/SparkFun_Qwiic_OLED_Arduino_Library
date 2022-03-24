![SparkFun Qwiic OLED Arduino Library](https://github.com/sparkfun/SparkFun_Qwiic_OLED_Arduino_Library/blob/main/docs/img/OLEDLibBanner.png "SparkFun Qwiic OLED Arduino Library")

# SparkFun Qwiic OLED Arduino Library
<p align="center">
	<a href="https://github.com/sparkfun/SparkFun_Qwiic_OLED_Arduino_Library/issues" alt="Issues">
		<img src="https://img.shields.io/github/issues/sparkfun/SparkFun_Qwiic_OLED_Arduino_Library.svg" /></a>
	<a href="https://github.com/sparkfun/SparkFun_Qwiic_OLED_Arduino_Library/blob/master/LICENSE" alt="License">
		<img src="https://img.shields.io/badge/license-MIT-blue.svg" /></a>
	<a href="https://twitter.com/intent/follow?screen_name=sparkfun">
        	<img src="https://img.shields.io/twitter/follow/sparkfun.svg?style=social&logo=twitter"
           	 alt="follow on Twitter"></a>
	
</p>
The SparkFun Qwiic OLED Arduino Library is a single graphics module that supports all SparkFun OLED boards based on the SSD1306 from Solomon Systech. Prior to this library, three different libraries were used to support our four different OLED boards. 

The SparkFun Qwiic OLED Library delivers a common implementation for all our Qwiic OLED products, delivering a unified, fast, and efficient solution that implements a familiar and easy to understand user experience.

## Key Features
*	Implements common graphics capabilities: pixel, line, rectangle, filled rectangle, circle, filled circle, bitmap, text and raster operators (i.e. XOR).
* Smart data transfer to the device – only sends _dirty_ regions of the graphics buffer to the OLED device, not the entire buffer. 
* High performance – 2x faster than our previous OLED library, often much higher. 
* Efficient memory usage. No dynamic memory utilized. Static resources are loaded once, and only on explicit declaration. 
* Implements a familiar interface, making migration from older libraries straight forward

## Documentation
A full library use overview, API reference guide and key example walk through are available on this repositories github page - [sparkfun.github.io/SparkFun_Qwiic_OLED_Arduino_Library](https://sparkfun.github.io/SparkFun_Qwiic_OLED_Arduino_Library/)

## How much faster?

The SparkFun Qwiic OLED Library is between 40% and 450% faster. The original Micro OLED library had a max output of ~75 frames per second. This library automatically only updates *what changed* we can reach more than 300 frames per second. What does that look like? 

[link text](https://cdn.sparkfun.com/assets/learn_tutorials/2/1/8/8/OLED_Lib_Truck_New_vs_Old.mp4)

[![Comparison of displays](https://cdn.sparkfun.com/assets/learn_tutorials/2/1/8/8/OLED_Lib_Truck_New_vs_Old.mp4)](https://cdn.sparkfun.com/assets/learn_tutorials/2/1/8/8/OLED_Lib_Truck_New_vs_Old.mp4)

Above, the OLED on the right is running the original Micro OLED library at 75fps. On the left the display is more than 348 frames per second. In real life it's quite smooth; the camera is not able to keep up. This is all over 400kHz I2C.

[link text](https://cdn.sparkfun.com/assets/learn_tutorials/2/1/8/8/OLED_Lib_Clock_New_vs_Old.mp4)

[![Comparison of displays](https://cdn.sparkfun.com/assets/learn_tutorials/2/1/8/8/OLED_Lib_Clock_New_vs_Old.mp4)](https://cdn.sparkfun.com/assets/learn_tutorials/2/1/8/8/OLED_Lib_Clock_New_vs_Old.mp4)

Above, the OLED on the right is running the original Micro OLED library at 75fps. On the left the display is more than 107 frames per second because the clock takes up a lot of the display, but not all of it. This is all over 400kHz I2C.

## Supported Products

* [LCD-14532](https://www.sparkfun.com/products/14532) - SparkFun Micro OLED Breakout (Qwiic)
* [LCD-17153](https://www.sparkfun.com/products/17153) - SparkFun Qwiic OLED Display (0.91 in, 128x32)
* [LCD-15173](https://www.sparkfun.com/products/15173) - SparkFun Transparent Graphical OLED Breakout (Qwiic) 
* [SPX-18996](https://www.sparkfun.com/products/18996) - smôl Display (0.91 in, 128x32) 

### Supported Microcontrollers - Arduino Environment

* [Artemis](https://www.sparkfun.com/products/15574)
* [SAMD51](https://www.sparkfun.com/products/14713)
* [ESP32](https://www.sparkfun.com/products/15663)
* [STM32](https://www.sparkfun.com/products/17712)
* [SAMD21](https://www.sparkfun.com/products/14812)
* [nrf5280](https://www.sparkfun.com/products/15025)
* [Teensy](https://www.sparkfun.com/products/16402)
* [ATMega328](https://www.sparkfun.com/products/18158)
