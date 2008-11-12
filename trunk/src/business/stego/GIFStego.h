#ifndef GIFSTEGO_H_
#define GIFSTEGO_H_
#include "LSBStegoBusiness.h"


class GIFStego: public LSBStegoBusiness
{
	public:
	
		GIFStego();
		GIFStego(std::string filename);
		virtual unsigned int setMessage(unsigned long int first_bit,char* mensaje);
		virtual std::string getMessage(unsigned long int first_bit,unsigned int longitud);
		virtual unsigned int getFreeSpace();
		virtual ~GIFStego();
		
};

#endif /*GIFSTEGO_H_*/

