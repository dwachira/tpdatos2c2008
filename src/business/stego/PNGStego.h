#ifndef PNGSTEGO_H_
#define PNGSTEGO_H_
#include "LSBStegoBusiness.h"

/**
 * PNGStego.h
 * Redefine los metodos de LSBStego para poder realizar las operaciones considerando
 * el canal alpha en caso de que se utilice el metodo optimizado (STEGO_BASICO=0) 
 */
class PNGStego:public LSBStegoBusiness
{
private:	
    unsigned long int getTransparentPixels();
    bool isFirstPixelTransparent();
public:
	PNGStego();
	PNGStego(std::string filename);
    virtual unsigned long int changePixel(BYTE *pixels,const char* mensaje,unsigned long int size);
	virtual std::string getMessageFromPixel(BYTE *pixels,unsigned long int longitud);
	virtual unsigned int getFirstFreeBit();
	virtual unsigned long int getFreeSpace();
	virtual ~PNGStego();
};

#endif /*PNGSTEGO_H_*/
