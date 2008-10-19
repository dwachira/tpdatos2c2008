#ifndef GIFSTEGO_H_
#define GIFSTEGO_H_
#include "LSBStegoBusiness.h"
#include "RGBColor.h"
#include <math.h>
#include <vector>

class GIFStego: public LSBStegoBusiness
{
private:
    unsigned int getIndex(std::vector<int>& vec,int elem);
public:
	GIFStego();
	GIFStego(std::string filename);
	virtual bool setMessage(Pixel& pixel,std::string mensaje);
	virtual std::string getMessage(Pixel& pixel,unsigned int longitud);
	virtual unsigned int getFreeSpace();
	virtual ~GIFStego();
};

#endif /*GIFSTEGO_H_*/