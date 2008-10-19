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
	virtual bool setMessage(Pixel& pixel,std::string mensaje);
	virtual std::string getMessage(Pixel& pixel,unsigned int longitud);
	/*Genera espacio para poder insertar mensaje,comprime la imagen*/
	bool generateSpace(float perc);
	virtual unsigned int getFreeSpace();
	virtual ~LossyCompressStegoBusiness();
};

#endif /*LOSSYCOMPRESSSTEGOBUSINESS_H_*/
