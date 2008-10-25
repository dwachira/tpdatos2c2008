#include "LSBStegoBusiness.h"
#include <iostream>


LSBStegoBusiness::LSBStegoBusiness(std::string filename):StegoBusiness(filename),error(false),enable_bpp(1),imagen(filename),palette(imagen)
{
	error=imagen.load();
}

unsigned int LSBStegoBusiness::getFirstFreeBit(){
	return (8-this->enable_bpp);
}

void LSBStegoBusiness::getPixel(unsigned long int first_bit,Pixel& pixel){
unsigned int y=0;
unsigned int x=0;
unsigned int bits_count=0;
unsigned int bit=0;

  if(first_bit> 8-this->enable_bpp){//no estoy en caso inicial
	  while(( y <imagen.getHeight())&&(bits_count<first_bit)){
		  /*Primer linea de pixels de la imagen*/
		 
		   while(( x <imagen.getWidth())&&(bits_count<first_bit)){
		   	    //siguiente pixel
		   	    bits_count+=8*imagen.getBpp();
		   	    if(bits_count>=first_bit)
		   	      bit= (first_bit-(y*imagen.getWidth()))%(imagen.getBpp());
		   	    //siguiente pixel
		      	x++;
		}//fin while_x
      //siguiente linea de la imagen
	  y++;
	}//fin while_y
  } else bit= first_bit;
    pixel.setPosX(x);
    pixel.setPosY(y);
    pixel.setNumero_de_bit(bit);
    
	
}

std::string LSBStegoBusiness::getMessageFromPixel(BYTE *pixels,unsigned int& pos,unsigned int longitud,unsigned int& bits_procesados){
	return getLSBMessage(pixels,pos,longitud,bits_procesados);
}

std::string LSBStegoBusiness::getLSBMessage(BYTE *pixels,unsigned int& pos,unsigned int longitud,unsigned int& bits_procesados){
std::string mensaje,binario;
unsigned int pos_pixel=0;
unsigned int bits_por_byte= 0;
char car;
int byte;
if(bits_procesados==0)
  pos_pixel= (pos/8);
  
  while((pos_pixel<imagen.getBpp()/8)&&(bits_procesados<longitud)){
      byte=(int)pixels[pos_pixel];
      util::BitsUtils::toBase(byte,2,binario);
	  util::BitsUtils::completeNBits(binario,8);
	  /*Ciclo en caso de modificar mas de un bit por byte*/
	  while((bits_por_byte<this->enable_bpp)&&(pos<8)&&(bits_procesados<longitud)){
          car=binario.at(pos);
          mensaje.append(1,car);  
          bits_procesados++;
          bits_por_byte++;
          pos++;
	  }  
      pos_pixel++;//paso al byte siguiente
      bits_por_byte=0;
      pos=8-this->enable_bpp;
      binario="";
  }//while de los bytes de un pixel
      return mensaje;      
}

unsigned int LSBStegoBusiness::changePixel(BYTE *pixels,std::string mensaje,unsigned int& pos,unsigned int& bits_procesados){
	return doLSBStego(pixels,mensaje,pos,bits_procesados);
}

unsigned int LSBStegoBusiness::doLSBStego(BYTE *pixels,std::string mensaje,unsigned int& pos,unsigned int& bits_procesados){
int new_byte;
char* aux;
std::string binario;
unsigned int bits_por_byte=0;
unsigned int pos_pixel=0;
unsigned int bits_count;

//debo posicionarme en el byte del pixel correcto
if(bits_procesados==0)
  pos_pixel= (pos/8);
  
/*Mientras queden bytes por pixel para recorrer*/              
while((pos_pixel<imagen.getBpp()/8)&&(bits_procesados<mensaje.size())){
   util::BitsUtils::toBase((int)pixels[pos_pixel],2,binario);
  /*Completo el binario para que sea mas facil el proceso*/
   util::BitsUtils::completeNBits(binario,8);
  /*Ciclo en caso de modificar mas de un bit por byte*/
  while((bits_por_byte<this->enable_bpp)&&(pos<8)&&(bits_procesados<mensaje.size())){
      /*Guardo un bit de informacion en el LSB del byte*/       
      binario.at(pos)=mensaje.at(bits_procesados); 
      bits_procesados++;
      bits_por_byte++;
      pos++;
  }  
  bits_count+=8;
  //puede que queden bits libres dentro un byte que pueden ser reutilizados
  if(bits_procesados==mensaje.size()){
  	if(bits_por_byte<this->enable_bpp)
       bits_count-=this->enable_bpp-bits_por_byte;
    //corro las posiciones necesarias para llegar al proximo bit disponible, es decir,
    //al bit del siguiente byte 
    else bits_count+=8-this->enable_bpp;
  }  
  new_byte=  strtol(binario.c_str(),&aux,2);	          
  /*Guardo el nuevo byte modificado*/
  pixels[pos_pixel]   = new_byte;
  binario="";
  pos_pixel++;//paso al byte siguiente
  bits_por_byte=0;
  pos=8-this->enable_bpp;
   }

  return bits_count;
  
}


/**
 * Permite ocultar el mensaje correspondiente dentro de la imagen
 * 
 */
unsigned int LSBStegoBusiness::setMessage(unsigned long int first_pos,std::string mensaje)
{
/*Cantidad de bits que ya se han insertado en la imagen*/
unsigned int bits_procesados=0;
Pixel pixel;
unsigned int last_pos=0;
getPixel(first_pos,pixel);
/*Posicion inicial del mensaje dentro del pixel*/
unsigned int pos=pixel.getNumero_de_bit();
if(!error){
        
       if((imagen.getBpp()<=8)&&(imagen.getColorType()>1))
            last_pos+=palette.doIndexesLSB(pixel,mensaje);
       else{//se puede trabajar con los pixeles
       	
       	BYTE *bits = imagen.getBits();
       	/*Me posiciono desde el comienzo de la imagen*/
       	bits+=imagen.getPitch()*(imagen.getHeight()-1);
	   	for (unsigned int y = pixel.getPosY(); y <imagen.getHeight(); y ++){
			  /*Primer linea de pixels de la imagen*/
			  BYTE *pixels = (BYTE*)bits;
		  	  for (unsigned int x = pixel.getPosX(); x < imagen.getWidth(); x ++){
		          if(bits_procesados<mensaje.size()){ 
             	    last_pos+=changePixel(pixels,mensaje,pos,bits_procesados);
             	    pos= 8 - this->enable_bpp;	
             	   	pixels += (imagen.getBpp()/8);//siguiente pixel
               }else{ //para terminar el ciclo for 
             		  x=imagen.getWidth();
            		  y=imagen.getHeight();
               }
		      }//fin for_x
              bits -= imagen.getPitch();//siguiente linea de la imagen
		
	     }//fin for_y
     }
	/*Guardo los cambios realizados en la imagen*/
	imagen.save();
  }
  
  return last_pos+first_pos;	
}

std::string LSBStegoBusiness::getMessage(unsigned long int first_bit,unsigned int longitud){
std::string mensaje;	
Pixel pixel;
getPixel(first_bit,pixel);
unsigned int bits_procesados=0;
/*posicion inicial*/
unsigned int pos=pixel.getNumero_de_bit();
std::string binario;
if(!error){
  	  
      if((imagen.getBpp()<=8)&&(imagen.getColorType()>1))
      	    mensaje.append(palette.getMessageFromIndexes(pixel,longitud));
      else{
       		BYTE *bits = imagen.getBits();
	   		bits+=imagen.getPitch()*(imagen.getHeight()-1);
	   		for (unsigned int y = pixel.getPosY(); y <imagen.getHeight(); y ++){
		  		BYTE *pixels = (BYTE*)bits;
		  		for (unsigned int x = pixel.getPosX(); x < imagen.getWidth(); x ++){
		 	   
            	  if(bits_procesados<longitud){ 
            	   	mensaje.append(getMessageFromPixel(pixels,pos,longitud,bits_procesados));
                   	pos= 8 - this->enable_bpp;
                   	pixels += (imagen.getBpp()/8); 
               
           		  }else{//para terminar el ciclo for 
            	  		x=imagen.getWidth();
            	  		y=imagen.getHeight();
                  }
		        }//fin for_x
                bits -= imagen.getPitch();//proxima linea de la imagen
	         }//fin for_y
	 
      }
 }
return mensaje;	
}

LSBStegoBusiness::~LSBStegoBusiness()
{
	
}
