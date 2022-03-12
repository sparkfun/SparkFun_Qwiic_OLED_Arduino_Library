

#pragma once


#include "qwiic_resdef.h"

class QwFont5x7 final : public fontSingleton<QwFont5x7> {

public:
	const uint8_t * data(void){

		// include font data (static const), and attribute defines.
		// Doing this here makes the data variable a static (aka only one instance ever)
		// variable in this method. 
#include "_fnt_5x7.h"

		return font5x7_data;
	}

	QwFont5x7(): fontSingleton<QwFont5x7>(FONT_5X7_WIDTH, 
										  FONT_5X7_HEIGHT,
										  FONT_5X7_START, 
										  FONT_5X7_NCHAR, 
										  FONT_5X7_MAP_WIDTH){}

};

#define QW_FONT_5X7 QwFont5x7::instance()