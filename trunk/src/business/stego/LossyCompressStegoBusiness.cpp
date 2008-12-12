#include "LossyCompressStegoBusiness.h"

LossyCompressStegoBusiness::LossyCompressStegoBusiness(std::string filename):StegoBusiness(filename),
free_space(0){
}

unsigned long int LossyCompressStegoBusiness::getFreeSpace(){
  generateSpace();
  return this->free_space;
}

unsigned int LossyCompressStegoBusiness::getFirstFreeBit(){
	return compress_size*8;
}
/**
 * Comprime mas la imagen y la rellena con ceros para que no cambie su tamanio
 */
bool LossyCompressStegoBusiness::generateSpace()
{
unsigned int veces=0;
bool space_generated=false;
unsigned int original_size;
std::fstream imageFile;

    /*Calculo el espacio original en la imagen*/
 	imageFile.open(this->filename.c_str(), std::fstream::in | std::fstream::binary);
 	if(imageFile.is_open()){
 		imageFile.seekg(0, std::ios::end); // Ir al final del fichero
 		original_size=imageFile.tellg();
 		imageFile.close();
    }else return false;

 		/*Cargo la imagen */
 		FIBitmap imagen(this->filename);
 		bool error=imagen.load();
 		int quality=getQuality();
 		int quality_original=quality;
 		if(!error){
 			while((veces<5)&&(!space_generated)){//mientras no se genere espacio libre

 				/* Guardo la imagen cambiandole el nivel de compresion para generar
 				 * espacio al final de la misma
 				 */

 				imagen.save(quality*PERC);
 				 /*Calculo el tamaño del nuevo archivo y completo hasta llegar a su tamaño original*/
 					 imageFile.open(filename.c_str(), std::fstream::in |std::fstream::out| std::fstream::binary);
 					 if(imageFile.is_open()){
 					   imageFile.seekg(0, std::ios::end); // Ir al final del fichero
 					   compress_size=imageFile.tellg();
 					   free_space=original_size-compress_size;
 				       /*Completar con bytes en cero*/
 				       if(compress_size<original_size){
 				          for(unsigned int i=0;i<free_space;i++) imageFile.put((char)0);
 				          space_generated=true;
 				       }else {
 				    	  space_generated=false;
 				    	  veces++;
 				    	  quality=getQuality();
 				       }
 				          imageFile.close();
 				    }else return false;
 			}
 			/*Se intento comprimir pero no se pudo-->no la uso y vuelvo al estado inicial*/
 			if(veces>=5){
 				imagen.save(quality_original);
 				free_space=0;
 			}
 		}else return false;

  	 return true;
}
//la primera vez que se invoque para una determinada imagen se debera pasar: compress_size=original_Size-free_space
unsigned long int LossyCompressStegoBusiness::setMessage(unsigned long int first_pos,const char* mensaje,unsigned long int size)
{
std::fstream imageFile;
imageFile.open(filename.c_str(), std::fstream::in |std::fstream::out| std::fstream::binary);
	 if(imageFile.is_open()){
	     imageFile.seekp(first_pos/8);//para pasar de bits a byte
	     for(unsigned int i=0;i<size;i++)
	         imageFile.put(mensaje[i]);

	     imageFile.close();

     }
     return size*8 + first_pos+1;
}

std::string LossyCompressStegoBusiness::getMessage(unsigned long int first_pos,unsigned long int longitud){
std::string mensaje;
std::ifstream imageFile;
char byte;

imageFile.open(filename.c_str(),std::fstream::binary);
    if(imageFile.is_open()){
       imageFile.seekg(first_pos/8);//para pasar de bits a byte
	   for(unsigned int i=0;i<longitud;i++){
	       imageFile.get(byte);
	       mensaje.append(1,byte);
	   }
	   imageFile.close();
    }
     return mensaje;
}

LossyCompressStegoBusiness::~LossyCompressStegoBusiness()
{
}
