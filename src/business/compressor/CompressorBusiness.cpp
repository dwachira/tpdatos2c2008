#include "CompressorBusiness.h"



CompressorBusiness::CompressorBusiness(){

}

CompressorBusiness::~CompressorBusiness(){
	
}
/*Comprime el archivo pasado por parametro creando un archivo temporal nuevo
 * con la informacion comprimida*/

int CompressorBusiness::compress(FILE * input, FILE *output){
	unsigned int next_code=256; //el siguiente libre en la tabla
	unsigned int character=0;
	unsigned int string_code=0;
	int i=0;       
	buffer=0L;
	j=0;
    for (i=0;i<TABLA;i++){  //limpia la tabla
    map_code[i]=-1;
    }
    
    string_code=getc(input); 

  
      while ((character=getc(input)) != (unsigned)EOF)  //obtiene el siguiente caracter (get next character)
      {
    	i=find_code(string_code,character);// Busca en la tabla 
        if (map_code[i] != -1)            // si lo encuentra   
            string_code=map_code[i];
        
        else                                    
        {                                       
          if (next_code <= MAX_CODE) //no encontro el codigo en la tabla
          {
            map_code[i]=next_code++;
            map_prefix[i]=string_code;
            map_append[i]=character;
          }
          put_code(output,string_code); 
          string_code=character;            
        }                                   
      }                                     
      
    put_code(output,string_code); 
    put_code(output,MAX_VALUE);   
    put_code(output,0);         
    return 0;
}


int CompressorBusiness::decompress(FILE *input,FILE *output){
	buffer=0L;
	j=0;
	unsigned int next_code=256;
	unsigned int new_code=0;
	unsigned int old_code=0;
	int character=0;
	unsigned char *string;
	unsigned char decode_stack[4000]; 
	unsigned int decode_code=0;
	int add=0; ;
	int i;
	for (i=0;i<TABLA;i++){  //limpia la tabla
	    map_code[i]=-1;
	    map_prefix[i]=-1;
	    map_append[i]=-1;
	    }
	old_code=get_code(input); 
	character=old_code;          
	putc(old_code,output);           
	  
	
	  while ((new_code=get_code(input)) != (MAX_VALUE)) 
	  {
	    	    	    
	//el caso especial 
	    if (new_code>=next_code)
	    {
	      *decode_stack=character;
	      add=1;
	      decode_code=old_code;
	    }
	    else{
	    	add=0;
	    	decode_code=new_code;
	    }
	    
	    unsigned char *buffer=decode_stack+add;
	    i=0;
	     while (decode_code > 255)
	     {
	       *buffer++ = map_append[decode_code];
	       decode_code=map_prefix[decode_code];
	      }
	     *buffer=decode_code;
	     string=buffer;
	
	    character=*string;
	    while (string >= decode_stack)
	    	putc(*string--,output);
	    	
	   
	
	    if (next_code <= MAX_CODE)
	    {
	      map_prefix[next_code]=old_code;
	      map_append[next_code]=character;
	      next_code++;
	    }
	    old_code=new_code;
	  }
	  return 0;
}


/*Busca el codigo en la tabla, 
 * si lo encuentra devuelve la posicion donde se encuentra
 * si no lo encuentra devuelve la posicion de uno libre
 */


int CompressorBusiness::find_code(int prefix,unsigned int character)
{
int index;
int offset;

  index = (character << SHIFT) ^ prefix;

  if (index == 0) offset = 1;
  else offset = TABLA - index;
  while (1)
  {
    if (map_code[index] == -1)     
    	return(index);
    if (map_prefix[index] == (unsigned)prefix && map_append[index] == character)
      return(index);
    index -= offset;
    if (index < 0) index += TABLA;
  }
}

/* Obtiene el codigo comprimido del archivo
 * y lo devuelve de a 8 bits  
 */

unsigned int CompressorBusiness::get_code(FILE *input)
{
unsigned int code=0;


  while (j <= 24)
  {
    buffer |= (unsigned long) getc(input) << (24-j);
    j += 8;
  }
  code=buffer >> (32-BITS);
  buffer <<= BITS;
  j -= BITS;
  return(code);
}

/* Va almacenando de a 8 bits el codigo comprimido en el archivo temporal */

void CompressorBusiness::put_code(FILE *output,unsigned int code)
{


  buffer |= (unsigned long) code << (32-BITS-j);
  j += BITS;
  while (j >= 8)
  {
    putc(buffer >> 24,output);
	buffer <<= 8;
    j -= 8;
  }
}
