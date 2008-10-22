#include "LossyCompressStegoBusiness.h"

LossyCompressStegoBusiness::LossyCompressStegoBusiness(std::string filename,FREE_IMAGE_FORMAT format):StegoBusiness(filename,format),
free_space(0){
}

unsigned int LossyCompressStegoBusiness::getFreeSpace(){
  return this->free_space;	
}
/**
 * Comprime mas la imagen y la rellena con ceros para que no cambie su tamanio
 */
bool LossyCompressStegoBusiness::generateSpace(float perc)
{
/*Cargo la imagen */
FIBITMAP *imagen = FreeImage_Load(this->format,this->filename.c_str(),0);
/*Si la imagen se cargo correctamente*/
 if(imagen){
 	 unsigned int tamanio_original;
 	 std::fstream imageFile;
	 imageFile.open(this->filename.c_str(), std::fstream::in | std::fstream::binary);
	 if(imageFile.is_open()){
	   imageFile.seekg(0, std::ios::end); // Ir al final del fichero
       tamanio_original=imageFile.tellg();
       imageFile.close();	
	   /* Guardo la imagen cambiandole el nivel de compresion para generar
	    * espacio al final de la misma
	    */
	   
	   //VER QUE NUMERO SE LE PASA AL ULT PARAMETRO 
	   FreeImage_Save(this->format,imagen,filename.c_str(),perc);
	   FreeImage_Unload(imagen);
	   /*Reabro la imagen y me posiciono al final para completar el espacio 
	    * que le falta*/
	 }else return false;

	 imageFile.open(filename.c_str(), std::fstream::in |std::fstream::out| std::fstream::binary);
	 if(imageFile.is_open()){
	   imageFile.seekg(0, std::ios::end); // Ir al final del fichero
	   original_size=imageFile.tellg();
       free_space=tamanio_original-original_size;
       /*Completar con bytes en cero*/
       for(unsigned int i=0;i<free_space;i++) imageFile<<"0";	 
       imageFile.close();
	 }else return false;
	
 }else return false;
  	 return true;	
}
//en este caso el pixel tendria que indiacar numero de bit a modificar por ej
//podria no usarse pixel y usarse simplemente ese numero <---VER
bool LossyCompressStegoBusiness::setMessage(unsigned long int offset,std::string mensaje)
{
std::fstream imageFile;

imageFile.open(filename.c_str(), std::fstream::in |std::fstream::out| std::fstream::binary);
	 if(imageFile.is_open()){
	 	 /*Deberia posicionarme en el archivo y escribir sobre el mismo*/
	 imageFile.seekg(original_size);
	 for(unsigned int i=0;i<mensaje.size();i++)
	 imageFile<<mensaje.at(i);
	 imageFile.close();
	 return true;
 }	
 return false;
}

std::string LossyCompressStegoBusiness::getMessage(unsigned long int offset,unsigned int longitud){
std::string mensaje;	
std::fstream imageFile;

imageFile.open(filename.c_str(), std::fstream::in| std::fstream::binary);
imageFile.seekg(original_size);
char car;
    for(unsigned int i=0;i<longitud;i++){
     	
	 imageFile>>car;
	 
	 mensaje.append(1,car);
    }
	 imageFile.close();	
return mensaje;	
}

LossyCompressStegoBusiness::~LossyCompressStegoBusiness()
{
}
