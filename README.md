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

<table class="table table-hover table-striped table-bordered">
  <tr align="center">
   <td><a href="https://www.sparkfun.com/products/14532"><img src="https://cdn.sparkfun.com//assets/parts/1/2/6/2/1/SparkFun_Qwiic_OLED_Tennis.gif"></a></td>
   <td><a href="https://www.sparkfun.com/products/17153"><img src="https://cdn.sparkfun.com//assets/parts/1/6/1/3/5/17153-SparkFun_Qwiic_OLED_Display__0.91_in__128x32_-05.jpg"></a></td>
   <td><a href="https://www.sparkfun.com/products/15173"><img src="https://cdn.sparkfun.com//assets/parts/1/3/5/8/8/SparkFun_Transparent_Graphical_OLED_Breakout__Qwiic__Hookup_Guide.gif"></a></td>
   <td><a href="https://www.sparkfun.com/products/18996"><img src="https://cdn.sparkfun.com//assets/parts/1/8/5/7/8/18996-smo__l_Display__0.91_in__128x32_-01.jpg"></a></td>
  </tr>
  <tr align="center">
    <td><a href="https://www.sparkfun.com/products/14532">SparkFun Micro OLED Breakout (Qwiic) (LCD-14532)</a></td>
    <td><a href="https://www.sparkfun.com/products/17153">SparkFun Qwiic OLED Display (0.91 in, 128x32) (LCD-17153)</a></td>
    <td><a href="https://www.sparkfun.com/products/15173">SparkFun Transparent Graphical OLED Breakout (Qwiic) (LCD-15173)</a></td>
    <td><a href="https://www.sparkfun.com/products/18996">smôl OLED Display (0.91 in, 128x32) (SPX-18996)</a></td>
  </tr>
</table>


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

The SparkFun Qwiic OLED Library is between 40% and 450% faster. The original [Micro OLED library](https://github.com/sparkfun/SparkFun_Micro_OLED_Arduino_Library) had a max output of ~75 frames per second. This library automatically only updates *what changed* we can reach more than 300 frames per second. What does that look like? 

https://user-images.githubusercontent.com/117102/160000377-b2de6d3f-a90f-42da-bd95-2a301c817c80.mp4

Above, the OLED on the right is running the original Micro OLED library at 75fps. On the left the display is more than 348 frames per second. In real life it's quite smooth; the camera is not able to keep up. This is at 400kHz I2C.

https://user-images.githubusercontent.com/117102/160000390-47a318ac-4158-4988-8831-44f7b9d32f01.mp4

Above, the OLED on the right is running the original Micro OLED library at 75fps. On the left the display is more than 107 frames per second because the clock takes up a lot of the display, but not all of it. This is at 400kHz I2C.

## Supported Products

* [LCD-13003](https://www.sparkfun.com/products/13003) - SparkFun Micro OLED Breakout
* [LCD-14532](https://www.sparkfun.com/products/14532) - SparkFun Micro OLED Breakout (Qwiic)
* [LCD-17153](https://www.sparkfun.com/products/17153) - SparkFun Qwiic OLED Display (0.91 in, 128x32)
* [LCD-15173](https://www.sparkfun.com/products/15173) - SparkFun Transparent Graphical OLED Breakout (Qwiic) 
* [SPX-18996](https://www.sparkfun.com/products/18996) - smôl OLED Display (0.91 in, 128x32) 

### Supported Microcontrollers - Arduino Environment

* [Artemis](https://www.sparkfun.com/products/15574)
* [SAMD51](https://www.sparkfun.com/products/14713)
* [ESP32](https://www.sparkfun.com/products/15663)
* [STM32](https://www.sparkfun.com/products/17712)
* [SAMD21](https://www.sparkfun.com/products/14812)
* [nrf5280](https://www.sparkfun.com/products/15025)
* [Teensy](https://www.sparkfun.com/products/16402)
* [ATMega328](https://www.sparkfun.com/products/18158)
