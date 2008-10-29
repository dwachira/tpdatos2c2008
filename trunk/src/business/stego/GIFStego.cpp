#include "GIFStego.h"
#include <fstream>
GIFStego::GIFStego(std::string filename):LSBStegoBusiness(filename)
{
}

unsigned int GIFStego::getFreeSpace(){
 if(sort_palette) {
 	 palette.sortPaletteByDistance();  //dejo todo preparado para realizar luego el lsb
     return ( ((imagen.getHeight())*(imagen.getWidth()))/8 );	
 }
    return (imagen.getPaletteSize()*3)/8;	
}
 
unsigned int GIFStego::setMessage(unsigned long int first_pos,std::string mensaje)
{
unsigned int last_pos;
if(!error){
	 
  	  if (sort_palette){
  	  	   Pixel pixel;
           getPixel(first_pos,pixel);
  	  	   last_pos=palette.doIndexesLSB(pixel,mensaje)+first_pos;
  	  }
  	  else last_pos= palette.doPaletteLSB(first_pos,mensaje);
	  
  }
  return last_pos;	
}

std::string GIFStego::getMessage(unsigned long int first_bit,unsigned int longitud){
std::string mensaje;	

if(!error) {	
	if(sort_palette){
			Pixel pixel;
    		getPixel(first_bit,pixel);
			mensaje.append(palette.getMessageFromIndexes(pixel,longitud));
	}else
	        mensaje.append(palette.getMessageFromPalette(first_bit,longitud));
}	 	
return mensaje;	
}

GIFStego::~GIFStego(){
}
