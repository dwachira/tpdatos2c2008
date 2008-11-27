#ifndef JPGSTEGO_H_
#define JPGSTEGO_H_
#include "LossyCompressStegoBusiness.h"

/**
 * JPGStego.h
 * Representacion de las imagenes JPEG, redefine el metodo getQUality
 */
class JPGStego: public LossyCompressStegoBusiness
{
public:
	JPGStego(std::string filename);
	virtual unsigned int getQuality()const;
	virtual ~JPGStego();
};

#endif /*JPGSTEGO_H_*/
