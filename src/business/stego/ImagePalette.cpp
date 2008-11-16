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
      std::ofstream file("paleta.txt");
     /*Calculo la distancias entre los colores de la paleta*/
     for (unsigned int i = 0; i < palette_size; i++) {
     	if((palette[i].rgbRed==background.rgbRed)&&(palette[i].rgbGreen==background.rgbGreen)&&(palette[i].rgbBlue==background.rgbBlue))
     	    background_index=i;
     	file<<(int)palette[i].rgbRed<<"-"<<(int)palette[i].rgbGreen<<"-"<<(int)palette[i].rgbBlue<<std::endl;    
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
           file.close();
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
        RGBQUAD dstcolors[256];
        std::vector<unsigned int>::iterator it;unsigned int i=0;
        
        for(it=new_palette_indexes.begin();it!=new_palette_indexes.end();it++){
            
            dstcolors[i].rgbRed=palette[*it].rgbRed;
            dstcolors[i].rgbGreen=palette[*it].rgbGreen;
            dstcolors[i].rgbBlue=palette[*it].rgbBlue;
           
            i++;
            
        }
      
        imagen.applyColorMapping(palette,dstcolors);
        
        /*Ordeno los indices*/
        BYTE srcindices[256];BYTE dstindices[256];
		for(unsigned int j=0;j<palette_size;j++){
			srcindices[j]=(BYTE)j;
		    dstindices[j]=(BYTE)getNewPaletteIndex(j);
		}
		std::cout<<"pixels modif "<<imagen.applyPaletteIndexMapping(srcindices,dstindices)<<std::endl;
      
        if (imagen.isTransparent()){
        	int transparent_index;
        	transparent_index=imagen.getTransparentIndex();
        	std::cout<<"indice_transparente"<<transparent_index<<std::endl;
   			imagen.setTransparentIndex(getNewPaletteIndex(transparent_index));
		}	
	    
        /*Guardo los cambios en los indices*/
        imagen.save(); 
        /*Guardo los cambios en la paleta de colores*/
        imagen.applyColorMapping(dstcolors,imagen.getPaletteSize());
        if(imagen.hasBackgroundColor())  	
	  	    imagen.setBackgroundColorIndex(getNewPaletteIndex(background_index));
	  	std::cout<<"PALETA ORDENADA"<<std::endl;
   }
}


std::string ImagePalette::getMessageFromIndexes(Pixel& pixel,unsigned long int longitud){
/*Cantidad de bits que ya se han insertado en la imagen*/
unsigned int bits_procesados=0;
BYTE pixel_index;
unsigned int height= imagen.getHeight();
unsigned int width=imagen.getWidth();
std::string mensaje;
unsigned byte_msj=0x0;
unsigned int pos_bit_msj=0;

	   for (unsigned int y = pixel.getPosY(); y <height; y ++){
		  
		  for (unsigned int x = pixel.getPosX(); x < width; x ++){
		 	   
            if(bits_procesados<longitud*8){ 
            	   //height-y-1
             	pixel_index=imagen.getPixelIndex(x,height-y-1);

             	if(util::BitsUtils::getHidenBit((int)pixel_index)) byte_msj = byte_msj | (1<<pos_bit_msj);
      			pos_bit_msj++;      
      			bits_procesados++;
      		    if(pos_bit_msj==8){
      	 			pos_bit_msj=0;
      	 			mensaje.push_back(byte_msj);
      	 			byte_msj = 0x0;
      			}
               
                
           }else{//para terminar el ciclo for 
            	  x=width;
            	  y=height;
           }
		}//fin for_x
        
	}//fin for_y
	
    return mensaje;
}


unsigned int ImagePalette::getRGBPos(unsigned int pos){

if((pos%3)==0) return 0;
if(((pos-1)%3)==0) return 1;
return 2;
	
	
}

//aca first bit seria el numero dentro de la paleta
unsigned int ImagePalette::doPaletteLSB(unsigned int first_pos,const char* mensaje,unsigned long int size){
RGBQUAD *palette = imagen.getPalette();
unsigned int bits_procesados=0;
unsigned int first_palette_pos=first_pos/3;
unsigned int i=first_palette_pos;
unsigned int rgb_pos=getRGBPos(first_pos);//determina en que color empiezo
unsigned int pos_bit_msj=0;
unsigned int pos_byte_msj=0;
int bit;

if(palette) {
       unsigned int palette_size=imagen.getPaletteSize();
       while((i<palette_size)&&(bits_procesados<size*8)){    
       	if(((bits_procesados==0)&&(rgb_pos==0))||  
           ((bits_procesados>0)&&(bits_procesados<size*8))){
           	
           	 if(pos_bit_msj==8){pos_bit_msj=0;pos_byte_msj++;}
           	  /*Guardo un bit de informacion en el LSB del byte*/ 
           	   bit= ((mensaje[pos_byte_msj])&(1<<pos_bit_msj))? 1:0 ;   
           	   
       		   palette[i].rgbRed= (BYTE)util::BitsUtils::hideInByte((int)palette[i].rgbRed,bit);
		     
              bits_procesados++;
              pos_bit_msj++;
              
   	       }
   	       
   	       if(((bits_procesados==0)&&(rgb_pos==1))||  
           ((bits_procesados>0)&&(bits_procesados<size*8))){
           	  if(pos_bit_msj==8){pos_bit_msj=0;pos_byte_msj++;}
           	
              bit= ((mensaje[pos_byte_msj])&(1<<pos_bit_msj))? 1:0 ; 
		      palette[i].rgbGreen= (BYTE)util::BitsUtils::hideInByte((int)palette[i].rgbGreen,bit);
		    
              bits_procesados++;
              pos_bit_msj++;
              
              
   	       }
   	       	if(((bits_procesados==0)&&(rgb_pos==2))||  
           ((bits_procesados>0)&&(bits_procesados<size*8))){
           	 if(pos_bit_msj==8){pos_bit_msj=0;pos_byte_msj++;}
           	  
           	  bit= ((mensaje[pos_byte_msj])&(1<<pos_bit_msj))? 1:0 ; 
		      palette[i].rgbBlue= (BYTE)util::BitsUtils::hideInByte((int)palette[i].rgbBlue,bit);
              bits_procesados++;
              pos_bit_msj++;
              
              
   	       }
          if(bits_procesados<size*8) i++;
       }
       /*Actualizo la paleta de colores para que los cambios se apliquen a la imagen*/
       std::cout<<"voy a actualizar desde "<<first_palette_pos<<" hasta "<<i<<std::endl;
       imagen.applyColorMapping(palette,i+1,first_palette_pos);
       
  }  
	return bits_procesados+first_pos;
}

std::string ImagePalette::getMessageFromPalette(unsigned int first,unsigned long int longitud){
RGBQUAD *palette = imagen.getPalette();
std::string mensaje;
unsigned int bits_procesados=0;
unsigned int first_palette_pos=first/3;
unsigned int i=first_palette_pos;
unsigned int rgb_pos=getRGBPos(first);//determina en que color empiezo
unsigned byte_msj=0x0;
unsigned int pos_bit_msj=0;
std::cout<<"rgb_pos "<<rgb_pos<<std::endl;
if(palette) {
       unsigned int palette_size=imagen.getPaletteSize();
       while((i<palette_size)&&(bits_procesados<longitud*8)){    
       	
       		if(((bits_procesados==0)&&(rgb_pos==0))||  
           	   ((bits_procesados>0)&&(bits_procesados<longitud*8))){     
           	   	if(util::BitsUtils::getHidenBit((int)palette[i].rgbRed)) byte_msj = byte_msj | (1<<pos_bit_msj);
      				pos_bit_msj++;      
      				bits_procesados++;
      				if(pos_bit_msj==8){
      	 				pos_bit_msj=0;
      	 				mensaje.push_back(byte_msj);
      	 				byte_msj = 0x0;
      				}
           	   	   	
    	    }
        	if(((bits_procesados==0)&&(rgb_pos==1))||  
           	   ((bits_procesados>0)&&(bits_procesados<longitud*8))){  
       	      	   	if(util::BitsUtils::getHidenBit((int)palette[i].rgbGreen)) byte_msj = byte_msj | (1<<pos_bit_msj); byte_msj = byte_msj | (1<<pos_bit_msj);
      				pos_bit_msj++;      
      				bits_procesados++;
      				if(pos_bit_msj==8){
      	 				pos_bit_msj=0;
      	 				mensaje.push_back(byte_msj);
      	 				byte_msj = 0x0;
      				}
           	   	  
     	    }
         	if(((bits_procesados==0)&&(rgb_pos==2))||  
           	   ((bits_procesados>0)&&(bits_procesados<longitud*8))){  
           	   	   	if(util::BitsUtils::getHidenBit((int)palette[i].rgbBlue)) byte_msj = byte_msj | (1<<pos_bit_msj); byte_msj = byte_msj | (1<<pos_bit_msj);
      				pos_bit_msj++;      
      				bits_procesados++;
      				if(pos_bit_msj==8){
      	 				pos_bit_msj=0;
      	 				mensaje.push_back(byte_msj);
      	 				byte_msj = 0x0;
      				}
           	   	   
            }i++;//siguiente posicion de la paleta
       }
  }  
	return mensaje;
}

unsigned int ImagePalette::doIndexesLSB(Pixel& pixel,const char* mensaje,unsigned long int size){

/*Cantidad de bits que ya se han insertado en la imagen*/
unsigned int bits_procesados=0;
BYTE pixel_index,new_pixel_index;
unsigned int height=imagen.getHeight();
unsigned int width=imagen.getWidth();
unsigned int bits_count=0;
unsigned int pos_bit_msj=0;
unsigned int pos_byte_msj=0;
int bit;
 	
for (unsigned int y = pixel.getPosY(); y <height; y ++){
		  
	for (unsigned int x = pixel.getPosX(); x < width; x ++){
		    
         if(bits_procesados<size*8){ 
         	//height-y-1
         	if(pos_bit_msj==8){ pos_bit_msj=0;pos_byte_msj++;}
         	pixel_index=imagen.getPixelIndex(x,height-y-1);
            
       		/*Guardo un bit de informacion en el LSB del byte*/  
       		bit= ((mensaje[pos_byte_msj])&(1<<pos_bit_msj))? 1:0 ;  
       		new_pixel_index= (BYTE)util::BitsUtils::hideInByte((int)pixel_index,bit);
		  
            pos_bit_msj++;
            bits_procesados++;
            bits_count+=8;
            imagen.setPixelIndex(x,height-y-1,&new_pixel_index);
             
         }else{ //para terminar el ciclo for 
            	  x=width;
            	  y=height;
          }
		}//fin for_x
      
		
	}//fin for_y
	
    /*Guardo los cambios realizados en la imagen*/
	imagen.save();
	return bits_count;
}




ImagePalette::~ImagePalette()
{
}
