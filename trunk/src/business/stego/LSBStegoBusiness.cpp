#include "LSBStegoBusiness.h"
#include <iostream>
LSBStegoBusiness::LSBStegoBusiness():enable_bpp(1)
{
}
/**
 * Permite ocultar el mensaje correspondiente dentro de la imagen
 * 
 */
bool LSBStegoBusiness::setMensaje(Pixel& pixel,std::string mensaje)
{
/*Cantidad de bits que ya se han insertado en la imagen*/
unsigned int bits_procesados=0;
/*Para controlar la cantidad de bits por byte a modificar*/
unsigned int bits_por_byte=0;
/*Posicion inicial del mensaje dentro del pixel*/
unsigned int pos=pixel.getNumero_de_bit();
/*Auxiliar para realizar la conversión de binario a entero*/
char* aux;
int byte;
/*Para almacenar el contenido del pixel en binario*/
std::string binario;

/*Cargo la imagen */
FIBITMAP *imagen = FreeImage_Load(this->format, filename.c_str(), 0);
 
/*Si la imagen se cargo correctamente*/
if(imagen){
  	   /*Cantidad de bits por pixel*/
       unsigned int bpp = FreeImage_GetBPP(imagen);
       /*Numero de byte dentro del pixel*/
       unsigned int pos_pixel=0;
       unsigned pitch = FreeImage_GetPitch(imagen);   
       unsigned int height=FreeImage_GetHeight(imagen);
       unsigned int width=FreeImage_GetWidth(imagen); 
       
       std::cout<<"se crea la imagen de bpp = "<<bpp<<std::endl;
       //unsigned int bits_por_byte= 8 - this->enable_bpp;
       BYTE *bits = (BYTE*)FreeImage_GetBits(imagen);
       /*Me posiciono desde el comienzo de la imagen*/
       bits+=pitch*(height-1);
       
	   for (unsigned int y = pixel.getPosY(); y <height; y ++){
		  /*Primer linea de pixels de la imagen*/
		  BYTE *pixels = (BYTE*)bits;
		  for (unsigned int x = pixel.getPosX(); x < width; x ++){
		  	 
             if(bits_procesados<mensaje.size()){ 
                /*Mientras queden bytes por pixel para recorrer*/              
                while((pos_pixel<bpp/8)&&(bits_procesados<mensaje.size())){
                   //std::cout<<"saco el byte"<<(int)pixels[pos_pixel]<<std::endl;
		           toBase((int)pixels[pos_pixel],2,binario);
		            /*Completo el binario para que sea mas facil el proceso*/
		           complete8Bits(binario);
		           /*Ciclo en caso de modificar mas de un bit por byte*/
		           while((bits_por_byte<this->enable_bpp)&&(pos<8)&&(bits_procesados<mensaje.size())){
                     /*Guardo un bit de informacion en el LSB del byte*/       
                     binario.at(pos)=mensaje.at(bits_procesados); 
                     bits_procesados++;
                     bits_por_byte++;
                     pos++;
		           }  
                   byte=  strtol(binario.c_str(),&aux,2);
                   /*Guardo el nuevo byte modificado*/
                   pixels[pos_pixel]   = byte;
                   binario="";
                   pos_pixel++;//paso al byte siguiente
                   bits_por_byte=0;
                   pos=8-this->enable_bpp;
                 }
                
                pixels += bpp/8;//siguiente pixel
                pos_pixel=0;//primer byte del pixel
            }else{ //para terminar el ciclo for 
            	  x=width;
            	  y=height;
             }
		}//fin for_x
      bits -= pitch;//siguiente linea de la imagen
		
	}//fin for_y
	
	/*Guardo los cambios realizados en la imagen*/
	FreeImage_Save(this->format,imagen,this->filename.c_str(),0);
    return true;
  }
  return false;	
}

std::string LSBStegoBusiness::getMensaje(Pixel& pixel,unsigned int longitud){
std::string mensaje;	
int byte;
char car;
unsigned int bits_procesados=0;
/*posicion inicial*/
unsigned int pos=pixel.getNumero_de_bit();
// unsigned int next_bit=0;
std::string binario;

/*Cargo la imagen */
FIBITMAP *imagen = FreeImage_Load(this->format,this->filename.c_str(), 0);
/*Si la imagen se cargo correctamente*/
if(imagen){
  	   /*Cantidad de bits por pixel*/
       unsigned int bpp = FreeImage_GetBPP(imagen);
       unsigned pitch = FreeImage_GetPitch(imagen); 
       unsigned int pos_pixel=0;
       unsigned int height=FreeImage_GetHeight(imagen);
       unsigned int width=FreeImage_GetWidth(imagen);
       unsigned int bits_por_byte= 0;
       
       BYTE *bits = (BYTE*)FreeImage_GetBits(imagen);
	   bits+=pitch*(height-1);
	   for (unsigned int y = pixel.getPosY(); y <height; y ++){
		  BYTE *pixels = (BYTE*)bits;
		  for (unsigned int x = pixel.getPosX(); x < width; x ++){
		 	   
            if(bits_procesados<longitud){ 
               /*Obtengo el pixel de la imagen*/
               while((pos_pixel<bpp/8)&&(bits_procesados<longitud)){
                  byte=(int)pixels[pos_pixel];
                  //std::cout<<"saco el byte"<<byte<<std::endl;
		          toBase(byte,2,binario);
		          complete8Bits(binario);
		          // std::cout<<"el pixel QUE SACO Es "<<binario<<std::endl;
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
                pixels += bpp/8;  pos_pixel=0;//siguiente pixel
           }else{//para terminar el ciclo for 
            	  x=width;
            	  y=height;
           }
		}//fin for_x
        bits -= pitch;//proxima linea de la imagen
	}//fin for_y
  }
	
return mensaje;	
}

LSBStegoBusiness::~LSBStegoBusiness()
{
}
