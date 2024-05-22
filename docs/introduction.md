![SparkFun Qwiic OLED Arduino Library](img/OLEDLibBanner.png "SparkFun Qwiic OLED Arduino Library")

The SparkFun Qwiic OLED Arduino Library is a single graphics module that supports all SparkFun OLED boards based on the SSD1306 from Solomon Systech. Prior to this library, three different libraries were used to support our four different OLED boards.

The SparkFun Qwiic OLED Library delivers a common implementation for all our Qwiic OLED products, delivering a unified, fast, and efficient solution that implements a familiar and easy to understand user experience.



### Key Features

*   Implements common graphics capabilities: pixel, line, rectangle, filled rectangle, circle, filled circle, bitmap, text and raster operators (i.e. XOR).
* Smart data transfer to the device – only sends _dirty_ regions of the graphics buffer to the OLED device, not the entire buffer.
* High performance – 2x faster than our previous OLED library, often much higher.
* Efficient memory usage. No dynamic memory utilized. Static resources are loaded once, and only on explicit declaration.
* Implements a familiar interface, making migration from older libraries straight forward



### Getting Started

The [Software Setup](software.md) outlines library installation and the general use of the Qwiic OLED library.

Detailed examples are included as part of the library installation process and available in the Arduino IDE menu:  **File** > **Examples** >  **SparkFun Qwiic OLED Arduino Library**. A walk-thru of key examples is contained in the [Examples](../sparkfun-qwiic-oled-arduino-library-examples/ex_01_hello/) section of this documentation set.

!!! note
    For v1.0.5 of the SparkFun Qwiic OLED Arduino Library, we named the library as SparkFun Qwiic OLED Graphics Library. After v1.0.6, we updated the name to say SparkFun Qwiic OLED Arduino Library. You may have multiple versions in your Arduino libraries folder if you installed the library more than once. To avoid confusion, issues compiling, and to use the latest version, we recommend removing the "_SparkFun Qwiic OLED Graphics Library_" folder should you decide to use the latest and greatest version. This will probably be located under **..Documents\Arduino\libraries**, that is if you are using Windows.

!!! note
    Note that we have more than one Arduino Library for the micro OLED. If you have the older Arduino Library, make sure to not confuse the two libraries. You will notice that the older library will be called "**SparkFun Micro OLED Breakout**". The example code will include the following line of code: `#include <SFE_MicroOLED.h>`.

A full [API Reference](../api_device) is also provided for the library.

<div style="text-align: center"><a href="../api_device" class="md-button md-button--primary">SparkFun Qwiic OLED Arduino Library: API Reference</a></div>



### Supported Products

The SparkFun Qwiic OLED Arduino Library supports the following SparkFun products.

<div class="grid cards col-4" markdown>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/22495">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/2/2/5/5/8/22495-OLED-front.jpg" style="width:140px; height:140px; object-fit:contain;" alt="SparkFun Micro OLED Breakout (Qwiic)">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/22495">
      <b>SparkFun Micro OLED Breakout (Qwiic)</b>
      <br />
      LCD-22495
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/24606">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/r/600-600/assets/parts/2/4/9/6/3/LCD-24606-Qwiic-OLED-Display-Action-11.jpg" style="width:140px; height:140px; object-fit:contain;" alt="SparkFun Qwiic OLED Display (0.91 in, 128x32)">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/24606">
      <b>SparkFun Qwiic OLED Display (0.91 in, 128x32)</b>
      <br />
      LCD-24606
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/15173">
      <figure markdown>
        <img src="https://cdn.sparkfun.com//assets/parts/1/3/5/8/8/15173-SparkFun_Transparent_Graphical_OLED_Breakout__Qwiic_-01a.jpg" style="width:140px; height:140px; object-fit:contain;" alt="SparkFun Transparent Graphical OLED Breakout (Qwiic)">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/15173">
      <b>SparkFun Transparent Graphical OLED Breakout (Qwiic)</b>
      <br />
      LCD-15173
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/23453">
      <figure markdown>
        <img src="https://cdn.sparkfun.com//assets/parts/2/3/7/6/0/23453-Qwiic-OLED-Feature-WithDisplay.jpg" style="width:140px; height:140px; object-fit:contain;" alt="SparkFun Qwiic OLED (1.3in., 128x64)">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/23453">
      <b>SparkFun Qwiic OLED (1.3in., 128x64)</b>
      <br />
      LCD-23453
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
</div>



### Supported Microcontrollers - Arduino Environment

The following architectures are supported in the Arduino Library.

* [Artemis](https://www.sparkfun.com/products/15574)
* [SAMD51](https://www.sparkfun.com/products/14713)
* [ESP32](https://www.sparkfun.com/products/20168)
* [STM32](https://www.sparkfun.com/products/17712)
* [SAMD21](https://www.sparkfun.com/products/14812)
* [nrf5280](https://www.sparkfun.com/products/15025)
* [Teensy](https://www.sparkfun.com/products/16402)
* [ATMega328P](https://www.sparkfun.com/products/18158)

Below are a few of those processors populated on Arduino boards from the [SparkFun catalog](https://www.sparkfun.com/categories/242). You will need to make sure to check the associated hookup guides for additional information about compatible cables, drivers, or board add-ons.

<div class="grid cards col-4" markdown>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/15574">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/1/4/1/7/0/15574-SparkFun_Thing_Plus_-_Artemis-01.jpg" style="width:140px; height:140px; object-fit:contain;" alt="SparkFun Thing Plus - Artemis">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/15574">
      <b>SparkFun Thing Plus - Artemis</b>
      <br />
      WRL-15574
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/14713">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/1/2/9/2/7/14713-SparkFun_Thing_Plus_-_SAMD51-01.jpg" style="width:140px; height:140px; object-fit:contain;" alt="SparkFun Thing Plus - SAMD51">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/14713">
      <b>SparkFun Thing Plus - SAMD51</b>
      <br />
      DEV-14713
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/20168">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/1/9/9/6/8/20168Diagonal.jpg" style="width:140px; height:140px; object-fit:contain;" alt="SparkFun Thing Plus - ESP32 WROOM (USB-C)">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/20168">
      <b>SparkFun Thing Plus - ESP32 WROOM (USB-C)</b>
      <br />
      WRL-20168
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/17712">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/1/6/8/1/5/17712-Sparkfun_Thing_Plus_-_STM32-01.jpg" style="width:140px; height:140px; object-fit:contain;" alt="SparkFun Thing Plus - STM32">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/17712">
      <b>SparkFun Thing Plus - STM32</b>
      <br />
      DEV-17712
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/14812">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/1/3/0/7/9/14812-SparkFun_RedBoard_Turbo_-_SAMD21_Development_Board-01b.jpg" style="width:140px; height:140px; object-fit:contain;" alt="SparkFun RedBoard Turbo - SAMD21 Development Board">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/14812">
      <b>SparkFun RedBoard Turbo - SAMD21 Development Board</b>
      <br />
      DEV-14812
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/15025">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/1/3/3/5/1/15025-SparkFun_Pro_nRF52840_Mini_-_Bluetooth_Development_Board-01.jpg" style="width:140px; height:140px; object-fit:contain;" alt="SparkFun Pro nRF52840 Mini - Bluetooth Development Board">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/15025">
      <b>SparkFun Pro nRF52840 Mini - Bluetooth Development Board</b>
      <br />
      DEV-15025
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/16402">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/1/5/1/3/2/16402-SparkFun_MicroMod_Teensy_Processor-01.jpg" style="width:140px; height:140px; object-fit:contain;" alt="SparkFun MicroMod Teensy Processor">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/16402">
      <b>SparkFun MicroMod Teensy Processor</b>
      <br />
      DEV-16402
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/18158">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/1/7/4/8/7/18158-SparkFun_RedBoard_Plus-01.jpg" style="width:140px; height:140px; object-fit:contain;" alt="SparkFun RedBoard Plus (ATmega328P)">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/18158">
      <b>SparkFun RedBoard Plus (ATmega328P)</b>
      <br />
      DEV-18158
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
</div>

!!! note
    Unfortunately, the ATmega32U4 is not supported under this library. We recommend either using a different microcontroller or rolling back to the previous library written for the display.


### License

The SparkFun Qwiic OLED Arduino Library is licensed using the Open Source [MIT License](https://github.com/sparkfun/SparkFun_Qwiic_OLED_Arduino_Library/blob/main/LICENSE.md#code):

    --8<-- "LICENSE.md:35:55"
