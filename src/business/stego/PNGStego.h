#ifndef PNGSTEGO_H_
#define PNGSTEGO_H_
#include "LSBStegoBusiness.h"
class PNGStego:public LSBStegoBusiness
{
private:	
    unsigned int getTransparentPixels();
public:
	PNGStego();
	PNGStego(std::string filename);
	virtual unsigned int changePixel(BYTE *pixels,const char* mensaje)	;
	virtual std::string getMessageFromPixel(BYTE *pixels,unsigned int longitud);
	virtual unsigned int getFreeSpace();
	virtual ~PNGStego();
};

#endif /*PNGSTEGO_H_*/
