#include "JPGStego.h"

JPGStego::JPGStego(std::string filename):LossyCompressStegoBusiness(filename,FIF_JPEG)
{
	
}

JPGStego::~JPGStego()
{
}
