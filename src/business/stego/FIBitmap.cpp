#include "FIBitmap.h"
#include <iostream>

FIBitmap::FIBitmap(){}

FIBitmap::FIBitmap(std::string filename):filename(filename),format(FIF_UNKNOWN),error(true)
{
	getFileType();
}

bool FIBitmap::getFileType(){
// check the file signature and deduce its format
// (the second argument is currently not used by FreeImage)
format = FreeImage_GetFileType(filename.c_str(), 0);
	if(format == FIF_UNKNOWN) {
  		 // no signature ?
  		 // try to guess the file format from the file extension
  		 format = FreeImage_GetFIFFromFilename(filename.c_str());
	}
	// check that the plugin has reading capabilities ...
	if((format != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(format))
   		return true;
	else return false;
}
	
	
bool FIBitmap::load(int flag){

  if (format!=FIF_UNKNOWN){
	
    /*Cargo la imagen */
    imagen = FreeImage_Load(this->format, filename.c_str(),flag);

    if(imagen){
    	    
  	   		/*Cantidad de bits por pixel*/
       		bpp = FreeImage_GetBPP(imagen);
       		pitch = FreeImage_GetPitch(imagen);   
       		height=FreeImage_GetHeight(imagen);
       		width=FreeImage_GetWidth(imagen); 
       		color_type=FreeImage_GetColorType(imagen);
       		palette_size=FreeImage_GetColorsUsed(imagen);
            palette_offset=getPaletteOffset();
            std::cout<<"palette_Offset"<<palette_offset<<std::endl;
       		error=false;
  	}else error=true;	
  } else error=true;	
  	
	return error;
}

void FIBitmap::save(int perc){
   FreeImage_Save(this->format,imagen,this->filename.c_str(),perc);	
}

RGBQUAD* FIBitmap::getPalette(){
	return FreeImage_GetPalette(imagen);
}

BYTE* FIBitmap::getBits(){
	return (BYTE*)FreeImage_GetBits(imagen);
}
BYTE FIBitmap::getPixelIndex(unsigned int x,unsigned int y){
BYTE pixel_index;
FreeImage_GetPixelIndex(imagen,x,y,&pixel_index);
return pixel_index;
}

void FIBitmap::setPixelIndex(unsigned int x,unsigned int y,BYTE* new_pixel_index){
     FreeImage_SetPixelIndex(imagen,x,y,new_pixel_index);
}

RGBQUAD FIBitmap::getBackgroundColor(){
RGBQUAD background;	
FreeImage_GetBackgroundColor(imagen,&background);
return background;
}
       	 
void FIBitmap::setBackgroundColor(RGBQUAD background){       	 
  FreeImage_SetBackgroundColor(imagen,&background);
}  
  
bool FIBitmap::hasBackgroundColor(){       	 
   return FreeImage_HasBackgroundColor(imagen);
} 

BYTE* FIBitmap::getTransparencyTable(){
BYTE* paleta_transparente=FreeImage_GetTransparencyTable(imagen);
return paleta_transparente;
}

int FIBitmap::getTransparencyCount(){
   	return FreeImage_GetTransparencyCount(imagen);
}
bool FIBitmap::isTransparent(){
	return FreeImage_IsTransparent(imagen);
}

int FIBitmap::getTransparentIndex(){
    return FreeImage_GetTransparentIndex(imagen);
}

void FIBitmap::setTransparentIndex(int index){
    return FreeImage_SetTransparentIndex(imagen,index);
}

int FIBitmap::applyPaletteIndexMapping(BYTE*srcindices, BYTE *dstindices){
	return FreeImage_ApplyPaletteIndexMapping(imagen,srcindices,dstindices,palette_size,FALSE);
}
int FIBitmap::applyColorMapping(RGBQUAD *srccolors, RGBQUAD *dstcolors){
 return FreeImage_ApplyColorMapping(imagen,srccolors,dstcolors,palette_size, TRUE, FALSE);
}

void FIBitmap::applyColorMapping(RGBQUAD *srccolors,unsigned int count,unsigned int from){
std::ofstream imageFile;
imageFile.open(filename.c_str(), std::fstream::binary);
  if(imageFile.is_open()){std::cout<<"Me posiciono en : "<<palette_offset<<std::endl;
  	 
	 imageFile.seekp(palette_offset);//pos 13/24=comienzo paleta de colores
	 for (unsigned int i=from;i<count;i++){
	 	 
	     imageFile.put((char)((int)srccolors[i].rgbRed));
	     imageFile.put((char)((int)srccolors[i].rgbGreen));
	     imageFile.put((char)((int)srccolors[i].rgbBlue));
	    
	 } 
     imageFile.close();
  }	

}

int FIBitmap::getPaletteOffset(){
std::ifstream imageFile;
int offset,byte_int;
char byte;
std::string binario;
imageFile.open(filename.c_str(), std::fstream::binary);
  if(imageFile.is_open()){ 
  	 imageFile.seekg(10);//pos 10=bits info paleta
	 imageFile.get(byte); 
	 byte_int=(int)byte;
	 if(byte_int<0) byte_int+=256;
	 
	 util::BitsUtils::toBase(byte_int,2,binario);
	 util::BitsUtils::completeNBits(binario,8);
	  //si el primer bit esta en uno --> paleta global 
	 if(binario.at(0)=='1')
	    offset=13;
	 //sino: paleta local
	 else offset=24;
     imageFile.close();
  }	
  return offset;
}

void FIBitmap::setBackgroundColorIndex(int new_index){
std::cout<<"nuevo indice"<<new_index<<std::endl;
std::ofstream imageFile;
imageFile.open(filename.c_str(), std::fstream::binary);
  if(imageFile.is_open()){
	 imageFile.seekp(11);//pos 11=indice del background color
     imageFile.put((char)new_index);
     imageFile.close();
  }
}

BITMAPINFO* FIBitmap::getInfo(){
 return FreeImage_GetInfo(imagen);
}

FIBitmap::~FIBitmap(){
	if(!error) FreeImage_Unload(imagen);
}

