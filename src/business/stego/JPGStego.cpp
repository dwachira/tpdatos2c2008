#include "JPGStego.h"

JPGStego::JPGStego(std::string filename):LossyCompressStegoBusiness(filename)
{
	 this->format=FIF_JPEG;
}

JPGStego::~JPGStego()
{
}
