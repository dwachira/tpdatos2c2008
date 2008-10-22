#include "TestCompressor.h"
#include <stdio.h>
#include <stdlib.h>

TestCompressor::TestCompressor() {
	// TODO Auto-generated constructor stub

}
TestCompressor::~TestCompressor() {
	// TODO Auto-generated destructor stub
}


int TestCompressor::test(){
	FILE *input_file;
	FILE *output_file;
	FILE *temp_file;
	
	CompressorBusiness * compresor = new CompressorBusiness();
	
	//COMPRIMIR
	input_file=fopen("consultas.txt","rb");
	temp_file=fopen("compressed.temp","wb");
	if (input_file==NULL || temp_file==NULL)
	{
	  printf("No se encuentra el archivo.");
	  return 1;
	};
	compresor->compress(input_file,temp_file);
	fclose(input_file);
	fclose(temp_file);
	
	//DESCOMPRIMIR  
	  
    temp_file=fopen("compressed.temp","rb");
    output_file=fopen("output.txt","wb");
    if (temp_file==NULL || output_file==NULL)
    {
      printf("No se encuentra el archivo.");
      return 1;
    };
	compresor->decompress(temp_file,output_file);
	printf("Descomrprimio!");
	fclose(temp_file);
    fclose(output_file);
	return 0;
	
};

