#include "ImagePalette.h"
#include <fstream>


ImagePalette::ImagePalette(FIBitmap &imagen):imagen(imagen){
	
}
unsigned int ImagePalette::getNewPaletteIndex(unsigned int index){
 bool found=false;unsigned int i=0;
 if(std::find(new_palette_indexes.begin(),new_palette_indexes.end(),index)==new_palette_indexes.end())
   std::cout<<"no encontrado: "<<index<<std::endl;
 //std::cout<<"index"<<index<<std::endl;
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
             
/*Reordeno la paleta de colores */
RGBQUAD *palette = imagen.getPalette();
if(palette) {
       unsigned int palette_size=imagen.getPaletteSize();
       
       std::vector<RGBColor> paleta_original;
       /*Inicializo matriz de distancias*/
       
       for (unsigned int i = 0; i < palette_size; i++){
         for (unsigned int j = 0; j < palette_size; j++) {
        	 if(i==j)	distancias[i][j]=256;
        	 else  distancias[i][j]=0;
           }
       }
       std::ofstream f("distancias.txt");
     /*Calculo la distancias entre los colores de la paleta*/
     for (unsigned int i = 0; i < palette_size; i++) {
        paleta_original.push_back(RGBColor((int)palette[i].rgbRed,(int)palette[i].rgbGreen,(int)palette[i].rgbBlue));
        
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
      std::vector<unsigned int>::iterator it;unsigned int i=0;
      std::ofstream file("paleta.txt");
      for(it=new_palette_indexes.begin();it!=new_palette_indexes.end();it++){
            file<<"paleta actual: "<<i;
            file<<" paleta original: "<<(*it);
            file<<std::endl;
            palette[i].rgbRed =(BYTE)paleta_original.at(*it).getRed(); 
            palette[i].rgbGreen=(BYTE)paleta_original.at(*it).getGreen(); 
            palette[i].rgbBlue=(BYTE)paleta_original.at(*it).getBlue();
            //palette[i].rgbReserved=(BYTE)15;
            i++;
        
        }
        if (imagen.isTransparent()){
        	int transparent_index;
        	transparent_index=imagen.getTransparentIndex();
        	std::cout<<"indice_transparente"<<transparent_index<<std::endl;
   			unsigned int transparentes=imagen.getTransparencyCount();
   			std::cout<<"transparencia "<<transparentes<<std::endl;
   			BYTE* paleta_transparente;
   			paleta_transparente=imagen.getTransparencyTable();
		}
		if(imagen.hasBackgroundColor()){ 
	  	
	  	RGBQUAD* background=imagen.getBackgroundColor();
       	 //FreeImage_GetBackgroundColor(imagen,&background);
       	std::cout<<"background colour"<<(int)background->rgbRed<<"-"<<(int)background->rgbGreen<<"-"<<(int)background->rgbBlue<<std::endl; 	
       	}
        
        std::cout<<"Tamaño de la paleta"<<palette_size<<std::endl;
       file.close();
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
void ImagePalette::doPaletteLSB(unsigned int first,std::string mensaje){
RGBQUAD *palette = imagen.getPalette();

unsigned int bits_procesados=0;
if(palette) {
       unsigned int palette_size=imagen.getPaletteSize();
       for(unsigned int i=first;i<palette_size;i++){      
           if(bits_procesados<mensaje.size()){
              palette[i].rgbRed=(BYTE)doLSB((int)palette[i].rgbRed,mensaje.at(bits_procesados));
              bits_procesados++;
   	       }else i=palette_size;
   	       
   	       if(bits_procesados<mensaje.size()){
              palette[i].rgbGreen=(BYTE)doLSB((int)palette[i].rgbGreen,mensaje.at(bits_procesados));
              bits_procesados++;
              
   	       }else i=palette_size;
   	       if(bits_procesados<mensaje.size()){
              palette[i].rgbBlue=(BYTE)doLSB((int)palette[i].rgbBlue,mensaje.at(bits_procesados));
              bits_procesados++;
            
   	      }else i=palette_size;
       
       }
  }  
	
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

void ImagePalette::doIndexesLSB(Pixel& pixel,std::string mensaje){

/*Cantidad de bits que ya se han insertado en la imagen*/
unsigned int bits_procesados=0;
/*Posicion inicial del mensaje dentro del pixel*/
unsigned int pos=pixel.getNumero_de_bit();
BYTE pixel_index,new_pixel_index;
unsigned int height=imagen.getHeight();
unsigned int width=imagen.getWidth();
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
            new_pixel_index= (BYTE)strtol(binario.c_str(),&aux,2);	          
            binario="";
                                
            imagen.setPixelIndex(x,height-y-1,&new_pixel_index);
             
         }else{ //para terminar el ciclo for 
            	  x=width;
            	  y=height;
          }
		}//fin for_x
      
		
	}//fin for_y
    
	
}


void ImagePalette::updateIndexes(){
BYTE pixel_index,new_pixel_index;
unsigned int height=imagen.getHeight();
unsigned int width=imagen.getWidth();

std::ofstream file("indices.txt");
       for (unsigned int y = 0; y <height; y ++){
		  
		  for (unsigned int x = 0; x < width; x ++){
		        pixel_index=imagen.getPixelIndex(x,height-y-1);
                new_pixel_index=(BYTE)getNewPaletteIndex((int)pixel_index);
                file<<"el indice de antes: "<<(int)pixel_index<<" el nuevo indice"<<(int)new_pixel_index<<std::endl;
           	    imagen.setPixelIndex(x,height-y-1,&new_pixel_index);
          }//fin for_x
       }//fin for_y
       file.close();
       
	  
	 
}

ImagePalette::~ImagePalette()
{
}
