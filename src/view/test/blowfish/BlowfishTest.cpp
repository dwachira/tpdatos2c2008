/*
 * BlowfishTest.cpp
 *
 *  Created on: 17/10/2008
 *      Author: maxi
 */

#include "BlowfishTest.h"
#include <iostream>
#include <fstream>


BlowfishTest::BlowfishTest() {
	// TODO Auto-generated constructor stub

}

BlowfishTest::~BlowfishTest() {
	// TODO Auto-generated destructor stub
}


int BlowfishTest::test() {
	BlowfishCrypto * crypto = new BlowfishCrypto();

	crypto->inicializar("betolanza");

	FILE *fe, *fs;
	char buffer[8];
	int bytesLeidos;

	fe = fopen("/home/gsenno/workspaceC++/tpDatos/Stegno/test_data/prueba_entrada.txt", "rb");
	fs = fopen("/home/gsenno/workspaceC++/tpDatos/Stegno/test_data/enc.txt", "wb");
	while((bytesLeidos = fread(buffer, 1, 8, fe))){
		std::string en = crypto->encrypt(buffer,bytesLeidos);
		fwrite(en.data(),1,en.size(),fs);
	}

	fclose(fe);
	fclose(fs);

	crypto->inicializar("betolanza");
	fe = fopen("/home/gsenno/workspaceC++/tpDatos/Stegno/test_data/enc.txt", "rb");
	fs = fopen("/home/gsenno/workspaceC++/tpDatos/Stegno/test_data/salidaenc.txt", "wb");
	while((bytesLeidos = fread(buffer, 1, 8, fe))){
		QWord msj(buffer,bytesLeidos);
		QWord en = crypto->desencrypt(msj);

		fwrite(en.xl.byte, 1, 4, fs);
		fwrite(en.xr.byte, 1,  4, fs);
	}

	fclose(fe);
	fclose(fs);


	return 0;
}



