#include "Pixel.h"

Pixel::Pixel(unsigned long int x,unsigned long int y,unsigned int bit):
posX(x),posY(y),numero_de_bit(bit){
	
}
unsigned long int Pixel::getPosX(){
 return this->posX;	
}

unsigned long int Pixel::getPosY(){
 return this->posY;	
}
unsigned int Pixel::getNumero_de_bit(){
 return this->numero_de_bit;	
}

Pixel::~Pixel()
{
}
