
// qwiic_grssd1306.cpp
// 
// This is a library written for SparkFun Qwiic OLED boards that use the SSD1306.
//
// SparkFun sells these at its website: www.sparkfun.com
//
// Do you like this library? Help support SparkFun. Buy a board!
//
//   Micro OLED             https://www.sparkfun.com/products/14532
//   Transparent OLED       https://www.sparkfun.com/products/15173
//   "Narrow" OLED          https://www.sparkfun.com/products/17153
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

#include "qwiic_grssd1306.h"

/////////////////////////////////////////////////////////////////////////////
// Class that implements graphics support for devices that use the SSD1306
//
/////////////////////////////////////////////////////////////////////////////
// Device Commands
// 
// The commands are codes used to communicate with the SSD1306 device and are 
// from the devices datasheet.
//

#define kCmdSetContrast                 0x81
#define kCmdDisplayAllOnResume          0xA4
#define kCmdDisplayAllOn                0xA5
#define kCmdNormalDisplay               0xA6
#define kCmdInvertDisplay               0xA7
#define kCmdDisplayOff                  0xAE
#define kCmdDisplayOn                   0xAF
#define kCmdSetDisplayOffset            0xD3
#define kCmdSetComPins                  0xDA
#define kCmdSetVComDeselect             0xDB
#define kCmdSetDisplayClockDiv          0xD5
#define kCmdSetPreCharge                0xD9
#define kCmdSetMultiplex                0xA8
#define kCmdSetLowColumn                0x00
#define kCmdSetHighColumn               0x10
#define kCmdSetStartLine                0x40
#define kCmdMemoryMode                  0x20
#define kCmdComScanInc                  0xC0
#define kCmdComScanDec                  0xC8
#define kCmdSegRemap                    0xA0
#define kCmdChargePump                  0x8D
#define kCmdExternalVCC                 0x01
#define kCmdSwitchCapVCC                0x02
#define kCmdPageAddress                 0x22
#define kCmdColumnAddress               0x21
#define kCmdActivateScroll              0x2F
#define kCmdDeactivateScroll            0x2E
#define kCmdSetVerticalScrollArea       0xA3
#define kCmdRightHorizontalScroll       0x26
#define kCmdLeftHorizontalScroll        0x27
#define kCmdVerticalRightHorzScroll     0x29
#define kCmdVerticalLeftHorzScroll      0x2A
#define kCmdPageModePageBase            0xB0
#define kCmdPageModeColTopBase          0x10
#define kCmdPageModeColLowBase          0x0F


//////////////////////////////////////////////////////////////////////////////////
// Screen Buffer 
//
// A key feature of this library is that it only sends "dirty" pixels to the device,
// minimizing data transfer over the I2C bus. To accomplish this, the dirty range
// of each graphics buffer page (see device memory layout in the datasheet) is 
// maintained during drawing operation. Whe data is sent to the device, only the pixels
// in these regions are sent to the device, not the entire page of data. 
//
// The below macros are used to manage the record keeping of dirty page ranges. Given 
// that these actions are taking place in the draw loop, macros are used for performance 
// considerations.
//
// These macros work with the pageState_t struct type.
//
// Define unique values just outside of the screen buffer (SSD1306) page range (0 base)
// Note: A page  is 128 bits in length 

#define  kPageMin                   -1   // outside bounds - low value
#define  kPageMax                  128   // outside bounds - high value

// clean/ no settings in the page
#define page_is_clean(_page_) (_page_.xmin == kPageMax)

// Macro to reset page descriptor
#define page_set_clean(_page_) \
    do{ \
        _page_.xmin = kPageMax; \
        _page_.xmax = kPageMin; \
    }while(false)

// Macro to check and adjust record bounds based on a single location
#define page_check_bounds(_page_, _x_) \
    do{ \
        if( _x_ < _page_.xmin) _page_.xmin = _x_; \
        if( _x_ > _page_.xmax) _page_.xmax = _x_; \
    }while(false)

// Macro to check and adjust record bounds using another page descriptor
#define page_check_bounds_desc(_page_, _page2_) \
    do{ \
        if( _page2_.xmin < _page_.xmin) _page_.xmin = _page2_.xmin; \
        if( _page2_.xmax > _page_.xmax) _page_.xmax = _page2_.xmax; \
    }while(false)

// Macro to check and adjust record bounds using bounds values
#define page_check_bounds_range(_page_, _x0_, _x1_) \
    do{ \
        if( _x0_ < _page_.xmin) _page_.xmin = _x0_; \
        if( _x1_ > _page_.xmax) _page_.xmax = _x1_; \
    }while(false)


//////////////////////////////////////////////////////////////////////////////////
// Communication 
//
// When communicating with the device, you either send commands or data. Define
// our codes for these two options - these are basically i2c registers/offsets.
// 
// See datasheet for details
//
#define kDeviceSendCommand  0x00
#define kDeviceSendData     0x40

////////////////////////////////////////////////////////////////////////////////////
// Pixel write/set operations
//
// Using LAMBDAs to create fast raster write/set operations. Using this pattern
// eleminates the need for switch/if statements in each draw routine. This is basically
// classic ROPs'
//
// NOTE - the order in the arrays is based on grRasterOp_t enum
//
// The Graphic operator functions (ROPS)
//      - Copy      - copy the pixel value in to the buffer (default)
//      - Not Copy  - copy the not of the pixel value to buffer
//      - Not       - Set the buffer value to not it's current value
//      - XOR       - XOR of color and current pixel value
//      - Black     - Set value to always be black
//      - White     - set value to always be white

typedef void (*rasterOPsFn)(uint8_t * dest, uint8_t src, uint8_t mask);

static const rasterOPsFn _rasterOps[] ={
    [](uint8_t *dst, uint8_t src, uint8_t mask)->void { *dst = (~mask & *dst) | src & mask;},   // COPY
    [](uint8_t *dst, uint8_t src, uint8_t mask)->void { *dst = (~mask & *dst) | !src & mask;},  // NOT COPY
    [](uint8_t *dst, uint8_t src, uint8_t mask)->void { *dst = (~mask & *dst) | !(*dst) & mask;},  // NOT DEST
    [](uint8_t *dst, uint8_t src, uint8_t mask)->void { *dst = (~mask & *dst) | (*dst ^ src) & mask;},  // XOR
    [](uint8_t *dst, uint8_t src, uint8_t mask)->void { *dst = ~mask & *dst;},  // Always Black 
    [](uint8_t *dst, uint8_t src, uint8_t mask)->void { *dst =  mask | *dst;},  // Always White
};

////////////////////////////////////////////////////////////////////////////////////
// Constructor
//
// Just a bunch of member variable inits

QwGrSSD1306::QwGrSSD1306(): 
    _pBuffer{nullptr}, 
    _i2cBus{nullptr}, 
    _i2c_address{0}, 
    _initHWComPins{kDefaultPinConfig},
    _initPreCharge{kDefaultPreCharge},
    _initVCOMDeselect{kDefaultVCOMDeselect},
    _initContrast{kDefaultContrast},
    default_address{0},
    _color{1},
    _rop{grROPCopy},
    _isInit{false}
{}


////////////////////////////////////////////////////////////////////////////////////
// init()
//
// Called by user when the device/system is up and ready to be "initialized."
//
// This implementation performs the basic setup for the SSD1306 device
//
// The startup sequence is as follows:
//
//      - Make sure a device is connected
//      - Call super class
//      - Shutdown the device (display off), initial device setup, turn on device
//      - Init the local graphics buffers/system
// 
// When this method is complete, the driver and device are ready for use
//
bool QwGrSSD1306::init(void){

    if(_isInit)
        return true;

    //  do we have a bus yet? Buffer? Note - buffer is set by subclass of this object
    if(!_i2cBus || !_i2c_address || !_pBuffer)
        return false;

    // Is the device connected?
    if(!_i2cBus->ping(_i2c_address))
        return false;

    // Super-class 
    if(!this->QwGrBufferDevice::init())
        return false; // something isn't right

    // At this point, the main device interface is filled in. This classes draw_line_vert()
    // can also draw filled rects, so hack the device table to have draw_rect_filled points
    // to this classes draw_line_vert(), which should be the method in the draw_line_vert entry
    // of the draw interface.

    _idraw.draw_rect_filled = _idraw.draw_line_vert;

    // Start the device setup - sending commands to device. See command defs in header, and
    // device datasheet
    send_dev_command(kCmdDisplayOff);

    send_dev_command(kCmdSetDisplayClockDiv, 0x80);
    send_dev_command(kCmdSetMultiplex, _viewport.height-1);
    send_dev_command(kCmdSetDisplayOffset, 0x0);

    send_dev_command(kCmdSetStartLine | 0x0);   
    send_dev_command(kCmdChargePump, 0x14);
    send_dev_command(kCmdMemoryMode, 0b10); // Page Addressing mode - see data sheet

    send_dev_command(kCmdNormalDisplay);
    send_dev_command(kCmdDisplayAllOnResume);
    send_dev_command(kCmdSegRemap | 0x1);

    send_dev_command(kCmdComScanDec);
    send_dev_command(kCmdSetComPins, _initHWComPins);
    send_dev_command(kCmdSetContrast, _initContrast);

    send_dev_command(kCmdSetPreCharge, _initPreCharge);
    send_dev_command(kCmdSetVComDeselect, _initVCOMDeselect);
    send_dev_command(kCmdDeactivateScroll);

    send_dev_command(kCmdDisplayOn);

    // Finish up setting up this object

    // Number of pages used for this device?
    _nPages = _viewport.height/kByteNBits;  // height / number of pixels per byte. TODO - support multiples != 8

    // init the graphics buffers
    init_buffers();

    _isInit = true; // we're ready to rock

    return true;

}
////////////////////////////////////////////////////////////////////////////////////
// Configuration API
//
// This allows sub-classes to setup for their device, while preserving encapsulation.
//
// These should be called/set before calling init
//
// For details of each of these settings -- see the datasheet
//
void QwGrSSD1306::set_comm_pins(uint8_t pin_code){

    _initHWComPins = pin_code;
}

void QwGrSSD1306::set_pre_charge(uint8_t pre_charge){
    _initPreCharge = pre_charge;
}

void QwGrSSD1306::set_vcom_deselect(uint8_t vcom_d){

    _initVCOMDeselect = vcom_d;
}

void QwGrSSD1306::set_contrast(uint8_t contrast){

    if(!_isInit)
        _initContrast = contrast;
    else
        send_dev_command(kCmdSetContrast, contrast);
}

////////////////////////////////////////////////////////////////////////////////////
// set_comm_bus()
//
// Method to set the bus object that is used to communicate with the device
//
// TODO -  In the *future&, generalize to match SDK

void QwGrSSD1306::set_comm_bus(QwI2C &theBus, uint8_t id_bus){

    _i2cBus = &theBus;
    _i2c_address = id_bus;
}

////////////////////////////////////////////////////////////////////////////////////
// set_buffer()
//
// Protected method - used by sub-class to set the graphics buffer array.
//
// The subclass knows the size of the specific device, so it statically defines
// the graphics buffer array. The buffer is often set in the subclasses on_initialize() 
// method.
//
// 
void QwGrSSD1306::set_buffer(uint8_t *pBuffer){

    if(pBuffer)
        _pBuffer = pBuffer;
}

////////////////////////////////////////////////////////////////////////////////////
// clear_screen_buffer()
//
// Clear out all the on-device memory. 
//
void QwGrSSD1306::clear_screen_buffer(void){

    // Clear out the screen buffer on the device
    uint8_t emptyPage[kPageMax]={0};

    for(int i=0; i < kMaxPageNumber; i++){
        set_screenbuffer_address(i, 0);                     // start of page
        send_dev_data((uint8_t*)emptyPage, kPageMax);       // clear out page
    }

}
////////////////////////////////////////////////////////////////////////////////////
// init_buffers()
//
// Will clear the local graphics buffer, and the devices screen buffer. Also
// resets page state descriptors to a "clean" state.

void QwGrSSD1306::init_buffers(void){

    int i;

    // clear out the local graphics buffer
    if(_pBuffer)
        memset(_pBuffer, 0, _viewport.width * _nPages);     

    // Set page descs to "clean" state
    for(i=0; i < _nPages; i++){
        page_set_clean(_pageState[i]);
        page_set_clean(_pageErase[i]);      
    }
    _pendingErase = false;

    // clear out the screen buffer
    clear_screen_buffer();
}
////////////////////////////////////////////////////////////////////////////////////
// resend_graphics()
//
// Re-send the region in the graphics buffer (local) that contains drawn graphics. 
// This region is defined by the contents of the _pageErase descriptors.
//
// Copy these to the page state, and call display
//

void QwGrSSD1306::resend_graphics(void){

    // Set the page state dirty bounds to the bounds of erase state
    for(int i=0; i < _nPages; i++)
        _pageState[i] = _pageErase[i];

    display(); // push bits to screen buffer
}
////////////////////////////////////////////////////////////////////////////////////
// Screen Control
////////////////////////////////////////////////////////////////////////////////////
// flip_vert()
//
// Flip the onscreen graphics vertically.

void QwGrSSD1306::flip_vert(bool bFlip){

    send_dev_command( (bFlip ? kCmdComScanInc : kCmdComScanDec));

}
////////////////////////////////////////////////////////////////////////////////////
// flip_horz()
//
// Flip the onscreen graphcis horizontally. This requires a resend of the graphics
// data to the device/screen buffer.
//

void QwGrSSD1306::flip_horz(bool bFlip){

    send_dev_command( kCmdSegRemap | (bFlip ? 0x0 : 0x1));
    clear_screen_buffer();
    resend_graphics();
}
////////////////////////////////////////////////////////////////////////////////////
// invert()
//
// Inverts the display contents on device
//

void QwGrSSD1306::invert(bool bInvert){

    send_dev_command( (bInvert ? kCmdInvertDisplay : kCmdNormalDisplay));

}

////////////////////////////////////////////////////////////////////////////////////
void QwGrSSD1306::scroll_stop(void){

    send_dev_command(kCmdDeactivateScroll);

    // After sending a deactivate command, the ram data in the device needs to be 
    // re-written. See datasheet
    //
    // First clear out the entire screen buffer (on device mem). The device uses this off screen area 
    // to scroll - if you don't erase it, when scroll starts back up, old graphics turds
    // will appear ...
    //
    // Second - Send over the graphics again to the display 

    clear_screen_buffer();
    resend_graphics();
}

////////////////////////////////////////////////////////////////////////////////////
// scroll()
//
// Set scroll parametes on the device and start scrolling
// 
void QwGrSSD1306::scroll(uint16_t scroll_type, uint8_t start, uint8_t stop, uint8_t interval){


    // parameter sanity?
    if(stop < start)
        return;

    // Setup a default command list
    uint8_t n_commands = 7; 
    uint8_t commands[7] = { kCmdRightHorizontalScroll, // default scroll right
                            0x00,           // dummy byte
                            start,          // start page address
                            interval,       // interval between scroll steps - in terms of frame fequency
                            stop,           // end page address 
                            0x00,           // dummy byte for non vert, for vert it's scroll offset
                            0xFF};          // Dummy byte for non vert - set to FFX, not used for vert.

    // Which way to scroll 
    switch(scroll_type){
        case SCROLL_RIGHT: 
            break;  // set in initializer of command array
        case SCROLL_LEFT: 
            commands[0] = kCmdLeftHorizontalScroll; 
            break;
        case SCROLL_VERT_LEFT:
            commands[0] = kCmdVerticalLeftHorzScroll;
            break;
        case SCROLL_VERT_RIGHT:
            commands[0] = kCmdVerticalRightHorzScroll;
            break;      
    }

    // If we are scrolling vertically, modify the command list, and set the vertical scroll area on display
    if(scroll_type & SCROLL_VERTICAL){

        commands[5]=0x01;   // set the scrolling offset
        n_commands--;       // don't use the last byte of command buffer

        // Set on display scroll area
        send_dev_command(kCmdSetVerticalScrollArea, 0x00);
        send_dev_command(_viewport.height);     
    }

    // send the scroll commands to the device
    
    // Do not use scroll_stop() - that method resets the display - memory ...etc - to it's 
    // start state - the graphics displayed before scrolling was initially started.
    //
    // Here, we just stop scrolling and keep device memory state as is. This allows scrolling
    // to change paraterms during a scroll session - gives a smooth presentation on  screen.
    send_dev_command(kCmdDeactivateScroll); 
    send_dev_command(commands, n_commands);
    send_dev_command(kCmdActivateScroll);
}

////////////////////////////////////////////////////////////////////////////////////
// Drawing Methods
////////////////////////////////////////////////////////////////////////////////////
// erase()
//
// Erase the graphics that are on screen and anything that's been draw but
// haven't been sent to the screen.
//

void QwGrSSD1306::erase(void){

    if(!_pBuffer)
        return;

    // Cleanup the dirty parts of each page in the graphics buffer. 
    for(uint8_t i=0; i < _nPages; i++){

        // _pageState 
        // The current "dirty" areas of the graphics [local] buffer.
        // Areas that haven't been sent to the screen/device but are
        // "dirty"
        //
        // Add the areas with pixels set and have been sent to the 
        // device - this is the contents of _pageErase

        page_check_bounds_desc(_pageState[i], _pageErase[i]);

        // if this page is clean, there is nothing to update
        if(page_is_clean(_pageState[i]))
            continue;

        // clear out memory that is dirty on this page
        memset(_pBuffer + i * _viewport.width + _pageState[i].xmin, 0,
               _pageState[i].xmax - _pageState[i].xmin + 1 ); // add one b/c values are 0 based

        // clear out any pending dirty range for this page - it's erased
        page_set_clean(_pageState[i]);
    }

    // Indicate that the data transfer to the device should include the erase region
    _pendingErase = true;

}

////////////////////////////////////////////////////////////////////////////////////
//
// draw_pixel()
//
// Used to set a pixel in the graphics buffer - uses the current write operator function
//

void QwGrSSD1306::draw_pixel(uint8_t x, uint8_t y, uint8_t clr){

    // quick sanity check on range
    if(x >= _viewport.width || y >= _viewport.height)
        return; // out of bounds

    uint8_t bit = byte_bits[mod_byte(y)];

    _rasterOps[_rop](_pBuffer + x + y/kByteNBits * _viewport.width, // pixel offset
                            (clr ? bit : 0), bit);  // which bit to set in byte

    page_check_bounds(_pageState[y/kByteNBits], x); // update dirty range for page

}
////////////////////////////////////////////////////////////////////////////////////
// draw_line_horz()
//
// Fast horizontal line drawing routine
//

void QwGrSSD1306::draw_line_horz(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr){

    // Basically we set a bit within a range in a page of our graphics buffer.

    // in range
    if(y0 >= _viewport.height)
        return;

    if(x0 > x1)
        swap_int(x0, x1);

    if(x1 >= _viewport.width)
        x1 = _viewport.width-1;

    uint8_t bit = byte_bits[mod_byte(y0)]; // bit to set
    rasterOPsFn curROP = _rasterOps[_rop]; // current raster op

    // Get the start of this line in the graphics buffer
    uint8_t *pBuffer = _pBuffer +  x0 + y0/kByteNBits * _viewport.width;

    // walk up x and set the target pixel using the pixel operator function
    for(int i=x0; i <= x1; i++, pBuffer++)
        curROP(pBuffer, (clr ? bit : 0), bit);
    
    // Mark the page dirty for the range drawn
    page_check_bounds_range(_pageState[y0/kByteNBits], x0, x1);
}
////////////////////////////////////////////////////////////////////////////////////
// draw_line_vert()
//
// Fast vertical line drawing routine - also supports fast filled rects
//
void QwGrSSD1306::draw_line_vert(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr){

    if(x0 >= _viewport.width) // out of bounds
        return;

    // want an accending order
    if(y0 > y1)
        swap_int(y0, y1);

    // keep on screen
    if(y1 >= _viewport.height)
        y1 = _viewport.height-1;

    uint8_t startBit, endBit, setBits;

    // Get the start and end pages we are writing to        
    uint8_t page0 = y0/kByteNBits;
    uint8_t page1 = y1/kByteNBits;

    // loop over the pages. For each page determine the range of pixels 
    // to set in the target page byte and then set them using the current
    // pixel operator function

    // Note: This function can also be used to draw filled rects - just iterate 
    //       in the x direction. The base rect fill (in grBuffer) calls this method x1-x0 times,
    //       and each of those calls has some overhead. So just iterating over each page - 
    //       x1-x0 times here - saves overhead costs.
    //  
    //       To make this work, make sure x0 > x1. Also, this method is wired in as the
    //       draw_rect_filled entry in the draw interface. This is done above in the 
    //       init process.     

    int xinc; 
    if(x0 > x1)
        swap_int(x0,x1);

    rasterOPsFn curROP = _rasterOps[_rop]; // current raster op

    for(int i=page0; i <= page1; i++){

        startBit = mod_byte(y0);    //start bit in this byte

        // last bit of this byte to set? Does the line end in this byte, or continue on...
        endBit = y0 + kByteNBits - startBit > y1 ? mod_byte(y1)  : kByteNBits-1;    

        // Set the bits from startBit to endBit
        setBits = (0xFF >> (kByteNBits - endBit)-1) << startBit; // what bits are being set in this byte        
        
        // set the bits in the graphics buffer using the current byte operator function
        
        // Note - We iterate over x to fill in a rect if specified.
        for(xinc = x0; xinc <= x1; xinc++)
            curROP(_pBuffer + i * _viewport.width + xinc, (clr ? setBits : 0), setBits);

        y0 += endBit - startBit + 1;  // increment Y0 to next page

        page_check_bounds_range(_pageState[i], x0, x1); // mark dirty range in page desc
    }

}

////////////////////////////////////////////////////////////////////////////////////
// draw_bitmap()
//
// Draw a 8 bit encoded (aka same y layout as this device) bitmap to the screen
//

void QwGrSSD1306::draw_bitmap(uint8_t x0, uint8_t y0, uint8_t dst_width, uint8_t dst_height, 
                              uint8_t *pBitmap, uint8_t bmp_width, uint8_t bmp_height ){

    // some simple checks
    if(x0 >= _viewport.width || y0 >= _viewport.height || !bmp_width || !bmp_height)
        return;


    uint8_t bmp_x = 0; // fix
    uint8_t bmp_y = 0; // fix

    uint8_t xinc, page0, page1;
    uint8_t startBit, endBit, grSetBits, grStartBit;    

    uint8_t bmp_mask[2];
    uint8_t bmp_data, bmpPage;

    uint8_t remainingBits;
    uint8_t neededBits; 

    uint8_t y1 = y0+dst_height-1;

    page0 = y0/kByteNBits;
    page1 = y1/kByteNBits;

    rasterOPsFn curROP = _rasterOps[_rop]; // current raster op

    // The Plan:
    //   - Walk down the graphics buffer range (y) one page at a time
    //   - For each page 
    //          - Determine needed number of bits for the destination
    //          - Determine what bits to pull from the bitmap
    //              - Create a mask to pull out bits - from one or two bytes
    //          - Loop over the x dimension
    //              - pull bits from bitmap, build byte of data of bitmap bits, in 
    //                right order for the destination (graphics buffer)
    //              - Write the bitmap bits to the graphis buffer using the current operator

    // Loop over the memory pages in the graphics buffer
    for(int i=page0; i <= page1; i++){

        // First, get the number of destination bits in the current page
        grStartBit = mod_byte(y0);  //start bit
        
        // last bit of this byte to set? Does the copy region end in this byte, or continue on...
        endBit = y0 + kByteNBits - grStartBit > y1 ? mod_byte(y1)  : kByteNBits-1;  

        // Set the bits from startBit to endBit
        grSetBits = (0xFF >> (kByteNBits - endBit - 1)) << grStartBit; // what bits are being set in this byte  

        // how many bits of data do we need to transfer from the bitmap?
        neededBits = endBit - grStartBit + 1;
        
        // Okay, we have how much data to transfer to the current page. Now build the data
        // from the bitmap.

        // First, build bit masks for pulling the data out of the bmp array. The data
        // might straddle two bytes, so build two masks

        // as above, get the start and end bites for the current position in the bmp.
        startBit = mod_byte(bmp_y);
        endBit = (kByteNBits - startBit > neededBits ? startBit + neededBits  : kByteNBits)-1;          
        
        // Set the bits from startBit to endBit
        bmp_mask[0] = (0xFF >> (kByteNBits - endBit-1)) << startBit; 

        // any remaining bits to get?
        remainingBits = neededBits - (endBit - startBit + 1); // +1 - needsBits is 1's based
        bmp_mask[1] = 0xFF >> (kByteNBits - remainingBits);

        // What row in the source bitmap
        bmpPage = bmp_y/kByteNBits;

        // we have the mask for the bmp - loop over the width of the copy region, pulling out
        // bmp data and writing it to the graphics buffer
        for(xinc = 0; xinc < dst_width; xinc++  ){

            // get data bits out of current bitmap location and shift if needed
            bmp_data = (pBitmap[bmp_width*bmpPage + bmp_x + xinc] & bmp_mask[0]) >> startBit;
            
            if(remainingBits) // more data to add from the next byte in this column
                bmp_data |= (pBitmap[bmp_width*(bmpPage+1) + bmp_x + xinc] & bmp_mask[1]) << (kByteNBits - remainingBits);

            // Write the bmp data to the graphics buffer - using current write op. Note,
            // if the location in the buffer didn't start at bit 0, we shift bmp_data
            curROP(_pBuffer + i * _viewport.width + xinc + x0, 
                                    bmp_data << grStartBit, grSetBits);

        }
        // move up our y values (graphics buffer and bitmap) by the number of bits transferred
        y0 += neededBits;
        bmp_y += neededBits;

        page_check_bounds_range(_pageState[i], x0, x0+dst_width); // mark dirty range in page desc
    }

}

////////////////////////////////////////////////////////////////////////////////////
// Device Update Methods
////////////////////////////////////////////////////////////////////////////////////
// set_screenbuffer_address()
//
// Sets the target screen buffer address for graphics buffer transfer to the device.
//
// The positon is specified by page and column
//
// The system runs in "page mode" - data is streamed along a page, based
// on the set starting position.
//
// This class takes advantage of this to just write the "dirty" ranges in a page.
//

bool QwGrSSD1306::set_screenbuffer_address(uint8_t page, uint8_t column){

    if(page >= _nPages || column >= _viewport.width)
        return false;

    // send the page address
    send_dev_command(kCmdPageModePageBase | page );

    // For the column start address, add the viewport x offset. Some devices (Micro OLED) don't
    // start at column 0 in the screen buffer
    send_dev_command( (kCmdPageModeColTopBase | (column >> 4)) + _viewport.x);
    send_dev_command(kCmdPageModeColLowBase & column);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////
// display()
//
// Send the "dirty" areas of the graphics buffer to the device's screen buffer. Only send 
// the areas that need to be updated. The update region is based on new graphics to
// display, and any currently displayed items that need to be erased.

void QwGrSSD1306::display(){

    // Loop over our page descriptors - if a page is dirty, send the graphics buffer
    // dirty region to the device for the current page

    pageState_t transferRange;

    for(int i=0; i < _nPages; i++){

        // We keep the erase rect seperate from dirty rect. Make temp copy of 
        // dirty rect page range, expand to include erase rect page range.

        transferRange = _pageState[i];

        // If an erase has happend, we need to transfer/include erase update range
        if(_pendingErase)   
            page_check_bounds_desc(transferRange, _pageErase[i]);

        if( page_is_clean(transferRange)) // both dirty and erase range for this page were null
            continue;  // next      

        // set the start address to write the updated data to the devices screen buffer
        set_screenbuffer_address(i, transferRange.xmin);

        // send the dirty data to the device
        send_dev_data( _pBuffer + (i * _viewport.width ) + transferRange.xmin, // this page start + xmin
                       transferRange.xmax - transferRange.xmin +1);  // dirty region xmax - xmin. Add 1 b/c 0 based

        // If we sent the erase bounds, zero out the erase bounds - this area is now clear
        if(_pendingErase)
            page_set_clean(_pageErase[i]);

        // add the just send dirty range (non erase rec)  to the erase rect
        page_check_bounds_desc(_pageErase[i], _pageState[i]);
        
        // this page is no longer dirty - mark it  clean
        page_set_clean(_pageState[i]);          
    }
    _pendingErase=false; // no longer pending 

}

////////////////////////////////////////////////////////////////////////////////////
// Device communication methods
////////////////////////////////////////////////////////////////////////////////////
// sendDeviceCommand()
//
// send a single command to the device via the current bus object

void QwGrSSD1306::send_dev_command(uint8_t command){

    _i2cBus->writeRegisterByte(_i2c_address, kDeviceSendCommand, command);

}

////////////////////////////////////////////////////////////////////////////////////
// sendDeviceCommand()
//
// send a single command and value to the device via the current bus object. 

void QwGrSSD1306::send_dev_command(uint8_t *commands, uint8_t n_commands){

    if(!commands || n_commands == 0 )
        return;

    _i2cBus->writeRegisterRegion(_i2c_address, kDeviceSendCommand, commands, n_commands);

}

////////////////////////////////////////////////////////////////////////////////////
// sendDeviceCommand()
//
// send a single command and value to the device via the current bus object. 
//

void QwGrSSD1306::send_dev_command(uint8_t command, uint8_t value){

    uint8_t buffer[] = {command, value};

    send_dev_command(buffer, 2);

}
////////////////////////////////////////////////////////////////////////////////////
// sendDeviceData()
//
// send a single command to the device via the current bus object

void QwGrSSD1306::send_dev_data(uint8_t *pData, uint8_t nData){

    _i2cBus->writeRegisterRegion(_i2c_address, kDeviceSendData, pData, nData);
}