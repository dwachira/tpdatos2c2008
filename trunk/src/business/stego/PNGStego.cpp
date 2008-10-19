#include "PNGStego.h"

PNGStego::PNGStego(std::string filename):LSBStegoBusiness(filename)
{
	 this->format=FIF_PNG;
	 loadImagen();
}

void PNGStego::changePixel(BYTE *pixels,std::string mensaje,unsigned int& pos,unsigned int& bits_procesados)	
{
/*Auxiliar para realizar la conversión de binario a entero*/
char* aux;
int byte;
/*Para almacenar el contenido del pixel en binario*/
std::string newbyte;

unsigned int bits_alpha=0;
unsigned int pos_pixel=0;

     /*Canal alpha en cero--> imagen transparente*/
     if((bpp==32)&&(pixels[3]==0)){
    	  while((bits_procesados<mensaje.size())&&(pos_pixel<3)){
             newbyte.append(1,mensaje.at(bits_procesados));
             bits_alpha++;
             bits_procesados++;
             if((bits_alpha%8)==0){
             	byte=strtol(newbyte.c_str(),&aux,2);
             	newbyte.clear();
             	pixels[pos_pixel] = byte;
             	pos_pixel++;
             }
          }
     }else{//sino utilizo el metodo comun
           doLSBStego(pixels,mensaje,pos,bits_procesados);	
     }
             
}
unsigned int PNGStego::getFreeSpace(){
	unsigned int space=(this->height)*(this->width)*(this->bpp)*(this->enable_bpp);
	if(bpp==32)
	  space+=  getTransparentPixels()*(24 - this->enable_bpp);
	return space;
	
}
/**
 * Calcula la cantidad total de pixels transparentes
 */
unsigned int PNGStego::getTransparentPixels(){
unsigned int transparent=0;
if(!error){
  	  
       BYTE *bits = (BYTE*)FreeImage_GetBits(imagen);
       /*Me posiciono desde el comienzo de la imagen*/
       bits+=pitch*(height-1);
       
	   for (unsigned int y =0; y <height; y ++){
		  /*Primer linea de pixels de la imagen*/
		  BYTE *pixels = (BYTE*)bits;
		  for (unsigned int x = 0; x < width; x ++){
		      if(pixels[3]==0)
	             transparent++;
       	   	  pixels += (bpp/8);//siguiente pixel
          }//fin for_x
		
      bits -= pitch;//siguiente linea de la imagen
		
	}//fin for_y
  }
  return transparent;
}
std::string PNGStego::getMessageFromPixel(BYTE *pixels,unsigned int& pos,unsigned int longitud,unsigned int& bits_procesados){
std::string mensaje,binario;
unsigned int byte;
unsigned int pos_pixel=0;

/*Canal alpha en cero--> imagen transparente*/
   if((bpp==32)&&(pixels[3]==0)){
     while((bits_procesados<longitud)&&(pos_pixel<3)){
          byte=(int)pixels[pos_pixel];
          util::BitsUtils::toBase(byte,2,binario);
          bits_procesados+=8;
          if(bits_procesados<longitud)
            util::BitsUtils::completeNBits(binario,8);
          else
            util::BitsUtils::completeNBits(binario,bits_procesados-longitud);
          mensaje.append(binario);
          pos_pixel++;
          binario.clear();
      }
      return mensaje;       	
  }else//sino utilizo el metodo comun
       return getLSBMensaje(pixels,pos,longitud,bits_procesados);
             		
}

PNGStego::~PNGStego()
{
}
