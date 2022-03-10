
//
//  TODO : HEADER
// 

/*
 * Buffer graphics device implementation file
 */

#include <stdio.h>
#include <stdlib.h>
#include "qwiic_grbuffer.h"


////////////////////////////////////////////////////////////////////////////////////
// Support for our drawing interface/vtable inspection
//
// function to get the vTable

_QwIDraw_vtable * GetIDrawVTable(_QwIDraw *obj){

	_QwIDraw_vtable ** vtable_ptr = (_QwIDraw_vtable**)obj;
	return *vtable_ptr;
}

////////////////////////////////////////////////////////////////////////////////////
// init_draw_functions()
//
// This method pulls out the vtable of this object and determines what methods are
// defined in the QwIDraw interface.
//
// Using what is defined, the functions in _idraw interfade are set. This class uses 
// the draw functions in this interface to skip call time vtable method lookup and call the 
// target method directly. 
//
// Ideally this pattern/implementation increase performace a slight bit. Also it 
// allows runtime method implementation determination in this class, which can then
// setup fallbacks if need.
//
//
bool QwGrBufferDevice::init_draw_functions(void){


	// Get our vtable, and that of a null _QwIDraw object/interface.
	// Use these to determine if a method was overriden/implemented and 
	// set defaults ... etc

	_QwIDraw null_obj_; // dummy object

	// copy in the vtable to our draw interface

	_idraw = *GetIDrawVTable(this);
	_QwIDraw_vtable *null_vtable = GetIDrawVTable(&null_obj_);

	// If we don't have a pixel set method, this is a fatal error
	if(_idraw.draw_pixel == null_vtable->draw_pixel){
		// ERROR - draw_pixel is required
		fprintf(stderr, "[ERROR][QwGrBufferDevice] Render class must implement draw_pixel method\n");
		return false;
	}

	// If the horz line function wasn't implemented (function pointer == null function) = set it toline

	if(_idraw.draw_line_horz == null_vtable->draw_line_horz)
		_idraw.draw_line_horz = _idraw.draw_line;

	// If the horz line function wasn't implemented (function pointer == null function) = set it toline
	if(_idraw.draw_line_vert == null_vtable->draw_line_vert)
		_idraw.draw_line_vert = _idraw.draw_line;

	// if rect_filled isn't implemented, just sub in rect - it' something ...
	if(_idraw.draw_rect_filled == null_vtable->draw_rect_filled)
		_idraw.draw_rect_filled = _idraw.draw_rect;	

	// if circle_filled isn't implemented, just sub in rect - it' something ...
	if(_idraw.draw_circle_filled == null_vtable->draw_circle_filled)
		_idraw.draw_circle_filled = _idraw.draw_circle;	

	return true;
}
////////////////////////////////////////////////////////////////////////////////////
bool QwGrBufferDevice::init(void){

	return init_draw_functions();
}

////////////////////////////////////////////////////////////////////////////////////////
// pixel()
//
void QwGrBufferDevice::pixel(uint8_t x, uint8_t y){


	if(x >= _viewport.width || y >= _viewport.height)
		return;

	(*_idraw.draw_pixel)(this, x, y);
}
////////////////////////////////////////////////////////////////////////////////////////
// line()
//
// Method that rasterizes a line, setting pixels in the buffer device via 
// the pixel command.
//
// Parameters
//
//		(x0, y0)   - Line origin
//      (x1, y1)   - Line end 
//
void QwGrBufferDevice::line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){

	// if we have a vertical or horizonal line, call the optimized drawing functions

	if(x0 == x1)
		(*_idraw.draw_line_vert)(this, x0, y0, x1, y1);
	else if(y0 == y1)
		(*_idraw.draw_line_horz)(this, x0, y0, x1, y1);	
	else
		(*_idraw.draw_line)(this, x0, y0, x1, y1);	

}

//////////////////////////////////////////////////////////////////////////////////////////
// draw_line()
//
// Core line drawing method - performs a line rasterization algorithm
//
void QwGrBufferDevice::draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){	

	// The rasterization method uses an increment of 1 to walk the desired line
	// to determine which pixels are being set in the buffer device. 
	//
	// To ensure a detailed rasterization, the dimention with the largest value is used.

	// TODO - sanity check bounds 

	// deltas for each dimension
	uint8_t dx = abs(x1 - x0);
	uint8_t dy = abs(y1 - y0);

	// zero len line - turns out that's a point
	if(!dx && !dy){
		(*_idraw.draw_pixel)(this, x0, y0);
		return;
	}
	// If this is a steep line, switch the axis to increment along
	bool isSteep = dy > dx;

	if(isSteep){
		swap_int(x0, y0);
		swap_int(x1, y1);
		swap_int(dx, dy);
	}

	// let's be positive about our increment - if negative, swap our vertex
	if (x0 > x1){
		swap_int(x0, x1);
		swap_int(y0, y1);
	}

	// value used to determine when to bump up Y as we walk the line
	int8_t err = dx / 2;
	int8_t ystep = y0 < y1 ? 1 : -1; 	// is y going up or down? 

	// Crank over our x range of the line and draw our pixels
	for (; x0 <= x1; x0++){

		if(isSteep)
			(*_idraw.draw_pixel)(this, y0, x0);
		else
			(*_idraw.draw_pixel)(this, x0, y0);

		// do we need to bump up y? or if the slow is shallow, stay of 
		// the same value?
		err -= dy;
		if (err < 0){
			y0 += ystep;
			err += dx;
		}
	}

}
////////////////////////////////////////////////////////////////////////////////////////
// Rectangles

void QwGrBufferDevice::rectangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){


	if(x0 == x1 || y0 == y1){
		// this is a line
		line(x0, y0, x1, y1);
		return;
	}

	// Sanity check
	if(x0 > x1 || y0 > y1){
		swap_int(x0, x1);
		swap_int(y0, y1);
	}

	// bounds check
	if(x0 >= _viewport.width || y0 >= _viewport.height)
		return; // not on screen

	// Note Lower level functions will deal with off screen sections of the rect

	// Send to drawing routine. 

	(*_idraw.draw_rect)(this, x0, y0, x1, y1);

}

////////////////////////////////////////////////////////////////////////////////////////
// draw_rect()
//
// Does the actual drawing/logic
void QwGrBufferDevice::draw_rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){


	// A rect is really just a series of lines 

	(*_idraw.draw_line_horz)(this, x0, y0, x1, y0);
	(*_idraw.draw_line_horz)(this, x0, y1, x1, y1);

	// If the height is less than 2, no need for vert lines b/c the line itself is 1 pixel

	if(y1-y0 < 3)
		return;

	// Vertical lines - note - do not draw over our horz lines b/c of potential xor turds
	(*_idraw.draw_line_vert)(this, x0, y0+1, x0 ,y1);
	(*_idraw.draw_line_vert)(this, x1, y0+1, x1 ,y1);


}
void QwGrBufferDevice::rectangle_fill(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){


	if(x0 == x1 || y0 == y1){
		// this is a line
		line(x0, y0, x1, y1);
		return;
	}

	// Sanity check
	if(x0 > x1 || y0 > y1){
		swap_int(x0, x1);
		swap_int(y0, y1);
	}

	// bounds check
	if(x0 >= _viewport.width || y0 >= _viewport.height)
		return; // not on screen

	// Note Lower level functions will deal with off screen sections of the rect

	// Send to drawing routine. 

	(*_idraw.draw_rect_filled)(this, x0, y0, x1, y1);

}
////////////////////////////////////////////////////////////////////////////////////////
// draw_rect_fill()
//
// Does the actual drawing/logic
void QwGrBufferDevice::draw_rect_filled(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){


	// Just draw vertical lines 
	for(int i=x0; i <= x1; i++)
		(*_idraw.draw_line_vert)(this, i, y0, i, y1);

}
////////////////////////////////////////////////////////////////////////////////////////
// circle()
//
void QwGrBufferDevice::circle(uint8_t x0, uint8_t y0, uint8_t radius){

	// Anything visible on screen?
	if(!radius || x0 - (int8_t)radius >= _viewport.width || y0 - (int8_t)radius  >= _viewport.height)	
		return;

	if(radius == 1){
		(*_idraw.draw_pixel)(this, x0, y0);
		return;
	}

	(*_idraw.draw_circle)(this, x0, y0, radius);
}
////////////////////////////////////////////////////////////////////////////////////////
// draw_circle()
//
void QwGrBufferDevice::draw_circle(uint8_t x0, uint8_t y0, uint8_t radius){

	int8_t f = 1 - radius;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * radius;
	int8_t x = 0;
	int8_t y = radius;

	(*_idraw.draw_pixel)(this, x0, y0 + radius);
	(*_idraw.draw_pixel)(this, x0, y0 - radius);
	(*_idraw.draw_pixel)(this, x0 + radius, y0);
	(*_idraw.draw_pixel)(this, x0 - radius, y0);

	while (x < y){

		if (f >= 0){
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		(*_idraw.draw_pixel)(this, x0 + x, y0 + y);
		(*_idraw.draw_pixel)(this, x0 - x, y0 + y);
		(*_idraw.draw_pixel)(this, x0 + x, y0 - y);
		(*_idraw.draw_pixel)(this, x0 - x, y0 - y);

		(*_idraw.draw_pixel)(this, x0 + y, y0 + x);
		(*_idraw.draw_pixel)(this, x0 - y, y0 + x);
		(*_idraw.draw_pixel)(this, x0 + y, y0 - x);
		(*_idraw.draw_pixel)(this, x0 - y, y0 - x);
	}


}
////////////////////////////////////////////////////////////////////////////////////////
// circle_fill()
//
void QwGrBufferDevice::circle_fill(uint8_t x0, uint8_t y0, uint8_t radius){

	// Anything visible on screen?
	if(!radius || x0 - (int8_t)radius >= _viewport.width || y0 - (int8_t)radius  >= _viewport.height)
		return;

	if(radius == 1){
		(*_idraw.draw_pixel)(this, x0, y0);
		return;
	}

	(*_idraw.draw_circle_filled)(this, x0, y0, radius);
}

////////////////////////////////////////////////////////////////////////////////////////
// draw_circle_fill()
//
void QwGrBufferDevice::draw_circle_filled(uint8_t x0, uint8_t y0, uint8_t radius){

	int8_t f = 1 - radius;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * radius;
	int8_t x = 0;
	int8_t y = radius;
	int8_t i;


	for (i = y0 - radius; i <= y0 + radius; i++)
		(*_idraw.draw_pixel)(this, x0, i);

	while (x < y){

		if (f >= 0){
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		for (i = y0 - y; i <= y0 + y; i++){
			(*_idraw.draw_pixel)(this, x0 + x, i);
			(*_idraw.draw_pixel)(this, x0 - x, i);
		}

		for(i = y0 - x; i <= y0 + x; i++){
			(*_idraw.draw_pixel)(this, x0 + y, i);
			(*_idraw.draw_pixel)(this, x0 - y, i);
		}
	}

}
////////////////////////////////////////////////////////////////////////////////////////
// bitmap()
//
void QwGrBufferDevice::bitmap(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, 
					 uint8_t *pBitmap, uint8_t bmp_width, uint8_t bmp_height ){
	
	(*_idraw.draw_bitmap)(this, x0, y0, x1, y1, pBitmap, bmp_width, bmp_height);
}
