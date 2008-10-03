#ifndef PIXEL_H_
#define PIXEL_H_

class Pixel
{
private:
    unsigned long int posX;
    unsigned long int posY;
    unsigned int numero_de_bit;
public:
	Pixel(unsigned long int x,unsigned long int y,unsigned int bit);
	unsigned long int getPosX();
	unsigned long int getPosY();
	unsigned int getNumero_de_bit();
	virtual ~Pixel();
};

#endif /*PIXEL_H_*/
