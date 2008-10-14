#ifndef GIFSTEGO_H_
#define GIFSTEGO_H_
#include "LSBStegoBusiness.h"
class GIFStego: public LSBStegoBusiness
{
public:
	GIFStego();
	GIFStego(std::string filename);
	virtual ~GIFStego();
};

#endif /*GIFSTEGO_H_*/
