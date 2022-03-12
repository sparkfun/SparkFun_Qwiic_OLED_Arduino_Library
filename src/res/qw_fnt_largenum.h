

#pragma once


#include "qwiic_resdef.h"

class QwFontLargeNum final : public fontSingleton<QwFontLargeNum> {

public:
	const uint8_t * data(void){

		// include font data (static const), and attribute defines.
		// Doing this here makes the data variable a static (aka only one instance ever)
		// variable in this method. 
#include "_fnt_largenum.h"

		return fontlargenum_data;
	}

	QwFontLargeNum(): fontSingleton<QwFontLargeNum>(FONT_LARGENUM_WIDTH, 
											  		FONT_LARGENUM_HEIGHT,
											  		FONT_LARGENUM_START, 
											  		FONT_LARGENUM_NCHAR, 
											  		FONT_LARGENUM_MAP_WIDTH){}

};

#define QW_FONT_LARGENUM QwFontLargeNum::instance()