/*
 * BlowfishCrypto.h
 *
 *  Created on: 12/10/2008
 *      Author: monique
 */

#ifndef BLOWFISHCRYPTO_H_
#define BLOWFISHCRYPTO_H_

//Tipos de datos
#define DWord 		unsigned long
#define Word  		unsigned short
#define Byte  		unsigned char

//Estructura de tipo union, para 4 bytes con los distintos tipos
union Doble{
	Doble(){}
	Doble(DWord ul){
		dword = ul;
	}
	Doble(const char* s){
		wchar.byte0 = s[0];
		wchar.byte1 = s[1];
		wchar.byte2 = s[2];
		wchar.byte3 = s[3];
	}
	DWord dword;
	Word word[2];
	Byte byte[4];
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
};
struct QWord{
	QWord(){}
	QWord(DWord l,DWord r){
			xl = l;
			xr = r;
	}
	Doble xl;
	Doble xr;
};

//Definicion de la macro F, para mejorar la velocidad de la red Feistel
#define F(s)   (((sBoxes[0][s.wint.byte0].dword + sBoxes[1][s.wint.byte1].dword) ^ sBoxes[2][s.wint.byte2].dword) + sBoxes[3][s.wint.byte3].dword)



#include "../CryptoBusiness.h"
#include "DigitsPI.h"

class BlowfishCrypto: public CryptoBusiness {
private:
	Doble pSubKeys[18];
	Doble sBoxes[4][256];
	void swap(Doble left, Doble right);
protected:
	void inicializar(string password);
public:
	BlowfishCrypto();
	virtual ~BlowfishCrypto();
	string encrypt(string data){return "";}
	QWord encrypt(QWord data);
	string desencrypt(string password, string data);
};

#endif /* BLOWFISHCRYPTO_H_ */
