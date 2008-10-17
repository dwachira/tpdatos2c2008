/*
 * BlowfishTest.cpp
 *
 *  Created on: 17/10/2008
 *      Author: maxi
 */

#include "BlowfishTest.h"

BlowfishTest::BlowfishTest() {
	// TODO Auto-generated constructor stub

}

BlowfishTest::~BlowfishTest() {
	// TODO Auto-generated destructor stub
}


int BlowfishTest::test() {
	CryptoBusiness * crypto = new BlowfishCrypto();
	crypto->inicializar("test");
	string resultado = crypto->encrypt("hola mundo");
	std::cout << "resultado: " << resultado << std::endl;
	return 0;
}
