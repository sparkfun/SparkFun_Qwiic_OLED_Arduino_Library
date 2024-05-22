## Introduction

The [SparkFun Qwiic OLED Display](https://www.sparkfun.com/products/24606) can display up to four lines of text and features 128x32 pixels in a small 0.91” (diagonal) frame. As an OLED, this display does not have a back light layer (unlike LCDs) and therefore it’s thinner, consumes less power, and has higher contrast.

<center>
<div class="grid cards" style="width:500px;" markdown>

-   <a href="https://www.sparkfun.com/products/24606">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/2/4/9/6/3/LCD-24606-Qwiic-OLED-Display-Action-11-Blue.jpg" style="width:140px; height:140px; object-fit:contain;" alt="SparkFun Qwiic OLED Display (0.91 in., 128x32)">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/24606">
      <b>SparkFun Qwiic OLED Display (0.91 in., 128x32)</b>
      <br />
      LCD-24606
      <br />
      <center>[Purchase from SparkFun :fontawesome-solid-cart-plus:](https://www.sparkfun.com/products/24606){ .md-button .md-button--primary }</center>
    </a>

</div>
</center>

In this section, we'll go over the hardware and how to hookup the breakout board.



### Required Materials

To follow along with this tutorial, you will need the following materials. You may not need everything though depending on what you have. Add it to your cart, read through the guide, and adjust the cart as necessary.

* 1x [SparkFun RedBoard Plus [DEV-18158]](https://www.sparkfun.com/products/18158)
* 1x [Reversible USB A to C Cable - 0.8m [CAB-15425]](https://www.sparkfun.com/products/15425)
* 1x Qwiic Cable
    * [Flexible Qwiic Cable - 50mm [PRT-17260]](https://www.sparkfun.com/products/17260), for short distances
    * [Flexible Qwiic Cable - 500mm [PRT-17257]](https://www.sparkfun.com/products/17257), for those that need to wire the board farther away from your microcontroller
* 1x [SparkFun Qwiic OLED Display (0.91 in., 128x32) [LCD-22495]](https://www.sparkfun.com/products/22495)



#### Microcontroller

To get started, you'll need a microcontroller to, well, control everything. We used the RedBoard with the ATmega328P for the Qwiic micro OLED. However, any of the other microcontrollers that are compatible with the Qwiic OLED Arduino Library will work as well. Below are a few from the list that we provided earlier.

<div class="grid cards hide col-4" markdown>
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

Of course, you will also need a Qwiic Micro OLED if you have not added that to you cart already.

<center>
<div class="grid cards" style="width:500px;" markdown>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
-   <a href="https://www.sparkfun.com/products/24606">
      <figure markdown>
        <img src="https://cdn.sparkfun.com/assets/parts/2/4/9/6/3/LCD-24606-Qwiic-OLED-Display-Action-11-Blue.jpg" style="width:140px; height:140px; object-fit:contain;" alt="SparkFun Qwiic OLED Display (0.91 in., 128x32)">
      </figure>
    </a>

    ---

    <a href="https://www.sparkfun.com/products/24606">
      <b>SparkFun Qwiic OLED Display (0.91 in., 128x32)</b>
      <br />
      LCD-24606
    </a>
<!-- ----------WHITE SPACE BETWEEN PRODUCTS---------- -->
</div>
</center>



## Hardware Overview

In this section, we will highlight the hardware and pins that are broken out on the SparkFun Qwiic OLED Display (0.91 in., 128x32).

<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/LCD-24606-Qwiic-OLED-Display_top.jpg?raw=true"><img src="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/LCD-24606-Qwiic-OLED-Display_top.jpg?raw=true" width="600px" height="600px" alt="Top View"></a></td>
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/LCD-24606-Qwiic-OLED-Display_bottom.jpg?raw=true"><img src="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/LCD-24606-Qwiic-OLED-Display_bottom.jpg?raw=true" width="600px" height="600px" alt="Bottom View"></a></td>
    </tr>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>Top View</i></td>
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>Bottom View</i></td>
    </tr>
  </table>
</div>



### OLED Display (0.91", 128x32)

The OLED screen has a pixel resolution of 128x32, a panel size of 30.0mm x 11.5mm x 1.2mm, and an active area of 22.384mm x 5.584mm. The driver chip is the SSD1306. For information can be found in the datasheet linked in the Resources.

<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/LCD-24606-Qwiic-OLED-Display_Highlighted.jpg?raw=true"><img src="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/LCD-24606-Qwiic-OLED-Display_Highlighted.jpg?raw=true" width="600px" height="600px" alt="OLED Highlighted"></a></td>
    </tr>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>OLED Highlighted</i></td>
    </tr>
  </table>
</div>

!!! note
    The SparkFun Qwiic OLED Arduino Library works for multiple displays. However, there are some caveats in the size of the display with the text. While you can technically display all fonts in the narrow OLED display, some characters (numbers, letters, and/or symbols depending on the font) will be too big to fully display on the screen. For example, the fonts for the 31x48 (i.e. `&QW_FONT_31X48`) and large numbers (i.e. `&QW_FONT_LARGENUM`) are too big to fit within the display.

    Using the OLED display (0.91", 128x32) we found that we were able to fit:

    * 4x lines, 21x characters using the 5x7 (i.e. `&QW_FONT_5X7`)
    * 2x lines, 14x characters using the 8x16 (i.e. `&QW_FONT_8X16`)
    * 2x lines, 11x characters using the 7 segment (i.e. `&QW_FONT_7SEGMENT`)



### Power

Power is applied through the vertical Qwiic connectors on the back of the board. The recommended input voltage is **3.3V**. The logic levels for the Qwiic OLED Display (0.9", 128x32) is **3.3V**.

<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/LCD-24606-Qwiic-OLED-Display_Qwiic_Connectors.jpg?raw=true"><img src="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/LCD-24606-Qwiic-OLED-Display_Qwiic_Connectors.jpg?raw=true" width="600px" height="600px" alt="Power"></a></td>
    </tr>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>Power</i></td>
    </tr>
  </table>
</div>



### Qwiic and I<sup>2</sup>C

There are two vertical Qwiic connectors populated on the back of the board. You can use either connectors to provide power and send data through I<sup>2</sup>C. The [Qwiic ecosystem](https://www.sparkfun.com/qwiic) is made for fast prototyping by removing the need for soldering. All you need to do is plug a Qwiic cable into the Qwiic connector and voila!

* **SCL** &mdash; I<sup>2</sup>C clock
* **SDA** &mdash; I<sup>2</sup>C data
* **3.3V** &mdash; Power
* **GND** &mdash; Ground

<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/LCD-24606-Qwiic-OLED-Display_Qwiic_Connectors.jpg?raw=true"><img src="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/LCD-24606-Qwiic-OLED-Display_Qwiic_Connectors.jpg?raw=true" width="600px" height="600px" alt="Qwiic Connectors"></a></td>
    </tr>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>Vertical Qwiic Connectors</i></td>
    </tr>
  </table>
</div>

The address of the display is **0x3C**.

!!! note
    On the back of the board, the power and I<sup>2</sup>C pins are broken out to test points. These are used in our production department for quality control using custom testbeds. These could be an alternative option to connect to the pins. However, we recommend using the Qwiic connectors to easily connect to the OLED display. Note that the I<sup>2</sup>C pins are also in a different order compared to a standard I<sup>2</sup>C Qwiic connector should you decide to solder to the test points.

    <div style="text-align: center;">
      <table>
        <tr style="vertical-align:middle;">
         <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/LCD-24606-Qwiic-OLED-Display-Back_I2C_Test_Points.jpg?raw=true"><img src="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/LCD-24606-Qwiic-OLED-Display-Back_I2C_Test_Points.jpg?raw=true" width="600px" height="600px" alt="I2C Test Points"></a></td>
        </tr>
        <tr style="vertical-align:middle;">
         <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>I2C Test Points</i></td>
        </tr>
      </table>
    </div>



### Jumpers

!!!note
    If this is your first time working with jumpers, check out the [How to Work with Jumper Pads and PCB Traces](https://learn.sparkfun.com/tutorials/how-to-work-with-jumper-pads-and-pcb-traces/all) tutorial for more information.

The board includes a 1x3 jumper on the back of the board.

* **I2C** &mdash; This three way jumper labeled **I2C** is connected to two 4.7k&ohm; pull-up resistors to the I<sup>2</sup>C data and clock lines. For users that have multiple Qwiic-enabled devices with pull-up resistors enabled, the parallel equivalent resistance will create too strong of a pull-up for the bus to operate correctly. As a general rule of thumb, [disable all but one pair of pull-up resistors](https://learn.sparkfun.com/tutorials/i2c/all#i2c-at-the-hardware-level) if multiple devices are connected to the bus.

<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/LCD-24606-Qwiic-OLED-Display_Jumpers.jpg?raw=true"><img src="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/LCD-24606-Qwiic-OLED-Display_Jumpers.jpg?raw=true" width="600px" height="600px" alt="Jumpers Highlighted"></a></td>
    </tr>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>Jumpers Highlighted</i></td>
    </tr>
  </table>
</div>



### Board Dimensions

Version 1.1 is a bit smaller than previous versions since the board includes vertical Qwiic connectors on the back of the board. The overall board size is 1.75 in x 0.5 in. The mounting holes have also moved to toward the top of the board.

<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/SparkFun_Qwiic_OLED-0p9in_Board_Dimensions.png?raw=true"><img src="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/SparkFun_Qwiic_OLED-0p9in_Board_Dimensions.png?raw=true" width="600px" height="600px" alt="Board Dimensions"></a></td>
    </tr>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>Board Dimensions</i></td>
    </tr>
  </table>
</div>



## Hardware Hookup

In this section, we'll go over how to connect to the display. We will go just a bit further and talk about how to mount the display.



### Connecting via Qwiic Connector

Insert a Qwiic cable between your chosen microcontroller and Qwiic OLED. Then insert a USB cable between the microcontroller and your computer's COM port. For the scope of this tutorial, the USB cable provides power and allows us to upload code to the microcontroller. Of course, you can also debug the display by opening a Serial Terminal.

<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/LCD-24606-Qwiic-OLED-Display-Arduino_RedBoard_Plus.jpg?raw=true"><img src="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/LCD-24606-Qwiic-OLED-Display-Arduino_RedBoard_Plus.jpg?raw=true" width="600px" height="600px" alt="USB Cable, RedBoard Plus (ATMega328P), Qwiic Cable, Qwiic OLED (0.9 in., 12x32)"></a></td>
    </tr>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>USB Cable, RedBoard Plus (ATMega328P), Qwiic Cable, Qwiic OLED (0.9 in., 12x32)</i></td>
    </tr>
  </table>
</div>

Once you have finished prototyping, you could continue to use the USB cable and add a 5V power supply or battery pack.



### Mounting the Qwiic OLED (0.9", 128x32)

Grab the board dimensions and cut out a rectangle in the enclosure. For users that want to mount the board so that the OLED display is flush against the enclosure, you will need to look at the dimensions based on the OLED. You will need to add a little tolerance so that the display can fit through the rectangle. For users that need to quickly mount the board, you will could also cut out rectangles based on the vertical Qwiic connector so that the wires can lead into the enclosure. Then cut out the mounting holes so that the board is right side up. In this case, we used a cardboard box as a quick example to demonstrate the Qwiic wires connecting leading into the enclosure.

<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/LCD-24606-Qwiic-OLED-Display_Enclosure_Qwiic_Cable.jpg?raw=true"><img src="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/LCD-24606-Qwiic-OLED-Display_Enclosure_Qwiic_Cable.jpg?raw=true" width="600px" height="600px" alt="5V Battery Pack, USB Cable, RedBoard Plus (ATMega328P), Qwiic Cable, and Qwiic OLED (0.9 in., 12x32), stuffed in a cardboard box."></a></td>
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/LCD-24606-Qwiic-OLED-Display_Enclosure_Mounted.jpg?raw=true"><img src="https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/img/LCD-24606-Qwiic-OLED-Display_Enclosure_Mounted.jpg?raw=true" width="600px" height="600px" alt="5V Battery Pack, USB Cable, RedBoard Plus (ATMega328P), Qwiic Cable, and Qwiic OLED (0.9 in., 12x32) Mounted"></a></td>
    </tr>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;" colspan="2"><i>Qwiic OLED Display Mounted in an Enclosure</i></td>
    </tr>
  </table>
</div>

!!! note
    To easily display text and graphics on the board, we recommend mounting the board right side up. There is an option in the example code to flip the text horizontally and vertically should you decide to mount the board upside down, but you would need to also determine the position of the text.

For a more durable enclosure, you could use wood, metal, or plastic. However, you will need additional tools to cut into the material.



## Software

The Qwiic OLED (0.91", 128x32) uses the SparkFun QWIIC OLED Arduino Library. The SparkFun Qwiic OLED Library's [Software Setup](../software/) has instructions and usage examples. Additionally, the full library API documentation is available in the SparkFun Qwiic OLED Library [API Reference](../api_device/) guide.

<div style="text-align: center">
    <a href="../api_device" class="md-button">SparkFun Qwiic OLED Library API Reference Guide</a>
</div>



## Resources

Now that you've successfully got your OLED Display (0.9", 128x36) up and running, it's time to incorporate it into your own project! For more information, check out the resources below:

* [Schematic (PDF)](https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/board_files/SparkFun_Qwiic_OLED-0p91_Schematic_v11.pdf)
* [Eagle Files (ZIP)](https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/assets/board_files/SparkFun_Qwiic_OLED-0p9in_v11.zip?raw=true)
* [Board Dimensions (PNG)](https://raw.githubusercontent.com/sparkfun/SparkFun_Qwiic_OLED-0.91/main/docs/assets/img/SparkFun_Qwiic_OLED-0p9in_Board_Dimensions.png)
* [Datasheet (PDF)](https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/blob/main/docs/component_documentation/SPEC%20AMO091-12832BFG02-H14%20VER%20A.pdf._看图王.pdf) (0.91", 128x32, SSD1306)
* [ReadtheDocs: Qwiic_OLED_Display_Py](https://qwiic-oled-display-py.readthedocs.io/en/latest/)
* [Qwiic OLED Display Python Package Repo](https://github.com/sparkfun/Qwiic_OLED_Display_Py)
* [Github Hardware Repo](https://github.com/sparkfun/SparkFun_Qwiic_OLED-0.91/)
