#include "GIFStego.h"

GIFStego::GIFStego(std::string filename):LSBStegoBusiness(filename)
{
	 this->format=FIF_GIF;
}

GIFStego::~GIFStego()
{
}
