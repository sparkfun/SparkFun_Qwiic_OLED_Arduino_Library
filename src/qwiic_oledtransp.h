//
//  TODO : HEADER
// 

#pragma once


#include "qwiic_grssd1306.h"

//////////////////////////////////////////////////////////////////
// Set the defaults for the SparkFun Qwiic MicroOLED

#define kOLEDTransWidth  	128
#define kOLEDTransHeight 	64

// The viewport x is off by 2 on this device - starts at value 2
#define kOLEDTransXOffset    0   
#define kOLEDTransYOffset    0   

// Parameters for this device
#define kOLEDTransPinConfig  0x12
#define kOLEDTransPreCharge  0x25
#define kOLEDTransVCOM	     0x40

#define kOLEDTransDefaultAddress 0x3C

class QwOLEDTransparent : public QwGrSSD1306 {


public:

	// Constructor - setup the viewport and default address for this device. 
	QwOLEDTransparent() : 
		QwGrSSD1306(kOLEDTransXOffset, kOLEDTransYOffset, kOLEDTransWidth, kOLEDTransHeight)
		{
			default_address =kOLEDTransDefaultAddress;
		};

	// set up the specific device settings
	bool init(void){

		set_buffer(_graphicsBuffer); // The buffer to use 

		set_comm_pins(kOLEDTransPinConfig);
		set_pre_charge(kOLEDTransPreCharge);
		set_vcom_deselect(kOLEDTransVCOM);

		// Call the super class to do all the work
		return this->QwGrSSD1306::init();
	};

private:
	// Graphics buffer for this device. 
	uint8_t _graphicsBuffer[kOLEDTransWidth * kOLEDTransHeight/8];

};