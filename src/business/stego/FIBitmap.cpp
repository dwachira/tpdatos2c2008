#include "FIBitmap.h"
#include <iostream>

FIBitmap::FIBitmap(){}

FIBitmap::FIBitmap(std::string filename,FREE_IMAGE_FORMAT format):filename(filename),format(format)
{
}
bool FIBitmap::load(){
/*Cargo la imagen */
imagen = FreeImage_Load(this->format, filename.c_str(), 0);

  if(imagen){
  	   /*Cantidad de bits por pixel*/
       bpp = FreeImage_GetBPP(imagen);
       pitch = FreeImage_GetPitch(imagen);   
       height=FreeImage_GetHeight(imagen);
       width=FreeImage_GetWidth(imagen); 
       color_type=FreeImage_GetColorType(imagen);
       palette_size=FreeImage_GetColorsUsed(imagen);
       
       error=false;
  }else error=true;	
	return error;
}

void FIBitmap::save(){
   FreeImage_Save(this->format,imagen,this->filename.c_str(),0);	
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

RGBQUAD* FIBitmap::getBackgroundColor(){
RGBQUAD* background;	
FreeImage_GetBackgroundColor(imagen,background);
return background;
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

FIBitmap::~FIBitmap()
{
	FreeImage_Unload(imagen);
}
