#include "GIFStego.h"
#include <fstream>
GIFStego::GIFStego(std::string filename):LSBStegoBusiness(filename)
{
}

unsigned int GIFStego::getFreeSpace(){

 if(imagen.isGrayScale()) std::cout<<"ESCALA DE GRISES"<<std::endl;
 if(imagen.isAnimated()) std::cout<<"GIF ANIMADOOOO"<<std::endl;

 if(imagen.getPaletteSize()>16) {
 	 //solo ordeno la paleta si se trata de una imagen que no es escala de grises
 	 if(!imagen.isGrayScale())
 	 	palette.sortPaletteByDistance();  //dejo todo preparado para realizar luego el lsb
     return ( ((imagen.getHeight())*(imagen.getWidth()))/8 );
 }
    return (imagen.getPaletteSize()*3)/8;
}

unsigned int GIFStego::setMessage(unsigned long int first_pos,char* mensaje, long int size)
{
unsigned int last_pos;
if(!error){

  	  if (imagen.getPaletteSize()>16){//set del mensaje sobre los indices
  	  	   Pixel pixel;
           getPixel(first_pos,pixel);
  	  	   last_pos=palette.doIndexesLSB(pixel,mensaje,size)+first_pos;
  	  }
  	  else last_pos= palette.doPaletteLSB(first_pos,mensaje,size);

  }
  return last_pos;
}

std::string GIFStego::getMessage(unsigned long int first_bit,unsigned int longitud){
std::string mensaje;

if(!error) {
	if(imagen.getPaletteSize()>16){
			Pixel pixel;
    		getPixel(first_bit,pixel);
			mensaje.append(palette.getMessageFromIndexes(pixel,longitud*8));
	}else
	        mensaje.append(palette.getMessageFromPalette(first_bit,longitud*8));
}
return mensaje;
}

GIFStego::~GIFStego(){
}
