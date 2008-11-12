#include "BitsUtils.h"
#include <iostream>

namespace util{

	BitsUtils::BitsUtils(){}

	BitsUtils:: ~BitsUtils(){}

/**
 * Rellena la cadena de bits con tantos ceros como se indique
 */
	void BitsUtils::completeNBits(std::string& binary,unsigned int n){
  			if(binary.size()!=n) {
      		  int complete= n-binary.size();
      		  std::string aux=binary;binary="";
      		  for(int i=0;i<complete;i++) binary.append("0");
      			  binary.append(aux);
  			}
	}

/**
 * Transforma un numero decimal en la base indicada
 */
	void BitsUtils::toBase(int num,int base,std::string&  binary){

		if(num<base) 
  			binary.append(StringUtils::toString(num));
		else{
        	toBase(num/base,base,binary);
    		binary.append(StringUtils::toString(num%base));
		}

	}
	

	int BitsUtils::getHidenBit(int byte,unsigned int pos){
		int lsb=(byte&pos);
		return (lsb>>pos-1);

	}
}


