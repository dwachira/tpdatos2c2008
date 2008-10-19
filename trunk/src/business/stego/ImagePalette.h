#ifndef IMAGEPALETTE_H_
#define IMAGEPALETTE_H_

#include <iostream>
#include "FreeImage.h"
#include "RGBColor.h"
#include "Pixel.h"
#include "../../util/bits/BitsUtils.h"
#include <math.h>
#include <vector>

class ImagePalette
{
private:
    std::vector<unsigned int> new_palette_indexes;
	
public:
	ImagePalette();
	unsigned int getNewPaletteIndex(unsigned int index);
	void sortPaletteByDistance(FIBITMAP *imagen);
	void updateIndexes(FIBITMAP *imagen);
	void doIndexesLSB(FIBITMAP *imagen,Pixel& pixel,std::string mensaje);
	std::string getMessageFromIndexes(FIBITMAP *imagen,Pixel& pixel,unsigned int longitud);
	virtual ~ImagePalette();
};

#endif /*IMAGEPALETTE_H_*/
