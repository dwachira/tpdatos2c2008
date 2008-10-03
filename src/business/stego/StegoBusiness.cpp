#include "StegoBusiness.h"

StegoBusiness::StegoBusiness()
{
	
}
std::string& StegoBusiness::getFilename()
{
 return this->filename;	
}

FREE_IMAGE_FORMAT StegoBusiness::getImageFormat()
{
 return this->format;	
}

void StegoBusiness::setFilename(std::string file)
{
  this->filename=file;
}

void StegoBusiness::setFormat(FREE_IMAGE_FORMAT f)
{
  this->format=f;
}
StegoBusiness::~StegoBusiness()
{
}
