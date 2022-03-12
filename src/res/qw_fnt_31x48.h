

#pragma once


#include "qwiic_resmngr.h"

class QwFont31x48 final : public fontSingleton<QwFont31x48> {

public:
	const uint8_t * data(void){

#include "fnt_31x48.h"

		return font31x48_data;
	}

	QwFont31x48(): fontSingleton<QwFont31x48>(FONT_31X48_WIDTH, FONT_31X48_HEIGHT,
				FONT_31X48_START, FONT_31X48_NCHAR, FONT_31X48_MAP_WIDTH){}

};

#define QW_FONT_31X48 QwFont31x48::instance()