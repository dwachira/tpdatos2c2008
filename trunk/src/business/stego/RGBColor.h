#ifndef RGBCOLOR_H_
#define RGBCOLOR_H_

class RGBColor
{
private:
    int red,green,blue;
public:
    RGBColor();
	RGBColor(int red,int green,int blue);
	int getRed(){return this->red;}
	int getGreen(){return this->green;}
	int getBlue(){return this->blue;}
	virtual ~RGBColor();
};

#endif /*RGBCOLOR_H_*/
