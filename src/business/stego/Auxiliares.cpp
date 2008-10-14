#include "Auxiliares.h"
#include <iostream>

using namespace util;
void completeNBits(std::string& binario,unsigned int n){
  if(binario.size()!=n) {
      				   int completo= n-binario.size();
      				   std::string aux=binario;binario="";
      				   for(int i=0;i<completo;i++) binario.append("0");
      					   binario.append(aux);
   			        }
}
void toBase(int num,int base,std::string&  binario){

if(num<base){
 
  binario.append(StringUtils::toString(num));

}
else{
    
    toBase(num/base,base,binario);
    binario.append(StringUtils::toString(num%base));
}

}
