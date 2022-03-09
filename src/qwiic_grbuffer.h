
//
//  TODO : HEADER
// 

// Graphics System for qwiic display devices - simple bmp based devices.
//
// 


#pragma once

#include <stdint.h>


// Some rouintes need a int swap function. Many ways to do this- all the cool kids
// use bitwise ops  - namely the xor. 
//
#define  swap_int(_a_, _b_)  ( ((_a_) ^= (_b_)), ((_b_) ^= (_a_)), ((_a_) ^= (_b_) ))

// Base class of the device driver -- encapulates everything needed to communicate to 
// the device and setting pixels in the frame buffer.


// RECT! 
struct QwRect
{
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
};

///////////////////////////////////////////////////////////////////////////////////////////
// _QwIDraw
//
// IDraw interface for the graphics system. Defines methods used to draw 2D primatives.
//
// Seperated out to enable easy vtable access and method dispatch short circuiting

class _QwIDraw{

	// Pixel Methods
	virtual void  draw_pixel(uint8_t x, uint8_t y){} // A subclass must implement this

	// Line Methods
	virtual void draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){}
	virtual void draw_line_horz(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){}
	virtual void draw_line_vert(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){}	

	// Rectangles
	virtual void draw_rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){}
	virtual void draw_rect_filled(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){}	

	//Circles
	virtual void draw_circle(uint8_t x0, uint8_t y0, uint8_t radius){}	
	virtual void draw_circle_filled(uint8_t x0, uint8_t y0, uint8_t radius){}		
};

// Drawing fuction typedefs 
typedef void (*QwDrawPntFn)(void*, uint8_t, uint8_t);
typedef void (*QwDrawTwoPntFn)(void*, uint8_t, uint8_t, uint8_t, uint8_t);
typedef void (*QwDrawCircleFn)(void*, uint8_t, uint8_t, uint8_t);

// Define the vtable struct for IDraw
struct _QwIDraw_vtable{
	QwDrawPntFn      draw_pixel;
	QwDrawTwoPntFn   draw_line;
	QwDrawTwoPntFn   draw_line_horz;
	QwDrawTwoPntFn   draw_line_vert;		
	QwDrawTwoPntFn   draw_rect;
	QwDrawTwoPntFn   draw_rect_filled;	
	QwDrawCircleFn   draw_circle;
	QwDrawCircleFn   draw_circle_filled;	

};

///////////////////////////////////////////////////////////////////////////////////////////
//  QwGrBufferDevice
//
//  Buffer class - defines basics for a memory buffer drawing class
class QwGrBufferDevice : protected _QwIDraw
{

public:
	// Constructors
	QwGrBufferDevice(){};
	QwGrBufferDevice(uint8_t width, uint8_t height) : QwGrBufferDevice(0, 0, width, height){};
	QwGrBufferDevice(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height){
		set_viewport(x0, y0, width, height);
	};


	// Buffer location on the device
	void set_viewport(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height){
		_viewport.x = x0;
		_viewport.y = y0;
		_viewport.width = width;
		_viewport.height = height;
	};

	QwRect   get_viewport(void){ return _viewport;};
	uint16_t get_origin_x(void){ return _viewport.x; };
	uint16_t get_origin_y(void){ return _viewport.y; };
	uint16_t get_width(void){ return _viewport.width; };
	uint16_t get_height(void){ return _viewport.height;};


	// Interface methods that a sublcass fills in.

	// Lifecycle
	virtual bool init(void);

	// Public Interface - Graphics interface TODO add color? 
	void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
	void circle(uint8_t x0, uint8_t y0, uint8_t radius);
	void circle_fill(uint8_t x0, uint8_t y0, uint8_t radius);	
	void pixel(uint8_t x, uint8_t y);
	void rectangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
	void rectangle_fill(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);	

	// subclass interface 
	virtual void display(void)=0; 
	virtual void erase(void)=0;		


protected:

	QwRect _viewport; 

	// Internal, fast draw routines 
 	// Pixels
 	//    At a minimum, a sub-class must implement a pixel set function
	virtual void  draw_pixel(uint8_t x, uint8_t y)=0; // A subclass must implement this

	// Lines
	//     Sub-class implementation is optional
	virtual void draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

	virtual void draw_rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

	virtual void draw_rect_filled(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

	virtual void draw_circle(uint8_t x0, uint8_t y0, uint8_t radius);

	virtual void draw_circle_filled(uint8_t x0, uint8_t y0, uint8_t radius);		

	// Our drawing interface - open to sub-classes ...
	_QwIDraw_vtable  _idraw;

private:

	bool init_draw_functions(void);


};