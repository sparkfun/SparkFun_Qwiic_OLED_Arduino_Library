




//
// Simple resource manager for font's and bitmaps.
//
#pragma once

#include <stdint.h>


// Resource ID Masks
#define FONT_ID_MASK   0x8000U
#define BITMAP_ID_MASK 0xA000U

#define isa_font(_id_)  ( _id_ & FONT_ID_MASK ? true : false)
#define isa_bitmap(_id_)  ( _id_ & BITMAP_ID_MASK ? true : false)
#define MAKE_FONT_ID(_id_) (_id_ | FONT_ID_MASK)
#define MAKE_BITMAP_ID(_id_) (_id_ | BITMAP_ID_MASK)

typedef uint16_t gwResourceID_t;

// Define a structure to hold a font definition

typedef struct _qwfont{
	uint8_t	  	      font_width;
	uint8_t           font_height;
	uint8_t           start_char;
	uint8_t     	  n_chars;
	uint16_t    	  map_width;
	const uint8_t   * data;
}QwFont;

typedef struct _qwfont_desc{
	QwFont    			    font;
	gwResourceID_t   	  	id_resource;	
	struct _qwfont_desc   * next;
}QwFontDesc;


////////////////////////////////////////////////////////////////////////////////
// A simple resource manager for fonts and bitmaps. Makes managing these things
// easlier that just including random byte arrays.
//
//  Making a singleton - this has no state. 
//
class QwResourceMngr_ {

public:
	// Singleton things
    static QwResourceMngr_& getInstance(void){

        static QwResourceMngr_ instance;
        return instance;
    }

    //---------------------------------------------------------
    bool add_font(QwFontDesc* theFont){

    	if(!theFont || !isa_font(theFont->id_resource))
    		return false;

		// already registered?
    	if(get_font(theFont->id_resource))
    		return true;

    	theFont->next = nullptr;

    	if(!_pFonts)
    		_pFonts = theFont;
    	else {
	    	QwFontDesc *pLast = _pFonts;    		
	    	while(pLast->next != nullptr)
	    		pLast = pLast->next;

	    	pLast->next = theFont;
    	}
    	_n_fonts++;

    	return true;
    }
    //---------------------------------------------------------
    QwFont * get_font(gwResourceID_t idFont){

    	if(!isa_font(idFont))
    		return nullptr;

    	QwFontDesc *pCurrent = _pFonts;
    	while(pCurrent != nullptr){

    		if(pCurrent->id_resource == idFont)
    			return &pCurrent->font;
    		pCurrent = pCurrent->next;
    	}

    	return nullptr;
    }
    //---------------------------------------------------------
    uint8_t n_fonts(void){ return _n_fonts;}

    //---------------------------------------------------------    
 	// copy and assign constructors - delete them to prevent extra copys being 	
    // made -- this is a singleton object.
    QwResourceMngr_(QwResourceMngr_ const&) = delete;
    void operator=(QwResourceMngr_ const&) = delete;


private:
	QwResourceMngr_(): _pFonts{nullptr}, _n_fonts{0}{}

	QwFontDesc     * _pFonts;

	uint8_t      _n_fonts;
};

typedef QwResourceMngr_& QwResourceMngr;

// Accessor for the signleton
#define QwResourceMngr()  QwResourceMngr_::getInstance()
#define RESMANAGER QwResourceMngr_::getInstance()

// Define a macro that makes registration of resources easy

#define QwResource_AddFont(_id_, _width_, _height_, _start_, _num_, _map_, _data_) \
static QwFontDesc fnt_##_id_##_ = { { _width_, _height_, _start_, _num_, _map_, _data_}, _id_, 0}; \
static bool rc_##_id_##_ = RESMANAGER.add_font(&fnt_##_id_##_);


////////////////////////////////////////////////////////////////////////
// Experimental - Template singletons for managing resources
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
    using QwBitmap::QwBitmap;
};		

class _QwFont{

public:
	uint8_t	  	      width;
	uint8_t           height;
	uint8_t           start_char;
	uint8_t     	  n_chars;
	uint16_t    	  map_width;

	virtual const uint8_t * data(void){return nullptr;};

protected:
	_QwFont(uint8_t w, uint8_t h, uint8_t st_chr, uint8_t n_chr, uint16_t m_w):
			width{w}, height{h}, start_char{st_chr}, n_chars{n_chr}, map_width{m_w}{}
};
// Template that creates a singleton for bitmaps.
template<typename T>
class fontSingleton : public _QwFont {
public:
    static T& instance(void){
        static T instance;
        return instance;
    }

    fontSingleton(const fontSingleton&) = delete;
    fontSingleton& operator= (const fontSingleton) = delete;

protected:
    fontSingleton() {}
    using _QwFont::_QwFont;
};		
