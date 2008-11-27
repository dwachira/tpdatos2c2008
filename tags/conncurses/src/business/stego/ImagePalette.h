#ifndef IMAGEPALETTE_H_
#define IMAGEPALETTE_H_

#define MAX_DISTANCE 445

#include "Pixel.h"
#include "FIBitmap.h"

#include <vector>

/**
 * ImagePalette.h
 * Tratamiento de la paleta de colores para imagenes gif e imagenes con bpp<= 8
 * Permite modificar los indices, ordenar la paleta de colores por proximidad
 */
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

		unsigned int getNewPaletteIndex(unsigned int index)const;
		void sortPaletteByDistance();

		/*Metodos sobre los indices: */
		unsigned int doIndexesLSB(Pixel& pixel,const char* mensaje,unsigned long int size);
		std::string getMessageFromIndexes(Pixel& pixel,unsigned long int longitud);

		/*Metodos sobre la paleta de colores: */
		unsigned int doPaletteLSB(unsigned int first_bit,const char* mensaje,unsigned long int size);
		std::string getMessageFromPalette(unsigned int first,unsigned long int longitud);

		virtual ~ImagePalette();

};

#endif /*IMAGEPALETTE_H_*/
