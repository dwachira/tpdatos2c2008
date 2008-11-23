#ifndef STEGOBUSINESS_H_
#define STEGOBUSINESS_H_
#include <string>

#include "FIBitmap.h"

/**
 * StegoBusiness.h
 * Definicion de los metodos necesarios para llevar a cabo la esteganografia
 * Los metodos declarados son definidos por las clases hijas en funcion de las 
 * caracteristicas propias de cada tipo de imagen
 */
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
	virtual unsigned long int setMessage(unsigned long int first_bit,const char* mensaje,unsigned long int size)=0;
	virtual std::string getMessage(unsigned long int first_bit,unsigned long int longitud)=0;
    virtual unsigned long int getFreeSpace()=0;
    virtual unsigned int getFirstFreeBit()=0;
    
	std::string& getFilename();
	void setFilename(std::string file);
			
	virtual ~StegoBusiness();
};
#endif /*STEGOBUSINESS_H_*/
