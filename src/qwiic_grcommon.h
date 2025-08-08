// Common defines for multiple 
// TODO: Probably should rework how this looks eventually...
// possibly we will define another abstraction class and can also have these defines here...

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
