// SparkFun_Qwiic_OLED.h
// 
// This is a library written for SparkFun Qwiic OLED boards that use the SSD1306.
//
// SparkFun sells these at its website: www.sparkfun.com
//
// Do you like this library? Help support SparkFun. Buy a board!
//
//   Micro OLED 			https://www.sparkfun.com/products/14532
//   Transparent OLED     	https://www.sparkfun.com/products/15173
//   "Narrow" OLED  		https://www.sparkfun.com/products/17153
// 
// 
// Written by Kirk Benell @ SparkFun Electronics, March 2022
//
// This library configures and draws graphics to OLED boards that use the 
// SSD1306 display hardware. The library only supports I2C.
// 
// Repository:
//     https://github.com/sparkfun/SparkFun_Qwiic_OLED_Arduino_Library
//
// Documentation:
//     https://sparkfun.github.io/SparkFun_Qwiic_OLED_Arduino_Library/
//
//
// SparkFun code, firmware, and software is released under the MIT License(http://opensource.org/licenses/MIT).
//
// SPDX-License-Identifier: MIT
//
//    The MIT License (MIT)
//
//    Copyright (c) 2022 SparkFun Electronics
//    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
//    associated documentation files (the "Software"), to deal in the Software without restriction,
//    including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
//    and/or sell copies of the Software, and to permit persons to whom the Software is furnished to
//    do so, subject to the following conditions:
//    The above copyright notice and this permission notice shall be included in all copies or substantial
//    portions of the Software.
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
//    NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#pragma once

// Arduino wrapper for the OLED library. Provides an Arduino experience and platform support 
// for the Qwiic OLED driver.

// include the underlying SDK implementation headers for the OLED devices
#include "qwiic_oledmicro.h"
#include "qwiic_olednarrow.h"
#include "qwiic_oledtransp.h"

#include <Arduino.h>
#include <Wire.h>
//

typedef QwFont 		QwiicFont;
typedef QwBitmap 	QwiicBitmap;

#define COLOR_WHITE 1
#define COLOR_BLACK 0

// The Plan:
//
// For each supported device the following is needed
//    
//    	- A commmon Arduino interface/implementation. One impl, one area to maintain
//		- Classes that are specialized for each device
//		- No abstract methods that a subclass must implement
//
// The solution
//    A templated base class that device specific sub-classes derive from.
//  			

#define kNoAddressSet 0

template <typename SSD1306DeviceType>
class QwiicOLEDBaseClass : public Print {   // NOTE: implementing Arduino Print
private:
	// our device driver
	SSD1306DeviceType  	_device;
	QwI2C				_i2cBus;  // our i2c object

	// for the Aruduino print functionaliyt
	uint8_t             _cursorX;
	uint8_t             _cursorY;	

	uint8_t             _color; 

public:
	bool begin(TwoWire &wirePort=Wire, uint8_t address=kNoAddressSet){

		// defaults for Arduino Print 
		_cursorX = 0;
		_cursorY = 0; 
		_color   = COLOR_WHITE;
		
		_i2cBus.init(wirePort);

    	_device.set_comm_bus(_i2cBus, 
    						(address == kNoAddressSet ? _device.default_address : address));

    	bool bStatus = _device.init();

    	// Want to start cursor at Y height of the current font, if we have a font.
    	//
    	// Get our font height ... a default font is set during init ...
    	if(bStatus){
			QwiicFont * pFont = _device.get_font();
			if(pFont)
				_cursorY = pFont->height;
		}

    	return bStatus;
    }

    uint8_t getWidth(void){
    	return _device.get_width();
    }

    uint8_t getHeight(void){
    	return _device.get_height();
    }

    void display(void){
    	_device.display();
    }

    void erase(void){
    	_device.erase();
    }

    void invert(bool bInvert){
    	_device.invert(bInvert);
    }

    void flipVertical(bool bFlip){
    	_device.flip_vert(bFlip);
    }

    void flipHorizontal(bool bFlip){
    	_device.flip_horz(bFlip);
    }

    void scrollStop(void){
    	_device.scroll_stop();
    }

    void scrollRight(uint8_t start, uint8_t stop, uint8_t interval){
    	_device.scroll(SCROLL_RIGHT, start, stop, interval);
    }

    void scrollVertRight(uint8_t start, uint8_t stop, uint8_t interval){
    	_device.scroll(SCROLL_VERT_RIGHT, start, stop, interval);
    }

    void scrollLeft(uint8_t start, uint8_t stop, uint8_t interval){
    	_device.scroll(SCROLL_LEFT, start, stop, interval);
    }

    void scrollVertLeft(uint8_t start, uint8_t stop, uint8_t interval){
    	_device.scroll(SCROLL_VERT_LEFT, start, stop, interval);
    }   

    void setFont(QwiicFont& theFont){
    	_device.set_font(theFont);
    }

    QwiicFont * getFont(void){
    	return _device.get_font();
    }

    void setDrawMode(grRasterOp_t rop){
    	_device.set_raster_op(rop);
    }

  	grRasterOp_t getDrawMode(void){
  		return _device.get_raster_op();
  	}

    void pixel(uint8_t x, uint8_t y, uint8_t clr=COLOR_WHITE){
    	_device.pixel(x, y, clr);
    }

    void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr=COLOR_WHITE){
        _device.line(x0, y0, x1, y1, clr);
    }

	void rectangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr=COLOR_WHITE){
		_device.rectangle(x0, y0, x1, y1, clr);
	}    

	void rectangleFill(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr=COLOR_WHITE){
		_device.rectangle_fill(x0, y0, x1, y1, clr);
	}

	void circle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t clr=COLOR_WHITE){
		_device.circle(x0, y0, radius, clr);
	}

	void circleFill(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t clr=COLOR_WHITE){
		_device.circle_fill(x0, y0, radius, clr);
	}

	void bitmap(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, 
				uint8_t *pBitmap, uint8_t bmp_width, uint8_t bmp_height ){
		_device.bitmap(x0, y0, x1, y1, pBitmap, bmp_width, bmp_height);
	}
	// Bitmap draw - using a bitmap object
	void bitmap(uint8_t x0, uint8_t y0, QwiicBitmap& bitmap){
		_device.bitmap(x0, y0, bitmap);
	}

	void text(uint8_t x0, uint8_t y0, const char * text, uint8_t clr=COLOR_WHITE){
		_device.text(x0, y0, text, clr);
	}

	void setCursor(uint8_t x, uint8_t y){

		if(x < 0 || x >= _device.get_width() || y < 0 || y >= _device.get_height())
			return;

		_cursorX = x;
		_cursorY = y;
	}

	void setColor(uint8_t clr){
		_color = (clr > 0 ?  COLOR_WHITE : COLOR_BLACK);
	}
	uint8_t getColor(void){
		return _color;
	}

	// For the Arduino Print interface
	virtual size_t write(uint8_t theChar){

		QwiicFont *pFont = _device.get_font();

		switch(theChar){
			case '\n':        	// Carriage return
				_cursorX = 0;
				_cursorY += pFont->height;
			case '\r':  		// Line feed - do nothing
				break;
			default:
				_device.text(_cursorX, _cursorY, (const char*)&theChar, _color);
				_cursorX += pFont->width;
				if( _cursorX > _device.get_width() - pFont->width){  // overflow
					_cursorX = 0;
					_cursorY += pFont->height;

				}
				break;
		}
		if(_cursorY > _device.get_height()) // check for overflow
			_cursorY = pFont->height;
		return 1;
	}

};

// For our actual implementations - just subclass from the Arduino template

class QwiicMicroOLED : public QwiicOLEDBaseClass<QwOLEDMicro>{
	// nothing here - see above
};

class QwiicNarrowOLED : public QwiicOLEDBaseClass<QwOLEDNarrow>{
	// nothing here - see above
};

class QwiicTransparentOLED : public QwiicOLEDBaseClass<QwOLEDTransparent>{
	// nothing here - see above
};
