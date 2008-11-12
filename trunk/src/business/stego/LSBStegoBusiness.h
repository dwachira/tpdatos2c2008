#ifndef LSBSTEGOBUSINESS_H_
#define LSBSTEGOBUSINESS_H_

#include "StegoBusiness.h"
#include "ImagePalette.h"


class LSBStegoBusiness: public StegoBusiness
{
	protected:  
    
    	bool error;
    	/*Cantidad de bits por pixel a utilizar para esteganografia*/
    	unsigned int enable_bpp;
    	/*Representacion de la imagen*/
    	FIBitmap imagen;
    	ImagePalette palette;
        /*Cantidad de bits que se van leyendo de la imagen o colocando en la imagen*/
        unsigned int bits_procesados;      
        unsigned int pos_bit_msj,pos_byte_msj,pos_pixel,max_pos_pixel;
        unsigned char byte_msj;
  		virtual unsigned int changePixel(BYTE *pixels,const char* mensaje);
    	virtual std::string getMessageFromPixel(BYTE *pixels,unsigned int longitud);
    	std::string getLSBMessage(BYTE *pixels,unsigned int longitud);
    	unsigned int doLSBStego(BYTE *pixels,const char* mensaje);
    	void getPixel(unsigned long int first_bit,Pixel& pixel);
    
	public:
	
		LSBStegoBusiness(std::string filename);
	
		virtual unsigned int setMessage(unsigned long int first_pos,const char* mensaje);
		virtual std::string getMessage(unsigned long int first_pos,unsigned int longitud);
		virtual unsigned int getFreeSpace()=0;
		virtual unsigned int getFirstFreeBit();
	
		virtual ~LSBStegoBusiness();
};

#endif /*LSBSTEGOBUSINESS_H_*/
