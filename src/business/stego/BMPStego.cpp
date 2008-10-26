#include "BMPStego.h"

BMPStego::BMPStego(std::string filename):LSBStegoBusiness(filename)
{

}

unsigned int BMPStego::getFreeSpace(){
 if((imagen.getBpp()<=8)&&(imagen.getColorType()>1))
    palette.sortPaletteByDistance();
 return ((imagen.getHeight())*(imagen.getWidth())*(imagen.getBpp()/8)*(this->enable_bpp));	
	
}
BMPStego::~BMPStego()
{
}
