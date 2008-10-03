#ifndef LSBSTEGOBUSINESS_H_
#define LSBSTEGOBUSINESS_H_

#include "StegoBusiness.h"
#include "Auxiliares.h"


class LSBStegoBusiness: public StegoBusiness
{
private:
    /*Cantidad de bits por pixel a utilizar para esteganografia*/
    unsigned int enable_bpp;
public:
	LSBStegoBusiness();
	virtual bool setMensaje(Pixel& pixel,std::string mensaje);
	virtual std::string getMensaje(Pixel& pixel,unsigned int longitud);
	virtual ~LSBStegoBusiness();
};

#endif /*LSBSTEGOBUSINESS_H_*/
