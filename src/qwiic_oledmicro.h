//
//  TODO : HEADER
// 

#pragma once


#include "qwiic_grssd1306.h"



// Set the defaults for the SparkFun Qwiic MicroOLED

#define kOLEDMicroWidth  	64
#define kOLEDMicroHeight 	48

// The viewport x is off by 2 on this device - starts at value 2
#define kOLEDMicroXOffset    2   
#define kOLEDMicroYOffset    0   


// Parameters for this device
#define kOLEDMicroPinConfig  0x12
#define kOLEDMicroPreCharge  0xF1
#define kOLEDMicroVCOM	     0x40

#define kOLEDMicroDefaultAddress 0x3D
#define kOLEDMicroAltAddress 0x3C

class QwOLEDMicro : public QwGrSSD1306 {


public:

	// Constructor - setup the viewport for this device. 
	QwOLEDMicro() : 
		QwGrSSD1306(kOLEDMicroXOffset, kOLEDMicroYOffset, kOLEDMicroWidth, kOLEDMicroHeight)
		{
			default_address =kOLEDMicroDefaultAddress;
		};


	// set up the specific values 
	bool init(void){

		set_buffer(_graphicsBuffer);

		set_comm_pins(kOLEDMicroPinConfig);
		set_pre_charge(kOLEDMicroPreCharge);
		set_vcom_deselect(kOLEDMicroVCOM);

		// Call the super class to do all the work
		return this->QwGrSSD1306::init();
	};

private:
	// Graphics buffer for this device. 
	uint8_t _graphicsBuffer[kOLEDMicroWidth * kOLEDMicroHeight/8];

};