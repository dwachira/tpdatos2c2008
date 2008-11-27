#include "BMPStego.h"

BMPStego::BMPStego(std::string filename):LSBStegoBusiness(filename)
{

}

unsigned long int BMPStego::getFreeSpace(){
 if((imagen.getBpp()<=8)&&(!imagen.isGrayScale())){
   if((!STEGO_BASICO)&&(imagen.getPaletteSize()>16))
   	  return ((imagen.getHeight())*(imagen.getWidth())/8);//lsb sobre los indices
   else return (imagen.getPaletteSize()*3)/8;//lsb sobre la paleta
 }
 //metodo lsb basico
 return( ((imagen.getHeight())*(imagen.getWidth())*(imagen.getBpp()/8)*(this->enable_bpp))/8 );

}
BMPStego::~BMPStego()
{
}
