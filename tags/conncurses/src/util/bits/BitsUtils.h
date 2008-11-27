#ifndef BITSUTILS_H_
#define BITSUTILS_H_

#include  "../string/StringUtils.h"


namespace util
{
/**
 * Se utiliza para el manejo de bits y para realizar conversiones
 */
class BitsUtils
{

	public:
	       BitsUtils();
	       virtual ~BitsUtils();
	       /*Pasa el numero a la base indicada, el resultado se devuelve en el string*/
	       static void toBase(int num,int base,std::string&  binary);
	       /*Completa con ceros*/
           static void completeNBits(std::string& binary,unsigned int n);
           /*Guarda un bit en el lsb del byte*/
           static int hideInByte(int byte,int bit){
					int lsb=(byte&1);
					int result=byte;

 					if(lsb!=bit){
 						if(bit==1){ result++;}
 						else result--;
 					}return result;

			}
           /*Toma el lsb del byte*/
           static int getHidenBit(int byte){
           	   return (byte&1);

           }
};
}
#endif /*BITSUTILS_H_*/
