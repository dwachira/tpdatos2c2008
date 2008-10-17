/*
 * TypeData.h
 *
 *  Created on: 17/10/2008
 *      Author: maxi
 */

#ifndef TYPEDATA_H_
#define TYPEDATA_H_

//Tipos de datos simples
#define Doble 		unsigned long   // 4bytes
#define Word  		unsigned short // 2bytes
#define Byte  		unsigned char // 1byte

//Tipo de datos complejos
//Estructura de tipo union, para 4 bytes con los distintos tipos
union DWord{
	Doble doble;
	Word word[2];
	Byte byte[4];
	char * toString;
	struct {
		unsigned int byte3:8;
		unsigned int byte2:8;
		unsigned int byte1:8;
		unsigned int byte0:8;
	} wint;
	struct {
		Byte byte3:8;
		Byte byte2:8;
		Byte byte1:8;
		Byte byte0:8;
	} wchar;
	//Constructores
	DWord(){}
	DWord(Doble ul){
		doble = ul;
	}
	DWord(const char* s){
		wchar.byte0 = s[0];
		wchar.byte1 = s[1];
		wchar.byte2 = s[2];
		wchar.byte3 = s[3];
	}
};
//Estructura de datos, que agrupa dos dobles
struct QWord{
	DWord xl;
	DWord xr;
	QWord(){}
	QWord(Doble l,Doble r){
			xl = l;
			xr = r;
	}
	QWord(const char* s){
		xl.wchar.byte0 = s[0];
		xl.wchar.byte1 = s[1];
		xl.wchar.byte2 = s[2];
		xl.wchar.byte3 = s[3];
		xr.wchar.byte0 = s[4];
		xr.wchar.byte1 = s[5];
		xr.wchar.byte2 = s[6];
		xr.wchar.byte3 = s[7];
	}

};



#endif /* TYPEDATA_H_ */
