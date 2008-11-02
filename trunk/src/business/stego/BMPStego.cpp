#include "BMPStego.h"

BMPStego::BMPStego(std::string filename):LSBStegoBusiness(filename)
{

}

unsigned int BMPStego::getFreeSpace(){
 if((imagen.getBpp()<=8)&&(!imagen.isGrayScale())){
   if(imagen.getPaletteSize()>16){ 
   	       palette.sortPaletteByDistance();
   	       return ((imagen.getHeight())*(imagen.getWidth())/8);   	  
   }
 } else return (imagen.getPaletteSize()*3)/8;//lsb sobre la paleta
	   
   return( ((imagen.getHeight())*(imagen.getWidth())*(imagen.getBpp()/8)*(this->enable_bpp))/8 );	
	
}
BMPStego::~BMPStego()
{
}
