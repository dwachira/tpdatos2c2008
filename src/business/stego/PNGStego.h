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
	virtual unsigned int changePixel(BYTE *pixels,std::string mensaje,unsigned int& pos,unsigned int& bits_procesados)	;
	virtual std::string getMessageFromPixel(BYTE *pixels,unsigned int& pos,unsigned int longitud,unsigned int& bits_procesados);
	virtual unsigned int getFreeSpace();
	virtual ~PNGStego();
};

#endif /*PNGSTEGO_H_*/
