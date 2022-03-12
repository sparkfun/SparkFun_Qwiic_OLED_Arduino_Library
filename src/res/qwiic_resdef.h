




//
// Simple resource manager for font's and bitmaps.
//
#pragma once

#include <stdint.h>

////////////////////////////////////////////////////////////////////////
// Notes  Template singletons for managing resources
////////////////////////////////////////////////////////////////////////		
//
// The common use/storage of bitmap and font data is creating a static 
// array and placing it in a header file. 
//
// This pattern is fine for simple uses, where the bitmap is only included
// in a single file. BUT if the bitmap header is included in multiple files,
// multiple copies of the bitmap are created.
//
// You could just make the bitmap array a const - but then multiple includes
// will lead to symbol collision at link time - confusing the user.
//
// These simple classes help place the bitmap data in a singleton class AND
// only create on copy/instance of the bitmap data no matter how many times a
// resource header file is included.
//
// The Plan:
//     	- Define a base resource class
//			- attributes as public instance vars
//			- Define a constructor that init's the instance vars
//			- Resource data access via virtual accessor - make it const
//			
//		- Create a template that subclasses the resource class, and defines a singleton 
//			- It ensures only once of these classes is ever created.
//
//		- In a seperate header file - one for each resource, define the resource
//			- For example - for a bitmap - the data array, #defines for width and height
//			- The data array is declard as const static
//
//		- In another header file - create a subclass of our singleton 
//			ex:
//            class QwBMPTruck final : public bitmapSingleton<QwBMPTruck> {
//
//			- Override the base class data accessor method
//			- In the body of this method, include the resource defined header file
//				- This defines the resource data as a static in this method. So only one copy is made
//					ex:
//						const uint8_t * bitmap(void){
//					#include "bmp_truck.h"
//							return bmp_truck_data;
//					 	}
//
//			- After this resource access method, define the constructor of this class
//				- This is done after so it can use the attribute #defines of the included resource def header
//				- Have the constructor call it's superclass constructor, passing in resource attributes
//					ex:
//						QwBMPTruck(): bitmapSingleton<QwBMPTruck>(BMP_TRUCK_WIDTH, BMP_TRUCK_HEIGHT){}
//
//			- Lastly, define a macro that makes the syntax of access easy. This macro calls the instance
//			   	method of the singleton, getting access to the object that contains the data of the resource
//				ex:
//					#define QW_BMP_TRUCK QwBMPTruck::instance()
//				
//				To the user, they just have a bitmap, referenced as QW_BMP_TRUCK and can do the following
//					ex:
//						uint8_t width = QW_BMP_TRUCK.width;
//						uint8_t height = QW_BMP_TRUCK.height;
//
//				And internal methods get the data of this bitmap object using the bitmap() method
//					ex:
//						const uint8_t * pData = QW_BMP_TRUCK.bitmap();
//

///////////////////////////////////////////////////////////////////////////////////////////////////
// Simple Bitmap class definition

class QwBitmap{

public:
	uint8_t	  	      width;
	uint8_t           height;
    virtual const uint8_t * data(void){return nullptr;};	

protected:
	QwBitmap(uint8_t w, uint8_t h): width{w}, height{h}{}
};

// Template that creates a singleton for bitmaps.
template<typename T>
class bmpSingleton : public QwBitmap {
public:
    static T& instance(void){
        static T instance;
        return instance;
    }

    bmpSingleton(const bmpSingleton&) = delete;
    bmpSingleton& operator= (const bmpSingleton) = delete;

protected:
    bmpSingleton() {}
    using QwBitmap::QwBitmap; // inherit contructor
};		

///////////////////////////////////////////////////////////////////////////////////////////////////
// Font things - class to hold font attributes

class QwFont{

public:
	uint8_t	  	      width;
	uint8_t           height;
	uint8_t           start;
	uint8_t     	  n_chars;
	uint16_t    	  map_width;

	virtual const uint8_t * data(void){return nullptr;};

protected:
	QwFont(uint8_t w, uint8_t h, uint8_t st_chr, uint8_t n_chr, uint16_t m_w):
			width{w}, height{h}, start{st_chr}, n_chars{n_chr}, map_width{m_w}{}
};


// Template that creates a singleton for bitmaps.
template<typename T>
class fontSingleton : public QwFont {
public:
    static T& instance(void){
        static T instance;
        return instance;
    }

    fontSingleton(const fontSingleton&) = delete;
    fontSingleton& operator= (const fontSingleton) = delete;

protected:
    fontSingleton() {}
    using QwFont::QwFont; // inherit constructor
};		
