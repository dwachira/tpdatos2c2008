#ifndef LSBSTEGOBUSINESS_H_
#define LSBSTEGOBUSINESS_H_

#include "StegoBusiness.h"
#include "ImagePalette.h"

/**
 * LSBStegoBusiness.h
 * Esteganografia definida para las imagenes que no pierden informacion al comprimir y/o
 * para aquellas que directamente no utilizan compresion
 * Se modifica el bit menos significativo para almacenar informacion
 */
class LSBStegoBusiness: public StegoBusiness
{
	protected:

    	bool error;
    	/*Cantidad de bits por pixel a utilizar para esteganografia*/
    	unsigned int enable_bpp;
    	/*Representacion de la imagen*/
    	FIBitmap imagen;
    	/*Paleta de colores para imagenes gif e imagenes con bpp<=8*/
    	ImagePalette palette;
        /*Cantidad de bits que se van leyendo de la imagen o colocando en la imagen*/
        unsigned long int bits_procesados;
        /*Datos necesarios para procesar los mensajes y pixels*/
        unsigned int pos_bit_msj,pos_byte_msj,pos_pixel,max_pos_pixel,bit_in_pixel;
        unsigned char byte_msj;
  		virtual unsigned long int changePixel(BYTE *pixels,const char* mensaje,unsigned long int size);
    	virtual std::string getMessageFromPixel(BYTE *pixels,unsigned long int longitud);
    	std::string getLSBMessage(BYTE *pixels,unsigned long int longitud);
    	unsigned long int doLSBStego(BYTE *pixels,const char* mensaje,unsigned long int size);
    	void getPixel(unsigned long int first_bit,Pixel& pixel);

	public:

		LSBStegoBusiness(std::string filename);

		virtual unsigned long int setMessage(unsigned long int first_pos,const char* mensaje,unsigned long int size);
		virtual std::string getMessage(unsigned long int first_pos,unsigned long int longitud);
		virtual unsigned long int getFreeSpace()=0;
		virtual unsigned int getFirstFreeBit();

		virtual ~LSBStegoBusiness();
};

#endif /*LSBSTEGOBUSINESS_H_*/
