#include "BMPStego.h"

BMPStego::BMPStego(std::string filename):LSBStegoBusiness(filename)
{
 this->format=FIF_BMP;
 loadImagen();
}

unsigned int BMPStego::getFreeSpace(){
 return ((this->height)*(this->width)*(this->bpp)*(this->enable_bpp));	
	
}
BMPStego::~BMPStego()
{
}
