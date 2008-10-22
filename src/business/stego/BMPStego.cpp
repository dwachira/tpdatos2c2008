#include "BMPStego.h"

BMPStego::BMPStego(std::string filename):LSBStegoBusiness(filename,FIF_BMP)
{

}

unsigned int BMPStego::getFreeSpace(){
 return ((imagen.getHeight())*(imagen.getWidth())*(imagen.getBpp())*(this->enable_bpp));	
	
}
BMPStego::~BMPStego()
{
}
