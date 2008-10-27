#include "StegoFactory.h"

StegoFactory::StegoFactory()
{
}

StegoBusiness* StegoFactory::newInstance(const std::string filename){
StegoBusiness* stego=NULL;
FIBitmap fibitmap(filename);


	switch (fibitmap.getFileFormat()){
	  case FIF_BMP:
	        		stego=new BMPStego(filename);
	        		break;
	  case FIF_GIF:
	  				stego=new GIFStego(filename);
	  				break;
	  case FIF_PNG:
	  				stego=new PNGStego(filename);
	  				break;
	  case FIF_JPEG:
	  				stego=new JPGStego(filename);
	  				break;

	  }

  return stego;
}

StegoFactory::~StegoFactory()
{
}
