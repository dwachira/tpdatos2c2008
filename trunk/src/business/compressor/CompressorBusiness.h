#ifndef COMPRESSORBUSINESS_H_
#define COMPRESSORBUSINESS_H_


#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <iostream>

#ifndef BITS_FOR_TABLE
#define BITS_FOR_TABLE 24
#endif
#define SHIFT (BITS_FOR_TABLE-8)
#define MAX_VALUE (1 << BITS_FOR_TABLE) - 1
#define MAX_CODE MAX_VALUE - 1
#define TABLA 16380


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

