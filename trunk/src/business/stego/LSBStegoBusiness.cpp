#include "LSBStegoBusiness.h"
#include <iostream>
LSBStegoBusiness::LSBStegoBusiness(std::string filename):StegoBusiness(filename),error(false),enable_bpp(1)
{
}

std::string LSBStegoBusiness::getMensajeFromPixel(BYTE *pixels,unsigned int& pos,unsigned int longitud,unsigned int& bits_procesados){
	return getLSBMensaje(pixels,pos,longitud,bits_procesados);
}

std::string LSBStegoBusiness::getLSBMensaje(BYTE *pixels,unsigned int& pos,unsigned int longitud,unsigned int& bits_procesados){
std::string mensaje,binario;
unsigned int pos_pixel=0;
unsigned int bits_por_byte= 0;
char car;
int byte;

  while((pos_pixel<bpp/8)&&(bits_procesados<longitud)){
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

void LSBStegoBusiness::changePixel(BYTE *pixels,std::string mensaje,unsigned int& pos,unsigned int& bits_procesados){
	doLSBStego(pixels,mensaje,pos,bits_procesados);
}

void LSBStegoBusiness::doLSBStego(BYTE *pixels,std::string mensaje,unsigned int& pos,unsigned int& bits_procesados){
int new_byte;
char* aux;
std::string binario;
unsigned int bits_por_byte=0;
unsigned int pos_pixel=0;

/*Mientras queden bytes por pixel para recorrer*/              
while((pos_pixel<bpp/8)&&(bits_procesados<mensaje.size())){
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
  new_byte=  strtol(binario.c_str(),&aux,2);	          
  /*Guardo el nuevo byte modificado*/
  pixels[pos_pixel]   = new_byte;
  binario="";
  pos_pixel++;//paso al byte siguiente
  bits_por_byte=0;
  pos=8-this->enable_bpp;
   }

  
  
}

void LSBStegoBusiness::loadImagen()
{
/*Cargo la imagen */
imagen = FreeImage_Load(this->format, filename.c_str(), 0);

  if(imagen){
  	   /*Cantidad de bits por pixel*/
       bpp = FreeImage_GetBPP(imagen);
       pitch = FreeImage_GetPitch(imagen);   
       height=FreeImage_GetHeight(imagen);
       width=FreeImage_GetWidth(imagen); 
       color_type=FreeImage_GetColorType(imagen);
       error=false;
  }else error=true;

}
/**
 * Permite ocultar el mensaje correspondiente dentro de la imagen
 * 
 */
bool LSBStegoBusiness::setMessage(Pixel& pixel,std::string mensaje)
{

/*Cantidad de bits que ya se han insertado en la imagen*/
unsigned int bits_procesados=0;

/*Posicion inicial del mensaje dentro del pixel*/
unsigned int pos=pixel.getNumero_de_bit();
if(!error){
       std::cout<<"se crea la imagen de bpp = "<<bpp<<std::endl;
       std::cout<<"color_type = "<<color_type<<std::endl;
      
       if((bpp<=8)&&(color_type>1)){
          palette.sortPaletteByDistance(imagen);
          palette.updateIndexes(imagen);
          palette.doIndexesLSB(imagen,pixel,mensaje);
       }else{//se puede trabajar con los pixeles
       	
       BYTE *bits = (BYTE*)FreeImage_GetBits(imagen);
       /*Me posiciono desde el comienzo de la imagen*/
       bits+=pitch*(height-1);
	   for (unsigned int y = pixel.getPosY(); y <height; y ++){
		  /*Primer linea de pixels de la imagen*/
		  BYTE *pixels = (BYTE*)bits;
		  for (unsigned int x = pixel.getPosX(); x < width; x ++){
		    
             if(bits_procesados<mensaje.size()){ 
             	
             	    changePixel(pixels,mensaje,pos,bits_procesados);	
             	   	pixels += (bpp/8);//siguiente pixel
             
            }else{ //para terminar el ciclo for 
            	  x=width;
            	  y=height;
             }
		}//fin for_x
      bits -= pitch;//siguiente linea de la imagen
		
	}//fin for_y
       }
	/*Guardo los cambios realizados en la imagen*/
	FreeImage_Save(this->format,imagen,this->filename.c_str(),0);
	
    return true;
  }
  return false;	
}

std::string LSBStegoBusiness::getMessage(Pixel& pixel,unsigned int longitud){
std::string mensaje;	

unsigned int bits_procesados=0;
/*posicion inicial*/
unsigned int pos=pixel.getNumero_de_bit();
std::string binario;
if(!error){
  	         
  	  
      if((bpp<=8)&&(color_type>1)){
      	    mensaje.append(palette.getMessageFromIndexes(imagen,pixel,longitud));
       }else{
       BYTE *bits = (BYTE*)FreeImage_GetBits(imagen);
	   bits+=pitch*(height-1);
	   for (unsigned int y = pixel.getPosY(); y <height; y ++){
		  BYTE *pixels = (BYTE*)bits;
		  for (unsigned int x = pixel.getPosX(); x < width; x ++){
		 	   
            if(bits_procesados<longitud){ 
            	
            	   mensaje.append(getMensajeFromPixel(pixels,pos,longitud,bits_procesados));
                   pixels += (bpp/8); 
               
           }else{//para terminar el ciclo for 
            	  x=width;
            	  y=height;
           }
		}//fin for_x
        bits -= pitch;//proxima linea de la imagen
	}//fin for_y
	 
  }
 }
return mensaje;	
}

LSBStegoBusiness::~LSBStegoBusiness()
{
	FreeImage_Unload(imagen);
}
