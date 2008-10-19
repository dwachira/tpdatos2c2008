#include "GIFStego.h"

GIFStego::GIFStego(std::string filename):LSBStegoBusiness(filename)
{
	 this->format=FIF_GIF;
	 loadImagen();
}

unsigned int GIFStego::getFreeSpace(){
 return ((this->height)*(this->width)*(this->bpp)*(this->enable_bpp));	
	
}
 
bool GIFStego::setMessage(Pixel& pixel,std::string mensaje)
{

if(!error){
	
  	  palette.sortPaletteByDistance(imagen);  
      palette.updateIndexes(imagen);
      palette.doIndexesLSB(imagen,pixel,mensaje);
	  /*Guardo los cambios realizados en la imagen*/
	  FreeImage_Save(this->format,imagen,this->filename.c_str(),0);
	
    return true;
  }
  return false;	
}

std::string GIFStego::getMessage(Pixel& pixel,unsigned int longitud){
std::string mensaje;	

if(!error) 	
	  mensaje.append(palette.getMessageFromIndexes(imagen,pixel,longitud));
	 	
return mensaje;	
}

GIFStego::~GIFStego(){
}
