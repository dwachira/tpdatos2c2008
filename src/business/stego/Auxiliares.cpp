#include "Auxiliares.h"
#include <iostream>

using namespace util;
void complete8Bits(std::string& binario){
  if(binario.size()!=8) {
      				   int completo= 8-binario.size();
      				   std::string aux=binario;binario="";
      				   for(int i=0;i<completo;i++) binario.append("0");
      					   binario.append(aux);
   			        }
}
void toBase(int num,int base,std::string&  binario){
//std::cout<<"el numero es "<<num<<std::endl;
if(num<base){
 
  binario.append(StringUtils::toString(num));

}
else{
    
    toBase(num/base,base,binario);
    binario.append(StringUtils::toString(num%base));
}

}

