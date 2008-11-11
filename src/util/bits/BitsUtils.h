#ifndef BITSUTILS_H_
#define BITSUTILS_H_

#include  "../string/StringUtils.h"

namespace util
{
class BitsUtils
{
	public:
	       BitsUtils();
	       virtual ~BitsUtils();
	       static void toBase(int num,int base,std::string&  binary);
           static void completeNBits(std::string& binary,unsigned int n);
           static int hideInByte(int byte,int bit,unsigned int pos);
           static int getHidenBit(int byte,unsigned int pos);
};
}
#endif /*BITSUTILS_H_*/