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
           static int hideInByte(int byte,int bit){
					int lsb=(byte&1);
					int result=byte;

 					if(lsb!=bit){
 						if(bit==1){ result++;}
 						else result--;
 					}return result;	
	
			}
           static int getHidenBit(int byte){
           	   return (byte&1);	
           	
           }
};
}
#endif /*BITSUTILS_H_*/
