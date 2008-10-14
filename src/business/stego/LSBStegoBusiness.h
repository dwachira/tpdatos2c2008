#ifndef LSBSTEGOBUSINESS_H_
#define LSBSTEGOBUSINESS_H_

#include "StegoBusiness.h"
#include "Auxiliares.h"


class LSBStegoBusiness: public StegoBusiness
{
private:
    /*Cantidad de bits por pixel a utilizar para esteganografia*/
    unsigned int enable_bpp;
protected:
    unsigned int bpp,pitch,height,width,color_type;
    virtual void changePixel(BYTE *pixels,std::string mensaje,unsigned int& pos,unsigned int& bits_procesados);
    virtual std::string getMensajeFromPixel(BYTE *pixels,unsigned int& pos,unsigned int longitud,unsigned int& bits_procesados);
    std::string getLSBMensaje(BYTE *pixels,unsigned int& pos,unsigned int longitud,unsigned int& bits_procesados);
    void doLSBStego(BYTE *pixels,std::string mensaje,unsigned int& pos,unsigned int& bits_procesados);
public:
	LSBStegoBusiness();
	LSBStegoBusiness(std::string filename);
	virtual bool setMensaje(Pixel& pixel,std::string mensaje);
	virtual std::string getMensaje(Pixel& pixel,unsigned int longitud);
	virtual ~LSBStegoBusiness();
};

#endif /*LSBSTEGOBUSINESS_H_*/
