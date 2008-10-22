/*
 * BlowfishCrypto.h
 *
 *  Created on: 12/10/2008
 *      Author: monique
 */

#ifndef BLOWFISHCRYPTO_H_
#define BLOWFISHCRYPTO_H_


//Definicion de la macro F, para mejorar la velocidad de la red Feistel
#define F(s)   (((sBoxes[0][s.wint.byte0].doble + sBoxes[1][s.wint.byte1].doble) ^ sBoxes[2][s.wint.byte2].doble) + sBoxes[3][s.wint.byte3].doble)



#include "../CryptoBusiness.h"
#include "TypeData.h"
#include "DigitsPI.h"

class BlowfishCrypto: public CryptoBusiness {
private:
	DWord pSubKeys[18];
	DWord sBoxes[4][256];
	void swap(DWord &left, DWord &right);
	string qWordToString(QWord qword);


public:
	BlowfishCrypto();
	virtual ~BlowfishCrypto();
	void inicializar(string password);
	string encrypt(string data);
	string desencrypt(string data);
	QWord encrypt(QWord data);
	QWord desencrypt(QWord data);

};

#endif /* BLOWFISHCRYPTO_H_ */
