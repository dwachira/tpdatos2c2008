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

/*Si la imagen se cargo correctamente*/
 if(imagen.load()){
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
unsigned int i;
char byte;
int byte_int;
std::string binario;
unsigned int pos_byte=0;
char* aux;

imageFile.open(filename.c_str(), std::fstream::in |std::fstream::out| std::fstream::binary);
	 if(imageFile.is_open()){
	 	 /*Deberia posicionarme en el archivo y escribir sobre el mismo*/
	     imageFile.seekg(first_pos/8);//para pasar de bits a byte
	     //un byte compartido por mas de un mensaje
	     pos_byte=first_pos%8;
	     //voy completando bytes con los datos del binario
	     if(pos_byte!=0){
	     	imageFile.get(byte);
	     	byte_int=(int)byte;
	     	util::BitsUtils::toBase(byte_int,2,binario);
	     	util::BitsUtils::completeNBits(binario,8);
	     	//completo con lo que alcance
	     	while((i<mensaje.size())&&(pos_byte<8)){
	           binario.append(1,mensaje.at(i));
	           i++;
	     	}	     	
	     	imageFile.seekp(first_pos/8);
            imageFile.put((char)strtol(binario.c_str(),&aux,2));
            binario="";
	     }
	     
	     while(i<mensaje.size()){
	     	binario=mensaje.substr(i,8);
	     	i+=binario.size();
	        imageFile.put((char)strtol(binario.c_str(),&aux,2));
	        binario="";
	     }
	     imageFile.close();
	     
     }	
     return mensaje.size()+first_pos;
}

std::string LossyCompressStegoBusiness::getMessage(unsigned long int first_pos,unsigned int longitud){
std::string mensaje;	
std::fstream imageFile;
unsigned int i=0;
unsigned int k=0;
char byte;
int byte_int;
unsigned int pos_byte=0;
std::string binario;

imageFile.open(filename.c_str(), std::fstream::in| std::fstream::binary);

    /*Deberia posicionarme en el archivo y escribir sobre el mismo*/
	imageFile.seekg(first_pos/8);//para pasar de bits a byte
	//un byte compartido por mas de un mensaje
	pos_byte=first_pos%8;
	//voy completando bytes con los datos del binario
	  if(pos_byte!=0){
	     	imageFile.get(byte);
	     	byte_int=(int)byte;
	     	util::BitsUtils::toBase(byte_int,2,binario);
	     	util::BitsUtils::completeNBits(binario,8);
	     	//from byte to the end
	     	mensaje.append(binario.substr(pos_byte));
	     	i+=pos_byte;
	     	binario="";
	  }   	
	     while(i<longitud){
	     	imageFile.get(byte);
	     	byte_int=(int)byte;
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
