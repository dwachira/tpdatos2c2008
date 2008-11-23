#ifndef LOSSYCOMPRESSSTEGOBUSINESS_H_
#define LOSSYCOMPRESSSTEGOBUSINESS_H_

#include "StegoBusiness.h"


#define PERC 0.95 //porcentaje utilizado para definir el nivel de compresion

/**
 * LossyCompressStegoBusiness.h
 * Esteganografia para aquellas imagenes que presentan compresion con perdida de datos
 * para ello se comprime mas la imagen y se aprovecha el espacio disponible al final del
 * archivo
 */
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
		bool generateSpace();
		virtual unsigned int getQuality()const=0;
		virtual unsigned long int getFreeSpace();
		virtual unsigned int getFirstFreeBit();
		virtual ~LossyCompressStegoBusiness();
		
};

#endif /*LOSSYCOMPRESSSTEGOBUSINESS_H_*/
