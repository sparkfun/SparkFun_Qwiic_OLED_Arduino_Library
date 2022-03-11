




//
// Simple resource manager for font's and bitmaps.
//
#pragma once

#include <stdint.h>

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
	QwFont    			   font;
	uint16_t     	  	   id_resource;	
	struct _qwfont_desc  * next;
}QwFontDesc;

typedef struct _qwbitmap{
	uint8_t	  	      width;
	uint8_t           height;
	const uint8_t * data;
}QwBitmap;

typedef struct _qwbitmap_desc{
	QwBitmap       		    bitmap;
	uint16_t     	        id_resource;	
	struct _qwbitmap_desc * next;
}QwBitmapDesc;


////////////////////////////////////////////////////////////////////////////////
// A simple resource manager for fonts and bitmaps. Makes managing these things
// easlier that just including random byte arrays.
//
//  Making a singleton - this has no state. 
//
class QwResourceMan_ {

public:
	// Singleton things
    static QwResourceMan_& getInstance(void){

        static QwResourceMan_ instance;
        return instance;
    }

    //---------------------------------------------------------
    bool add_font(QwFontDesc* theFont){

    	if(!theFont)
    		return false;

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
    QwFont * get_font(uint16_t idFont){

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

    	if(!theBitmap)
    		return false;

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
    QwBitmap * get_bitmap(uint16_t idBitmap){

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
    QwResourceMan_(QwResourceMan_ const&) = delete;
    void operator=(QwResourceMan_ const&) = delete;


private:
	QwResourceMan_(): _pFonts{nullptr}, _pBitmaps{nullptr}, _n_fonts{0}, _n_bmp{0}{}

	QwFontDesc     * _pFonts;
	QwBitmapDesc   * _pBitmaps;

	uint8_t      _n_fonts;
	uint8_t      _n_bmp;

};

typedef QwResourceMan_& QwResourceMan;

// Accessor for the signleton
#define QwResourceMan()  QwResourceMan_::getInstance()