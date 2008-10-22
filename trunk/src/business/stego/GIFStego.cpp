#include "GIFStego.h"
#include <fstream>
GIFStego::GIFStego(std::string filename):LSBStegoBusiness(filename,FIF_GIF)
{
}

unsigned int GIFStego::getFreeSpace(){
 return ((imagen.getHeight())*(imagen.getWidth())*(imagen.getBpp())*(this->enable_bpp));	
	
}
 
bool GIFStego::setMessage(unsigned long int first_bit,std::string mensaje)
{

if(!error){
	  Pixel pixel;
      getPixel(first_bit,pixel);
  	  palette.sortPaletteByDistance();  
  	  palette.updateIndexes();
      //palette.doIndexesLSB(pixel,mensaje);
	  /*Guardo los cambios realizados en la imagen*/
	  imagen.save();
	
    return true;
  }
  return false;	
}

std::string GIFStego::getMessage(unsigned long int first_bit,unsigned int longitud){
std::string mensaje;	

if(!error) {	
	Pixel pixel;
    getPixel(first_bit,pixel);
	mensaje.append(palette.getMessageFromIndexes(pixel,longitud));
}	 	
return mensaje;	
}

GIFStego::~GIFStego(){
}
