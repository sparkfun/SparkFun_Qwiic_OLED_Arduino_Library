# Example 2 - Shapes

An example that shows drawing simple shapes using the  SparkFun Qwiic OLED Library.

**Key Demo Features**

* Drawing lines, rectangles and circles
* Demonstrating how graphics size impacts display speed
* Drawing and erasing graphics quickly
* XOR operations using raster operators

## Setup

After installing this library in your local Arduino environment, begin with a standard Arduino sketch, and include the header file for this library.
```C++
// Include the SparkFun qwiic OLED Library
#include <SparkFun_Qwiic_OLED.h>
```
The next step is to declare the object for the SparkFun qwiic OLED device used. Like most Arduino sketches, this is done at a global scope (after the include file declaration), not within the ```setup()``` or ```loop()``` functions. 

The user selects from one of the following classes:

| Class | Qwiic OLED Device |
| :--- | :--- |
| `QwiicMicroOLED` | [SparkFun Qwiic Micro OLED ]( https://www.sparkfun.com/products/14532)| 
| `QwiicNarrowOLED` | [SparkFun Qwiic OLED Display (128x32) ]( https://www.sparkfun.com/products/17153)| 
| `QwiicTransparentOLED` | [SparkFun Transparent Graphical OLED]( https://www.sparkfun.com/products/15173)| 

The Example code supports all of the SparkFun Qwiic OLED boards. To select the board being used, uncomment the `#define` for the demo board. 

For this example, the Qwiic Micro OLED is used.
```C++
#define MICRO
//#define NARROW
//#define TRANSPARENT
``` 
Which results in myOLED being declared as:

```C++
QwiicMicroOLED myOLED;
```

## Drawing Shapes

The shapes drawn are broken into a set of functions that perform one test, which is part of the overall example.

###Lines

This test starts with a short, horizontal line that is animated from the top to bottom of the display. After each iteration, the line size is increased and the animating sequence repeated.

To animate the line, the display is erased, then the line drawn. Once the line is draw, the updated graphics is sent to the OLED device by calling the `display()` method. 

!!! note
    When `display()` is called, only the range of modified pixels is sent to the OLED device, greatly reducing the data transferred for small graphic changes. 

    This is demonstrated by this test. When small lines are drawn, the update rate is fast, but as the line length increases, the update rate of the device is noticeably slower. A longer line requires more data to be sent to the device.

```C++
void line_test_1(void){

    int x, y, i;

    int mid = width/2;
    int delta = mid/8;
    
    for(int j =1; j < 8; j++ ){

        x = delta*j;

        for(i=0; i < height*2; i++){

            y = i % height;
            myOLED.erase();
            myOLED.line(mid-x, y, mid+x, y);
            myOLED.display();
        }
    }
}
```
This test is followed up with a series of lines that span from a single point to the bottom of the screen, showing the flexibility of the line to raster algorithm used by the library.

```C++
void line_test_2(void){

    for(int i=0; i < width; i +=6){
        myOLED.line(0, 0, i, height-1);
        myOLED.display();
    }
    delay(200);
    myOLED.erase();
    for(int i=width-1; i >= 0; i -=6){
        myOLED.line(width-1, 0, i, height-1);
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
// Iterator function - called to animate graphic
void line_test_vert_iter(uint8_t y0, uint8_t y1){

    for(int i=0; i < width; i += 8)
        myOLED.line( i, y0, i, y1);

    // end off the vertical lines
    myOLED.line( width-1, y0, width-1, y1);        

    // End lines and cross lines
    myOLED.line(0, y0, width-1, y0);
    myOLED.line(0, y1, width-1, y1);
    myOLED.line(0, y0, width-1, y1);
    myOLED.line(0, y1, width-1, y0);    
}

// Entry point for test
void line_test_vert(void){

    int mid = height/2;

    for(int i=0; i < height; i+=4){

        myOLED.erase();
        line_test_vert_iter( mid - i/2, mid + i/2 );
        myOLED.display();
        delay(10);
    }
}
```

### Rectangles

Several rectangle routines are shown in this example. A key test is a fast drawing routine which animates a small rectangle being drawn diagonally across the screen. 

In this test, the rectangle is drawn, sent to the device via using `display()`, then the rectangle is drawn again, but this time in black. This effectively erases the rectangle. The position is incremented and the process loops, causing the rectangle to appear to fly across the screen.

The animation is quick, since only the portions of the screen that need updated are actually updated.

The animation algorithm:

```C++
  for(int i = 0; i < steps; i++){

        // Draw the rectangle and send it to device
        myOLED.rectangle(x, y, side, side);
        myOLED.display(); // sends erased rect and new rect pixels to device

        // Erase the that rect, increment and loop
        myOLED.rectangle(x, y, side, side, 0);

        x += xinc;
        y += yinc;       
    }
```

The next rectangle test draws a series of filled rectangles on the screen. The unique aspect of this test is that is uses the XOR functionally to overlay a rectangle on the device, presenting a alternating color pattern.

The XOR raster operation is set by calling the `setDrawMode()` method on the OLED device, and providing the `grROPXOR` code. This switch the device into a XOR drawing mode. Graphic operations are restored to normal by calling `setDrawMode()` and providing the `grROPCopy` code, which copies the new pixel value to the destination. 

Filled rectangles and XOR operations:

```C++
void rect_fill_test(void){

    myOLED.rectangleFill(4, 4, width/2-8, height-8);

    myOLED.rectangleFill(width/2+4, 4, width/2-8, height-8);

    myOLED.setDrawMode(grROPXOR);   // xor
    myOLED.rectangleFill(width/4, 8, width/2, height-16);
    myOLED.setDrawMode(grROPCopy);  // back to copy op (default)
}
```

### Circles

The final shape drawn by this example is a series of circles and filled circles. Using the geometry of the screen, a set of circles are drawn and displayed. 

```C++
void circle_test(void){

    // Lets draw some circles that fit on the device
    myOLED.circle(width/4, height/2, height/3);

    myOLED.circleFill(width - width/4, height/2, height/3);    

    myOLED.circle(4, height/2, height/3);

    myOLED.circleFill(width - width/2 , height/2, height/4);    

}

```