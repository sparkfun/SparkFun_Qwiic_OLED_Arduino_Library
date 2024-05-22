# Example 2 - Shapes

An example that shows drawing simple shapes using the SparkFun Qwiic OLED Library.

**Key Demo Features**

* Drawing lines, rectangles and circles
* Demonstrating how graphics size impacts display speed
* Drawing and erasing graphics quickly
* XOR operations using raster operators



## Setup

After installing this library in your local Arduino environment, begin with a standard Arduino sketch, and include the header file for this library.


```C++
// Include the SparkFun Qwiic OLED Library
#include <SparkFun_Qwiic_OLED.h>
```


The next step is to declare the object for the SparkFun Qwiic OLED device used. Like most Arduino sketches, this is done at a global scope (after the include file declaration), not within the ```setup()``` or ```loop()``` functions.

The user selects from one of the following classes:

<div style="text-align: center;">
    <table>
        <tr>
            <th style="text-align: center; border: solid 1px #cccccc;">Class
            </th>
            <th style="text-align: center; border: solid 1px #cccccc;">Qwiic OLED Device
            </th>
        </tr>
        <tr>        
            <td style="text-align: center; border: solid 1px #cccccc;"><code>QwiicMicroOLED</code>
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;"><a href="https://www.sparkfun.com/products/14532">SparkFun Qwiic Micro OLED</a>
            </td>
        </tr>
        <tr>        
            <td style="text-align: center; border: solid 1px #cccccc;"><code>QwiicTransparentOLED</code>
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;"><a href="https://www.sparkfun.com/products/15173">SparkFun Transparent Graphical OLED</a>
            </td>
        </tr>
        <tr>        
            <td style="text-align: center; border: solid 1px #cccccc;"><code>QwiicNarrowOLED</code>
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;"><a href="https://www.sparkfun.com/products/17153">SparkFun Qwiic OLED Display (128x32)</a>
            </td>
        </tr>
        <tr>        
            <td style="text-align: center; border: solid 1px #cccccc;"><code>Qwiic1in3OLED</code>
            </td>
            <td style="text-align: center; border: solid 1px #cccccc;"><a href="https://www.sparkfun.com/products/23453">SparkFun Qwiic OLED 1.3" Display (128x32)</a>
            </td>
        </tr>
    </table>
</div>



The example code supports all of the SparkFun Qwiic OLED boards. By default, the Qwiic Micro OLED is selected. To select a different board being used, add a single line comment (i.e. `//`) in front of "`QwiicMicroOLED myOLED;`" and uncomment the device being used for the demo board.


```C++
QwiicMicroOLED myOLED;
//QwiicTransparentOLED myOLED;
//QwiicNarrowOLED myOLED;
//Qwiic1in3OLED myOLED;

```


!!! note
    As of version 1.0.2+, users will need to use the class as explained above instead of using a `#define`.

    ```C++
    #define MICRO
    //#define NARROW
    //#define TRANSPARENT
    ```



## Drawing Shapes

!!! note
    As of version 1.0.2+, the modular functions have a slightly different name. Some functions defined in the example code will have the `_` removed or words spelled out. For example, version v1.0.1 and below defined the function to test the line as `line_test_1()` while version v1.0.2+ defined the function as `lineTest1()`.

The shapes drawn are broken into a set of functions that perform one test, which is part of the overall example.



###Lines

This test starts with a short, horizontal line that is animated from the top to bottom of the display. After each iteration, the line size is increased and the animating sequence repeated.

To animate the line, the display is erased, then the line drawn. Once the line is draw, the updated graphics is sent to the OLED device by calling the `display()` method.

!!! note
    When `display()` is called, only the range of modified pixels is sent to the OLED device, greatly reducing the data transferred for small graphic changes.

    This is demonstrated by this test. When small lines are drawn, the update rate is fast, but as the line length increases, the update rate of the device is noticeably slower. A longer line requires more data to be sent to the device.

```C++
void lineTest1(void)
{
    int x, y, i;

    int mid = width / 2;
    int delta = mid / 8;

    for (int j = 1; j < 8; j++)
    {

        x = delta * j;

        for (i = 0; i < height * 2; i++)
        {

            y = i % height;
            myOLED.erase();
            myOLED.line(mid - x, y, mid + x, y);
            myOLED.display();
        }
    }
}
```



This test is followed up with a series of lines that span from a single point to the bottom of the screen, showing the flexibility of the line to raster algorithm used by the library.

```C++
void lineTest2(void)
{
    for (int i = 0; i < width; i += 6)
    {
        myOLED.line(0, 0, i, height - 1);
        myOLED.display();
    }
    delay(200);
    myOLED.erase();
    for (int i = width - 1; i >= 0; i -= 6)
    {
        myOLED.line(width - 1, 0, i, height - 1);
        myOLED.display();
    }
}
```



And the last line test draws a series of lines to test all three internal line drawing algorithms. Specifically:

* Angled lines drawn by the general purpose line algorithm
* Vertical lines drawn by an optimized line routine
* Horizontal lines draw by an optimized line routine

The test animates to show a growing box, giving an idea of the speed and flexibility of the system.

```C++
void lineTestVerticalIterative(uint8_t y0, uint8_t y1)
{
    for (int i = 0; i < width; i += 8)
        myOLED.line(i, y0, i, y1);

    // end off the vertical lines
    myOLED.line(width - 1, y0, width - 1, y1);

    // End lines and cross lines
    myOLED.line(0, y0, width - 1, y0);
    myOLED.line(0, y1, width - 1, y1);
    myOLED.line(0, y0, width - 1, y1);
    myOLED.line(0, y1, width - 1, y0);
}

// Entry point for test
void lineTestVertical(void)
{
    int mid = height / 2;

    for (int i = 0; i < height; i += 4)
    {

        myOLED.erase();
        lineTestVerticalIterative(mid - i / 2, mid + i / 2);
        myOLED.display();
        delay(10);
    }
}
```



### Rectangles

Several rectangle routines are shown in this example. A key test is a fast drawing routine which animates a small rectangle being drawn diagonally across the screen.

In this test, the rectangle is drawn, sent to the device via using `display()`, then the rectangle is drawn again, but this time in black. This effectively erases the rectangle. The position is incremented and the process loops, causing the rectangle to appear to fly across the screen.

The animation is quick, since only the portions of the screen that need updated are actually updated.

The animation algorithm is listed in the `rectangleTestMove() function.

```C++
void rectangleTestMove(void)
{
    float steps = height;
    float xinc = width / steps;
    float yinc = height / steps;
    int side = 10;
    float x = 0;
    float y = 0;

    for (int i = 0; i < steps; i++)
    {
        // Draw the rectangle and send it to device
        myOLED.rectangle(x, y, side, side);
        myOLED.display(); // sends erased rect and new rect pixels to device

        // Erase the that rect, increment and loop
        myOLED.rectangle(x, y, side, side, 0);

        x += xinc;
        y += yinc;
    }
}
```



The next rectangle test draws a series of filled rectangles on the screen. The unique aspect of this test is that is uses the XOR functionally to overlay a rectangle on the device, presenting a alternating color pattern.

The XOR raster operation is set by calling the `setDrawMode()` method on the OLED device, and providing the `grROPXOR` code. This switch the device into a XOR drawing mode. Graphic operations are restored to normal by calling `setDrawMode()` and providing the `grROPCopy` code, which copies the new pixel value to the destination.

Filled rectangles and XOR operations:

```C++
void rectangleFillTest(void)
{
    myOLED.rectangleFill(4, 4, width / 2 - 8, height - 8);

    myOLED.rectangleFill(width / 2 + 4, 4, width / 2 - 8, height - 8);

    myOLED.setDrawMode(grROPXOR); // xor
    myOLED.rectangleFill(width / 4, 8, width / 2, height - 16);
    myOLED.setDrawMode(grROPCopy); // back to copy op (default)
}
```



### Circles

The final shape drawn by this example is a series of circles and filled circles. Using the geometry of the screen, a set of circles are drawn and displayed.

```C++
void circleTest(void)
{
    // Let's draw some circles that fit on the device
    myOLED.circle(width / 4, height / 2, height / 3);

    myOLED.circleFill(width - width / 4, height / 2, height / 3);

    myOLED.circle(4, height / 2, height / 3);

    myOLED.circleFill(width - width / 2, height / 2, height / 4);
}
```
