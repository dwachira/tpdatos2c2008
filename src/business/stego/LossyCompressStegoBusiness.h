#ifndef LOSSYCOMPRESSSTEGOBUSINESS_H_
#define LOSSYCOMPRESSSTEGOBUSINESS_H_

#define PERC 48

#include "StegoBusiness.h"

#include <iostream>
#include <fstream>

class LossyCompressStegoBusiness: public StegoBusiness
{
	private:
	
    	unsigned long int free_space;
    	unsigned long int compress_size;
    
	public:
	
    	LossyCompressStegoBusiness();
		LossyCompressStegoBusiness(std::string filename);
		virtual unsigned long int setMessage(unsigned long int first_pos,const char* mensaje,unsigned long int size);
		virtual std::string getMessage(unsigned long int first_pos,unsigned long int longitud);
		/*Genera espacio para poder insertar mensaje,comprime la imagen*/
		bool generateSpace(float perc);
		virtual unsigned long int getFreeSpace();
		virtual unsigned int getFirstFreeBit();
		virtual ~LossyCompressStegoBusiness();
		
};

#endif /*LOSSYCOMPRESSSTEGOBUSINESS_H_*/
