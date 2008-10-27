#ifndef PIXEL_H_
#define PIXEL_H_

class Pixel
{
private:
    unsigned long int posX;
    unsigned long int posY;
    unsigned int numero_de_bit;
public:
	Pixel();
	Pixel(unsigned long int x,unsigned long int y,unsigned int bit);
	unsigned long int getPosX();
	unsigned long int getPosY();
	unsigned long int getNumero_de_bit();
	void setPosX(unsigned long int x);
	void setPosY(unsigned long int y);
	void setNumero_de_bit(unsigned long int bit);
	virtual ~Pixel();
};

#endif /*PIXEL_H_*/
