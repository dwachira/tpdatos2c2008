#include "JPGStego.h"

#include <Magick++.h>
using namespace Magick;
JPGStego::JPGStego(std::string filename):LossyCompressStegoBusiness(filename)
{

}
unsigned int JPGStego::getQuality()const{
unsigned int quality=0;
      Image im(filename.c_str());
      quality=im.quality();
	  return quality;
}
JPGStego::~JPGStego()
{
}
