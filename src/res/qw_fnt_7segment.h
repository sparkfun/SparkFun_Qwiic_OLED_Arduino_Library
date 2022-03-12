

#pragma once


#include "qwiic_resdef.h"

class QwFont7Segment final : public fontSingleton<QwFont7Segment> {

public:
	const uint8_t * data(void){

		// include font data (static const), and attribute defines.
		// Doing this here makes the data variable a static (aka only one instance ever)
		// variable in this method. 
#include "_fnt_7segment.h"

		return segment7_data;
	}

	QwFont7Segment(): fontSingleton<QwFont7Segment>(FONT_7SEG_WIDTH, 
											  		FONT_7SEG_HEIGHT,
											  		FONT_7SEG_START, 
											  		FONT_7SEG_NCHAR, 
											  		FONT_7SEG_MAP_WIDTH){}

};

#define QW_FONT_7SEGMENT QwFont7Segment::instance()