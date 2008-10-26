#include "LossyCompressStegoBusiness.h"

LossyCompressStegoBusiness::LossyCompressStegoBusiness(std::string filename):StegoBusiness(filename),
free_space(0){
}

unsigned int LossyCompressStegoBusiness::getFreeSpace(){
  generateSpace(PERC);
  return this->free_space;	
}

unsigned int LossyCompressStegoBusiness::getFirstFreeBit(){
	return compress_size*8;
}
/**
 * Comprime mas la imagen y la rellena con ceros para que no cambie su tamanio
 */
bool LossyCompressStegoBusiness::generateSpace(float perc)
{
/*Cargo la imagen */
FIBitmap imagen(this->filename);

bool error=imagen.load();
/*Si la imagen se cargo correctamente*/
 if(!error){
 	 unsigned int original_size;
 	 std::fstream imageFile;
	 imageFile.open(this->filename.c_str(), std::fstream::in | std::fstream::binary);
	 if(imageFile.is_open()){
	   imageFile.seekg(0, std::ios::end); // Ir al final del fichero
       original_size=imageFile.tellg();
       imageFile.close();	
	   /* Guardo la imagen cambiandole el nivel de compresion para generar
	    * espacio al final de la misma
	    */
	   //VER QUE NUMERO SE LE PASA AL ULT PARAMETRO 
	   imagen.save(perc);
	  
	   /*Reabro la imagen y me posiciono al final para completar el espacio 
	    * que le falta*/
	 }else return false;

     /*Calculo el tamaño del nuevo archivo y completo hasta llegar a su tamaño original*/
	 imageFile.open(filename.c_str(), std::fstream::in |std::fstream::out| std::fstream::binary);
	 if(imageFile.is_open()){
	   imageFile.seekg(0, std::ios::end); // Ir al final del fichero
	   compress_size=imageFile.tellg();
	   free_space=original_size-compress_size;
       /*Completar con bytes en cero*/
       for(unsigned int i=0;i<free_space;i++) imageFile<<"0";	 
       imageFile.close();
	 }else return false;
	
 }else return false;
  	 return true;	
}
//la primera vez que se invoque para una determinada imagen se debera pasar: compress_size=original_Size-free_space
unsigned int LossyCompressStegoBusiness::setMessage(unsigned long int first_pos,std::string mensaje)
{
std::fstream imageFile;
unsigned int i=0;
unsigned int k=0;
char byte;
int byte_int;
std::string binario;
char* aux;

imageFile.open(filename.c_str(), std::fstream::in |std::fstream::out| std::fstream::binary);
	 if(imageFile.is_open()){
	 	 /*Deberia posicionarme en el archivo y escribir sobre el mismo*/
	     imageFile.seekg(first_pos/8);//para pasar de bits a byte
	     /*Si en un mismo byte hay dos mensajes*/
	     if((first_pos%8)!=0){
	       	imageFile.get(byte);
	     	byte_int=(int)byte;
	     	if(byte_int<0) byte_int+=256;
	     	util::BitsUtils::toBase(byte_int,2,binario);
	     	util::BitsUtils::completeNBits(binario,8);
	     	k=first_pos%8;
	     	while((k<8)&&(i<mensaje.size())){
	     	   binario.at(k)=mensaje.at(i);
	     	   i++;k++;
	     	}
	     	byte_int=strtol(binario.c_str(),&aux,2);
	     	imageFile.seekg(first_pos/8);
	     	imageFile.put((char)byte_int);
	     }
	     while(i<mensaje.size()){
	     	
	     	/*Un byte compartido por mas de un mensaje..al final*/
	     	if((int)i>((int)mensaje.size()-8)){
	     		imageFile.get(byte);
	     		byte_int=(int)byte;
	     		if(byte_int<0) byte_int+=256;
	     		util::BitsUtils::toBase(byte_int,2,binario);
	     	    k=0;
	     	    while(i<mensaje.size()){
	     	        binario.at(k)=mensaje.at(i);
	     	        i++;k++;
	     	    }
	     	    byte_int=strtol(binario.c_str(),&aux,2);
	     	    imageFile.seekg(first_pos/8);
	     	    imageFile.put((char)byte_int);
	     	    
	     	}else{
	     			binario=mensaje.substr(i,8);
	     			i+=8;
	     			byte_int=strtol(binario.c_str(),&aux,2);
	     			imageFile.put((char)byte_int);
	        		binario="";
	     	}
	     }
	     imageFile.close();
	     
     }	
     return mensaje.size()+first_pos+1;
}

std::string LossyCompressStegoBusiness::getMessage(unsigned long int first_pos,unsigned int longitud){
std::string mensaje;	
std::fstream imageFile;
unsigned int i=0;
unsigned int k=0;
char byte;
int byte_int;
std::string binario;

imageFile.open(filename.c_str(), std::fstream::in| std::fstream::binary);

    /*Deberia posicionarme en el archivo y escribir sobre el mismo*/
	imageFile.seekg(first_pos/8);//para pasar de bits a byte
	  if((first_pos%8)!=0){
	       	imageFile.get(byte);
	     	byte_int=(int)byte;
	     	if(byte_int<0) byte_int+=256;
	     	util::BitsUtils::toBase(byte_int,2,binario);
	     	util::BitsUtils::completeNBits(binario,8);
	     	k=first_pos%8;
	     	while((k<8)&&(i<longitud)){
	     	   mensaje.append(1,binario.at(k));
	     	   i++;k++;
	     	}
	     	
	     }	
	while(i<longitud){
	     	imageFile.get(byte);
	     	byte_int=(int)byte;
	     	if(byte_int<0) byte_int+=256;
	     	util::BitsUtils::toBase(byte_int,2,binario);
	     	util::BitsUtils::completeNBits(binario,8);
	     	k=0;
	     	while((i<longitud)&&(k<binario.size())){	     	
	     	     mensaje.append(1,binario.at(k));
	     	     k++;i++;
	     	}
	     		     	
	     	binario="";
	     }
	 imageFile.close();	
     return mensaje;	
}

LossyCompressStegoBusiness::~LossyCompressStegoBusiness()
{
}
