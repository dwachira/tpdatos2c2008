#ifndef FIBITMAP_H_
#define FIBITMAP_H_

#include "FreeImage.h"
#include "../../util/bits/BitsUtils.h"

#include <fstream>
#include <math.h>

/**
 * FIBitmap.h
 * Encapsulamiento de los metodos de FreeImage y de todas las operaciones
 * que requieran del acceso a la imagen
 */
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
    	/*Datos extraidos de la imagen*/
    	unsigned int bpp,pitch,height,width,color_type,palette_size,palette_offset;

	public:

		FIBitmap();
		FIBitmap(std::string filename);
		std::string getFilename()const{ return filename;}
		unsigned int getBpp()const{ return bpp;};
		unsigned int getPitch()const{ return pitch;};
		unsigned int getHeight()const{ return height;};
		unsigned int getWidth()const{ return width;};
		unsigned int getColorType()const{ return color_type;};
		unsigned int getPaletteSize()const{ return palette_size;};
		int getFileFormat()const{ return format;};
		bool getFileType();
		RGBQUAD* getPalette()const;
		BYTE* getBits();
		BYTE getPixelIndex(unsigned int x,unsigned int y)const;
		void setPixelIndex(unsigned int x,unsigned int y,BYTE* new_pixel_index);
		RGBQUAD getBackgroundColor()const;
		void setBackgroundColor(RGBQUAD background);
		bool hasBackgroundColor()const;
    	BYTE* getTransparencyTable();
		int getTransparencyCount()const;
		bool isTransparent()const;
		int getTransparentIndex()const;
		void setTransparentIndex(int index);
		void setBackgroundColorIndex(int new_index);
		/*Mapeo entre los indices de la imagen*/
		int applyPaletteIndexMapping(BYTE*srcindices, BYTE *dstindices);
		/*Mapeo entre los los indices de la paleta de colores*/
		int applyColorMapping(RGBQUAD *srccolors, RGBQUAD *dstcolors);
		void applyColorMapping(RGBQUAD *srccolors,unsigned int count,unsigned int from=0);
		int getPaletteOffset();
		/*Determina si la imagen es en escala de grises*/
		bool isGrayScale()const;
		/*Determina si el gif es animado*/
		bool isAnimated()const;

		bool load(int flag=0);
		void save(int perc=0);
		virtual ~FIBitmap();
};

#endif /*FIBITMAP_H_*/
