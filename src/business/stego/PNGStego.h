#ifndef PNGSTEGO_H_
#define PNGSTEGO_H_
#include "LSBStegoBusiness.h"
class PNGStego:public LSBStegoBusiness
{
public:
	PNGStego();
	PNGStego(std::string filename);
	virtual void changePixel(BYTE *pixels,std::string mensaje,unsigned int& pos,unsigned int& bits_procesados)	;
	virtual std::string getMensajeFromPixel(BYTE *pixels,unsigned int& pos,unsigned int longitud,unsigned int& bits_procesados);
	virtual ~PNGStego();
};

#endif /*PNGSTEGO_H_*/
