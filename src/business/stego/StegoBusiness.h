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
	
	/*A implementar por las clases hijas: */
	virtual bool setMessage(Pixel& pixel,std::string mensaje)=0;
	virtual std::string getMessage(Pixel& pixel,unsigned int longitud)=0;
    virtual unsigned int getFreeSpace()=0;
    
	std::string& getFilename();
	FREE_IMAGE_FORMAT getImageFormat();
	void setFilename(std::string file);
	void setFormat(FREE_IMAGE_FORMAT f);
		
	virtual ~StegoBusiness();
};
#endif /*STEGOBUSINESS_H_*/
