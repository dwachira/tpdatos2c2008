#ifndef FIBITMAP_H_
#define FIBITMAP_H_

#include "FreeImage.h"
#include "../../util/bits/BitsUtils.h"

#include <string>
#include <fstream>

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
		FIBitmap(std::string filename);
		std::string getFilename(){ return filename;}
		unsigned int getBpp(){ return bpp;};
		unsigned int getPitch(){ return pitch;};
		unsigned int getHeight(){ return height;};
		unsigned int getWidth(){ return width;};
		unsigned int getColorType(){ return color_type;};
		unsigned int getPaletteSize(){ return palette_size;};
		int getFileFormat(){ return format;};
		bool getFileType();
		RGBQUAD* getPalette();
		BYTE* getBits();
		BYTE getPixelIndex(unsigned int x,unsigned int y);
		void setPixelIndex(unsigned int x,unsigned int y,BYTE* new_pixel_index);
		RGBQUAD getBackgroundColor(); 
		void setBackgroundColor(RGBQUAD background);    	 
		bool hasBackgroundColor();
    	BYTE* getTransparencyTable();
		int getTransparencyCount();
		bool isTransparent();
		int getTransparentIndex();	
		void setTransparentIndex(int index);
		void setBackgroundColorIndex(int new_index);
		int applyPaletteIndexMapping(BYTE*srcindices, BYTE *dstindices, unsigned count);
		int applyColorMapping(RGBQUAD *srccolors, RGBQUAD *dstcolors);
		void applyColorMapping(RGBQUAD *srccolors);
		int getPaletteOffset();
	
		bool load(int flag=0);
		void save(int perc=0);
		virtual ~FIBitmap();
};

#endif /*FIBITMAP_H_*/
