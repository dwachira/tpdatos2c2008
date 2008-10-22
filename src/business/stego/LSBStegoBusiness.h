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
    
    virtual void changePixel(BYTE *pixels,std::string mensaje,unsigned int& pos,unsigned int& bits_procesados);
    virtual std::string getMessageFromPixel(BYTE *pixels,unsigned int& pos,unsigned int longitud,unsigned int& bits_procesados);
    std::string getLSBMessage(BYTE *pixels,unsigned int& pos,unsigned int longitud,unsigned int& bits_procesados);
    void doLSBStego(BYTE *pixels,std::string mensaje,unsigned int& pos,unsigned int& bits_procesados);
    void loadImagen();
    void getPixel(unsigned long int first_bit,Pixel& pixel);
    
public:
	LSBStegoBusiness(std::string filename,FREE_IMAGE_FORMAT format);
	
	virtual bool setMessage(unsigned long int first_bit,std::string mensaje);
	virtual std::string getMessage(unsigned long int first_bit,unsigned int longitud);
	virtual unsigned int getFreeSpace()=0;
	
	virtual ~LSBStegoBusiness();
};

#endif /*LSBSTEGOBUSINESS_H_*/
