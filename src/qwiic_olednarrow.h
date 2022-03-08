
//
//  TODO : HEADER
// 

#pragma once


#include "qwiic_grssd1306.h"



// Set the defaults for the SparkFun Qwiic MicroOLED

#define kOLEDNarrowWidth  	128
#define kOLEDNarrowHeight 	32


// Parameters for this device
#define kOLEDNarrowPinConfig  0x02
#define kOLEDNarrowPreCharge  0xF1
#define kOLEDNarrowVCOM	     0x40

#define kOLEDNarrowDefaultAddress 0x3C

class QwOLEDNarrow : public QwGrSSD1306 {

public:

	// Constructor - setup the viewport for this device. 
	QwOLEDNarrow() : 
		QwGrSSD1306(kOLEDNarrowWidth, kOLEDNarrowHeight)
		{
			default_address = kOLEDNarrowDefaultAddress;
		};

	// set up the specific values 
	bool init(void){

		set_buffer(_graphicsBuffer);

		set_comm_pins(kOLEDNarrowPinConfig);
		set_pre_charge(kOLEDNarrowPreCharge);
		set_vcom_deselect(kOLEDNarrowVCOM);

		// Call the super class to do all the work
		return this->QwGrSSD1306::init();
	};

private:
	// Graphics buffer for this device. 
	uint8_t _graphicsBuffer[kOLEDNarrowWidth * kOLEDNarrowHeight/8];

};