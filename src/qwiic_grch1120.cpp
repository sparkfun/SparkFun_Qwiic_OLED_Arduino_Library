
// qwiic_grch1120.cpp
//
// This is a library written for SparkFun Qwiic OLED boards that use the
// CH1120. This driver is VERY similar to the SSD1306 driver, but with a few 
// subtle differences. Currently, our only device with the CH1120 driver is 
// the Qwiic OLED 1.5".
//
// SparkFun sells these at its website: www.sparkfun.com
//
// Do you like this library? Help support SparkFun. Buy a board!
//
// Qwiic OLED 1.5in       https://www.sparkfun.com/products/29530
//
// Written by SparkFun Electronics, August 2025
//
// This library configures and draws graphics to OLED boards that use the
// CH1120 display hardware. The library only supports I2C.
//
// Repository:
//     https://github.com/sparkfun/SparkFun_Qwiic_OLED_Arduino_Library
//
// Documentation:
//     https://sparkfun.github.io/SparkFun_Qwiic_OLED_Arduino_Library/
//
//
// SparkFun code, firmware, and software is released under the MIT
// License(http://opensource.org/licenses/MIT).
//
// SPDX-License-Identifier: MIT
//
//    The MIT License (MIT)
//
//    Copyright (c) 2022 SparkFun Electronics
//    Permission is hereby granted, free of charge, to any person obtaining a
//    copy of this software and associated documentation files (the "Software"),
//    to deal in the Software without restriction, including without limitation
//    the rights to use, copy, modify, merge, publish, distribute, sublicense,
//    and/or sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following conditions: The
//    above copyright notice and this permission notice shall be included in all
//    copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED
//    "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
//    NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
//    PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
//    ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "qwiic_grch1120.h"
#include <map>

/////////////////////////////////////////////////////////////////////////////
// Class that implements graphics support for devices that use the CH1120
//

//////////////////////////////////////////////////////////////////////////////////
// Screen Buffer
//
// A key feature of this library is that it only sends "dirty" pixels to the
// device, minimizing data transfer over the I2C bus. To accomplish this, the
// dirty range of each graphics buffer page (see device memory layout in the
// datasheet) is maintained during drawing operation. Whe data is sent to the
// device, only the pixels in these regions are sent to the device, not the
// entire page of data.
//
// The below macros are used to manage the record keeping of dirty page ranges.
// Given that these actions are taking place in the draw loop, macros are used
// for performance considerations.
//
// These macros work with the pageState_t struct type.
//
// Define unique values just outside of the screen buffer (valid is 0 - 159)

#define kPageMin -1  // outside bounds - low value
#define kPageMax 160 // outside bounds - high value

// clean/ no settings in the page
#define pageIsClean(_page_) (_page_.xmin == kPageMax)

// Macro to reset page descriptor
#define pageSetClean(_page_)                                                                                           \
    do                                                                                                                 \
    {                                                                                                                  \
        _page_.xmin = kPageMax;                                                                                        \
        _page_.xmax = kPageMin;                                                                                        \
    } while (false)

// Macro to check and adjust record bounds based on a single location
#define pageCheckBounds(_page_, _x_)                                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
        if (_x_ < _page_.xmin)                                                                                         \
            _page_.xmin = _x_;                                                                                         \
        if (_x_ > _page_.xmax)                                                                                         \
            _page_.xmax = _x_;                                                                                         \
    } while (false)

// Macro to check and adjust record bounds using another page descriptor
#define pageCheckBoundsDesc(_page_, _page2_)                                                                           \
    do                                                                                                                 \
    {                                                                                                                  \
        if (_page2_.xmin < _page_.xmin)                                                                                \
            _page_.xmin = _page2_.xmin;                                                                                \
        if (_page2_.xmax > _page_.xmax)                                                                                \
            _page_.xmax = _page2_.xmax;                                                                                \
    } while (false)

// Macro to check and adjust record bounds using bounds values
#define pageCheckBoundsRange(_page_, _x0_, _x1_)                                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
        if (_x0_ < _page_.xmin)                                                                                        \
            _page_.xmin = _x0_;                                                                                        \
        if (_x1_ > _page_.xmax)                                                                                        \
            _page_.xmax = _x1_;                                                                                        \
    } while (false)


/////////////////////////////////////////////////////////////////////////////
// Device Commands
//
// The commands are codes used to communicate with the CH1120 device and are
// from the devices datasheet. (See )
//
// Control Bytes:
// Control Bytes consist of C0 and D~C Bits followed by the rest of the command byte:

//            | ---- Command Byte --- |     
// | C0 | D~C | 0 | 0 | 0 | 0 | 0 | 0 |

// The Co bit (MSB) has the following meaning: 
// 0 : This is the last control byte, only data bytes follow.
// 1: More control bytes follow

// The D~C bit (2nd MSB) has the following meaning:
// 0: The data byte is for command operation
// 1: The byte is for RAM (data) operation

#define kCmdControlByte              ((uint8_t)0x00)
#define kCmdAnotherControlByte       ((uint8_t)0xC0)
#define kCmdControlDataByteFollow    ((uint8_t)0x80)
#define kCmdRamControlByte           ((uint8_t)0x40)

#define kCmdRowStartEnd              ((uint8_t)0x22)  // Two byte command - command, start, stop
#define kCmdColStartEnd              ((uint8_t)0x21)  // Two byte command - command, start, stop
#define kCmdStartRow                 ((uint8_t)0xB0) 
#define kCmdStartColLow              ((uint8_t)0x0F) // Start Column = StartColHigh (MSB) | StartColLow (LSB)
#define kCmdStartColHigh             ((uint8_t)0x10)
#define kCmdStartLine                ((uint8_t)0xA2) // Notice how this is different from the same cmd on the 1306 (0x40)

#define kCmdContrastControl          ((uint8_t)0x81)
#define kCmdHorizAddressing          ((uint8_t)0x20)
#define kCmdDownVerticalScroll       ((uint8_t)0x24) // Down vertical scroll
#define kCmdUpVerticalScroll         ((uint8_t)0x25) // Up vertical scroll
#define kCmdRightHorizontalScroll    ((uint8_t)0x26) // Right horizontal scroll
#define kCmdLeftHorizontalScroll     ((uint8_t)0x27) // Left horizontal scroll
#define kCmdDeactivateScroll         ((uint8_t)0x2E) // Stop scrolling (default)
#define kCmdActivateScroll           ((uint8_t)0x2F) // Activate Scrolling scrolling
#define kCmdDisplayOn                ((uint8_t)0xAF)
#define kCmdDisplayOff               ((uint8_t)0xAE)
#define kCmdPanelID                  ((uint8_t)0xE1)
#define kCmdDriverID                 ((uint8_t)0xE2) // BUSY+ON/OFF+0x60
#define kCmdGrayMono                 ((uint8_t)0xAC)
#define kCmdSegRemapDown             ((uint8_t)0xA0) // default
#define kCmdSegRemapUp               ((uint8_t)0xA1) // reverse
#define kCmdComOutScan0First         ((uint8_t)0xC0) // scan COM0 to COM[n-1] (default)
#define kCmdComOutScan0Last          ((uint8_t)0xC8) // scan COM[n-1] to COM0 (reverse)
#define kCmdDisplayRotation          ((uint8_t)0xA3)
#define kCmdDisableEntireDisplay     ((uint8_t)0xA4)
#define kCmdEnableEntireDisplay      ((uint8_t)0xA5)
#define kCmdNormalDisplay            ((uint8_t)0xA6) // default
#define kCmdReverseDisplay           ((uint8_t)0xA7)
#define kCmdMultiplexRatio           ((uint8_t)0xA8)
#define kCmdDisplayOffset            ((uint8_t)0xD3)
#define kCmdDisplayDivideRatio       ((uint8_t)0xD5)
#define kCmdDischargeFront           ((uint8_t)0x93)
#define kCmdDischargeBack            ((uint8_t)0xD8)
#define kCmdPreCharge                ((uint8_t)0xD9)
#define kCmdSEGpads                  ((uint8_t)0xDA)
#define kCmdVCOMDeselectLevel        ((uint8_t)0xDB)
#define kCmdExternalIREF             ((uint8_t)0xAD)

/////////////////////////////////////////////////////////////////////////////
// Manufacturer Default Settings
//
// The default settings given in the sample quickstart code.
//
#define kDefaultMonoMode                 ((uint8_t)0x01) // grayscale by default "0x00"
#define kDefaultRowStart                 ((uint8_t)0x00) // default
#define kDefaultColStart                 ((uint8_t)0x00) // default
#define kDefaultRowEnd                   ((uint8_t)0x3F) // default
#define kDefaultColEnd                   ((uint8_t)0x1F) // default
#define kDefaultDisplayStart             ((uint8_t)0x00) // default
// #define kDefaultHorizontalAddressing     ((uint8_t)0x00) // default
#define kDefaultHorizontalAddressing     ((uint8_t)0x01) // default
#define kDefaultContrast                 ((uint8_t)0xC8) // default = 0x80, upper end = 0xFF
#define kDefaultRotateDisplayNinety      ((uint8_t)0x01) // Default is 0 degrees (NOTE: start and end values for row/column need adjusting if rotate=0)
#define kDefaultMultiplexRatio           ((uint8_t)0x7F) // default is 0x9F
#define kDefaultDisplayOffset            ((uint8_t)0x10) // default is 0x00
#define kDefaultDivideRatio              ((uint8_t)0x00) // default
#define kDefaultDischargeFront           ((uint8_t)0x02) // default is 0x02
#define kDefaultDischargeBack            ((uint8_t)0x02) // default
#define kDefaultPreCharge                ((uint8_t)0x1F) // 0x1F is default
#define kDefaultSegPads                  ((uint8_t)0x00) // default
#define kDefaultVCOMDeselect             ((uint8_t)0x3F) // default
#define kDefaultExternalIREF             ((uint8_t)0x02)

////////////////////////////////////////////////////////////////////////////////////
// Pixel write/set operations
//
// Using LAMBDAs to create fast raster write/set operations. Using this pattern
// eleminates the need for switch/if statements in each draw routine. This is
// basically classic ROPs'
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

typedef void (*rasterOPsFn)(uint8_t *dest, uint8_t src, uint8_t mask);

static const rasterOPsFn m_rasterOps[] = {
    // COPY
    [](uint8_t *dst, uint8_t src, uint8_t mask) -> void { *dst = (~mask & *dst) | (src & mask); },
    // NOT COPY
    [](uint8_t *dst, uint8_t src, uint8_t mask) -> void { *dst = (~mask & *dst) | ((!src) & mask); },
    // NOT DEST
    [](uint8_t *dst, uint8_t src, uint8_t mask) -> void { *dst = (~mask & *dst) | ((!(*dst)) & mask); },
    // XOR
    [](uint8_t *dst, uint8_t src, uint8_t mask) -> void { *dst = (~mask & *dst) | ((*dst ^ src) & mask); },
    // Always Black
    [](uint8_t *dst, uint8_t src, uint8_t mask) -> void { *dst = ~mask & *dst; },
    // Always White
    [](uint8_t *dst, uint8_t src, uint8_t mask) -> void { *dst = mask | *dst; }};


/////////////////////////////////////////////////////////////////////////////
// Map for scrolling. See qwiic_grcommon.h for original definitions and descriptions
/////////////////////////////////////////////////////////////////////////////

const std::map<uint8_t, uint8_t> scrollIntervals = {
    {SCROLL_INTERVAL_6_FRAMES, 0x00},
    {SCROLL_INTERVAL_32_FRAMES, 0x01},
    {SCROLL_INTERVAL_64_FRAMES, 0x02},
    {SCROLL_INTERVAL_128_FRAMES, 0x03},
    {SCROLL_INTERVAL_3_FRAMES, 0x04},
    {SCROLL_INTERVAL_4_FRAMES, 0x05},
    {SCROLL_INTERVAL_5_FRAMES, 0x06},
    {SCROLL_INTERVAL_2_FRAMES, 0x07},
    // The following intervals are not directly available for the CH1120
    // so we will include their closest mappings for compatiblity with other driver/examples
    {SCROLL_INTERVAL_25_FRAMES, 0x01}, // closest to 25 frames is 32 frames (0x01)
    {SCROLL_INTERVAL_256_FRAMES, 0x03}  // closest to 256 frames is 128 frames (0x03)
};

////////////////////////////////////////////////////////////////////////////////////
// setup defaults - called from constructors
//
// Just a bunch of member variable inits

void QwGrCH1120::setupDefaults(void)
{
    default_address = {0};
    m_pBuffer = {nullptr};
    m_color = {1};
    m_rop = {grROPCopy};
    m_i2cBus = {nullptr};
    m_i2cAddress = {0x3C}; // address of the device (0x3D for closed)
    m_initPreCharge = {kDefaultPreCharge};
    m_initContrast = {kDefaultContrast};
    m_isInitialized = {false};
}

////////////////////////////////////////////////////////////////////////////////////
// init()
//
// Called by user when the device/system is up and ready to be "initialized."
//
// This implementation performs the basic setup for the CH1120 device
//
// The startup sequence is as follows:
//
//      - Make sure a device is connected
//      - Call super class
//      - Shutdown the device (display off), initial device setup, turn on
//      device
//      - Init the local graphics buffers/system
//
// When this method is complete, the driver and device are ready for use
//
bool QwGrCH1120::init(void)
{
    if (m_isInitialized)
        return true;

    // do we have a bus yet? Buffer? Note - buffer is set by subclass of this
    // object
    if (!m_i2cBus || !m_i2cAddress || !m_pBuffer)
        return false;

    // Is the device connected?
    if (!m_i2cBus->ping(m_i2cAddress))
        return false;
    
    // Call super class init
    if (!(this->QwGrBufferDevice::init()))
        return false;

    // setup the oled device
    setupOLEDDevice();

    // Finish up setting up this object

    // number of pages used for this device?
    // By default, we are operating this device in "rotate 90" mode with "horizontal addressing"
    // Each "page" is a byte representing 8 pixels
    // so the number of pages that it takes to span entire row is the width divided by the number of bits in a byte
    m_nPages = m_viewport.width / kByteNBits;  // width / number of pixels per byte.
                                               // TODO - support multiples != 8

    // init the graphics buffers
    initBuffers();

    m_isInitialized = true;

    return true;
}


////////////////////////////////////////////////////////////////////////////////////
// reset()
//
// Return the OLED system to its initial state
//
// Returns true on success, false on failure

bool QwGrCH1120::reset(bool clearDisplay)
{
    // If we are not in an init state, just call init
    if (!m_isInitialized)
        return init();

    // is the device connected?
    if (!m_i2cBus->ping(m_i2cAddress))
        return false;

    // setup oled
    setupOLEDDevice(clearDisplay);

    // Init internal/drawing buffers and device screen buffer
    if (clearDisplay)
        initBuffers();

    return true;
}

////////////////////////////////////////////////////////////////////////////////////
// Configuration API
//
// This allows sub-classes to setup for their device, while preserving
// encapsulation.
//
// These should be called/set before calling init
//
// For details of each of these settings -- see the datasheet
//
void QwGrCH1120::setCommPins(uint8_t pin_code)
{
    // NOTE: For now, we won't really use this for the CH1120, but is here for compatibility/symmetry with other OLED drivers
    m_initHWComPins = pin_code;
}

void QwGrCH1120::setPreCharge(uint8_t pre_charge)
{
    m_initPreCharge = pre_charge;
}

void QwGrCH1120::setVcomDeselect(uint8_t vcom_d)
{
    m_initVCOMDeselect = vcom_d;
}

void QwGrCH1120::setContrast(uint8_t contrast)
{
    if (!m_isInitialized)
        m_initContrast = contrast;
    else
        sendDevCommand(kCmdContrastControl, contrast);
}

////////////////////////////////////////////////////////////////////////////////////
// setupOLEDDevice()
//
// Method sends the init/setup commands to the OLED device, placing
// it in a state for use by this driver/library.
void QwGrCH1120::setupOLEDDevice(bool clearDisplay){
    if (clearDisplay)
        sendDevCommand(kCmdDisplayOff);

    setScreenBufferAddress(kDefaultRowStart, kDefaultRowEnd);

    sendDevCommand(kCmdStartLine, kDefaultDisplayStart); 
    sendDevCommand(kCmdContrastControl, m_initContrast);
    sendDevCommand(kCmdGrayMono, kDefaultMonoMode);
    sendDevCommand(kCmdHorizAddressing, kDefaultHorizontalAddressing);
    sendDevCommand(kCmdSegRemapDown);
    sendDevCommand(kCmdComOutScan0First);
    sendDevCommand(kCmdDisplayRotation, kDefaultRotateDisplayNinety);
    sendDevCommand(kCmdDisableEntireDisplay);

    sendDevCommand(kCmdDisplayOffset, kDefaultDisplayOffset);
    sendDevCommand(kCmdDischargeFront, kDefaultDischargeFront);
    sendDevCommand(kCmdDischargeBack, kDefaultDischargeBack);
    sendDevCommand(kCmdPreCharge, m_initPreCharge);
    sendDevCommand(kCmdSEGpads, kDefaultSegPads);
    sendDevCommand(kCmdVCOMDeselectLevel, kDefaultVCOMDeselect);
    sendDevCommand(kCmdExternalIREF, kDefaultExternalIREF);

    if (clearDisplay)
        // now, turn it back on
        sendDevCommand(kCmdDisplayOn);
}

////////////////////////////////////////////////////////////////////////////////////
// setCommBus()
//
// Method to set the bus object that is used to communicate with the device
//
// TODO -  In the *future*, generalize to match SDK

void QwGrCH1120::setCommBus(QwI2C &theBus, uint8_t id_bus)
{
    m_i2cBus = &theBus;
    m_i2cAddress = id_bus;
}

////////////////////////////////////////////////////////////////////////////////////
// setBuffer()
//
// Protected method - used by sub-class to set the graphics buffer array.
//
// The subclass knows the size of the specific device, so it statically defines
// the graphics buffer array. The buffer is often set in the subclasses
// on_initialize() method.
//
//
void QwGrCH1120::setBuffer(uint8_t *pBuffer)
{
    if (pBuffer)
        m_pBuffer = pBuffer;
}

////////////////////////////////////////////////////////////////////////////////////
// clearScreenBuffer()
//
// Clear out all the on-device memory.
//
void QwGrCH1120::clearScreenBuffer(void)
{
    // Clear out the screen buffer on the device
    uint8_t emptyPage[kPageMax] = {0};

    for (int i = 0 ; i < kMaxPageNumberCH1120; i++)
    {
        setScreenBufferAddress(0, i);
        sendDevData(emptyPage, kPageMax);
    }
}

////////////////////////////////////////////////////////////////////////////////////
// initBuffers()
//
// Will clear the local graphics buffer, and the devices screen buffer. Also
// resets page state descriptors to a "clean" state.
void QwGrCH1120::initBuffers(void)
{
    int i;

    // clear out the local graphics buffer
    if (m_pBuffer)
        memset(m_pBuffer, 0, m_viewport.width * m_nPages);

    // Set page descs to "clean" state
    for (i = 0; i < m_nPages; i++)
    {
        pageSetClean(m_pageState[i]);
        pageSetClean(m_pageErase[i]);
    }

    m_pendingErase = false;

    // clear out the screen buffer
    clearScreenBuffer();
}

////////////////////////////////////////////////////////////////////////////////////
// resendGraphics()
//
// Re-send the region in the graphics buffer (local) that contains drawn
// graphics. This region is defined by the contents of the m_pageErase
// descriptors.
//

void QwGrCH1120::resendGraphics(void)
{
    // Set the page state dirty bounds to the bounds of erase state
    for (int i = 0; i < m_nPages; i++)
        m_pageState[i] = m_pageErase[i];

    display(); // push bits to screen buffer
}

////////////////////////////////////////////////////////////////////////////////////
// Screen Control

////////////////////////////////////////////////////////////////////////////////////
// flip_vert()
//
// Flip the onscreen graphics vertically.
void QwGrCH1120::flipVert(bool bFlip){
    sendDevCommand(bFlip ?  kCmdComOutScan0Last : kCmdComOutScan0First);
}

////////////////////////////////////////////////////////////////////////////////////
// flip_horz()
//
// Flip the onscreen graphics horizontally. This requires a resend of the
// graphics data to the device/screen buffer.
void QwGrCH1120::flipHorz(bool bFlip){ 

    if (bFlip){
        // If we are flipping to horizontal, we need to adjust row start and end to the end of the display memory
        // This is because when we horizontally flip, if our viewport is smaller than the total area, we will flip in some garbage
        // When flipping we need to offset by the max width minus the viewport width (maybe should be height, may have to update with non-square display)
        horz_flip_offset = kMaxCH1120Width - m_viewport.width;

        sendDevCommand(kCmdSegRemapUp);
    }

    else{
        // If in normal mode, just set to the defaults
        horz_flip_offset = 0;

        sendDevCommand(kCmdSegRemapDown);
    }

    clearScreenBuffer();
    resendGraphics();
}

////////////////////////////////////////////////////////////////////////////////////
// invert()
//
// Inverts the display contents on device
//
void QwGrCH1120::invert(bool bInvert)
{
    sendDevCommand(bInvert ? kCmdReverseDisplay : kCmdNormalDisplay);
}

////////////////////////////////////////////////////////////////////////////////////
void QwGrCH1120::stopScroll(void)
{
    sendDevCommand(kCmdDeactivateScroll);

    // After sending a deactivate command, the ram data in the device needs to be
    // re-written. See datasheet
    //
    // First clear out the entire screen buffer (on device mem). The device uses
    // this off screen area to scroll - if you don't erase it, when scroll starts
    // back up, old graphics turds will appear ...
    //
    // Second - Send over the graphics again to the display

    clearScreenBuffer();
    resendGraphics();
}

////////////////////////////////////////////////////////////////////////////////////
// scroll()
//
// Set scroll parameters on the device and start scrolling
//
void QwGrCH1120::scroll(uint16_t scroll_type, uint8_t start, uint8_t stop, uint8_t interval)
{
    // parameter sanity?
    if (stop < start)
        return;

    // Setup a default command list
    uint8_t n_commands = 6;

    // See page 48 of the CH1120 datasheet. This differs from the SSD1306
    // This is a 6 Byte Command With the Bytes:
    // 0) Scroll Direction Set (0x24 - down, 0x25 - up, 0x26 - right, 0x27 - left)
    // 1) Start Column Position Set (0x00 to 0x9F)
    // 2) End Column Position Set (0x00 to 0x9F)
    // 3) Start Row Position Set (0x00 to 0x9F)
    // 4) End Row Position Set (0x00 to 0x9F)
    // 5) Time Interval Set (X + B0) where B0 is 0b000 (6frames) to 0b111 (2 frames) (with several options up to 128 frames in between)

    uint8_t commands[n_commands] = {kCmdRightHorizontalScroll, // default scroll right
                           0x00,                        // default 0x00 start column
                           0x7F,                        // let's default to 128 for the 128x128 display
                           0x00,                        // default 0x00 start row
                           0x7F,                        // let's default to 128 for the 128x128 display
                           scrollIntervals.at(interval) // map passed interval to correct value
                          };

    
    // Which way to scroll
    // Note how we multiply the start/stop values passed in by 8 such that it more closely aligns with
    // the behavior of the other SSD1306 driver (which had 64 rows as 8 pages), and example code can be similar between them
    // We add 7 to the stop value so it is the last row of the page
    switch (scroll_type)
    {
    case SCROLL_LEFT:
        commands[0] = kCmdLeftHorizontalScroll;
        commands[1] = start * 8;
        commands[2] = stop * 8 + 7;
        break;
    case SCROLL_VERT_MODE1:
        commands[0] = kCmdUpVerticalScroll;
        commands[3] = start * 8;
        commands[4] = stop * 8 + 7;
        break;
    case SCROLL_VERT_MODE2:
        commands[0] = kCmdDownVerticalScroll;
        commands[3] = start * 8;
        commands[4] = stop * 8 + 7;
        break;
    case SCROLL_RIGHT:
    default:
        // Note that the 1306 (and thus the higher level driver) support scroll right vert etc. which we don't. We'll stick w/ horizontal right in this case
        commands[0] = kCmdRightHorizontalScroll;
        commands[1] = start * 8;
        commands[2] = stop * 8 + 7;
    }

    // send the scroll commands to the device

    // Do not use scroll_stop() - that method resets the display - memory ...etc -
    // to it's start state - the graphics displayed before scrolling was initially
    // started.
    //
    // Here, we just stop scrolling and keep device memory state as is. This
    // allows scrolling to change paraterms during a scroll session - gives a
    // smooth presentation on  screen.
    sendDevCommand(kCmdDeactivateScroll);
    sendDevCommand(commands, n_commands);
    sendDevCommand(kCmdActivateScroll);
}

////////////////////////////////////////////////////////////////////////////////////
// displayPower()
//
// Used to set the power of the screen.
void QwGrCH1120::displayPower(bool enable)
{
    if (!m_isInitialized)
        return;

    sendDevCommand((enable ? kCmdDisplayOn : kCmdDisplayOff));
}

////////////////////////////////////////////////////////////////////////////////////
// Drawing Methods

////////////////////////////////////////////////////////////////////////////////////
// erase()
//
// Erase the graphics that are on screen and anything that's been draw but
// haven't been sent to the screen.
//

// This function sort of becomes useless because the entire page is rewritten each time now, so there isn't so much a concept of "erasing"
void QwGrCH1120::erase(void)
{
    if (!m_pBuffer)
        return;

    // Cleanup the dirty parts of each page in the graphics buffer.
    for (uint8_t i = 0; i < m_nPages; i++)
    {
        // The current "dirty" areas of the graphics [local] buffer.
        // Areas that haven't been sent to the screen/device but are
        // "dirty"
        //
        // Add the areas with pixels set and have been sent to the
        // device - this is the contents of m_pageErase
        pageCheckBoundsDesc(m_pageState[i], m_pageErase[i]);

        // if this page is clean, there is nothing to update
        if (pageIsClean(m_pageState[i]))
            continue;


        // clear out memory that is dirty on this page
        memset(m_pBuffer + i * m_viewport.width + m_pageState[i].xmin, 0,
               m_pageState[i].xmax - m_pageState[i].xmin + 1); // add one b/c values are 0 based

        // clear out any pending dirty range for this page - it's erased
        pageSetClean(m_pageState[i]);
    }

    // Indicate that the data transfer to the device should include the erase
    // region
    m_pendingErase = true;
}

////////////////////////////////////////////////////////////////////////////////////
//
// draw_pixel()
//
// Used to set a pixel in the graphics buffer - uses the current write operator
// function
//

void QwGrCH1120::drawPixel(uint8_t x, uint8_t y, uint8_t clr)
{
    // quick sanity check on range
    if (x >= m_viewport.width || y >= m_viewport.height)
        return; // out of bounds

    uint8_t bit = byte_bits[mod_byte(y)];

    m_rasterOps[m_rop](m_pBuffer + x + y / kByteNBits * m_viewport.width, // pixel offset
                       (clr ? bit : 0), bit);                             // which bit to set in byte
    
    // print Buffer after drawing pixel:
    pageCheckBounds(m_pageState[y / kByteNBits],
                x); // update dirty range for page
}

////////////////////////////////////////////////////////////////////////////////////
// draw_line_horz()
//
// Fast horizontal line drawing routine
//
void QwGrCH1120::drawLineHorz(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr)
{
    // Basically we set a bit within a range in a page of our graphics buffer.

    // in range
    if (y0 >= m_viewport.height)
        return;

    if (x0 > x1)
        swap_int(x0, x1);

    if (x1 >= m_viewport.width)
        x1 = m_viewport.width - 1;

    uint8_t bit = byte_bits[mod_byte(y0)];   // bit to set
    rasterOPsFn curROP = m_rasterOps[m_rop]; // current raster op

    // Get the start of this line in the graphics buffer
    uint8_t *pBuffer = m_pBuffer + x0 + y0 / kByteNBits * m_viewport.width;

    // walk up x and set the target pixel using the pixel operator function
    for (int i = x0; i <= x1; i++, pBuffer++)
        curROP(pBuffer, (clr ? bit : 0), bit);

    // Mark the page dirty for the range drawn
    pageCheckBoundsRange(m_pageState[y0 / kByteNBits], x0, x1);
}

////////////////////////////////////////////////////////////////////////////////////
// draw_line_vert()
//
// Fast vertical line drawing routine - also supports fast filled rects
//
void QwGrCH1120::drawLineVert(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clr)
{
    if (x0 >= m_viewport.width) // out of bounds
        return;

    // want an accending order
    if (y0 > y1)
        swap_int(y0, y1);

    // keep on screen
    if (y1 >= m_viewport.height)
        y1 = m_viewport.height - 1;

    uint8_t startBit, endBit, setBits;

    // Get the start and end pages we are writing to
    uint8_t page0 = y0 / kByteNBits;
    uint8_t page1 = y1 / kByteNBits;

    // loop over the pages. For each page determine the range of pixels
    // to set in the target page byte and then set them using the current
    // pixel operator function

    // Note: This function can also be used to draw filled rects - just iterate
    //       in the x direction. The base rect fill (in grBuffer) calls this
    //       method x1-x0 times, and each of those calls has some overhead. So
    //       just iterating over each page - x1-x0 times here - saves overhead
    //       costs.
    //
    //       To make this work, make sure x0 > x1. Also, this method is wired in
    //       as the draw_rect_filled entry in the draw interface. This is done
    //       above in the init process.

    int xinc;
    if (x0 > x1)
        swap_int(x0, x1);

    rasterOPsFn curROP = m_rasterOps[m_rop]; // current raster op

    for (int i = page0; i <= page1; i++)
    {
        startBit = mod_byte(y0); // start bit in this byte

        // last bit of this byte to set? Does the line end in this byte, or continue
        // on...
        endBit = y0 + kByteNBits - startBit > y1 ? mod_byte(y1) : kByteNBits - 1;

        // Set the bits from startBit to endBit
        setBits = (0xFF >> ((kByteNBits - endBit) - 1)) << startBit; // what bits are being set in this byte

        // set the bits in the graphics buffer using the current byte operator
        // function

        // Note - We iterate over x to fill in a rect if specified.
        for (xinc = x0; xinc <= x1; xinc++)
            curROP(m_pBuffer + i * m_viewport.width + xinc, (clr ? setBits : 0), setBits);

        y0 += endBit - startBit + 1; // increment Y0 to next page

        pageCheckBoundsRange(m_pageState[i], x0,
                        x1); // mark dirty range in page desc
    }
}

////////////////////////////////////////////////////////////////////////////////////////
// draw_rect_fill()
//
// Does the actual drawing/logic

void QwGrCH1120::drawRectFilled(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, uint8_t clr)
{
    uint8_t x1 = x0 + width - 1;
    uint8_t y1 = y0 + height - 1;

    // just call vert line
    drawLineVert(x0, y0, x1, y1, clr);
}

////////////////////////////////////////////////////////////////////////////////////
// draw_bitmap()
//
// Draw a 8 bit encoded (aka same y layout as this device) bitmap to the screen
//

void QwGrCH1120::drawBitmap(uint8_t x0, uint8_t y0, uint8_t dst_width, uint8_t dst_height, uint8_t *pBitmap,
                             uint8_t bmp_width, uint8_t bmp_height)
{
    // some simple checks
    if (x0 >= m_viewport.width || y0 >= m_viewport.height || !bmp_width || !bmp_height)
        return;

    // Bounds check
    if (x0 + dst_width > m_viewport.width) // out of bounds
        dst_width = m_viewport.width - x0;

    if (bmp_width < dst_width)
        dst_width = bmp_width;

    if (y0 + dst_height > m_viewport.height) // out of bounds
        dst_height = m_viewport.height - y0;

    if (bmp_height < dst_height)
        dst_height = bmp_height;

    // current position in the bitmap
    uint8_t bmp_x = 0;
    uint8_t bmp_y = 0;

    uint8_t page0, page1;
    uint8_t startBit, endBit, grSetBits, grStartBit;

    uint8_t bmp_mask[2], bmp_data, bmpPage;
    uint8_t remainingBits, neededBits;

    uint8_t y1 = y0 + dst_height - 1;

    page0 = y0 / kByteNBits;
    page1 = y1 / kByteNBits;

    rasterOPsFn curROP = m_rasterOps[m_rop]; // current raster op

    // The Plan:
    //   - Walk down the graphics buffer range (y) one page at a time
    //   - For each page
    //          - Determine needed number of bits for the destination
    //          - Determine what bits to pull from the bitmap
    //              - Create a mask to pull out bits - from one or two bytes
    //          - Loop over the x dimension
    //              - pull bits from bitmap, build byte of data of bitmap bits, in
    //                right order for the destination (graphics buffer)
    //              - Write the bitmap bits to the graphis buffer using the
    //              current operator

    // Loop over the memory pages in the graphics buffer
    for (int iPage = page0; iPage <= page1; iPage++)
    {
        // First, get the number of destination bits in the current page
        grStartBit = mod_byte(y0); // start bit

        // last bit of this byte to set? Does the copy region end in this byte, or
        // continue on...
        endBit = y0 + kByteNBits - grStartBit > y1 ? mod_byte(y1) : kByteNBits - 1;

        // Set the bits from startBit to endBit
        grSetBits = (0xFF >> (kByteNBits - endBit - 1)) << grStartBit; // what bits are being set in this byte

        // how many bits of data do we need to transfer from the bitmap?
        neededBits = endBit - grStartBit + 1;

        // Okay, we have how much data to transfer to the current page. Now build
        // the data from the bitmap.

        // First, build bit masks for pulling the data out of the bmp array. The
        // data might straddle two bytes, so build two masks

        // as above, get the start and end bites for the current position in the
        // bmp.
        startBit = mod_byte(bmp_y);
        endBit = (kByteNBits - startBit > neededBits ? startBit + neededBits : kByteNBits) - 1;

        // Set the bits from startBit to endBit
        bmp_mask[0] = (0xFF >> (kByteNBits - endBit - 1)) << startBit;

        // any remaining bits to get?
        remainingBits = neededBits - (endBit - startBit + 1); // +1 - needsBits is 1's based
        bmp_mask[1] = 0xFF >> (kByteNBits - remainingBits);

        // What row in the source bitmap
        bmpPage = bmp_y / kByteNBits;

        // we have the mask for the bmp - loop over the width of the copy region,
        // pulling out bmp data and writing it to the graphics buffer
        for (bmp_x = 0; bmp_x < dst_width; bmp_x++)
        {
            // get data bits out of current bitmap location and shift if needed
            bmp_data = (pBitmap[bmp_width * bmpPage + bmp_x] & bmp_mask[0]) >> startBit;

            if (remainingBits) // more data to add from the next byte in this column
                bmp_data |= (pBitmap[bmp_width * (bmpPage + 1) + bmp_x] & bmp_mask[1]) << (neededBits - remainingBits);

            // Write the bmp data to the graphics buffer - using current write op.
            // Note, if the location in the buffer didn't start at bit 0, we shift
            // bmp_data
            curROP(m_pBuffer + iPage * m_viewport.width + bmp_x + x0, bmp_data << grStartBit, grSetBits);
        }
        // move up our y values (graphics buffer and bitmap) by the number of bits
        // transferred
        y0 += neededBits;
        bmp_y += neededBits;
        
        pageCheckBoundsRange(m_pageState[iPage], x0,
                        x0 + dst_width - 1); // mark dirty range in page desc
    }
}

////////////////////////////////////////////////////////////////////////////////////
// Device Update Methods
////////////////////////////////////////////////////////////////////////////////////
// setScreenBufferAddress()
//
// Sets the target screen buffer address for graphics buffer transfer to the
// device.
//
//
// row can be 0 to 0x9F
// Column can be 0 to 0x9F

bool QwGrCH1120::setScreenBufferAddress(uint8_t row, uint8_t column)
{
    if (row >= m_viewport.height || column >= m_viewport.width)
        return false;

    // send the (row) address
    sendDevCommand(kCmdStartRow, row); // difference from the 1306, bytes sent after each other instead of OR'd together...

    // For the column start address, add the viewport x offset. Some devices
    // (Micro OLED) don't start at column 0 in the screen buffer
    sendDevCommand((kCmdStartColHigh | (column >> 4)) + m_viewport.x);
    sendDevCommand(kCmdStartColLow & column);

    return true;
}

bool QwGrCH1120::getPixel(uint8_t x, uint8_t y) {
    if (x >= m_viewport.width || y >= m_viewport.height)
        return 0;

    return (m_pBuffer[( y * (m_viewport.width / kByteNBits)) + (x / kByteNBits)] >> (x % kByteNBits)) & 0x01;
}

// Print the buffer as a 2D array of bits
void QwGrCH1120::printBuffer() {
    for (uint8_t y = 0; y < m_viewport.height; y++) {
        for (uint8_t x = 0; x < m_viewport.width; x++) {
            Serial.print(QwGrCH1120::getPixel(x, y) ? "1" : "0");
        }
        Serial.println();
    }
}

void QwGrCH1120::rawPrintBuffer() {
    for (uint8_t y = 0; y < m_viewport.height; y++) {
        for (uint8_t x = 0; x < m_viewport.width / kByteNBits; x++) {
            // Serial.print(m_pBuffer[x + y * (m_viewport.width / kByteNBits)], HEX);
            uint8_t val = m_pBuffer[x + y * (m_viewport.width / kByteNBits)];
            Serial.printf(" %02X", val);
        }
        Serial.println();
    }
}

////////////////////////////////////////////////////////////////////////////////////
// display()
//

// Send the "dirty" areas of the graphics buffer to the device's screen buffer.
// Only send the areas that need to be updated. The update region is based on
// new graphics to display, and any currently displayed items that need to be
// erased.

void QwGrCH1120::display()
{   
    // Loop over our page descriptors - if a page is dirty, send the graphics
    // buffer dirty region to the device for the current page
    pageState_t transferRange;

    for (int i = 0 ; i < m_nPages; i++) {
        // We keep the erase rect seperate from dirty rect. Make temp copy of
        // dirty rect page range, expand to include erase rect page range.

        transferRange = m_pageState[i];

        // If an erase has happend, we need to transfer/include erase update range
        if (m_pendingErase)
            pageCheckBoundsDesc(transferRange, m_pageErase[i]);

        if (pageIsClean(transferRange)) // both dirty and erase range for this
                                        // page were null
            continue;                   // next

        // set the start address to write the updated data to the devices screen
        // buffer

        // write out the xmin and xmax for each page descriptor
        // setScreenBufferAddress(i, transferRange.xmin + horz_flip_offset);
        setScreenBufferAddress(transferRange.xmin + horz_flip_offset, i);

        // send the dirty data to the device
        sendDevData(m_pBuffer + (i * m_viewport.width) + transferRange.xmin, // this page start + xmin
                    transferRange.xmax - transferRange.xmin + 1); // dirty region xmax - xmin. Add 1 b/c 0 based

        // If we sent the erase bounds, zero out the erase bounds - this area is now
        // clear
        if (m_pendingErase)
            pageSetClean(m_pageErase[i]);

        // add the just send dirty range (non erase rec)  to the erase rect
        pageCheckBoundsDesc(m_pageErase[i], m_pageState[i]);

        // this page is no longer dirty - mark it  clean
        pageSetClean(m_pageState[i]);
    }
    m_pendingErase = false; // no longer pending
}


////////////////////////////////////////////////////////////////////////////////////
// Device communication methods
////////////////////////////////////////////////////////////////////////////////////
// sendDeviceCommand()
//
// send a single command to the device via the current bus object

void QwGrCH1120::sendDevCommand(uint8_t command)
{
    m_i2cBus->writeRegisterByte(m_i2cAddress, kCmdControlByte, command);
}

////////////////////////////////////////////////////////////////////////////////////
// sendDeviceCommand()
//
// send a single command and value to the device via the current bus object.

void QwGrCH1120::sendDevCommand(uint8_t *commands, uint8_t n_commands)
{
    if (!commands || n_commands == 0)
        return;

    m_i2cBus->writeRegisterRegion(m_i2cAddress, kCmdControlByte, commands, n_commands);
}

////////////////////////////////////////////////////////////////////////////////////
// sendDeviceCommand()
//
// send a single command and value to the device via the current bus object.
//

void QwGrCH1120::sendDevCommand(uint8_t command, uint8_t value)
{
    uint8_t buffer[] = {command, value};

    sendDevCommand(buffer, 2);
}

////////////////////////////////////////////////////////////////////////////////////
// sendDeviceCommand()
//
// send a single command and value to the device via the current bus object.
//
void QwGrCH1120::sendDevCommand(uint8_t command, uint8_t start, uint8_t stop)
{
    uint8_t buffer[] = {command, start, stop};

    sendDevCommand(buffer, 3);
}

////////////////////////////////////////////////////////////////////////////////////
// sendDeviceData()
//
// send a block of data to the RAM of the device via the current bus object
void QwGrCH1120::sendDevData(uint8_t *pData, uint16_t nData)
{
    if (!pData || nData == 0){
        Serial.println("Invalid data");
        Serial.print("pData: ");
        Serial.println((uintptr_t)pData, HEX);
        Serial.print("nData: ");
        Serial.println(nData);
        return;
    }

    m_i2cBus->writeRegisterRegion(m_i2cAddress, kCmdRamControlByte, pData, nData);
}