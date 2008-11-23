#ifndef BMPSTEGO_H_
#define BMPSTEGO_H_
#include "LSBStegoBusiness.h"

/**
 * BMPStego.h
 * Redefine el metodo getFreeSpace para adaptarlo a las imagenes BMP
 */
class BMPStego:public LSBStegoBusiness
{
public:
	BMPStego();
	BMPStego(std::string filename);
	virtual unsigned long int getFreeSpace();
	virtual ~BMPStego();
};

#endif /*BMPSTEGO_H_*/
