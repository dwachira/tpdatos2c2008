#include "GIFStego.h"
#include <fstream>

GIFStego::GIFStego(std::string filename):LSBStegoBusiness(filename)
{

}

//si uso el lsb sobre la paleta --->devolver cero
unsigned int GIFStego::getFirstFreeBit(){
	//solo se aplicara lsb a la paleta si esta posee un tamaño inferior a 16 colores
	//y no es en escala de grises
	if (!STEGO_BASICO) {
	    if(((!imagen.isGrayScale())&&(imagen.getPaletteSize()<=16))||(imagen.isAnimated())) return 0;
	    return (8-this->enable_bpp);
	}
	else
	    return 0;

}

unsigned long int GIFStego::getFreeSpace(){

 if ( !STEGO_BASICO ){
 	if((imagen.getPaletteSize()>16)&&(!imagen.isAnimated()))
     	return ( ((imagen.getHeight())*(imagen.getWidth()))/8 );//lsb sobre los indices
 }
    //lsb sobre la paleta de colores
    return (imagen.getPaletteSize()*3)/8;
}

unsigned long int GIFStego::setMessage(unsigned long int first_pos,const char* mensaje,unsigned long int size)
{
unsigned int last_pos;
if(!error){
	  if ( !STEGO_BASICO){
  	  	if ((imagen.getPaletteSize()>16)&&(!imagen.isAnimated())){//set del mensaje sobre los indices
  	  	   Pixel pixel;
           getPixel(first_pos,pixel);
     	   last_pos=palette.doIndexesLSB(pixel,mensaje,size)+first_pos;
  	  	}
  	  	else last_pos= palette.doPaletteLSB(first_pos,mensaje,size);

	  }else
	       last_pos= palette.doPaletteLSB(first_pos,mensaje,size);

  }
  return last_pos;
}

std::string GIFStego::getMessage(unsigned long int first_bit,unsigned long int longitud){
std::string mensaje;

if(!error) {
	if ( !STEGO_BASICO ){
		if((imagen.getPaletteSize()>16)&&(!imagen.isAnimated())){
			Pixel pixel;
    		getPixel(first_bit,pixel);
			mensaje.append(palette.getMessageFromIndexes(pixel,longitud));
		}else
	        mensaje.append(palette.getMessageFromPalette(first_bit,longitud));
	}else
            mensaje.append(palette.getMessageFromPalette(first_bit,longitud));

     }
return mensaje;
}

GIFStego::~GIFStego(){
}
