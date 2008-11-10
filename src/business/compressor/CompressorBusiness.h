#ifndef COMPRESSORBUSINESS_H_
#define COMPRESSORBUSINESS_H_


#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <map>

#define BITS 12                   /* Cantidad de bits para la tabla*/
#define SHIFT (BITS-8)    
#define MAX_VALUE (1 << BITS) - 1 
#define MAX_CODE MAX_VALUE - 1    
#define TABLA 5021


using namespace std;
class CompressorBusiness {
	
private:
	int j;
	unsigned long buffer;
	map<int,int> map_code;
	map<int,unsigned int> map_prefix;
	map<int,unsigned char> map_append;
	int find_code(int prefijo,unsigned int character);
	void put_code(FILE *output,unsigned int code);
	unsigned int get_code(FILE *input);
	
	
public:
	CompressorBusiness();
	virtual ~CompressorBusiness();
	int compress(FILE * input,FILE * output);
	int decompress(FILE * input,FILE * output);
};



#endif /*COMPRESSORBUSINESS_H_*/

