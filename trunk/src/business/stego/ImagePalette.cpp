#include "ImagePalette.h"

ImagePalette::ImagePalette()
{
}
unsigned int ImagePalette::getNewPaletteIndex(unsigned int index){
 bool found=false;unsigned int i=0;
 if(std::find(new_palette_indexes.begin(),new_palette_indexes.end(),index)==new_palette_indexes.end())
   std::cout<<"no encontrado"<<std::endl;
 //std::cout<<"index"<<index<<std::endl;
 while((!found)&&(i<new_palette_indexes.size())){
 	 
   	if(new_palette_indexes[i]==index)
   	 found=true;
   	else i++;
 }	

 return i;
 }
void ImagePalette::sortPaletteByDistance(FIBITMAP *imagen){

double distancias[256][256];
int min=256;
unsigned int k;
unsigned int color_i,color_j;
unsigned long int red,green,blue;
      
if (FreeImage_IsTransparent(imagen)){
   unsigned int transparentes=FreeImage_GetTransparencyCount(imagen);
   std::cout<<"transparencia "<<transparentes<<std::endl;
   BYTE* paleta_transparente;
   paleta_transparente=FreeImage_GetTransparencyTable(imagen);
}
       
/*Reordeno la paleta de colores */
RGBQUAD *palette = FreeImage_GetPalette(imagen);
if(palette) {
       unsigned int palette_size=FreeImage_GetColorsUsed(imagen);
       std::cout<<"Tamaño de la paleta"<<palette_size<<std::endl;
       std::vector<RGBColor> paleta_original;
       /*Inicializo matriz de distancias*/
       
       for (unsigned int i = 0; i < palette_size; i++){
         for (unsigned int j = 0; j < palette_size; j++) {
        	 if(i==j)	distancias[i][j]=256;
        	 else  distancias[i][j]=0;
           }
       }
       
     /*Calculo la distancias entre los colores de la paleta*/
     for (unsigned int i = 0; i < palette_size; i++) {
        paleta_original.push_back(RGBColor((int)palette[i].rgbRed,(int)palette[i].rgbGreen,(int)palette[i].rgbBlue));
        for (unsigned int j = i+1; j < palette_size; j++) {
             red=pow(((int)palette[i].rgbRed-(int)palette[j].rgbRed),2);
             green=pow(((int)palette[i].rgbGreen-(int)palette[j].rgbGreen),2);
             blue=pow(((int)palette[i].rgbBlue-(int)palette[j].rgbBlue),2);
             distancias[i][j]=sqrt(red+green+blue);
             distancias[j][i]=distancias[i][j];
             if(distancias[i][j]<min){
              	min=distancias[i][j];
              	color_i=i;color_j=j;
              }
           }
        }
     /*Color ordenado por menor distancia*/
     new_palette_indexes.push_back(color_i);
     new_palette_indexes.push_back(color_j);
     /*Reordeno la paleta de colores en funcion de las distancias*/
      while(new_palette_indexes.size()<palette_size){
        min=256; color_i=0;k=color_j;
        /*Busco el siguiente color menor*/
        while(color_i<palette_size){
           
           if((distancias[k][color_i]<min)&&(std::find(new_palette_indexes.begin(),new_palette_indexes.end(),color_i)==new_palette_indexes.end())){
           	 min=distancias[k][color_i];
           	 color_j=color_i;
           }
           color_i++;  
        }
        new_palette_indexes.push_back(color_j);
      }
      std::vector<unsigned int>::iterator it;unsigned int i=0;
      for(it=new_palette_indexes.begin();it!=new_palette_indexes.end();it++){
           
            palette[i].rgbRed =(BYTE)paleta_original.at(*it).getRed(); 
            palette[i].rgbGreen=(BYTE)paleta_original.at(*it).getGreen(); 
            palette[i].rgbBlue=(BYTE)paleta_original.at(*it).getBlue();
            i++;
        
        }
       
   }
}

std::string ImagePalette::getMessageFromIndexes(FIBITMAP *imagen,Pixel& pixel,unsigned int longitud){
/*Cantidad de bits que ya se han insertado en la imagen*/
unsigned int bits_procesados=0;
/*Posicion inicial del mensaje dentro del pixel*/
unsigned int pos=pixel.getNumero_de_bit();
BYTE pixel_index;
unsigned int height=FreeImage_GetHeight(imagen);
unsigned int width=FreeImage_GetWidth(imagen);
std::string binario,mensaje;

	   for (unsigned int y = pixel.getPosY(); y <height; y ++){
		  
		  for (unsigned int x = pixel.getPosX(); x < width; x ++){
		 	   
            if(bits_procesados<longitud){ 
            	            	
             	   FreeImage_GetPixelIndex(imagen,x,height-y-1,&pixel_index);
             	   util::BitsUtils::toBase((int)pixel_index,2,binario);
                   /*Completo el binario para que sea mas facil el proceso*/
                   util::BitsUtils::completeNBits(binario,8);
                   mensaje.append(1,binario.at(pos));
                   binario="";
                   bits_procesados++;
                
           }else{//para terminar el ciclo for 
            	  x=width;
            	  y=height;
           }
		}//fin for_x
        
	}//fin for_y
    return mensaje;
}

void ImagePalette::doIndexesLSB(FIBITMAP *imagen,Pixel& pixel,std::string mensaje){

/*Cantidad de bits que ya se han insertado en la imagen*/
unsigned int bits_procesados=0;
/*Posicion inicial del mensaje dentro del pixel*/
unsigned int pos=pixel.getNumero_de_bit();
BYTE pixel_index,new_pixel_index;
unsigned int height=FreeImage_GetHeight(imagen);
unsigned int width=FreeImage_GetWidth(imagen);
 /*Me posiciono desde el comienzo de la imagen*/
std::string binario;char* aux;

for (unsigned int y = pixel.getPosY(); y <height; y ++){
		  
	for (unsigned int x = pixel.getPosX(); x < width; x ++){
		    
         if(bits_procesados<mensaje.size()){ 
            FreeImage_GetPixelIndex(imagen,x,height-y-1,&pixel_index);
            //std::cout<<"Pixel_index"<<(int)pixel_index<<std::endl;
		    util::BitsUtils::toBase((int)pixel_index,2,binario);
            /*Completo el binario para que sea mas facil el proceso*/
            util::BitsUtils::completeNBits(binario,8);
            /*Ciclo en caso de modificar mas de un bit por byte*/
            binario.at(pos)=mensaje.at(bits_procesados); 
            bits_procesados++;
            new_pixel_index= (BYTE)strtol(binario.c_str(),&aux,2);	          
            binario="";
                                
            FreeImage_SetPixelIndex(imagen,x,height-y-1,&new_pixel_index);
             
         }else{ //para terminar el ciclo for 
            	  x=width;
            	  y=height;
          }
		}//fin for_x
      
		
	}//fin for_y
    
	
}


void ImagePalette::updateIndexes(FIBITMAP *imagen){
BYTE pixel_index,new_pixel_index;
unsigned int height=FreeImage_GetHeight(imagen);
unsigned int width=FreeImage_GetWidth(imagen);
       for (unsigned int y = 0; y <height; y ++){
		  
		  for (unsigned int x = 0; x < width; x ++){
		        FreeImage_GetPixelIndex(imagen,x,height-y-1,&pixel_index);
                new_pixel_index=(BYTE)getNewPaletteIndex((int)pixel_index);
           	    FreeImage_SetPixelIndex(imagen,x,height-y-1,&new_pixel_index);
          }//fin for_x
       }//fin for_y
       
	  if(FreeImage_HasBackgroundColor(imagen)){ 
	  	 /*RGBQUAD *background;
       	 FreeImage_GetBackgroundColor(imagen,background);int index,new_index;
       	 for (unsigned int i = 0; i < tamanio_paleta; i++){
       	 	  index=(int)background[i].rgbReserved;
       	 	  std::cout<<index<<std::endl;
       	 	  new_index=getNewPaletteIndex(index);
       	 	  background[i].rgbReserved=(BYTE)new_index;
       	 	  std::cout<<"nuevo indice"<<new_index<<std::endl;
       	      //background[i].rgbReserved=(BYTE)getNewPaletteIndex((int)background[i].rgbReserved);
       	 }*/
	  }
}

ImagePalette::~ImagePalette()
{
}
