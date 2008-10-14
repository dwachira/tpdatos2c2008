#ifndef JPGSTEGO_H_
#define JPGSTEGO_H_
#include "LossyCompressStegoBusiness.h"

class JPGStego: public LossyCompressStegoBusiness
{
public:
	JPGStego(std::string filename);
	virtual ~JPGStego();
};

#endif /*JPGSTEGO_H_*/
