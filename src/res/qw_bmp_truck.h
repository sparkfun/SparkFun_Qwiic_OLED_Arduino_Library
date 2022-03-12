

#pragma once


#include "qwiic_resmngr.h"

class QwBMPTruck final : public bitmapSingleton<QwBMPTruck> {

public:
	const uint8_t * bitmap(void){

#include "bmp_truck.h"

		return bmp_truck_data;
	}

	QwBMPTruck(): bitmapSingleton<QwBMPTruck>(BMP_TRUCK_WIDTH, BMP_TRUCK_HEIGHT){}

};

#define QW_BMP_TRUCK QwBMPTruck::instance()