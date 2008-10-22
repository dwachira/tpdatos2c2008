#ifndef IMAGEPALETTE_H_
#define IMAGEPALETTE_H_

#define MAX_DISTANCE 445

#include <iostream>
#include "RGBColor.h"
#include "Pixel.h"
#include "FIBitmap.h"

#include <math.h>
#include <vector>

class ImagePalette
{
private:
    /*Representacion de la imagen*/
    FIBitmap &imagen;
    std::vector<unsigned int> new_palette_indexes;
	int doLSB(int byte,char value);
	char getLSB(int byte);
public:
	ImagePalette();
	ImagePalette(FIBitmap &imagen);
	
	unsigned int getNewPaletteIndex(unsigned int index);
	void sortPaletteByDistance();
	void updateIndexes();
	void doIndexesLSB(Pixel& pixel,std::string mensaje);
	void doPaletteLSB(unsigned int first_bit,std::string mensaje);
	std::string getMessageFromIndexes(Pixel& pixel,unsigned int longitud);
	std::string getMessageFromPalette(unsigned int first,unsigned int longitud);
	
	virtual ~ImagePalette();
};

#endif /*IMAGEPALETTE_H_*/
