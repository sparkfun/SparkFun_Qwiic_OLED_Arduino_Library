// Common defines for multiple 
// TODO: Probably should rework how this looks eventually...

/////////////////////////////////////////////////////////////////////////////
// The graphics Raster Operator functions (ROPS)
/////////////////////////////////////////////////////////////////////////////
//      - Copy      - copy the pixel value in to the buffer (default)
//      - Not Copy  - copy the not of the pixel value to buffer
//      - Not       - Set the buffer value to not it's current value
//      - XOR       - XOR of color and current pixel value
//      - Black     - Set value to always be black
//      - White     - set value to always be white

#pragma once

#include <stdint.h>

typedef enum gr_op_funcs_
{
    grROPCopy = 0,
    grROPNotCopy = 1,
    grROPNot = 2,
    grROPXOR = 3,
    grROPBlack = 4,
    grROPWhite = 5
} grRasterOp_t;

typedef struct
{
    int16_t xmin;
    int16_t xmax;
} pageState_t;

// Scrolling Macros: Note these ultimately get exposed to the user and used in examples etc. for scrolling
// These are the defines for the SSD1306, but we'll also treat them as the 
// common defines for other drivers such that that the user can pass these in
#define SCROLL_VERTICAL 0x01
#define SCROLL_RIGHT 0x02
#define SCROLL_LEFT 0x04

// SCROLL_VERT_RIGHT on SSDS1306, SCROLL_UP on CH1120
#define SCROLL_VERT_MODE1 (SCROLL_VERTICAL | SCROLL_RIGHT)
//SCROLL_VERT_LEFT on SSDS1306, SCROLL_DOWN on CH1120
#define SCROLL_VERT_MODE2 (SCROLL_VERTICAL | SCROLL_LEFT)

// More readable aliases for the vertical scrolling modes above
// For the CH1120 (1.5 inch display)
#define SCROLL_UP SCROLL_VERT_MODE1
#define SCROLL_DOWN SCROLL_VERT_MODE2

// For the SSD1306 (all other displays)
#define SCROLL_VERT_RIGHT SCROLL_VERT_MODE1
#define SCROLL_VERT_LEFT SCROLL_VERT_MODE2

// Reminder these are only valid for the SSD1306, other drivers should observe if these are passed 
// in and map them to the correct constants to actually send to their displays
#define SCROLL_INTERVAL_5_FRAMES 0x00
#define SCROLL_INTERVAL_64_FRAMES 0x01
#define SCROLL_INTERVAL_128_FRAMES 0x02
#define SCROLL_INTERVAL_256_FRAMES 0x03 // NOTE: NOT AVAILABLE FOR CH1120
#define SCROLL_INTERVAL_3_FRAMES 0x04
#define SCROLL_INTERVAL_4_FRAMES 0x05
#define SCROLL_INTERVAL_25_FRAMES 0x06 // NOTE: NOT AVAILABLE FOR CH1120
#define SCROLL_INTERVAL_2_FRAMES 0x07

// These are valid in the CH1120 driver and not present in the SSD1306 driver
#define SCROLL_INTERVAL_6_FRAMES 0x00
#define SCROLL_INTERVAL_32_FRAMES 0x01