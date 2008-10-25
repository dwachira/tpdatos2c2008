#include "ImagePalette.h"
#include <fstream>


ImagePalette::ImagePalette(FIBitmap &imagen):imagen(imagen){
	
}
unsigned int ImagePalette::getNewPaletteIndex(unsigned int index){
 bool found=false;unsigned int i=0;

 while((!found)&&(i<new_palette_indexes.size())){
   	if(new_palette_indexes[i]==index)
   	   found=true;
   	else i++;
 }	

 return i;
}
 
void ImagePalette::sortPaletteByDistance(){

double distancias[256][256];
int min=MAX_DISTANCE;
unsigned int k;
unsigned int color_i,color_j;
unsigned long int red,green,blue;
unsigned int background_index;
RGBQUAD background;
/*Reordeno la paleta de colores */
RGBQUAD *palette = imagen.getPalette();
if(palette) {
	 
	   //int background_index;
	   if(imagen.hasBackgroundColor())	  	
	  	 background=imagen.getBackgroundColor();
      	   
       unsigned int palette_size=imagen.getPaletteSize();
       for (unsigned int i = 0; i < palette_size; i++){
         for (unsigned int j = 0; j < palette_size; j++) {
        	 if(i==j)	distancias[i][j]=256;
        	 else  distancias[i][j]=0;
           }
       }
       std::ofstream f("distancias.txt");
     /*Calculo la distancias entre los colores de la paleta*/
     for (unsigned int i = 0; i < palette_size; i++) {
     	if((palette[i].rgbRed==background.rgbRed)&&(palette[i].rgbGreen==background.rgbGreen)&&(palette[i].rgbBlue==background.rgbBlue))
     	    background_index=i;
        for (unsigned int j = i+1; j < palette_size; j++) {
             red=pow(((int)palette[i].rgbRed-(int)palette[j].rgbRed),2);
             green=pow(((int)palette[i].rgbGreen-(int)palette[j].rgbGreen),2);
             blue=pow(((int)palette[i].rgbBlue-(int)palette[j].rgbBlue),2);
             distancias[i][j]=sqrt(red+green+blue);
             f<<"distancia i,j "<<i<<","<<j<<" "<<distancias[i][j]<<std::endl;
             distancias[j][i]=distancias[i][j];
             if(distancias[i][j]<min){
              	min=distancias[i][j];
              	color_i=i;color_j=j;
              }
           }
        }f.close();
           
     /*Color ordenado por menor distancia*/
     new_palette_indexes.push_back(color_i);
     new_palette_indexes.push_back(color_j);
     
     /*Reordeno la paleta de colores en funcion de las distancias*/
      while(new_palette_indexes.size()<palette_size){
        min=MAX_DISTANCE; color_i=0;k=color_j;
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
      
        /*Ordeno la paleta de colores*/
        RGBQUAD dstcolors[256];std::ofstream file("paleta.txt");    
        std::vector<unsigned int>::iterator it;unsigned int i=0;
        
        for(it=new_palette_indexes.begin();it!=new_palette_indexes.end();it++){
            file<< (int)palette[i].rgbRed<<"-"<<(int)palette[i].rgbGreen<<"-"<<(int)palette[i].rgbBlue<<std::endl;                
            dstcolors[i].rgbRed=palette[*it].rgbRed;
            dstcolors[i].rgbGreen=palette[*it].rgbGreen;
            dstcolors[i].rgbBlue=palette[*it].rgbBlue;
            file<< "nueva: "<<(int)dstcolors[i].rgbRed<<"-"<<(int)dstcolors[i].rgbGreen<<"-"<<(int)dstcolors[i].rgbBlue<<std::endl;       
            i++;
            
        }
        imagen.applyColorMapping(dstcolors);
        imagen.applyColorMapping(palette,dstcolors);
       
        file.close();
        
        /*Ordeno los indices*/
        BYTE srcindices[256];
		BYTE dstindices[256];
		for(unsigned int j=0;j<palette_size;j++){
			srcindices[j]=(BYTE)j;
		    dstindices[j]=(BYTE)getNewPaletteIndex(j);
		}
		std::cout<<"pixels modif "<<imagen.applyPaletteIndexMapping(srcindices,dstindices,palette_size)<<std::endl;
      
        if (imagen.isTransparent()){
        	int transparent_index;
        	transparent_index=imagen.getTransparentIndex();
        	std::cout<<"indice_transparente"<<transparent_index<<std::endl;
   			unsigned int transparentes=imagen.getTransparencyCount();
   			std::cout<<"transparencia "<<transparentes<<std::endl;
   			BYTE* paleta_transparente;
   			paleta_transparente=imagen.getTransparencyTable();
   			imagen.setTransparentIndex(getNewPaletteIndex(transparent_index));
		}
		
	
	    if(imagen.hasBackgroundColor())  	
	  	    imagen.setBackgroundColorIndex(getNewPaletteIndex(background_index));
       	
        std::cout<<"Tamaño de la paleta"<<palette_size<<std::endl;
       
   }
}


std::string ImagePalette::getMessageFromIndexes(Pixel& pixel,unsigned int longitud){
/*Cantidad de bits que ya se han insertado en la imagen*/
unsigned int bits_procesados=0;
/*Posicion inicial del mensaje dentro del pixel*/
unsigned int pos=pixel.getNumero_de_bit();
BYTE pixel_index;
unsigned int height= imagen.getHeight();
unsigned int width=imagen.getWidth();
std::string binario,mensaje;

	   for (unsigned int y = pixel.getPosY(); y <height; y ++){
		  
		  for (unsigned int x = pixel.getPosX(); x < width; x ++){
		 	   
            if(bits_procesados<longitud){ 
            	   
             	   pixel_index=imagen.getPixelIndex(x,height-y-1);
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

int ImagePalette::doLSB(int byte,char value){
std::string binario;
char* aux;
	util::BitsUtils::toBase((int)byte,2,binario);
    util::BitsUtils::completeNBits(binario,8);
    /*Guardo un bit de informacion en el LSB del byte*/       
    binario.at(7)=value; 
    return strtol(binario.c_str(),&aux,2);
}

char ImagePalette::getLSB(int byte){
std::string binario;
     util::BitsUtils::toBase(byte,2,binario);
     util::BitsUtils::completeNBits(binario,8);
     return (binario.at(7)); 
}
//aca first bit seria el numero dentro de la paleta
unsigned int ImagePalette::doPaletteLSB(unsigned int first_pos,std::string mensaje){
RGBQUAD *palette = imagen.getPalette();
unsigned int last_pos=0;
unsigned int bits_procesados=0;
if(palette) {
       unsigned int palette_size=imagen.getPaletteSize();
       for(unsigned int i=first_pos;i<palette_size;i++){      
           if(bits_procesados<mensaje.size()){
              palette[i].rgbRed=(BYTE)doLSB((int)palette[i].rgbRed,mensaje.at(bits_procesados));
              bits_procesados++;
              last_pos++;
   	       }else i=palette_size;
   	       
   	       if(bits_procesados<mensaje.size()){
              palette[i].rgbGreen=(BYTE)doLSB((int)palette[i].rgbGreen,mensaje.at(bits_procesados));
              bits_procesados++;
              last_pos++;
   	       }else i=palette_size;
   	       if(bits_procesados<mensaje.size()){
              palette[i].rgbBlue=(BYTE)doLSB((int)palette[i].rgbBlue,mensaje.at(bits_procesados));
              bits_procesados++;
              last_pos++;
   	      }else i=palette_size;
       
       }
  }  
	return last_pos+first_pos;
}

std::string ImagePalette::getMessageFromPalette(unsigned int first,unsigned int longitud){
RGBQUAD *palette = imagen.getPalette();
std::string mensaje;

unsigned int bits_procesados=0;
if(palette) {
       unsigned int palette_size=imagen.getPaletteSize();
       for(unsigned int i=first;i<palette_size;i++){      
          if(bits_procesados<longitud){
              mensaje.append(1,getLSB((int)palette[i].rgbRed)); 
              bits_procesados++;
              
   	      }else i=palette_size;
          if(bits_procesados<longitud){
       	      mensaje.append(1,getLSB((int)palette[i].rgbGreen)); 
              bits_procesados++;
             
          }else i=palette_size;
          if(bits_procesados<longitud){
          	  mensaje.append(1,getLSB((int)palette[i].rgbBlue));
              bits_procesados++;
             
          }else i=palette_size;
       }
  }  
	return mensaje;
}

 
unsigned int ImagePalette::doIndexesLSB(Pixel& pixel,std::string mensaje){

/*Cantidad de bits que ya se han insertado en la imagen*/
unsigned int bits_procesados=0;
/*Posicion inicial del mensaje dentro del pixel*/
unsigned int pos=pixel.getNumero_de_bit();
BYTE pixel_index,new_pixel_index;
unsigned int height=imagen.getHeight();
unsigned int width=imagen.getWidth();
unsigned int bits_count=0;
 /*Me posiciono desde el comienzo de la imagen*/
std::string binario;char* aux;

for (unsigned int y = pixel.getPosY(); y <height; y ++){
		  
	for (unsigned int x = pixel.getPosX(); x < width; x ++){
		    
         if(bits_procesados<mensaje.size()){ 
         	pixel_index=imagen.getPixelIndex(x,height-y-1);
            //std::cout<<"Pixel_index"<<(int)pixel_index<<std::endl;
		    util::BitsUtils::toBase((int)pixel_index,2,binario);
            /*Completo el binario para que sea mas facil el proceso*/
            util::BitsUtils::completeNBits(binario,8);
            /*Ciclo en caso de modificar mas de un bit por byte*/
            binario.at(pos)=mensaje.at(bits_procesados); 
            bits_procesados++;
            if(bits_procesados==1) bits_count++;
            else bits_count+=8;
            new_pixel_index= (BYTE)strtol(binario.c_str(),&aux,2);	          
            binario="";
                                
            imagen.setPixelIndex(x,height-y-1,&new_pixel_index);
             
         }else{ //para terminar el ciclo for 
            	  x=width;
            	  y=height;
          }
		}//fin for_x
      
		
	}//fin for_y
    
	return bits_count+7;//Sumo 7 para caer justo en el lsb del prox indice
}




ImagePalette::~ImagePalette()
{
}
