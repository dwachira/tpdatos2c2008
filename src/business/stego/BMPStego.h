#ifndef BMPSTEGO_H_
#define BMPSTEGO_H_
#include "LSBStegoBusiness.h"

class BMPStego:public LSBStegoBusiness
{
public:
	BMPStego();
	BMPStego(std::string filename);
	virtual ~BMPStego();
};

#endif /*BMPSTEGO_H_*/
