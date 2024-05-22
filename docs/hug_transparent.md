## Introduction

The future is here! You asked and we delivered - our [Qwiic Transparent Graphical OLED Breakout](https://www.sparkfun.com/products/15173) allows you to display custom images on a transparent screen using either I<sup>2</sup>C or SPI connections.

With Qwiic connectors it's quick (ha ha) and easy to get started with your own images. However, we still have broken out 0.1"-spaced pins in case you prefer to use a breadboard. Brilliantly lit in the dark and still visible by daylight, this OLED sports a display area of 128x64 pixels, 128x56 of which are completely transparent. Control of the OLED is based on the [HyperDisplay library](https://learn.sparkfun.com/tutorials/transparent-graphical-oled-breakout-hookup-guide#software-setup-and-programming) or SparkFun Qwiic OLED Arduino Library! For the scope of this tutorial, we will be using the SparkFun Qwiic OLED Arduino Library.

<center>
<div class="grid cards" style="width:500px;" markdown>

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
      <br />
      <center>[Purchase from SparkFun :fontawesome-solid-cart-plus:](https://www.sparkfun.com/products/15173){ .md-button .md-button--primary }</center>
    </a>

</div>
</center>

<div style="text-align: center">
  <iframe width="560" height="315" src="https://www.youtube.com/embed/OBOgxnctzwI?si=y-7UMvSFyld0ZiUp" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>
</div>

This hookup guide will show you how to get started drawing objects and characters on your OLED.

### Required Materials

To follow along with this tutorial, you will need the following materials. You may not need everything though depending on what you have. Add it to your cart, read through the guide, and adjust the cart as necessary.

* 1x [SparkFun Thing Plus - ESP32 WROOOM (USB-C) [WRL-20168]](https://www.sparkfun.com/products/18158)
* 1x [Reversible USB A to C Cable - 0.8m [CAB-15425]](https://www.sparkfun.com/products/15425)
* 1x Qwiic Cable
    * [Flexible Qwiic Cable - 50mm [PRT-17260]](https://www.sparkfun.com/products/17260), for short distances
    * [Flexible Qwiic Cable - 500mm [PRT-17257]](https://www.sparkfun.com/products/17257), for those that need to wire the board farther away from your microcontroller
* 1x [SparkFun Transparent Graphical OLED Breakout (Qwiic)[LCD-15173]](https://www.sparkfun.com/products/15173)



#### Microcontroller

To get started, you'll need a microcontroller to, well, control everything. We used the SparkFun Thing Plus - ESP32 WROOOM. However, any of the other microcontrollers that are compatible with the Qwiic OLED Arduino Library will work as well. Below are a few from the list that we provided earlier.

<div class="grid cards hide col-4" markdown>
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
</div>



#### USB Cable

Below are a few USB cables from the SparkFun catalog. Make sure to grab the associated USB cable that is compatible with your microcontroller.

<div class="grid cards hide col-4" markdown>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/21272">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/2/1/0/5/0/21272-_CAB-_01.jpg" style="width:140px; height:140px; object-fit:contain;" alt="SparkFun 4-in-1 Multi-USB Cable - USB-A Host">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/21272">
      <b>SparkFun 4-in-1 Multi-USB Cable - USB-A Host</b>
      <br />
      CAB-21272
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/15425">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/1/3/9/8/4/15425-Reversible_USB_A_to_C_Cable_-_0.8m-02.jpg" style="width:140px; height:140px; object-fit:contain;" alt="Reversible USB A to C Cable - 0.8m">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/15425">
      <b>Reversible USB A to C Cable - 0.8m</b>
      <br />
      CAB-15425
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/15428">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/1/3/9/8/7/15428-Reversible_USB_A_to_Reversible_Micro-B_Cable_-_0.8m-02.jpg" style="width:140px; height:140px; object-fit:contain;" alt="Reversible USB A to Reversible Micro-B Cable - 0.8m">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/15428">
      <b>Reversible USB A to Reversible Micro-B Cable - 0.8m</b>
      <br />
      CAB-15428
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
</div>


#### Qwiic

If the controller you choose doesn't have a built-in Qwiic connector, one of the following Qwiic shields that matches your preference of microcontroller is needed:

<div class="grid cards hide col-4" markdown>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/14352">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/1/2/3/4/3/14352-01.jpg" style="width:140px; height:140px; object-fit:contain;" alt="SparkFun Qwiic Shield for Arduino">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/14352">
      <b>SparkFun Qwiic Shield for Arduino</b>
      <br />
      DEV-14352
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/17119">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/1/6/0/8/7/17119-SparkFun_Qwiic_Shield_for_Teensy-07.jpg" style="width:140px; height:140px; object-fit:contain;" alt="SparkFun Qwiic Shield for Teensy">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/17119">
      <b>SparkFun Qwiic Shield for Teensy</b>
      <br />
      DEV-17119
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/16789">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/1/6/0/8/7/17119-SparkFun_Qwiic_Shield_for_Teensy-07.jpg" style="width:140px; height:140px; object-fit:contain;" alt="SparkFun Qwiic Shield for Arduino Nano">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/16789">
      <b>SparkFun Qwiic Shield for Arduino Nano</b>
      <br />
      DEV-16789
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
</div>



You will also need a Qwiic cable to connect the shield to your OLED, choose a length that suits your needs.

<div class="grid cards hide col-4" markdown>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/17260">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/1/6/2/4/7/17260-Flexible_Qwiic_Cable_-_50mm-01.jpg" style="width:140px; height:140px; object-fit:contain;" alt="Flexible Qwiic Cable - 50mm">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/17260">
      <b>Flexible Qwiic Cable - 50mm</b>
      <br />
      PRT-17260
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/17259">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/1/6/2/4/6/17259-Flexible_Qwiic_Cable_-_100mm-01.jpg" style="width:140px; height:140px; object-fit:contain;" alt="Flexible Qwiic Cable - 100mm">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/17259">
      <b>Flexible Qwiic Cable - 100mm</b>
      <br />
      PRT-17259
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/17258">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/1/6/2/4/5/17258-Flexible_Qwiic_Cable_-_200mm-01.jpg" style="width:140px; height:140px; object-fit:contain;" alt="Flexible Qwiic Cable - 200mm">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/17258">
      <b>Flexible Qwiic Cable - 200mm</b>
      <br />
      PRT-17258
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/17257">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/1/6/2/4/4/17257-Flexible_Qwiic_Cable_-_500mm-01.jpg" style="width:140px; height:140px; object-fit:contain;" alt="Flexible Qwiic Cable - 500mm">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/17257">
      <b>Flexible Qwiic Cable - 500mm</b>
      <br />
      PRT-17257
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
</div>

Of course, you will also need A Tranparent Graphical OLED Breakout if you have not added that to you cart already.

<center>
<div class="grid cards" style="width:500px;" markdown>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/15173">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/1/3/5/8/8/15173-SparkFun_Transparent_Graphical_OLED_Breakout__Qwiic_-01a.jpg" style="width:140px; height:140px; object-fit:contain;" alt="SparkFun Transparent Graphical OLED Breakout (Qwiic)">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/15173">
      <b>SparkFun Transparent Graphical OLED Breakout (Qwiic)</b>
      <br />
      LCD-15173
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
</div>
</center>



###Suggested Reading

If you aren't familiar with the Qwiic Connection System, we recommend reading [here for an overview](https://www.sparkfun.com/qwiic).

<div style="text-align: center">
  <table style="border-style:none">
    <tr>
     <td style="text-align: center; vertical-align: middle;">
     <div style="text-align: center"><a href="https://www.sparkfun.com/qwiic"><img src="../assets/Qwiic-registered-updated.png" alt="Qwiic Connection System" title="Click to learn more about the Qwiic Connection System!"></a>
     </div>
    </td>
    </tr>
    <tr>
      <td style="text-align: center; vertical-align: middle;"><div style="text-align: center"><i><a href="https://www.sparkfun.com/qwiic">Qwiic Connection System</a></i></div></td>
    </tr>
  </table>
</div>

We would also recommend taking a look at the following tutorials if you aren't familiar with them.

<div class="grid cards hide col-4" markdown>
<!-- ----------WHITE SPACE BETWEEN GRID CARDS---------- -->
-   <a href="https://learn.sparkfun.com/tutorials/i2c">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/c/264-148/assets/learn_tutorials/8/2/I2C-Block-Diagram.jpg" style="width:264px; height:148px; object-fit:contain;" alt="I2C">
      </figure>
    </a>

    ---

    <a href="https://learn.sparkfun.com/tutorials/i2c">
      <b>I2C</b>
    </a>
<!-- ----------WHITE SPACE BETWEEN GRID CARDS---------- -->
-   <a href="https://learn.sparkfun.com/tutorials/qwiic-shield-for-arduino--photon-hookup-guide">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/learn_tutorials/6/8/4/Qwic_Shield_Hookup_Guide-05.jpg" style="width:264px; height:148px; object-fit:contain;" alt="Qwiic Shield for Arduino & Photon Hookup Guide">
      </figure>
    </a>

    ---

    <a href="https://learn.sparkfun.com/tutorials/qwiic-shield-for-arduino--photon-hookup-guide">
      <b>Qwiic Shield for Arduino & Photon Hookup Guide</b>
    </a>
<!-- ----------WHITE SPACE BETWEEN GRID CARDS---------- -->
</div>


## Hardware Overview

Listed below are some of the operating ranges and characteristics of the Transparent Graphical OLED Breakout.

<div style="text-align: center;">
    <table>
        <tr>
            <th style="text-align: center; border: solid 1px #cccccc;">Characteristic
            </th>
            <th style="text-align: center; border: solid 1px #cccccc;">Range
            </th>
        </tr>
        <tr style="vertical-align:middle;">
            <td style="text-align: center; border: solid 1px #cccccc;">Voltage
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;"><b>1.65V-3.3V</b>,<br />typically 3.3V via the Qwiic Cable
            </td>
        </tr>
        <tr style="vertical-align:middle;">        
            <td style="text-align: center; border: solid 1px #cccccc;">Supply Current
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;">400 mA
            </td>
        </tr>
        <tr style="vertical-align:middle;">        
            <td style="text-align: center; border: solid 1px #cccccc;">I<sup>2</sup>C Address
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;">0X3C (Default), 0X3D (Closed Jumper)
            </td>
        </tr>
    </table>
</div>



###Graphical Display

The graphical display is where all the fun stuff happens. The glass itself measures 42mm x 27.16mm, with a pixel display that is 35.5 x 18mm. It houses 128x64 pixels, 128x56 of which are transparent.

<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://cdn.sparkfun.com/assets/learn_tutorials/6/1/2/GraphicalDisplay1.jpg"><img src="https://cdn.sparkfun.com/r/600-600/assets/learn_tutorials/6/1/2/GraphicalDisplay1.jpg" width="600px" height="600px" alt="Display Screen"></a></td>
    </tr>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>Graphical Display</i></td>
    </tr>
  </table>
</div>



###Qwiic Connectors

There are two Qwiic connectors on the board such that you can daisy-chain the boards should you choose to do so. If you're unfamiliar with our Qwiic Connect System, head on over to our [Qwiic page](https://www.sparkfun.com/qwiic) to see the advantages!

<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://cdn.sparkfun.com/assets/learn_tutorials/6/1/2/QwiicConnectors1.jpg"><img src="https://cdn.sparkfun.com/r/600-600/assets/learn_tutorials/6/1/2/QwiicConnectors1.jpg" width="600px" height="600px" alt="Qwiic Connectors"></a></td>
    </tr>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>Qwiic Connectors</i></td>
    </tr>
  </table>
</div>



###GPIO Pins

When you look at the GPIO pins, you'll notice that the labels are different from one side to the other. One side is labeled for I<sup>2</sup>C, the other side is labeled for SPI.


<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://cdn.sparkfun.com/assets/learn_tutorials/6/1/2/GPIOPinsFront1.jpg"><img src="https://cdn.sparkfun.com/r/600-600/assets/learn_tutorials/6/1/2/GPIOPinsFront1.jpg" width="600px" height="600px" alt="I2C Pins"></a></td>
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://cdn.sparkfun.com/assets/learn_tutorials/6/1/2/15173-GPIOBackCorrected.jpg"><img src="https://cdn.sparkfun.com/r/600-600/assets/learn_tutorials/6/1/2/15173-GPIOBackCorrected.jpg" width="600px" height="600px" alt="SPI Pins"></a></td>
    </tr>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>I<sup>2</sup>C Labels</i></td>
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>SPI Labels</i></td>
    </tr>
  </table>
</div>



###Power LED

This bad boy will light up when the board is powered up correctly.

<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://cdn.sparkfun.com/assets/learn_tutorials/6/1/2/PowerLED1.jpg"><img src="https://cdn.sparkfun.com/r/600-600/assets/learn_tutorials/6/1/2/PowerLED1.jpg" width="600px" height="600px" alt="I2C Pins"></a></td>
    </tr>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>Power LED</i></td>
    </tr>
  </table>
</div>

You can disable the power LED by cutting the LED jumpers on the back of the board.

<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://cdn.sparkfun.com/assets/learn_tutorials/6/1/2/15173-LEDJumper.jpg"><img src="https://cdn.sparkfun.com/r/600-600/assets/learn_tutorials/6/1/2/15173-LEDJumper.jpg" width="600px" height="600px" alt="Power LED Jumpers"></a></td>
    </tr>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>Power LED Jumpers</i></td>
    </tr>
  </table>
</div>



###JPX Jumpers

The JPX jumpers are used to either change the I<sup>2</sup>C address or configure the board to use SPI communications. The other two jumpers allow you to disconnect the power LED and to disconnect the I<sup>2</sup>C pull-up resistors when chaining several Qwiic devices.

<div style="text-align: center;">
    <table>
        <tr>
            <th style="text-align: center; border: solid 1px #cccccc;">Jumper
            </th>
            <th style="text-align: center; border: solid 1px #cccccc;">Function
            </th>
        </tr>
        <tr style="vertical-align:middle;">
            <td style="text-align: center; border: solid 1px #cccccc;">JP1
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;">Holds the Chip Select line low when closed. Close for I<sup>2</sup>C, open for SPI
            </td>
        </tr>        
        <tr style="vertical-align:middle;">
            <td style="text-align: center; border: solid 1px #cccccc;">JP2
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;">Selects the address in I<sup>2</sup>C mode. Closed for <b>0x30 by default</b> and open for 0x31. Open for SPI mode to release the D/C pin
            </td>
        </tr>        
        <tr style="vertical-align:middle;">
            <td style="text-align: center; border: solid 1px #cccccc;">JP3
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;">Used to select I<sup>2</sup>C or SPI mode. Close for I<sup>2</sup>C, open for SPI
            </td>
        </tr>        
        <tr style="vertical-align:middle;">
            <td style="text-align: center; border: solid 1px #cccccc;">JP4
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;">This jumper should be closed for I<sup>2</sup>C and open for SPI. This connection allows SDA to be bi-directional
            </td>
        </tr>
    </table>
</div>

<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://cdn.sparkfun.com/r/600-600/assets/learn_tutorials/6/1/2/JPJumpersBackCorrected.jpg"><img src="https://cdn.sparkfun.com/r/600-600/assets/learn_tutorials/6/1/2/JPJumpersBackCorrected.jpg" width="600px" height="600px" alt="JP1-JP4"></a></td>
    </tr>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>JPX Jumper</i></td>
    </tr>
  </table>
</div>



###I<sup>2</sup>C Pull-Up Jumper

I<sup>2</sup>C devices contain open drains so we include resistors on our boards to allow these devices to pull pins high. This becomes a problem if you have a large number of I<sup>2</sup>C devices chained together. If you plan to daisy chain more than a few Qwiic boards together, you'll need to <a href="https://learn.sparkfun.com/tutorials/how-to-work-with-jumper-pads-and-pcb-traces#cutting-a-trace-between-jumper-pads">cut this I<sup>2</sup>C pull-up jumper</a>.


<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://cdn.sparkfun.com/assets/learn_tutorials/6/1/2/15173-I2CPUJumper.jpg"><img src="https://cdn.sparkfun.com/r/600-600/assets/learn_tutorials/6/1/2/15173-I2CPUJumper.jpg" width="600px" height="600px" alt="JP1 to JP4"></a></td>
    </tr>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>I<sup>2</sup>C PU Jumper</i></td>
    </tr>
  </table>
</div>



## Hardware Hookup

Now that you know what's available on your breakout board we can check out the options for connecting it to the brains of your project. There are two options to use - either I<sup>2</sup>C or SPI - and they each have their own advantages and drawbacks. Read on to choose the best option for your setup.

!!! warning
    <b>Reminder!</b> This breakout can only handle up to <b>3.3V</b> on the pins, so make sure to do some <a href="https://learn.sparkfun.com/tutorials/bi-directional-logic-level-converter-hookup-guide"><b>level shifting</b></a> if you're using a 5V microcontroller.



### I<sup>2</sup>C (Qwiic)

The easiest way to start using the Transparent Graphical OLED is to use a [Qwiic Cable](https://www.sparkfun.com/products/15081) along with a Qwiic compatible microcontroller (such as the [ESP32 Thing Plus](https://www.sparkfun.com/products/14689)). You can also use the [Qwiic Breadboard Cable](https://www.sparkfun.com/products/14425) to attach any I<sup>2</sup>C capable microcontroller, or take the scenic route and [solder](https://learn.sparkfun.com/tutorials/how-to-solder---through-hole-soldering) in all the I<sup>2</sup>C wires to the plated-through connections on the board.

<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://cdn.sparkfun.com/assets/learn_tutorials/6/1/2/QwiicConnectors1.jpg"><img src="https://cdn.sparkfun.com/r/300-300/assets/learn_tutorials/6/1/2/QwiicConnectors1.jpg" width="600px" height="600px" alt="Qwiic Connector"></a></td>
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://cdn.sparkfun.com/assets/learn_tutorials/6/1/2/GPIOPinsFront.jpg"><img src="https://cdn.sparkfun.com/r/300-300/assets/learn_tutorials/6/1/2/GPIOPinsFront.jpg" width="600px" height="600px" alt="I2C Pinout"></a></td>
    </tr>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>Top View</i></td>
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>I<sup>2</sup>C Pinout/i></td>
    </tr>
  </table>
</div>


So why use I<sup>2</sup>C? It's easy to connect with the Qwiic system, and you can put up to two of the Transparent Graphical Breakouts on the same bus without using any more microcontroller pins. That simplicity comes at a cost to performance though. The maximum clock speed of the I<sup>2</sup>C bus is 400 kHz, and there is additional overhead in data transmission to indicate which bytes are data and which are commands. This means that the I<sup>2</sup>C connection is best for showing static images.

<div style="text-align: center;">
    <table>
        <tr>
            <th style="text-align: center; border: solid 1px #cccccc;">Breakout Pin
            </th>
            <th style="text-align: center; border: solid 1px #cccccc;">Microcontroller Pin Requirements
            </th>
        </tr>
        <tr style="vertical-align:middle;">
            <td style="text-align: center; border: solid 1px #cccccc;">GND
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;">Ground pin. Connect these so the two devices agree on voltages
            </td>
        </tr>
        <tr style="vertical-align:middle;">
            <td style="text-align: center; border: solid 1px #cccccc;">3V3
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;">3.3V supply pin, capable of up to 400 mA output
            </td>
        </tr>
        <tr style="vertical-align:middle;">
            <td style="text-align: center; border: solid 1px #cccccc;">SDA
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;">SDA - the bi-directional data line of your chosen I2C port
            </td>
        </tr>
        <tr style="vertical-align:middle;">
            <td style="text-align: center; border: solid 1px #cccccc;">SCL
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;">SCL - the clock line of your chosen I2C port
            </td>
        </tr>
        <tr style="vertical-align:middle;">
            <td style="text-align: center; border: solid 1px #cccccc;">SA0
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;"><i>Optional</i> : change the I2C address of the breakout. Make sure to cut JP2
            </td>
        </tr>
        <tr style="vertical-align:middle;">
            <td style="text-align: center; border: solid 1px #cccccc;">RST
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;"><i>Optional</i> : reset the breakout to a known state by pulsing this low
            </td>
        </tr>
    </table>
</div>  



### SPI

SPI solves the I<sup>2</sup>C speed problems. With SPI there is a control signal that indicates data or command and the maximum clock speed is 10 MHz -- giving SPI 50x more speed! However,  it doesn't have the same conveniences of the polarized Qwiic connector and low pin usage. You'll need to [solder](https://learn.sparkfun.com/tutorials/how-to-solder---through-hole-soldering) to the pins.

<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://cdn.sparkfun.com/assets/learn_tutorials/6/1/2/15173-GPIOBackCorrected.jpg"><img src="https://cdn.sparkfun.com/r/300-300/assets/learn_tutorials/6/1/2/15173-GPIOBackCorrected.jpg" width="600px" height="600px" alt="SPI Pinout"></a></td>
    </tr>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>SPI Pinout</i></td>
    </tr>
  </table>
</div>

You can use SPI to connect as many breakouts as you want. For N displays you will need to use at least N + 3 data pins. That's because the MOSI, SCLK, and D/C pins can be shared between displays but each breakout needs its own dedicated Chip Select (CS) pin.

<div style="text-align: center;">
    <table>
        <tr>
            <th style="text-align: center; border: solid 1px #cccccc;">Breakout Pin
            </th>
            <th style="text-align: center; border: solid 1px #cccccc;">Microcontroller Pin Requirements
            </th>
        </tr>
        <tr style="vertical-align:middle;">
            <td style="text-align: center; border: solid 1px #cccccc;">CS
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;">A GPIO pin, set low when talking to the breakout
            </td>
        </tr>
        <tr style="vertical-align:middle;">
            <td style="text-align: center; border: solid 1px #cccccc;">D/C
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;">A GPIO pin, indicates if bytes are data or commands
            </td>
        </tr>
        <tr style="vertical-align:middle;">
            <td style="text-align: center; border: solid 1px #cccccc;">SCLK
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;">The clock output of your chosen SPI port
            </td>
        </tr>
        <tr style="vertical-align:middle;">
            <td style="text-align: center; border: solid 1px #cccccc;">MOSI
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;">The data output of your chosen SPI port
            </td>
        </tr>
        <tr style="vertical-align:middle;">
            <td style="text-align: center; border: solid 1px #cccccc;">3V3
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;">3.3V supply pin, capable of up to 400 mA output
            </td>
        </tr>
        <tr style="vertical-align:middle;">
            <td style="text-align: center; border: solid 1px #cccccc;">GND
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;">Ground pin. Connect these so the two devices agree on voltages
            </td>
        </tr>
    </table>
</div>  

!!! warning
    Make sure to cut jumpers JP1, JP2, JP3, and JP4 when using SPI mode!

    <div style="text-align: center;">
      <table>
        <tr style="vertical-align:middle;">
         <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://cdn.sparkfun.com/r/600-600/assets/learn_tutorials/6/1/2/15173-CutJumpers.jpg"><img src="https://cdn.sparkfun.com/r/600-600/assets/learn_tutorials/6/1/2/15173-CutJumpers.jpg" width="600px" height="600px" alt="Cut Jumpers for SPI Mode"></a></td>
        </tr>
        <tr style="vertical-align:middle;">
         <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>Cut Jumpers for SPI Mode</i></td>
        </tr>
      </table>
    </div>



## Software

The Transparent OLED Breakout (Qwiic) uses the SparkFun QWIIC OLED Arduino Library. The [SparkFun Qwiic OLED library Getting Started guide](software.md) has library setup instructions and usage examples. Additionally, the full library API documentation is available in the [SparkFun Qwiic OLED Library API Reference guide](api_device.md).

<div style="text-align: center">
    <a href="../api_device" class="md-button">SparkFun Qwiic OLED Library API Reference Guide</a>
</div>



## Resources

For more information on the Transparent Graphical OLED Breakout, check out some of the links here:

* [Schematic (PDF)](https://cdn.sparkfun.com/assets/5/e/7/b/5/SparkFun_Transparent_Graphical_OLED_Breakout.pdf)
* [Eagle Files (ZIP)](https://cdn.sparkfun.com/assets/2/0/2/c/b/TransparentGraphicalOLEDBreakout.zip)
* [GitHub Hardware Repo](https://github.com/sparkfun/Qwiic_Transparent_Graphical_OLED)
* [SFE Product Showcase](https://youtu.be/vzFuVbxBfXI)
