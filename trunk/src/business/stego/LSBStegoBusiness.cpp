#include "LSBStegoBusiness.h"
#include <iostream>


LSBStegoBusiness::LSBStegoBusiness(std::string filename):StegoBusiness(filename),
error(false),enable_bpp(1),imagen(filename),palette(imagen),bits_procesados(0),
pos_bit_msj(0),pos_byte_msj(0),pos_pixel(0),byte_msj(0x0)
{

	error=imagen.load();
    if(!error) max_pos_pixel=imagen.getBpp()/8;
}

//si uso el lsb sobre la paleta --->devolver cero
unsigned int LSBStegoBusiness::getFirstFreeBit(){
	//solo se aplicara lsb a la paleta si esta posee un tamaño inferior a 16 colores
	//y no es en escala de grises
	if((imagen.getBpp()<=8)&&(!imagen.isGrayScale())&&(imagen.getPaletteSize()<=16)) return 0;
	return (8-this->enable_bpp);
}

void LSBStegoBusiness::getPixel(unsigned long int first_bit,Pixel& pixel){
unsigned int y=0;
unsigned int x=0;
unsigned int bits_count=0;
unsigned int bit=0;

unsigned int bpp;
if(imagen.getBpp()<8) bpp=1;
else bpp=imagen.getBpp()/8;
std::cout<<" bpp: "<<bpp<<std::endl;
  if(first_bit> (bpp*8-1)){//no estoy en caso inicial

	  while(( y <imagen.getHeight())&&(bits_count<first_bit)){
		  /*Primer linea de pixels de la imagen*/
		   x=0;
		   while(( x <imagen.getWidth())&&(bits_count<first_bit)){
		   	    //siguiente pixel
		   	    bits_count+=8*bpp;
		   	    if(bits_count>=first_bit)
		   	      bit= first_bit-(bits_count-8*bpp);
		   	    //siguiente pixel
		      	if(bits_count<first_bit) x++;

		}//fin while_x
      //siguiente linea de la imagen
	  if(bits_count<first_bit) y++;

	}//fin while_y
  } else bit= first_bit;
    pixel.setPosX(x);
    pixel.setPosY(y);
    pixel.setNumero_de_bit(bit);
    std::cout<<"x: "<<x<<" y: "<<y<<" bit: "<<bit<<std::endl;

}

std::string LSBStegoBusiness::getMessageFromPixel(BYTE *pixels,unsigned int longitud){
	return getLSBMessage(pixels,longitud);
}

std::string LSBStegoBusiness::getLSBMessage(BYTE *pixels,unsigned int longitud){
std::string mensaje;
int byte_pixel;

  while((pos_pixel<max_pos_pixel)&&(bits_procesados<longitud)){

      byte_pixel=(int)pixels[pos_pixel];
      if((byte_pixel&1)==1) byte_msj = byte_msj | (1<<pos_bit_msj);
      pos_bit_msj++;
      bits_procesados++;
      pos_pixel++;//paso al byte siguiente
      if(pos_bit_msj==8){
      	 pos_bit_msj=0;
      	 mensaje.push_back(byte_msj);
      	 byte_msj = 0x0;
      }
  }//while de los bytes de un pixel

      return mensaje;
}

unsigned int LSBStegoBusiness::changePixel(BYTE *pixels,const char* mensaje, long int size){
	return doLSBStego(pixels,mensaje,size);
}

unsigned int LSBStegoBusiness::doLSBStego(BYTE *pixels,const char* mensaje, long int size){
unsigned int bits_count=0;
int bit,lsb;
int new_byte;

/*Mientras queden bytes por pixel para recorrer*/
while((pos_pixel<max_pos_pixel)&&(bits_procesados<size*8)){

   	 if(pos_bit_msj==8){
   	 		pos_bit_msj=0;
   	 		pos_byte_msj++;
   	 }
     /*Guardo un bit de informacion en el LSB del byte*/
     if ((mensaje[pos_byte_msj])&(1<<pos_bit_msj)) bit=1;
     else bit=0;

     lsb= (((int)pixels[pos_pixel])&1);
     pos_bit_msj++;
     bits_procesados++;
     bits_count+=8;

    /*Guardo el nuevo byte modificado*/
    if(lsb!=bit){
  		if(bit==1) new_byte=(int)pixels[pos_pixel]+1;
  		else new_byte=(int)pixels[pos_pixel]-1;
  		pixels[pos_pixel]=(BYTE)new_byte;
  	}

  	pos_pixel++;//paso al byte siguiente

   }
  if((pos_pixel==3)&&(max_pos_pixel==(imagen.getBpp()/8 - 1)))
     bits_count+=8;//caso de PNG:  omitir el byte transparente

  return bits_count;

}


/**
 * Permite ocultar el mensaje correspondiente dentro de la imagen
 *
 */
unsigned int LSBStegoBusiness::setMessage(unsigned long int first_pos,const char* mensaje, long int size)
{

Pixel pixel;
unsigned int last_pos=0;
getPixel(first_pos,pixel);
/*Posicion inicial del mensaje dentro del pixel*/
pos_pixel=pixel.getNumero_de_bit()/8;
if(!error){

       if((imagen.getBpp()<=8)&&(!imagen.isGrayScale())){
           if(imagen.getPaletteSize()>16) last_pos+=palette.doIndexesLSB(pixel,mensaje,size);
           else last_pos=palette.doPaletteLSB(first_pos,mensaje,size);
       }
       else{//se puede trabajar con los pixeles

       	BYTE *bits = imagen.getBits();
       	/*Me posiciono desde el comienzo de la imagen*/
       	bits+=imagen.getPitch()*(imagen.getHeight()-1);
       	bits -= imagen.getPitch()*pixel.getPosY();//Me posiciono en la linea correspondiente

	   	for (unsigned int y = pixel.getPosY(); y <imagen.getHeight(); y ++){
			  /*Primer linea de pixels de la imagen*/
			  BYTE *pixels = (BYTE*)bits;
			  //Me posiciono en el pixel correspondiente
		  	  if(bits_procesados==0) pixels += (imagen.getBpp()/8)*pixel.getPosX();

		  	  for (unsigned int x = pixel.getPosX(); x < imagen.getWidth(); x ++){
		          if(bits_procesados<size*8){
             	    last_pos+=changePixel(pixels,mensaje,size);
             	    pos_pixel=0;//para reiniciar el ciclo
             	   	pixels += (imagen.getBpp()/8);//siguiente pixel
               }else{ //para terminar el ciclo for
             		  x=imagen.getWidth();
            		  y=imagen.getHeight();
               }
		      }//fin for_x
              bits -= imagen.getPitch();//siguiente linea de la imagen

	     }//fin for_y
     }
	/*Guardo los cambios realizados en la imagen*/
	imagen.save();
  }

  return last_pos+first_pos;
}

std::string LSBStegoBusiness::getMessage(unsigned long int first_pos,unsigned int longitud){
std::string mensaje;
Pixel pixel;
getPixel(first_pos,pixel);
unsigned int bits_procesados=0;
/*posicion inicial*/
pos_pixel=pixel.getNumero_de_bit()/8;

if(!error){

      if((imagen.getBpp()<=8)&&(!imagen.isGrayScale()))
      	    if(imagen.getPaletteSize()>16) mensaje.append(palette.getMessageFromIndexes(pixel,longitud*8));
      	    else mensaje.append(palette.getMessageFromPalette(first_pos,longitud*8));
      else{
       		BYTE *bits = imagen.getBits();
	   		bits+=imagen.getPitch()*(imagen.getHeight()-1);//primera linea
	   		bits -= imagen.getPitch()*pixel.getPosY();//Me posiciono en la linea correspondiente
	   		for (unsigned int y = pixel.getPosY(); y <imagen.getHeight(); y ++){
		  		BYTE *pixels = (BYTE*)bits;
		  		//Me posiciono en el pixel correspondiente
		  		if(bits_procesados==0) pixels += (imagen.getBpp()/8)*pixel.getPosX();
		  		for (unsigned int x = pixel.getPosX(); x < imagen.getWidth(); x ++){

            	  if(bits_procesados<longitud*8){
            	   	mensaje.append(getMessageFromPixel(pixels,longitud*8));
            	   	pos_pixel=0;//para reiniciar el ciclo
                   	pixels += (imagen.getBpp()/8);

           		  }else{//para terminar el ciclo for
            	  		x=imagen.getWidth();
            	  		y=imagen.getHeight();
                  }
		        }//fin for_x
                bits -= imagen.getPitch();//proxima linea de la imagen
	         }//fin for_y

      }
 }
return mensaje;
}

LSBStegoBusiness::~LSBStegoBusiness()
{

}
