#ifndef LOSSYCOMPRESSSTEGOBUSINESS_H_
#define LOSSYCOMPRESSSTEGOBUSINESS_H_

#include "StegoBusiness.h"

#include <iostream>
#include <fstream>

class LossyCompressStegoBusiness: public StegoBusiness
{
private:
    unsigned long int free_space;
    unsigned long int original_size;
public:
    LossyCompressStegoBusiness();
	LossyCompressStegoBusiness(std::string filename);
	virtual bool setMensaje(Pixel& pixel,std::string mensaje);
	virtual std::string getMensaje(Pixel& pixel,unsigned int longitud);
	bool generarEspacio(float perc);
	virtual ~LossyCompressStegoBusiness();
};

#endif /*LOSSYCOMPRESSSTEGOBUSINESS_H_*/
