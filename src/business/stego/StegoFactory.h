#ifndef STEGOFACTORY_H_
#define STEGOFACTORY_H_

#include "JPGStego.h"
#include "PNGStego.h"
#include "BMPStego.h"
#include "GIFStego.h"

/**
 * StegoFactory.h
 * Fabrica de objetos _Stego  
 */
class StegoFactory
{
public:
	StegoFactory();
	static StegoBusiness* newInstance(std::string filename);
	virtual ~StegoFactory();
};

#endif /*STEGOFACTORY_H_*/
