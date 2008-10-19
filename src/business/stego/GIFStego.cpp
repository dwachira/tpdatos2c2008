#include "GIFStego.h"

GIFStego::GIFStego(std::string filename):LSBStegoBusiness(filename)
{
	 this->format=FIF_GIF;
	 loadImagen();
}

unsigned int GIFStego::getFreeSpace(){
 return ((this->height)*(this->width)*(this->bpp)*(this->enable_bpp));	
	
}

unsigned int GIFStego::getIndex(std::vector<int>& vec,int elem){
 bool found=false;unsigned int i=0;
 
 while((!found)&&(i<vec.size())){
   	if(vec.at(i)==elem)
   	 found=true;
   	else i++;
 }	
 std::cout<<"antes indice era "<<elem<<"ahora es "<<i<<std::endl;
 return i;
 }
 
bool GIFStego::setMessage(Pixel& pixel,std::string mensaje)
{

/*Cantidad de bits que ya se han insertado en la imagen*/
unsigned int bits_procesados=0;

/*Posicion inicial del mensaje dentro del pixel*/
//unsigned int pos=pixel.getNumero_de_bit();
//double distancias[256][256];

std::vector<int> colores;
//int min=256;
BYTE pixel_index,new_pixel_index;
//unsigned int k;
//unsigned int color_i,color_j;

if(!error){
  	  
  	   //unsigned long int red,green,blue;
       std::cout<<"se crea la imagen de bppppp = "<<bpp<<std::endl;
       std::cout<<"color_type = "<<color_type<<std::endl;
       if (FreeImage_IsTransparent(imagen)){
       	unsigned int transparentes=FreeImage_GetTransparencyCount(imagen);
       	std::cout<<"transparencia "<<transparentes<<std::endl;
       	BYTE* paleta_transparente;
       	paleta_transparente=FreeImage_GetTransparencyTable(imagen);
       	
       	for(unsigned int i=0;i<transparentes;i++)
       	   std::cout<<(int)paleta_transparente[i]<<std::endl;
       	
       }/*else{
       unsigned int tamanio_paleta=FreeImage_GetColorsUsed(imagen);
       std::cout<<"Tamaño de la paleta"<<tamanio_paleta<<std::endl;
       /*Reordeno la paleta de colores por luminancia*/
      /*  RGBQUAD *pal = FreeImage_GetPalette(imagen);
        if(!pal) std::cout<<"no hay paleta"<<std::endl;
        std::vector<RGBColor> paleta_original;
        /*Inicializo matriz de distancias*/
     /*   for (unsigned int i = 0; i < tamanio_paleta; i++){
        	 
        	 for (unsigned int j = 0; j < tamanio_paleta; j++) {
        	     if(i==j)	distancias[i][j]=256;
        	     else  distancias[i][j]=0;
        	 }
        }
       
        /*Calculo la distancias entre los colores de la paleta*/
     /*   for (unsigned int i = 0; i < tamanio_paleta; i++) {
        paleta_original.push_back(RGBColor((int)pal[i].rgbRed,(int)pal[i].rgbGreen,(int)pal[i].rgbBlue));
        
           for (unsigned int j = i+1; j < tamanio_paleta; j++) {
              red=pow(((int)pal[i].rgbRed-(int)pal[j].rgbRed),2);
              green=pow(((int)pal[i].rgbGreen-(int)pal[j].rgbGreen),2);
              blue=pow(((int)pal[i].rgbBlue-(int)pal[j].rgbBlue),2);
              distancias[i][j]=sqrt(red+green+blue);
              distancias[j][i]=distancias[i][j];
              if(distancias[i][j]<min){
              	min=distancias[i][j];
              	color_i=i;color_j=j;
              }
           }
        }
         /*Color ordenado por menor distancia*/
     /*   colores.push_back(color_i);
        colores.push_back(color_j);
        /*Reordeno la paleta de colores en funcion de las distancias*/
     /*  while(colores.size()<tamanio_paleta){
        min=256; color_i=0;k=color_j;
        /*Busco el siguiente color menor*/
      /*  while(color_i<tamanio_paleta){
           
           if((distancias[k][color_i]<min)&&(std::find(colores.begin(),colores.end(),color_i)==colores.end())){
           	 min=distancias[k][color_i];
           	 color_j=color_i;
           }
           color_i++;  
        }
        colores.push_back(color_j);
        }
        std::vector<int>::iterator it;unsigned int i=0;
        for(it=colores.begin();it!=colores.end();it++){
            
            pal[i].rgbRed =(BYTE)paleta_original.at(*it).getRed(); 
            pal[i].rgbGreen=(BYTE)paleta_original.at(*it).getGreen(); 
            pal[i].rgbBlue=(BYTE)paleta_original.at(*it).getBlue();
            i++;
        
        }
       
       } 
	*/
	  BYTE *bits = (BYTE*)FreeImage_GetBits(imagen);
       /*Me posiciono desde el comienzo de la imagen*/
       bits+=pitch*(height-1);
	std::string binario;char* aux;
	  for (unsigned int y = pixel.getPosY(); y <height; y ++){
		  /*Primer linea de pixels de la imagen*/
		  //BYTE *pixels = (BYTE*)bits;
		  for (unsigned int x = pixel.getPosX(); x < width; x ++){
		    
             if(bits_procesados<mensaje.size()){ 
               FreeImage_GetPixelIndex(imagen,x,height-y-1,&pixel_index);
               std::cout<<"Pixel_index"<<(int)pixel_index<<std::endl;
		       util::BitsUtils::toBase((int)pixel_index,2,binario);
               /*Completo el binario para que sea mas facil el proceso*/
               util::BitsUtils::completeNBits(binario,8);
               /*Ciclo en caso de modificar mas de un bit por byte*/
               binario.at(7)=mensaje.at(bits_procesados); 
               bits_procesados++;
               new_pixel_index=  strtol(binario.c_str(),&aux,2);	          
               binario="";
		   	   FreeImage_SetPixelIndex(imagen,x,height-y-1,&new_pixel_index);
              
             	   
               
            }else{ //para terminar el ciclo for 
            	  x=width;
            	  y=height;
             }
		}//fin for_x
      bits -= pitch;//siguiente linea de la imagen
		
	}//fin for_y
    
	/*Guardo los cambios realizados en la imagen*/
	FreeImage_Save(this->format,imagen,this->filename.c_str(),0);
	
    return true;
  }
  return false;	
}

std::string GIFStego::getMessage(Pixel& pixel,unsigned int longitud){
std::string mensaje;	

unsigned int bits_procesados=0;
/*posicion inicial*/
//unsigned int pos=pixel.getNumero_de_bit();
BYTE value;std::string binario;
if(!error){
  	
	   for (unsigned int y = pixel.getPosY(); y <height; y ++){
		  
		  for (unsigned int x = pixel.getPosX(); x < width; x ++){
		 	   
            if(bits_procesados<longitud){ 
            	            	
             	   FreeImage_GetPixelIndex(imagen,x,height-y-1,&value);
             	   util::BitsUtils::toBase((int)value,2,binario);
                   /*Completo el binario para que sea mas facil el proceso*/
                   util::BitsUtils::completeNBits(binario,8);
                   mensaje.append(1,binario.at(7));
                   binario="";
                   bits_procesados++;
                
           }else{//para terminar el ciclo for 
            	  x=width;
            	  y=height;
           }
		}//fin for_x
        
	}//fin for_y
	 
  }
	
return mensaje;	
}

GIFStego::~GIFStego(){
}
