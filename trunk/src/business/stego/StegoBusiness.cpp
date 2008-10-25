#include "StegoBusiness.h"

StegoBusiness::StegoBusiness(){};

StegoBusiness::StegoBusiness(std::string filename):filename(filename)
{
	
}
std::string& StegoBusiness::getFilename()
{
 return this->filename;	
}


void StegoBusiness::setFilename(std::string file)
{
  this->filename=file;
}


StegoBusiness::~StegoBusiness()
{
}
