#include "CompressorBusiness.h"



CompressorBusiness::CompressorBusiness(){

}

CompressorBusiness::~CompressorBusiness(){

}
/*Comprime el archivo pasado por parametro creando un archivo temporal nuevo
 * con la informacion comprimida*/

int CompressorBusiness::compress(FILE * input, FILE *output){
	map<string,int> map_string;
	unsigned int next_code=256;
	unsigned int character;
	string new_char;
	string append_string;


	buffer=0L;
	j=0;
	unsigned int code=0;

    append_string.append("");

    while ((character=getc(input)) != (unsigned) EOF)  //obtiene el siguiente caracter (get next character)
          {
        	  new_char=(char)character;
        	  append_string.append(new_char);
        	  map<string,int>::iterator it = map_string.find(append_string);
        		  		if(it != map_string.end())	   //lo encuentra en la tabla
        				  			code=it->second;
        	         	else{ //no esta en tabla
							if (append_string.size() == 1)
        		  				code = character;
							else{
								if (next_code < TABLA){
									pair<string, int> item(append_string, next_code);
									map_string.insert(item);
									next_code++;
								}

	        		  			put_code(code,output);
	        		  		}
							append_string=new_char;
        		  			code=character;
        		  		}
        	  }

        put_code(code,output);
        put_code(MAX_VALUE,output);
        put_code(0,output);
        return 0;
}




int CompressorBusiness::decompress(FILE *input,FILE *output){
	map<int,string> map_decompress;
	unsigned int next_code=256;
	string old_code;
	buffer=0L;
	j=0;
	unsigned int character;
	string text;
	unsigned int code;
	unsigned int i;
	unsigned int old_character;


	character=get_code(input);
	old_code=(char)character;
	putc(character,output);
	old_character=character;
	while ((character=get_code(input)) != (unsigned) MAX_VALUE){
		if (character< 256){
			putc(character,output);
			text=(char)character;
			old_code.append(text);

		}
		else {
			map<int,string>::iterator it = map_decompress.find(character);
			if(it != map_decompress.end()){
				text=it->second;
				old_code.append(text.substr(0,1));
				i=0;
				while (i< text.size()){
					code=(unsigned int)(text[i]);
					putc(code,output);
					i++;
				}

			}else{//caso especial
				if (old_character > 256){
				map<int,string>::iterator it = map_decompress.find(old_character);
				old_code= it->second;
				text=old_code;
				text.append(old_code.substr(0,1));
				old_code=text;

			}else{ old_code=(char)old_character;
			old_code.append(old_code);
			text=old_code;
			}
			i=0;
			while (i< old_code.size()){
				code=(unsigned int)(old_code[i]);
				putc(code,output);
				i++;
				}
			}

		}
		if (next_code < TABLA){
			map_decompress[next_code]=old_code;
			next_code++;

		}
		old_code=text;
		old_character=character;
	}
	 return 0;
}




/* Obtiene el codigo comprimido del archivo
 * y lo devuelve de a 8 bits
 */

unsigned int CompressorBusiness::get_code(FILE *input)
{
unsigned int code=0;
  while (j <= BITS_ARQUITECTURA-8)
  {
    buffer |= (unsigned long) getc(input) << (BITS_ARQUITECTURA-8-j);
    j += 8;
  }
  code=buffer >> (BITS_ARQUITECTURA-BITS_FOR_TABLE);
  buffer <<= BITS_FOR_TABLE;
  j -= BITS_FOR_TABLE;
  return(code);
}

/* Va almacenando de a 8 bits el codigo comprimido en el archivo comprimido */

void CompressorBusiness::put_code(unsigned int code,FILE *output)
{
  buffer |= (unsigned long) code << (BITS_ARQUITECTURA-BITS_FOR_TABLE-j);
  j += BITS_FOR_TABLE;
  while (j >= 8)
  {
    putc(buffer >> (BITS_ARQUITECTURA-8),output);
	buffer <<= 8;
    j -= 8;
  }
}
