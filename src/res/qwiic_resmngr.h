




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

typedef struct _qwbitmap{
	uint8_t	  	      width;
	uint8_t           height;
	const uint8_t * data;
}QwBitmap;

typedef struct _qwbitmap_desc{
	QwBitmap       		    bitmap;
	gwResourceID_t     	    id_resource;	
	struct _qwbitmap_desc * next;
}QwBitmapDesc;


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
	bool add_bitmap(QwBitmapDesc* theBitmap){

    	if(!theBitmap || !isa_bitmap(theBitmap->id_resource))
    		return false;

    	// already registered?
    	if(get_bitmap(theBitmap->id_resource))
    		return true;

    	theBitmap->next = nullptr;

    	if(!_pBitmaps)
    		_pBitmaps = theBitmap;
    	else {
	    	QwBitmapDesc *pLast = _pBitmaps;    		
	    	while(pLast->next != nullptr)
	    		pLast = pLast->next;

	    	pLast->next = theBitmap;
    	}
    	_n_bmp++;
    	return true;
    }
    //---------------------------------------------------------
    QwBitmap * get_bitmap(gwResourceID_t idBitmap){

    	if(!isa_bitmap(idBitmap))
    		return nullptr;

    	QwBitmapDesc *pCurrent = _pBitmaps;

    	while(pCurrent != nullptr){

    		if(pCurrent->id_resource == idBitmap)
    			return &pCurrent->bitmap;
    		pCurrent = pCurrent->next;
    	}

    	return nullptr;
    }
    //---------------------------------------------------------    
    uint8_t n_bitmaps(){return _n_bmp;}

    //---------------------------------------------------------    
 	// copy and assign constructors - delete them to prevent extra copys being 	
    // made -- this is a singleton object.
    QwResourceMngr_(QwResourceMngr_ const&) = delete;
    void operator=(QwResourceMngr_ const&) = delete;


private:
	QwResourceMngr_(): _pFonts{nullptr}, _pBitmaps{nullptr}, _n_fonts{0}, _n_bmp{0}{}

	QwFontDesc     * _pFonts;
	QwBitmapDesc   * _pBitmaps;

	uint8_t      _n_fonts;
	uint8_t      _n_bmp;

};

typedef QwResourceMngr_& QwResourceMngr;

// Accessor for the signleton
#define QwResourceMngr()  QwResourceMngr_::getInstance()

// Define a macro that makes registration of resources easy

#define QwResource_AddFont(_id_, _width_, _height_, _start_, _num_, _map_, _data_) \
		static QwFontDesc fnt_##_id_##_ = { { _width_, _height_, _start_, _num_, _map_, _data_}, _id_, 0}; \
		static bool rc_##_id_##_ = QwResourceMngr().add_font(&fnt_##_id_##_);

#define QwResource_AddBitmap(_id_, _name_, _width_, _height_, _data_) \
		static QwBitmapDesc bmp_##_id_##_desc = { { _width_, _height_,  _data_}, _id_, 0}; \
		static QwBitmap * qwbmp_##_name_ = &bmp_##_id_##_desc.bitmap; \
		static bool rc_##_id_##_ = QwResourceMngr().add_bitmap(&bmp_##_id_##_desc);

