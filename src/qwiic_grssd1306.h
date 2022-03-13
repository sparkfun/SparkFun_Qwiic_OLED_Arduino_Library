//
//  TODO : HEADER
// 

/*
 * Header file for the SSD1306 bitmap graphics driver device.
 */

#pragma once

#include "qwiic_grbuffer.h"
#include "qwiic_i2c.h"
#include "res/qwiic_resdef.h"



/////////////////////////////////////////////////////////////////////////////
// Device Config
//
// Defaults
// Each device can have a different Hardware pin configuration, which must
// be set in the device. These are the pins that connect the display to
// the SSD1306.
//
#define kDefaultPinConfig		0x12
#define kDefaultPreCharge		0xF1
#define kDefaultVCOMDeselect	0x40
#define kDefaultContrast		0x8F

// Graphic pixel write operator function types
typedef enum gr_op_funcs {
	GrOpPixClear = 0,
	GrOpPixSet   = 1,
	GrOpPixXOR   = 2,
} grOpFunction_t;

// Flags for scrolling

#define SCROLL_VERTICAL 	0x01
#define SCROLL_RIGHT    	0x02
#define SCROLL_LEFT     	0x04
#define SCROLL_VERT_RIGHT   SCROLL_VERTICAL|SCROLL_RIGHT
#define SCROLL_VERT_LEFT    SCROLL_VERTICAL|SCROLL_LEFT
/////////////////////////////////////////////////////////////////////////////
// Buffer Management
//
// The memory/back buffer of the SSD1306 is based on the concept of pages -
// each page is a stream of bytes, and defined as follows:
//
//		- X pixel position is an offset in a byte array
//		- Y pixel position is a bit in a byte, so a page can have 8 Y locations
//
// A pixel value of 1, turn on the corresponding pixel, 0 turns it off. 
//
// The device has different data transfer modes - see the data sheet - mostly
// outline how received a recieved byte is placed in the device framebuffer and the 
// next update locaton set. 
//
// This implementation uses the Page mode for buffer transfer. This is defined by:
//     - A start position is set - a page number and column in that page.
// 	   - As data is transferred, it is written to the screenbuffer, based on this start 
//     	 position
//	   - If the end of the page is reached, the next entry location is the start of that page
//
// >> Implementation <<
//
// This implementation uses the concept of "dirty rects" at the page level to minimize data
// transfers to the device. The min and max x locations set for each page is recorded as 
// graphics are draw to the graphics buffer. When the transfering the display buffer to
// the devices screen buffer, the following takes place:
//
//		For each page:
//			- if page is dirty
//				- Set the screen buffer current location to this page, xmin dirty value
//				- Write buffer bytes to the device - starting at xmin for the page, ending at xmax
// 				- Mark the buffer as "clean"
//
//
// Define variables to manage page state

#define  kMaxPageNumber  			8

typedef struct {
	int16_t		xmin;
	int16_t    	xmax;
} pageState_t; 


/////////////////////////////////////////////////////////////////////////////
// QwGrSSD1306
// A buffer graphics device to support the SSD1306 graphics hardware

class QwGrSSD1306 :  public QwGrBufferDevice {

public:

	QwGrSSD1306();    // default constructor - always called 
	using QwGrBufferDevice::QwGrBufferDevice; // inherit constructors 

	// Public draw methods
	void display(void);   		// send graphics buffer to the devices screen buffer
	void erase(void);
	
	// Device setup
	virtual bool init(void );
	bool is_initialized(void){ return _isInit;};
	
	// method to set the communication bus this object should use
	void set_comm_bus(QwI2C &theBus, uint8_t id_bus);

	// Set the current color/pixel write operation
	void set_color(uint8_t color);
	void set_xor(bool bEnable);

	// Settings/operational methods
	void set_contrast(uint8_t);

	// default address of the device - expect the sub to fill in.
	uint8_t default_address;

	
	// screen control
	void flip_vert(bool);
	void flip_horz(bool);	

	// screen scrolling
	void scroll_stop(void);
	void scroll(uint16_t scroll_type, uint8_t start, uint8_t stop, uint8_t interval);

protected:

	// Subclasses of this class define the specifics of the device, including size.
	// Subclass needs to define the graphics buffer array - stack based - and pass in 
	void set_buffer(uint8_t *pBuffer);      

	// Pixel operator getter/setter -- how to manage current set function
	void set_pixel_op(grOpFunction_t optype){
		_pix_op = optype;
	}
	grOpFunction_t get_pixel_op(void){
		return _pix_op;
	}

	///////////////////////////////////////////////////////////////////////////
	// Internal, fast draw routines - this are used in the overall 
	// draw interface (_QwIDraw) for this object/device/system. 
	//
	// >> Pixels <<
	void  draw_pixel(uint8_t x, uint8_t y);

	// >> Fast Lines <<
	void draw_line_horz(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
	void draw_line_vert(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);	

	// >> Fast Bitmap <<
	void draw_bitmap(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, 
					 uint8_t *pBitmap, uint8_t bmp_width, uint8_t bmp_height );
	
	///////////////////////////////////////////////////////////////////////////
	// configuration methods for sub-classes. Settings unique to a device 
	void set_comm_pins(uint8_t);
	void set_pre_charge(uint8_t);
	void set_vcom_deselect(uint8_t);

private:

	bool set_screenbuffer_address(uint8_t page, uint8_t column);
	void init_buffers(void); // clear graphics and screen buffer
	
	// device communication methods
	void send_dev_command(uint8_t command);
	void send_dev_command(uint8_t command, uint8_t value);	
	void send_dev_command(uint8_t *commands, uint8_t n);		
	void send_dev_data(uint8_t *pData, uint8_t nData);

	// instance vars
	uint8_t  		  * _pBuffer;           // Pointer to the graphics buffer
	uint8_t    		  	_nPages; 			// number of pages for current device
	pageState_t 		_pageState[kMaxPageNumber];   // page state descriptors 
	pageState_t			_pageErase[kMaxPageNumber];   // keep track of erase boundaries 
	bool                _pendingErase; 

	grOpFunction_t	    _pix_op;    // op type
	grOpFunction_t      _color; // current color

	// I2C  things
	QwI2C 			  * _i2cBus;       // pointer to our i2c bus object
	uint8_t				_i2c_address;  // address of the device

	// Stash values for settings that are unique to each device.

	uint8_t				_initHWComPins;
	uint8_t    	        _initPreCharge;
	uint8_t				_initVCOMDeselect;
	uint8_t				_initContrast;

	bool				_isInit; // general init flat
};
