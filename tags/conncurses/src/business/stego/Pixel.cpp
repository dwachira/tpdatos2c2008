#include "Pixel.h"

Pixel::Pixel(unsigned long int x,unsigned long int y,unsigned int bit):
posX(x),posY(y),numero_de_bit(bit){

}
unsigned long int Pixel::getPosX()const{
 return this->posX;
}

unsigned long int Pixel::getPosY()const{
 return this->posY;
}
unsigned long int Pixel::getNumero_de_bit()const{
 return this->numero_de_bit;
}

void Pixel::setPosX(unsigned long int x){
	this->posX=x;
}

void Pixel::setPosY(unsigned long int y){
	this->posY=y;
}

void Pixel::setNumero_de_bit(unsigned long int bit){
	this->numero_de_bit=bit;
}

Pixel::~Pixel()
{
}
