#ifndef FIBITMAP_H_
#define FIBITMAP_H_

#include "FreeImage.h"
#include "../../util/bits/BitsUtils.h"
#include <string>

class FIBitmap
{
private:
    
    FIBitmap(const FIBitmap&);
    FIBitmap & operator=(const FIBitmap&);
    /*Representacion de la imagen*/
    FIBITMAP *imagen;
    std::string filename;
    FREE_IMAGE_FORMAT format;
    bool error;
    unsigned int bpp,pitch,height,width,color_type,palette_size;
public:
	FIBitmap();
	FIBitmap(std::string filename,FREE_IMAGE_FORMAT format);
	unsigned int getBpp(){ return bpp;};
	unsigned int getPitch(){ return pitch;};
	unsigned int getHeight(){ return height;};
	unsigned int getWidth(){ return width;};
	unsigned int getColorType(){ return color_type;};
	unsigned int getPaletteSize(){ return palette_size;};
	RGBQUAD* getPalette();
	BYTE* getBits();
	BYTE getPixelIndex(unsigned int x,unsigned int y);
	void setPixelIndex(unsigned int x,unsigned int y,BYTE* new_pixel_index);
	RGBQUAD* getBackgroundColor();       	 
	bool hasBackgroundColor();
    BYTE* getTransparencyTable();
	int getTransparencyCount();
	bool isTransparent();
	int getTransparentIndex();	
	
	bool load();
	void save();
	virtual ~FIBitmap();
};

#endif /*FIBITMAP_H_*/
