#include "PNGStego.h"

PNGStego::PNGStego(std::string filename):LSBStegoBusiness(filename)
{
	 this->format=FIF_PNG;
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

     /*Canal alpha en cero--> image transparente*/
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
     }else{
           doLSBStego(pixels,mensaje,pos,bits_procesados);	
     }
             
}
std::string PNGStego::getMensajeFromPixel(BYTE *pixels,unsigned int& pos,unsigned int longitud,unsigned int& bits_procesados){
std::string mensaje,binario;
unsigned int byte;
unsigned int pos_pixel=0;

/*Canal alpha en cero--> image transparente*/
   if((bpp==32)&&(pixels[3]==0)){
     while((bits_procesados<longitud)&&(pos_pixel<3)){
          byte=(int)pixels[pos_pixel];
          toBase(byte,2,binario);
          bits_procesados+=8;
          if(bits_procesados<longitud)
            completeNBits(binario,8);
          else
            completeNBits(binario,bits_procesados-longitud);
          mensaje.append(binario);
          pos_pixel++;
          binario.clear();
      }
      return mensaje;       	
  }else
       return getLSBMensaje(pixels,pos,longitud,bits_procesados);
             		
}

PNGStego::~PNGStego()
{
}
