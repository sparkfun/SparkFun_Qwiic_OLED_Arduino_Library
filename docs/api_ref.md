# Library API

The device objects in the SparkFun qwiic OLED Library all implement the following methods.

### begin()
This method is called to initialize the OLED library and connection to the OLED device. This method must be called before calling any graphics methods. 

```c++
bool begin(TwoWire &wirePort, uint8_t address)
```

| Parameter | Type | Description |
| :------------ | :---------- | :---------------------------------------------- |
| `wirePort` | `TwoWire` | **optional**. The Wire port. If not provided, the default port is used|
| `address` | `uint8_t` | **optional**. I2C Address. If not provided, the default address is used.|
| return value | `bool` | ```true``` on success, ```false``` on startup failure |

## Device

### getWidth()
This method returns the width, in pixels, of the connected OLED device

```c++
uint8_t getWidth(void)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| return value | `uint8_t` | The width in pixels of the connected OLED device |

### getHeight()
This method returns the height, in pixels, of the connected OLED device

```c++
uint8_t getHeight(void)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| return value | `uint8_t` | The height in pixels of the connected OLED device |

### invert()
This method inverts the current graphics on the display. This results of this command happen immediatly.

```c++
void invert(bool bInvert)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| ```bInvert``` | `bool` | ```true``` - the screen is inverted. ```false``` - the screen is set to normal |

### flipVertical()
When called, the screen contents are flipped vertically if the flip parameter is true, or restored to normal display if the flip parameter is false. 

```c++
void flipVertical(bool bFlip)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| ```bFlip``` | `bool` | ```true``` - the screen is flipped vertically. ```false``` - the screen is set to normal |

### flipHorizontal()
When called, the screen contents are flipped horizontally if the flip parameter is true, or restored to normal display if the flip parameter is false. 

```c++
void flipHorizontal(bool bFlip)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| ```bFlip``` | `bool` | ```true``` - the screen is flipped horzontally. ```false``` - the screen is set to normal |

## Scrolling

### scrollStop()
If the device is in a scrolling mode, calling this method stops the scroll, and restores the device to normal display operation. This action is performed immediately.

```c++
void scrollStop(void)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| NONE|  |  |



### scrollRight()
This method is called to start the device scrolling the displayed graphics to the right. This action is performed immediately.

The screen will scroll until the ```scrollStop()``` method is called.

```c++
void scrollRight(uint8_t start, uint8_t stop, uint8_t interval)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| `start` | `uint8_t` | The start page address of the scroll - valid values are 0 thru 7|
| `stop` | `uint8_t` | The stop/end page address of the scroll - valid values are 0 thru 7|
| `interval` | `uint8_t` | The time interval between scroll step - values listed below |

Defined values for the `interval` parameter:

| Defined Symbol | Time Interval Between Steps |
| :--- | :---|
|`SCROLL_INTERVAL_2_FRAMES`   |	2 |
|`SCROLL_INTERVAL_3_FRAMES`   | 3 |
|`SCROLL_INTERVAL_4_FRAMES`   | 4 |
|`SCROLL_INTERVAL_5_FRAMES`   | 5  |
|`SCROLL_INTERVAL_25_FRAMES`  | 25 |
|`SCROLL_INTERVAL_64_FRAMES`  | 64 |
|`SCROLL_INTERVAL_128_FRAMES` | 128 |
|`SCROLL_INTERVAL_256_FRAMES` | 256 |

### scrollVertRight()
This method is called to start the device scrolling the displayed graphics verticall and to the right. This action is performed immediately.

The screen will scroll until the ```scrollStop()``` method is called.

```c++
void scrolVertlRight(uint8_t start, uint8_t stop, uint8_t interval)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| `start` | `uint8_t` | The start page address of the scroll - valid values are 0 thru 7|
| `stop` | `uint8_t` | The stop/end page address of the scroll - valid values are 0 thru 7|
| `interval` | `uint8_t` | The time interval between scroll step - values listed in ```scrollRight``` |

### scrollLeft()
This method is called start to the device scrolling the displayed graphics to the left. This action is performed immediately.

The screen will scroll until the ```scrollStop()``` method is called.

```c++
void scrollLeft(uint8_t start, uint8_t stop, uint8_t interval)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| `start` | `uint8_t` | The start page address of the scroll - valid values are 0 thru 7|
| `stop` | `uint8_t` | The stop/end page address of the scroll - valid values are 0 thru 7|
| `interval` | `uint8_t` | The time interval between scroll step - values listed in ```scrollRight``` |

### scrollVertLeft()
This method is called to start the device scrolling the displayed graphics verticall and to the left. This action is performed immediately.

The screen will scroll until the ```scrollStop()``` method is called.

```c++
void scrolVertlLeft(uint8_t start, uint8_t stop, uint8_t interval)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| `start` | `uint8_t` | The start page address of the scroll - valid values are 0 thru 7|
| `stop` | `uint8_t` | The stop/end page address of the scroll - valid values are 0 thru 7|
| `interval` | `uint8_t` | The time interval between scroll step - values listed in ```scrollRight``` |

## Drawing State

### setFont()

This method is called to set the current font in the library. The current font is used when calling the ```text()``` method on this device. 

The default font for the device is `5x7`.

```c++
void setFont(QwiicFont& theFont)
void setFont(const QwiicFont * theFont)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| `theFont` | `QwiicFont` | The font to set as current in the device|
| `theFont` | `QwiicFont*` | Pointer to the font to set as current in the device.|

For the library, fonts are added to your program by including them via include files which are part of this library. 

The following fonts are included:

| Font | Include File | Font Variable | Description|
| :--- | :--- | :--- | :--- |
| 5x7 | `<res/qw_fnt_5x7.h>` | `QW_FONT_5X7`| A full, 5 x 7 font|
| 31x48 | `<res/qw_fnt_31x48.h>` |`QW_FONT_31X48`| A full, 31 x 48 font|
| Seven Segment | `<res/qw_fnt_7segment.h>` |`QW_FONT_7SEGMENT`| Numbers only|
| 8x16 | `<res/qw_fnt_8x16.h>` | `QW_FONT_8X16`| A full, 8 x 16 font|
| Large Numbers | `<res/qw_fnt_largenum.h>` |`QW_FONT_LARGENUM`| Numbers only|

For each font, the font variables are objects with the following attributes:

| Attribute | Value |
| :--- | :--- | 
| `width` | The font width in pixels|
| `height` | The font height in pixels|
| `start` | The font start character offset|
| `n_chars` | The number of characters|
| `map_width` | The width of the font map|

Example use of a font object attribute:
```C++
#include <res/qw_fnt_31x48.h>
   
int myFontWidth = QW_FONT_31X48.width;
```

### getFont()
This method returns the current font for the device.

```c++
QwiicFont * getFont(void)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| return value | `QwiicFont*` | A pointer to the current font. See `setFont()` for font object details.|

### setDrawMode()
This method sets the current draw mode for the library. The draw mode determines how pixels are set on the screen during drawing operations. 

```c++
void setDrawMode(grRasterOp_t rop)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| rop | `grRasterOp_t` | The raster operation (ROP) to set the graphics system to.|

Raster operations device how source (pixels to draw) are represented on the destination device. The available Raster Operation (ROP) codes are:

| ROP Code | Description|
| :--- | :--- |
| grROPCopy | **default** Drawn pixel values are copied to the device screen|
| grROPNotCopy | A not operation is applied to the source value before copying to screen|
| grROPNot | A not operation is applied to the destination (screen) value |
| grROPXOR | A XOR operation is performed between the source and destination values|
| grROPBlack | A value of 0, or black is drawn to the destination |
| grROPWhite | A value of 1, or black is drawn to the destination |


### getDrawMode()
This method returns the current draw mode for the library. The draw mode determines how pixels are set on the screen during drawing operations. 

```c++
grRasterOp_t getDrawMode(void)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| return value | `grRasterOp_t` | The current aster operation (ROP) of the graphics system.|

## Drawing Methods

### display()
When called, any pending display updates are sent to the connected OLED device. This includes drawn graphics and erase commands.

```c++
void display(void)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| NONE|  |  |

### erase()
Erases all graphics on the device, placing the display in a blank state. The erase update isn't sent to the device until the next ```display()``` call on the device.

```c++
void erase(void)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| NONE|  |  |

### pixel()

Set the value of a pixel on the screen.

```c++
void pixel(uint8_t x, uint8_t y, uint8_t clr)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| x | `uint8_t` | The X coordinate of the pixel to set|
| y | `uint8_t` | The Y coordinate of the pixel to set|
| clr | `uint8_t` | **optional** The color value to set the pixel. This defaults to white (1).|

### line()

Draw a line on the screen. 

Note: If a line is horizontal (y0 = y1) or vertical (x0 = x1), optimized draw algorithms are used by the library.

```c++
void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| x0 | `uint8_t` | The start X coordinate of the line|
| y0 | `uint8_t` | The start Y coordinate of the line|
| x1 | `uint8_t` | The end X coordinate of the line|
| y1 | `uint8_t` | The end Y coordinate of the line|
| clr | `uint8_t` | **optional** The color value to draw the line. This defaults to white (1).|

### rectangle()

Draw a rectangle on the screen. 

```c++
void rectangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| x0 | `uint8_t` | The start X coordinate of the rectangle - upper left corner|
| y0 | `uint8_t` | The start Y coordinate of the rectangle - upper left corner|
| x1 | `uint8_t` | The end X coordinate of the rectangle - lower right corner|
| y1 | `uint8_t` | The end Y coordinate of the rectangle - lower right corner|
| clr | `uint8_t` | **optional** The color value to draw the line. This defaults to white (1).|

### rectangleFill()

Draw a filled rectangle on the screen. 

```c++
void rectangleFill(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| x0 | `uint8_t` | The start X coordinate of the rectangle - upper left corner|
| y0 | `uint8_t` | The start Y coordinate of the rectangle - upper left corner|
| x1 | `uint8_t` | The end X coordinate of the rectangle - lower right corner|
| y1 | `uint8_t` | The end Y coordinate of the rectangle - lower right corner|
| clr | `uint8_t` | **optional** The color value to draw the line. This defaults to white (1).|

### circle()

Draw a circle on the screen. 

```c++
void circle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t clr)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| x0 | `uint8_t` | The X coordinate of the circle center|
| y0 | `uint8_t` | The Y coordinate of the circle center|
| radius | `uint8_t` | The radius of the circle|
| clr | `uint8_t` | **optional** The color value to draw the circle. This defaults to white (1).|

### circleFill()

Draw a filled circle on the screen. 

```c++
void circleFill(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t clr)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| x0 | `uint8_t` | The X coordinate of the circle center|
| y0 | `uint8_t` | The Y coordinate of the circle center|
| radius | `uint8_t` | The radius of the circle|
| clr | `uint8_t` | **optional** The color value to draw the circle. This defaults to white (1).|

### bitmap()

Draws a bitmap on the screen.

The bitmap should be 8 bit encoded - each pixel contains 8 y values.

```c++
void bitmap(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, 
				uint8_t *pBitmap, uint8_t bmp_width, uint8_t bmp_height )
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| x0 | `uint8_t` | The X coordinate to place the bitmap - upper left corner|
| y0 | `uint8_t` | The Y coordinate to place the bitmap - upper left corner|
| x1 | `uint8_t` | The end X coordinate of the bitmap - lower right corner|
| y1 | `uint8_t` | The end Y coordinate of the bitmap - lower right corner|
| pBitmap | `uint8_t *` | A pointer to the bitmap array|
| bmp_width | `uint8_t` | The width of the bitmap|
| bmp_height | `uint8_t` | The height of the bitmap|

### bitmap()

Draws a bitmap on the screen using a Bitmap object for the bitmap data.

```c++
void bitmap(uint8_t x0, uint8_t y0, QwiicBitmap& bitmap)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| x0 | `uint8_t` | The X coordinate to place the bitmap - upper left corner|
| y0 | `uint8_t` | The Y coordinate to place the bitmap - upper left corner|
| Bitmap | `QwiicBitmap` | A bitmap object|

### text()

Draws a string using the current font on the screen.

```c++
void text(uint8_t x0, uint8_t y0, const char * text, uint8_t clr)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| x0 | `uint8_t` | The X coordinate to start drawing the text|
| y0 | `uint8_t` | The Y coordinate to start drawing the text|
| text | `const char*` | The string to draw on the screen |
| clr | `uint8_t` | **optional** The color value to draw the circle. This defaults to white (1).|

## Arduino Print

### setCursor()

This method is called set the "cursor" position in the device. The library supports the Arduino `Print` interface, enabling the use of a `print()` and `println()` methods. The set cursor position defines where to start text output for this functionality.

```c++
void setCursor(uint8_t x, uint8_t y)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| x | `uint8_t` | The X coordinate of the cursor|
| y | `uint8_t` | The Y coordinate of the cursor|

### setColor()

This method is called to set the current color of the system. This is used by the Arduino `Print` interface functionality


```c++
void setColor(uint8_t clr)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| `clr` | `uint8_t` | The color to set. 0 = black, > 0 = white|

### getColor()

This method is called to get the current color of the system. This is used by the Arduino `Print` interface functionality


```c++
uint8_t getColor(void)
```

| Parameter | Type | Description |
| :--- | :--- | :--- |
| return value| `uint8_t` | The current color|
