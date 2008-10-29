#include "PNGStego.h"

PNGStego::PNGStego(std::string filename):LSBStegoBusiness(filename)
{
	
}

unsigned int PNGStego::changePixel(BYTE *pixels,std::string mensaje,unsigned int& pos,unsigned int& bits_procesados)	
{
/*Auxiliar para realizar la conversión de binario a entero*/
char* aux;
int byte;
/*Para almacenar el contenido del pixel en binario*/
std::string newbyte;
unsigned int bits_alpha=0;
unsigned int pos_pixel=0;

     /*Canal alpha en cero--> imagen transparente*/
     if((imagen.getBpp()==32)&&(pixels[3]==0)){std::cout<<"pixel transparente "<<std::endl;
    	  while((bits_procesados<mensaje.size())&&(pos_pixel<3)){
             newbyte.append(1,mensaje.at(bits_procesados));
             bits_alpha++;
             bits_procesados++;
             if((bits_alpha%8)==0){//guardo el nuevo byte completo
             	byte=strtol(newbyte.c_str(),&aux,2);
             	newbyte.clear();
             	pixels[pos_pixel] = byte;
             	pos_pixel++;
             }
          }
          return bits_alpha;
     }else{//sino utilizo el metodo comun
           return doLSBStego(pixels,mensaje,pos,bits_procesados);	
     }
             
}
unsigned int PNGStego::getFreeSpace(){
	if((imagen.getBpp()<=8)&&(imagen.getColorType()>1))
       if (sort_palette){
       	    palette.sortPaletteByDistance();
       	    return ((imagen.getHeight())*(imagen.getWidth())/8);
       }
       else return (imagen.getPaletteSize()*3)/8;
      
	unsigned int space;
	space=((((imagen.getHeight())*(imagen.getWidth())*(imagen.getBpp()/8)*(this->enable_bpp)))/8);
	if(imagen.getBpp()==32)
	  space+=  ((getTransparentPixels()*(24 - this->enable_bpp*(imagen.getBpp()/8)))/8);
	return space;
	
}
/**
 * Calcula la cantidad total de pixels transparentes
 */
unsigned int PNGStego::getTransparentPixels(){
unsigned int transparent=0;
if(!error){
  	  
       BYTE *bits = imagen.getBits();
       /*Me posiciono desde el comienzo de la imagen*/
       bits+=imagen.getPitch()*(imagen.getHeight()-1);
       
	   for (unsigned int y = 0; y <imagen.getHeight(); y ++){
		  /*Primer linea de pixels de la imagen*/
		  BYTE *pixels = (BYTE*)bits;
		  for (unsigned int x = 0; x < imagen.getWidth(); x ++){
		      if(pixels[3]==0)
	             transparent++;
       	   	  pixels += (imagen.getBpp()/8);//siguiente pixel
          }//fin for_x
		
      bits -= imagen.getPitch();//siguiente linea de la imagen
		
	}//fin for_y
  }
  return transparent;
}
std::string PNGStego::getMessageFromPixel(BYTE *pixels,unsigned int& pos,unsigned int longitud,unsigned int& bits_procesados){
std::string mensaje,binario;
unsigned int byte;
unsigned int pos_pixel=0;
if(bits_procesados==0)
  pos_pixel= (pos/8);
/*Canal alpha en cero--> imagen transparente*/
   if((imagen.getBpp()==32)&&(pixels[3]==0)){
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
       return getLSBMessage(pixels,pos,longitud,bits_procesados);
             		
}

PNGStego::~PNGStego()
{
}
