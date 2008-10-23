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
	/*crypto->inicializar("betolanza");
	string msj = "NO al paco";
	string resEncript = crypto->encrypt(msj);
	std::cout << "Encriptacion es: " << resEncript << std::endl;
	crypto->inicializar("betolanza");
	string resDesencript = crypto->desencrypt(resEncript);
	std::cout << "La Desencriptacion es: " << resDesencript << std::endl;*/
	/*crypto->inicializar("betolanza");
	string linea;
	ifstream in ("/home/maxi/in.txt");
	ofstream out ("/home/maxi/encrypt.txt");
	if (in.is_open() && out.is_open())
	{
		//Mientras que no sea fin de archivo
		while (! in.eof() )
		{
			getline (in,linea);
			string resEncript = crypto->encrypt(linea);
			out << resEncript << std::endl;


		}
		in.close();
		out.close();
	}
	else
		cout << "No se pudo abrir algun archivo para encriptar.";


	crypto->inicializar("betolanza");


	ifstream in2 ("/home/maxi/encrypt.txt");
	ofstream out2 ("/home/maxi/desencrypt.txt");
	if (in2.is_open() && out2.is_open())
	{
		//Mientras que no sea fin de archivo
		while (! in2.eof() )
		{
			getline (in2,linea);
			string resEncript = crypto->desencrypt(linea);
			out2 << resEncript << std::endl;


		}
		in2.close();
		out2.close();
	}
	else
		cout << "No se pudo abrir algun archivo para desencriptar.";*/





	/*for ( unsigned int x = 0; msj.length() != x; x++ ) {
		std::cout<< resEncript.at(x) <<" = "<< int ( resEncript.at(x) ) <<'\n';
	}*/
	/*QWord msj;
	msj.xl.wchar.byte0 = 'h';
	msj.xl.wchar.byte1 = 'o';
	msj.xl.wchar.byte2 = 'l';
	msj.xl.wchar.byte3 = 'a';
	msj.xr.wchar.byte0 = 'm';
	msj.xr.wchar.byte1 = 'a';
	msj.xr.wchar.byte2 = 'x';
	msj.xr.wchar.byte3 = 'i';
	QWord en = crypto->encrypt(msj);
	crypto->inicializar("zambayoni");
	QWord desen = crypto->desencrypt(en);*/
	//std::cout << "Encriptacion es: " << en.xl.byte << std::endl;





	crypto->inicializar("betolanza");

	FILE *fe, *fs;
	unsigned char buffer[8];
	int bytesLeidos;

	fe = fopen("/home/maxi/programas/in.txt.zip", "rb");
	fs = fopen("/home/maxi/enc.txt", "wb");
	while((bytesLeidos = fread(buffer, 1, 8, fe))){
		QWord msj(buffer,bytesLeidos);

		QWord en = crypto->encrypt(msj);
		/*if (bytesLeidos > 4){
			fwrite(en.xl.byte, 1, 4, fs);
			fwrite(en.xr.byte, 1, bytesLeidos - 4, fs);
		}else{
			fwrite(en.xl.byte, 1, bytesLeidos, fs);
		}*/
		fwrite(en.xl.byte, 1, 4, fs);
		fwrite(en.xr.byte, 1, 4, fs);
	}

	fclose(fe);
	fclose(fs);

	crypto->inicializar("betolanza");
	fe = fopen("/home/maxi/enc.txt", "rb");
	fs = fopen("/home/maxi/in.txt.zip", "wb");
	while((bytesLeidos = fread(buffer, 1, 8, fe))){
		QWord msj(buffer,bytesLeidos);
		QWord en = crypto->desencrypt(msj);
		/*if (bytesLeidos > 4){
			fwrite(en.xl.byte, 1, 4, fs);
			fwrite(en.xr.byte, 1, bytesLeidos - 4, fs);
		}else{
			fwrite(en.xl.byte, 1, bytesLeidos, fs);
		}*/

		fwrite(en.xl.byte, 1, 4, fs);
		fwrite(en.xr.byte, 1,  4, fs);

	}

	fclose(fe);
	fclose(fs);


	return 0;
}



