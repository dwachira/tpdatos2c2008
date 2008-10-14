#include "BMPStego.h"

BMPStego::BMPStego(std::string filename):LSBStegoBusiness(filename)
{
 this->format=FIF_BMP;
}

BMPStego::~BMPStego()
{
}
