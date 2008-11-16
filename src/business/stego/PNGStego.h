#ifndef PNGSTEGO_H_
#define PNGSTEGO_H_
#include "LSBStegoBusiness.h"
class PNGStego:public LSBStegoBusiness
{
private:	
    unsigned long int getTransparentPixels();
public:
	PNGStego();
	PNGStego(std::string filename);
	virtual unsigned long int changePixel(BYTE *pixels,const char* mensaje,unsigned long int size);
	virtual std::string getMessageFromPixel(BYTE *pixels,unsigned long int longitud);
	virtual unsigned long int getFreeSpace();
	virtual ~PNGStego();
};

#endif /*PNGSTEGO_H_*/
