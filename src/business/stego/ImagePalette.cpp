#include "ImagePalette.h"


ImagePalette::ImagePalette(FIBitmap &imagen):imagen(imagen){

}
unsigned int ImagePalette::getNewPaletteIndex(unsigned int index)const{
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
double min=MAX_DISTANCE;
double min_primero,min_segundo;
unsigned int k;
unsigned int color_i,color_j;
unsigned long int red,green,blue;

RGBQUAD *palette = imagen.getPalette();
if(palette) {

	   /*Inicializo la matriz de distancias entre colores*/
       unsigned int palette_size=imagen.getPaletteSize();
       for (unsigned int i = 0; i < palette_size; i++){
         for (unsigned int j = 0; j < palette_size; j++) {
        	 if(i==j)	distancias[i][j]=256;
        	 else  distancias[i][j]=0;
           }
       }

     /*Calculo la distancias entre los colores de la paleta*/
     for (unsigned int i = 0; i < palette_size; i++) {

        /*Calculo las distancias del color i a los colores j*/
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

     unsigned int color_primero,color_segundo,color_k;
     /*Reordeno la paleta de colores en funcion de las distancias*/
      while(new_palette_indexes.size()<palette_size){//mientras me queden colores x analizar
        min_primero=MAX_DISTANCE*2; color_i=0;k=color_j;
        /* Busco el siguiente color menor, de manera tal que la distancia de este color al siguiente
         * y del siguiente al siguiente sea minima-->
         * minimizar (distancia(k,color_j)+distancia(k,color_j+1))*/
        while(color_i<palette_size){

           if((k!=color_i)&&(distancias[k][color_i]<min_primero)&&(std::find(new_palette_indexes.begin(),new_palette_indexes.end(),color_i)==new_palette_indexes.end())){
           	  min_primero=distancias[k][color_i];
           	  color_primero=color_i;
           	  color_k=0;
           	  min_segundo=MAX_DISTANCE*2;
           	  /*Busco el siguiente menor con respecto al actual: k*/
           	  while(color_k<palette_size){
           	  	if((color_k!=color_i)&&(min_primero+distancias[color_i][color_k]<min_segundo)&&(std::find(new_palette_indexes.begin(),new_palette_indexes.end(),color_k)==new_palette_indexes.end())){
           	  		min_segundo=min_primero+distancias[color_i][color_k];
           	        color_segundo=color_k;
           	  	}
           	  	color_k++;
           	  }

           }
           color_i++;
        }
        new_palette_indexes.push_back(color_primero);
        new_palette_indexes.push_back(color_segundo);
      }

   }
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

sortPaletteByDistance();

unsigned int valor_x= pixel.getPosX();

for (unsigned int y = pixel.getPosY(); y <height; y ++){

	for (unsigned int x = valor_x; x < width; x ++){

         if(bits_procesados<size*8){
         	if(pos_bit_msj==8){ pos_bit_msj=0;pos_byte_msj++;}
         	pixel_index=imagen.getPixelIndex(x,height-y-1);
         	/*Busco el indice en la paleta ordenada*/
            pixel_index=(BYTE)getNewPaletteIndex(pixel_index);
       		/*Guardo un bit de informacion en el LSB del byte*/
       		bit= ((mensaje[pos_byte_msj])&(1<<pos_bit_msj))? 1:0 ;
       		new_pixel_index= (BYTE)util::BitsUtils::hideInByte((int)pixel_index,bit);
		    /*Busco el indice en la paleta original*/
		    new_pixel_index=(BYTE)new_palette_indexes.at(new_pixel_index);
            pos_bit_msj++;
            bits_procesados++;
            bits_count+=8;
            imagen.setPixelIndex(x,height-y-1,&new_pixel_index);

         }else{ //para terminar el ciclo for
            	  x=width;
            	  y=height;
          }
		}//fin for_x
      valor_x=0;

	}//fin for_y

    /*Guardo los cambios realizados en la imagen*/
	imagen.save();
	return bits_count;
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
unsigned int valor_x= pixel.getPosX();

sortPaletteByDistance();

	   for (unsigned int y = pixel.getPosY(); y <height; y ++){

		  for (unsigned int x = valor_x; x < width; x ++){

            if(bits_procesados<longitud*8){
             	pixel_index=imagen.getPixelIndex(x,height-y-1);
                /*Busco el indice en la paleta ordenada*/
                pixel_index=(BYTE)getNewPaletteIndex(pixel_index);
             	if(util::BitsUtils::getHidenBit((int)pixel_index)) byte_msj = byte_msj | (1<<pos_bit_msj);
      			pos_bit_msj++;
      			bits_procesados++;
      		    if(pos_bit_msj==8){//guardo un byte del msj
      	 			pos_bit_msj=0;
      	 			mensaje.push_back(byte_msj);
      	 			byte_msj = 0x0;
      			}

           }else{//para terminar el ciclo for
            	  x=width;
            	  y=height;
           }
		}//fin for_x
        valor_x=0;
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

           if(pos_bit_msj==8){pos_bit_msj=0;pos_byte_msj++;}//paso al siguiente byte del msj
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
       if(imagen.getFileFormat()==FIF_GIF)
         imagen.applyColorMapping(palette,i+1,first_palette_pos);
       else
         imagen.applyColorMapping(imagen.getPalette(),palette);


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

if(palette) {

       while(bits_procesados<longitud*8){

       		if(((bits_procesados==0)&&(rgb_pos==0))||
           	   ((bits_procesados>0)&&(bits_procesados<longitud*8))){

           	   	if(util::BitsUtils::getHidenBit((int)palette[i].rgbRed))
           	   	    byte_msj = byte_msj | (1<<pos_bit_msj);

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

       	      	   	if(util::BitsUtils::getHidenBit((int)palette[i].rgbGreen))
       	      	   	   byte_msj = byte_msj | (1<<pos_bit_msj);
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

           	   	   	if(util::BitsUtils::getHidenBit((int)palette[i].rgbBlue))
           	   	   	   byte_msj = byte_msj | (1<<pos_bit_msj);
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





ImagePalette::~ImagePalette()
{
}
