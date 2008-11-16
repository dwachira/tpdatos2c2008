#ifndef GIFSTEGO_H_
#define GIFSTEGO_H_
#include "LSBStegoBusiness.h"


class GIFStego: public LSBStegoBusiness
{
	public:
	
		GIFStego();
		GIFStego(std::string filename);
		virtual unsigned long int setMessage(unsigned long int first_bit,const char* mensaje,unsigned long int size);
		virtual std::string getMessage(unsigned long int first_bit,unsigned long int longitud);
		virtual unsigned long int getFreeSpace();
		virtual ~GIFStego();
		
};

#endif /*GIFSTEGO_H_*/

