## Introduction

The [Qwiic Micro OLED](https://www.sparkfun.com/products/22495) is a Qwiic enabled version of our micro OLED display! This small monochrome, blue-on-black OLED display displays incredibly clear images.

<center>
<div class="grid cards" style="width:500px;" markdown>

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
      <br />
      <center>[Purchase from SparkFun :fontawesome-solid-cart-plus:](https://www.sparkfun.com/products/22495){ .md-button .md-button--primary }</center>
    </a>

</div>
</center>

<div style="text-align: center">
    <iframe width="560" height="315" src="https://www.youtube.com/embed/OBOgxnctzwI?si=2Q6-raKh9iPL8xgN" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>
</div>

This hookup guide will show you how to get started drawing objects and characters on your OLED.



### Required Materials

To follow along with this tutorial, you will need the following materials. You may not need everything though depending on what you have. Add it to your cart, read through the guide, and adjust the cart as necessary.

* 1x [SparkFun RedBoard Plus [DEV-18158]](https://www.sparkfun.com/products/18158)
* 1x [Reversible USB A to C Cable - 0.8m [CAB-15425]](https://www.sparkfun.com/products/15425)
* 1x Qwiic Cable
    * [Flexible Qwiic Cable - 50mm [PRT-17260]](https://www.sparkfun.com/products/17260), for short distances
    * [Flexible Qwiic Cable - 500mm [PRT-17257]](https://www.sparkfun.com/products/17257), for those that need to wire the board farther away from your microcontroller
* 1x [SparkFun Micro OLED Breakout (Qwiic) [LCD-22495]](https://www.sparkfun.com/products/22495)



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
</div>
</center>


### Suggested Reading

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



### Hardware Overview

Listed below are some of the operating ranges and characteristics of the Qwiic Micro OLED.

<div style="text-align: center;">
    <table>
        <tr>
            <th style="text-align: center; border: solid 1px #cccccc;">Characteristic
            </th>
            <th style="text-align: center; border: solid 1px #cccccc;">Range
            </th>
        </tr>
        <tr style="vertical-align:middle;">
            <td style="text-align: center; border: solid 1px #cccccc;" bgcolor="#ffffff"><font color="#000000">Voltage</font>
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;" bgcolor="#ffffff"><font color="#000000"><b>3.3V</b></font>
            </td>
        </tr>
        <tr style="vertical-align:middle;">        
            <td style="text-align: center; border: solid 1px #cccccc;" bgcolor="#ffffff"><font color="#000000">Temperature</font>
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;" bgcolor="#ffffff"><font color="#000000">-40&deg;C to 85&deg;C</font>
            </td>
        </tr>
        <tr style="vertical-align:middle;">        
            <td style="text-align: center; border: solid 1px #cccccc;" bgcolor="#ffffff"><font color="#000000">I<sup>2</sup>C Address</font>
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;" bgcolor="#ffffff"><font color="#000000">0X3D (Default) or 0X3C (Closed Jumper)</font>
            </td>
        </tr>
    </table>
</div>


###Pins

Power and I<sup>2</sup>C pins are broken out to the 1x4 PTH pins as well as the two horizontal Qwiic connectors.

<div style="text-align: center;">
    <table>
        <tr>
            <th style="text-align: center; border: solid 1px #cccccc;">Pin
            </th>
            <th style="text-align: center; border: solid 1px #cccccc;">Description
            </th>
            <th style="text-align: center; border: solid 1px #cccccc;">Direction
            </th>
        </tr>
        <tr style="vertical-align:middle;">
            <td style="text-align: center; border: solid 1px #cccccc;" bgcolor="#DDDDDD"><font color="#000000">GND</font>
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;" bgcolor="#DDDDDD"><font color="#000000">Ground</font>
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;" bgcolor="#DDDDDD"><font color="#000000">In</font>
            </td>
        </tr>
        <tr style="vertical-align:middle;">        
            <td style="text-align: center; border: solid 1px #cccccc;" bgcolor="#f2dede"><font color="#000000">3.3V</font>
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;" bgcolor="#f2dede"><font color="#000000">Power</font>
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;" bgcolor="#f2dede"><font color="#000000">In</font>
            </td>
        </tr>
        <tr style="vertical-align:middle;">        
            <td style="text-align: center; border: solid 1px #cccccc;" bgcolor="#cce5ff"><font color="#000000">SDA</font>
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;" bgcolor="#cce5ff"><font color="#000000">I<sup>2</sup>C Data</font>
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;" bgcolor="#cce5ff"><font color="#000000">In</font>
            </td>
        </tr>
        <tr style="vertical-align:middle;">        
            <td style="text-align: center; border: solid 1px #cccccc;" bgcolor="#fff3cd"><font color="#000000">SCL</font>
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;" bgcolor="#fff3cd"><font color="#000000">I<sup>2</sup>C Clock</font>
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;" bgcolor="#fff3cd"><font color="#000000">In</font>
            </td>
        </tr>
    </table>
</div>



### Jumpers

There are several jumpers on board that can be changed to facilitate several different functions. The first of which is the I<sup>2</sup>C pull-up jumper to disable the 2.2k&ohm; pull up resistors on the I<sup>2</sup>C data and clock lines, highlighted below. If multiple boards are connected to the I<sup>2</sup>C bus, the equivalent resistance goes down, increasing your pull up strength. If multiple boards are connected on the same bus, make sure [only one board has the pull-up resistors connected](https://learn.sparkfun.com/tutorials/i2c/i2c-at-the-hardware-level).

<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://cdn.sparkfun.com/assets/learn_tutorials/7/1/8/pu.PNG"><img src="https://cdn.sparkfun.com/r/600-600/assets/learn_tutorials/7/1/8/pu.PNG" width="600px" height="600px" alt="I2C Pull-Up Jumper"></a></td>
    </tr>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>I2C Pull-Up Jumper</i></td>
    </tr>
  </table>
</div>


The ADDR jumper (highlighted below) can be used to change the I<sup>2</sup>C address of the board. The default jumper is open by default, pulling the address pin high and giving us an I<sup>2</sup>C address of **0X3D**. Closing this jumper will ground the address pin, giving us an I<sup>2</sup>C address of 0X3C.

<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://cdn.sparkfun.com/assets/learn_tutorials/7/1/8/addr.PNG"><img src="https://cdn.sparkfun.com/r/600-600/assets/learn_tutorials/7/1/8/addr.PNG" width="600px" height="600px" alt="Address Jumper"></a></td>
    </tr>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>Address Jumper</i></td>
    </tr>
  </table>
</div>



### Board Dimensions

!!! note "Revision Change"
    For V11, we have optimized the two mounting hole locations to match the mounting holes located on a standard 1.0" x 1.0" Qwiic-sized board. The overall functionality of the Qwiic Micro OLED breakout board is the same as the previous version!

The overall board size is 1.08" x 1.17". There is a v-score for users that want to remove the mounting holes.

<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://cdn.sparkfun.com/assets/c/4/a/9/c/Qwiic_OLED_Breakout_Board_Dimensions_v11.png"><img src="https://cdn.sparkfun.com/assets/c/4/a/9/c/Qwiic_OLED_Breakout_Board_Dimensions_v11.png" width="600px" height="600px" alt="Board Dimensions"></a></td>
    </tr>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><i>Board Dimensions</i></td>
    </tr>
  </table>
</div>





## Hardware Assembly

If you haven't yet [assembled your Qwiic Shield](https://learn.sparkfun.com/tutorials/qwiic-shield-for-arduino--photon-hookup-guide), now would be the time to head on over to that tutorial. With the shield assembled, SparkFun's Qwiic environment means that connecting the screen could not be easier. Just plug one end of the Qwiic cable into the OLED display, the other into the Qwiic Shield and you'll be ready to start displaying images on your little display.

<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://cdn.sparkfun.com/assets/learn_tutorials/7/1/8/Qwiic_OLED-03.jpg"><img src="https://cdn.sparkfun.com/r/600-600/assets/learn_tutorials/7/1/8/Qwiic_OLED-03.jpg" width="600px" height="600px" alt="Qwiic Micro OLED Connected to Arduino and Qwiic Shield"></a></td>
    </tr>
  </table>
</div>


If you have a built-in Qwiic connector, you can skip the hardware assembly of the Qwiic Shield and simply insert a Qwiic cable between the two boards. Have more than one Qwiic-enabled device? You can daisy chain it to the board as well! Below is an example with the SAM-M8Q and the Qwiic Micro OLED daisy chained together to the RedBoard Qwiic.

<div style="text-align: center;">
  <table>
    <tr style="vertical-align:middle;">
     <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://cdn.sparkfun.com/assets/learn_tutorials/1/2/6/3/Qwiic_GPS_Clock_microOLED_Arduino_RedBoard.jpg"><img src="https://cdn.sparkfun.com/r/600-600/assets/learn_tutorials/1/2/6/3/Qwiic_GPS_Clock_microOLED_Arduino_RedBoard.jpg" width="600px" height="600px" alt="Qwiic Micro OLED Connected to RedBoard Qwiic and Qwiic GPS(SAM-M8Q)"></a></td>
    </tr>
  </table>
</div>

!!! note
    The initial launch of the Qwiic micro OLED breakout board had the OLED loosely attached to the breakout board. For users that received those boards, be careful handling it! You can either use your own enclosure for the OLED display, or you can use some double sided foam tape for a less permanent solution.

    <div style="text-align: center;">
      <table>
        <tr style="vertical-align:middle;">
         <td style="text-align: center; border: solid 1px #cccccc; vertical-align: middle;"><a href="https://cdn.sparkfun.com/assets/learn_tutorials/7/1/8/Qwiic_OLED-01.jpg"><img src="https://cdn.sparkfun.com/r/600-600/assets/learn_tutorials/7/1/8/Qwiic_OLED-01.jpg" width="600px" height="600px" alt="Taped Screen"></a></td>
        </tr>
      </table>
    </div>

    The current production of the boards includes the double sided foam tape.



## Software

The SparkFun Micro OLED Breakout (Qwiic) uses the SparkFun QWIIC OLED Arduino Library. The [SparkFun Qwiic OLED library Getting Started guide](software.md) has library setup instructions and usage examples. Additionally, the full library API documentation is available in the [SparkFun Qwiic OLED Library API Reference guide](api_device.md).

<div style="text-align: center">
    <a href="../api_device" class="md-button">SparkFun Qwiic OLED Library API Reference Guide</a>
</div>



## Resources

Now that you've successfully got your OLED displaying things, it's time to incorporate it into your own project!

For more on the Qwiic Micro OLED, check out the links below:

* [Schematic (PDF)](https://cdn.sparkfun.com/assets/d/0/e/4/1/Qwiic_OLED_Breakout.pdf)
* [Eagle Files (ZIP)](https://cdn.sparkfun.com/assets/c/b/c/f/d/Qwiic_OLED_Breakout_1.zip)
* [Datasheet (PDF)](https://cdn.sparkfun.com/assets/learn_tutorials/3/0/8/SSD1306.pdf)
* [Bitmap Generator](http://en.radzio.dxp.pl/bitmap_converter/)
* [Qwiic System Landing Page](https://www.sparkfun.com/qwiic)
* [Qwiic Micro OLED Python Package](https://github.com/sparkfun/Qwiic_Micro_OLED_Py)
* [GitHub Hardware Repo](https://github.com/sparkfun/Qwiic_Micro_OLED) -- Board design files for the Qwiic Micro OLED.
* [Product Showcase: Qwiic Presence Sensor & OLED](https://www.youtube.com/watch?v=OBOgxnctzwI)
