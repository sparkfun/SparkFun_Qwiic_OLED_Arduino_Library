

#pragma once


#include "qwiic_resdef.h"

class QwFont8x16 final : public fontSingleton<QwFont8x16> {

public:
	const uint8_t * data(void){

		// include font data (static const), and attribute defines.
		// Doing this here makes the data variable a static (aka only one instance ever)
		// variable in this method. 
#include "_fnt_8x16.h"

		return font8x16_data;
	}

	QwFont8x16(): fontSingleton<QwFont8x16>(FONT_8X16_WIDTH, 
											FONT_8X16_HEIGHT,
											FONT_8X16_START, 
											FONT_8X16_NCHAR, 
											FONT_8X16_MAP_WIDTH){}

};

#define QW_FONT_8X16 QwFont8x16::instance()