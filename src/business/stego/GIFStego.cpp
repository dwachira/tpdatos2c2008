#include "GIFStego.h"
#include <fstream>
GIFStego::GIFStego(std::string filename):LSBStegoBusiness(filename)
{
}

unsigned int GIFStego::getFreeSpace(){
 palette.sortPaletteByDistance();  //dejo todo preparado para realizar luego el lsb
 //si uso solo la paleta devolver imagen.getPaletteSize()*8
 return ( ((imagen.getHeight())*(imagen.getWidth()))/8 );	
	
}
 
unsigned int GIFStego::setMessage(unsigned long int first_pos,std::string mensaje)
{
unsigned int last_pos;
if(!error){
	  Pixel pixel;
      getPixel(first_pos,pixel);
      //last_pos= palette.doPaletteLSB(first_pos,mensaje);
  	  last_pos=palette.doIndexesLSB(pixel,mensaje);
	  /*Guardo los cambios realizados en la imagen*/
	  imagen.save();
	 
    
  }
  return last_pos+first_pos;	
}

std::string GIFStego::getMessage(unsigned long int first_bit,unsigned int longitud){
std::string mensaje;	

if(!error) {	
	Pixel pixel;
    getPixel(first_bit,pixel);
	mensaje.append(palette.getMessageFromIndexes(pixel,longitud));
	//mensaje.append(palette.getMessageFromPalette(first_bit,longitud));
}	 	
return mensaje;	
}

GIFStego::~GIFStego(){
}
