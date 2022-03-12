

#pragma once


#include "qwiic_resdef.h"

class QwBMPSparkFun final : public bmpSingleton<QwBMPSparkFun> {

public:
	const uint8_t * data(void){

#include "_bmp_sparkfun.h"

		return bmp_sparkfun_data;
	}

	QwBMPSparkFun(): bmpSingleton<QwBMPSparkFun>(BMP_SPARKFUN_WIDTH, BMP_SPARKFUN_HEIGHT){}

};

#define QW_BMP_SPARKFUN QwBMPSparkFun::instance()