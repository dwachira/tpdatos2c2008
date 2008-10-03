#include "LossyCompressStegoBusiness.h"

LossyCompressStegoBusiness::LossyCompressStegoBusiness()
{
}
/**
 * Comprime mas la imagen y la rellena con ceros para que no cambie su tamanio
 */
bool LossyCompressStegoBusiness::generarEspacio()
{
/*Cargo la imagen */
FIBITMAP *imagen = FreeImage_Load(this->format,this->filename.c_str(), 0);
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
	   FreeImage_Save(this->format,imagen,this->filename.c_str(),0);
	   /*Reabro la imagen y me posiciono al final para completar el espacio 
	    * que le falta*/
	 }else return false;
	 imageFile.open(this->filename.c_str(), std::fstream::in |std::fstream::out| std::fstream::binary);
	 if(imageFile.is_open()){
	   imageFile.seekg(0, std::ios::end); // Ir al final del fichero
       free_space=tamanio_original-imageFile.tellg();
       /*Completar con bytes en cero*/
       for(unsigned int i=0;i<free_space;i++) imageFile<<"0";	 
       imageFile.close();
	 }else return false;
	
 }else return false;
  	 return true;	
}
//en este caso el pixel tendria que indiacar numero de bit a modificar por ej
//podria no usarse pixel y usarse simplemente ese numero <---VER
bool LossyCompressStegoBusiness::setMensaje(Pixel& pixel,std::string mensaje)
{
std::fstream imageFile;
imageFile.open(this->filename.c_str(), std::fstream::in |std::fstream::out| std::fstream::binary);
	 if(imageFile.is_open()){
	 	 /*Deberia posicionarme en el archivo y escribir sobre el mismo*/
	 imageFile.close();
	 return true;
 }	
 return false;
}

std::string LossyCompressStegoBusiness::getMensaje(Pixel& pixel,unsigned int longitud){
std::string mensaje;	
	
return mensaje;	
}
LossyCompressStegoBusiness::~LossyCompressStegoBusiness()
{
}
