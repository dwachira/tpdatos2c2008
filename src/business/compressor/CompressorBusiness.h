#ifndef COMPRESSORBUSINESS_H_
#define COMPRESSORBUSINESS_H_


#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <iostream>

#define BITS 12                   /* Cantidad de bits para la tabla*/
#define SHIFT (BITS-8)
#define MAX_VALUE (1 << BITS) - 1
#define MAX_CODE MAX_VALUE - 1
#define TABLA 4094


using namespace std;
class CompressorBusiness {

private:
	int j;
	unsigned long buffer;


	int find_code(int prefijo,unsigned int character);
	void put_code(unsigned int code,FILE *output);
	unsigned int get_code(FILE *input);


public:
	CompressorBusiness();
	virtual ~CompressorBusiness();
	int compress(FILE * input,FILE * output);
	int decompress(FILE * input,FILE * output);
};



#endif /*COMPRESSORBUSINESS_H_*/

