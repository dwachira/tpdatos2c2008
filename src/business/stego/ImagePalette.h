#ifndef IMAGEPALETTE_H_
#define IMAGEPALETTE_H_

#define MAX_DISTANCE 445

#include <iostream>
#include "Pixel.h"
#include "FIBitmap.h"

#include <vector>

class ImagePalette
{
	private:

    	/*Representacion de la imagen*/
   		FIBitmap &imagen;
   	  	std::vector<unsigned int> new_palette_indexes;
		unsigned int getRGBPos(unsigned int pos);

	public:

		ImagePalette();
		ImagePalette(FIBitmap &imagen);

		unsigned int getNewPaletteIndex(unsigned int index);
		void sortPaletteByDistance();
		unsigned int doIndexesLSB(Pixel& pixel,const char* mensaje, long int size);
		unsigned int doPaletteLSB(unsigned int first_bit,const char* mensaje, long int size);
		std::string getMessageFromIndexes(Pixel& pixel,unsigned int longitud);
		std::string getMessageFromPalette(unsigned int first,unsigned int longitud);

		virtual ~ImagePalette();

};

#endif /*IMAGEPALETTE_H_*/
