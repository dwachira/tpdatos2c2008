#ifndef STEGOBUSINESS_H_
#define STEGOBUSINESS_H_
#include <string>

#include "FreeImage.h"
#include "Pixel.h"
class StegoBusiness
{
protected:
    std::string filename;
    FREE_IMAGE_FORMAT format;

public:
	StegoBusiness(std::string filename);
	virtual bool setMensaje(Pixel& pixel,std::string mensaje)=0;
	virtual std::string getMensaje(Pixel& pixel,unsigned int longitud)=0;
  
	std::string& getFilename();
	FREE_IMAGE_FORMAT getImageFormat();
	void setFilename(std::string file);
	void setFormat(FREE_IMAGE_FORMAT f);
		
	virtual ~StegoBusiness();
};

#endif /*STEGOBUSINESS_H_*/
