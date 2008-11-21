#include "PNGStego.h"

PNGStego::PNGStego(std::string filename):LSBStegoBusiness(filename)
{
	if(imagen.getBpp()==32) max_pos_pixel--;//para no modificar el indicador de transparencia
}


unsigned long int PNGStego::getFreeSpace(){
	if((imagen.getBpp()<=8)&&(!imagen.isGrayScale())){
       if (imagen.getPaletteSize()>16)
       	   return ((imagen.getHeight())*(imagen.getWidth())/8);
       else return (imagen.getPaletteSize()*3)/8;//solo se hara lsb sobre la paleta de colores
	} 
	unsigned long int space;
	space=((((imagen.getHeight())*(imagen.getWidth())*(max_pos_pixel)*(this->enable_bpp)))/8);

	if(imagen.getBpp()>=32) space=(space/10);
	else space=(space/2);
	return space;
	
}
/*unsigned long int PNGStego::getFreeSpace(){
	if((imagen.getBpp()<=8)&&(!imagen.isGrayScale())){
       if (imagen.getPaletteSize()>16)
       	   return ((imagen.getHeight())*(imagen.getWidth())/8);
       else return (imagen.getPaletteSize()*3)/8;//solo se hara lsb sobre la paleta de colores
	} 
	unsigned long int space;
	space=((((imagen.getHeight())*(imagen.getWidth())*(max_pos_pixel)*(this->enable_bpp)))/8);
	if(imagen.getBpp()==32)
	  space+=  ((getTransparentPixels()*this->enable_bpp*max_pos_pixel*7)/8);

	return space;
	
}*/

unsigned int PNGStego::getFirstFreeBit(){
	//solo se aplicara lsb a la paleta si esta posee un tamaño inferior a 16 colores
	//y no es en escala de grises
	if((imagen.getBpp()<=8)&&(!imagen.isGrayScale())&&(imagen.getPaletteSize()<=16)) return 0;
	//if((imagen.getBpp()==32)&&(isFirstPixelTransparent())) return 0;
	return (8-this->enable_bpp);
}
bool PNGStego::isFirstPixelTransparent(){
	
	 BYTE *bits = imagen.getBits();
     /*Me posiciono desde el comienzo de la imagen*/
     BYTE *pixels = (BYTE*)bits;
	 return(pixels[3]==0);
	
}

/**
 * Calcula la cantidad total de pixels transparentes
 */
unsigned long int PNGStego::getTransparentPixels(){
unsigned long int transparent=0;
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
  }std::cout<<"hay transparentes: "<<transparent<<std::endl;
  return transparent;
}

/*unsigned long int PNGStego::changePixel(BYTE *pixels,const char* mensaje,unsigned long int size)	
{*/
//Auxiliar para realizar la conversión de binario a entero
/*char* aux;
BYTE byte_pixel;
char bit;*/
//Para almacenar el contenido del pixel en binario
/*std::string newbyte;
unsigned long int bits_alpha=0;
unsigned int j=0;*/
     /*Canal alpha en cero--> imagen transparente*/
    // if((imagen.getBpp()==32)&&(pixels[3]==0)){
     	
     	 /*Un byte puede estar formado por distintos mensajes*/
     	/* if(bits_procesados==0){
     	 	bits_alpha++;//para pasar al proximo libre
     	 	if((bit_in_pixel-8*pos_pixel)!=0){
     	 */	 
     	 	 //Tomo el byte actual y lo relleno
     /*	 	  byte_pixel=(int)pixels[pos_pixel];
              std::string binario;
              util::BitsUtils::toBase(byte_pixel,2,binario);
              util::BitsUtils::completeNBits(binario,8);
              j=bit_in_pixel-8*pos_pixel;
              newbyte.append(binario.substr(0,j));
     	 	}
     	 	 
     	 }*/
     	  //Recorro los 3 bytes del pixel,sin modificar el byte alpha
    	//  while((bits_procesados<size*8)&&(pos_pixel<3)){
    	  	
    	  	//ciclo para completar un byte del pixel
    	 /* 	while((bits_procesados<size*8)&&(j<8)){
    	  	    if(pos_bit_msj==8){ 
   	 			   pos_bit_msj=0;
   	 			   pos_byte_msj++;
   	     	     }
   	     	     j++;*/
   	     	   //Guardo un bit de informacion en el LSB del byte   
     /*   	   if ((mensaje[pos_byte_msj])&(1<<pos_bit_msj)) bit='1';
        	   else bit='0';
               newbyte.push_back(bit);
               pos_bit_msj++;
               bits_procesados++;
               bits_alpha++;
    	  	}
               j=0;*/
               //Ultimo byte que se modifica
       //        if(bits_procesados==(size*8)){
                  //completo con ceros al final 
       /*           newbyte.append(8-newbyte.size(),'0');
                  
               }
               //std::cout<<newbyte<<std::endl;
               byte_pixel=(BYTE)strtol(newbyte.c_str(),&aux,2);
               newbyte.clear();
               pixels[pos_pixel] = byte_pixel;
               if(bits_procesados<(size*8))
                  pos_pixel++;
               if((pos_pixel==3)&&(bits_procesados<(size*8))) bits_alpha+=8;//salteo el pixel transparente
             }
                      
          return bits_alpha;
     }else{//sino utilizo el metodo comun
           return doLSBStego(pixels,mensaje,size);	
     }
             
}*/


/*std::string PNGStego::getMessageFromPixel(BYTE *pixels,unsigned long int longitud){
std::string mensaje,binario;
int byte_pixel;
unsigned int j=0;
*/
//Canal alpha en cero--> imagen transparente
  // if((imagen.getBpp()==32)&&(pixels[3]==0)){
     //Un byte puede estar formado por distintos mensajes
  /*   if((bits_procesados==0)&&((bit_in_pixel-8*pos_pixel)!=0)){
        std::cout<<"byte compartido"<<std::endl;
        j=bit_in_pixel-8*pos_pixel;
    
     }
     while((bits_procesados<longitud*8)&&(pos_pixel<3)){
          byte_pixel=(int)pixels[pos_pixel];
          binario="";
          util::BitsUtils::toBase(byte_pixel,2,binario);
          util::BitsUtils::completeNBits(binario,8);
         
          while((bits_procesados<longitud*8)&&(j<8)){     
                
          		if(binario.at(j)=='1') byte_msj = byte_msj | (1<<pos_bit_msj);
                j++; 
                bits_procesados++;
                pos_bit_msj++;
                if(pos_bit_msj==8){
      	 			pos_bit_msj=0;
      	 			mensaje.push_back(byte_msj);
      	 			byte_msj = 0x0;
      			}
               
          } pos_pixel++;//paso al byte siguiente
             j=0; 
          
      }
      return mensaje;       	
  }else//sino utilizo el metodo comun
       return getLSBMessage(pixels,longitud);
             		
}*/

PNGStego::~PNGStego()
{
}
