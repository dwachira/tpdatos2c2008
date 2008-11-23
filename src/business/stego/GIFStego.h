#ifndef GIFSTEGO_H_
#define GIFSTEGO_H_
#include "LSBStegoBusiness.h"

/**
 * GIFStego.h
 * Redefine los metodos de LSBStego para poder realizar las operaciones considerando
 * la paleta de colores y los indices 
 */
class GIFStego: public LSBStegoBusiness
{
	public:
	
		GIFStego();
		GIFStego(std::string filename);
		virtual unsigned long int setMessage(unsigned long int first_bit,const char* mensaje,unsigned long int size);
		virtual std::string getMessage(unsigned long int first_bit,unsigned long int longitud);
		virtual unsigned long int getFreeSpace();
		virtual unsigned int getFirstFreeBit();
		virtual ~GIFStego();
		
};

#endif /*GIFSTEGO_H_*/

