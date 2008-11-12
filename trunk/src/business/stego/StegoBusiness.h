#ifndef STEGOBUSINESS_H_
#define STEGOBUSINESS_H_
#include <string>


#include "Pixel.h"
#include "FIBitmap.h"

class StegoBusiness
{
private:
    StegoBusiness(const StegoBusiness&);
    StegoBusiness & operator=(const StegoBusiness&);
protected:
    std::string filename;

public:
    StegoBusiness();
  
	StegoBusiness(std::string filename);
	
	/*A implementar por las clases hijas: */
	virtual unsigned int setMessage(unsigned long int first_bit,const char* mensaje)=0;
	virtual std::string getMessage(unsigned long int first_bit,unsigned int longitud)=0;
    virtual unsigned int getFreeSpace()=0;
    virtual unsigned int getFirstFreeBit()=0;
    
	std::string& getFilename();
	void setFilename(std::string file);
			
	virtual ~StegoBusiness();
};
#endif /*STEGOBUSINESS_H_*/
