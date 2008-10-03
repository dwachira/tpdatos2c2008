#ifndef LOSSYCOMPRESSSTEGOBUSINESS_H_
#define LOSSYCOMPRESSSTEGOBUSINESS_H_

#include "StegoBusiness.h"

#include <iostream>
#include <fstream>

class LossyCompressStegoBusiness: public StegoBusiness
{
private:
    unsigned long int free_space;
public:
	LossyCompressStegoBusiness();
	virtual bool setMensaje(Pixel& pixel,std::string mensaje);
	virtual std::string getMensaje(Pixel& pixel,unsigned int longitud);
	bool generarEspacio();
	virtual ~LossyCompressStegoBusiness();
};

#endif /*LOSSYCOMPRESSSTEGOBUSINESS_H_*/
