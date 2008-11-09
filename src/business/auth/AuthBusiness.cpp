/*
 * AuthBusiness.cpp
 *
 *  Created on: 30/10/2008
 *      Author: maxi
 */

#include "AuthBusiness.h"

AuthBusiness::AuthBusiness(string systemPath) {
	this->systemPath = systemPath;


}

AuthBusiness::~AuthBusiness() {

}

/** Retornar true, si se puedo logear el usuario*/
bool AuthBusiness::login(string pass){
	string archivoHash = "";
	archivoHash.append(this->systemPath);
	archivoHash.append("passHash.hsh");
	FILE *fe;
	unsigned char buffer[16];
	//Abrir el arhivo de hash y leer el hash del pass
	fe = fopen(archivoHash.c_str(), "rb");
	fread(buffer, 1, 16, fe);
	stringstream ss;
	ss << buffer;
	string passSystem(ss.str());
	// Hashear el pass del usuario
	md5wrapper md5;
	std::string passUser = md5.getHashFromString(pass);
	//Comparar el pass del usuario con el del sistema, si no da 0 es por que NO SON IGUALES
	if (passSystem.compare(passUser)){
		return false;
	}


	BlowfishCrypto * crypto = new BlowfishCrypto();
	crypto->inicializar(pass);
	executeFunctionInPath(crypto,desencryptFile);
	delete crypto;
	this->pass = pass;
	return true;
}

/** Retornar true. si se pudo cambiar el pass*/
bool  AuthBusiness::changePass(string oldPass,string newPass){
	this->pass = newPass;
	return true;
}

/** Retornar true, si se pudo deslogear correctamente el usuario*/
bool  AuthBusiness::logout(){
	BlowfishCrypto * crypto = new BlowfishCrypto();
	crypto->inicializar(pass);
	executeFunctionInPath(crypto,encryptFile);
	delete crypto;

	return true;
}


void AuthBusiness::executeFunctionInPath(BlowfishCrypto * crypto, void (* Funcion)(BlowfishCrypto * crypto,string,string)){
	string archivo="";

	struct dirent **resultados = NULL;
	int numeroResultados;
	//Scanear el path
	numeroResultados = scandir (this->systemPath.c_str(), &resultados, NULL, NULL);
	int i;
	for (i=0; i<numeroResultados; i++){
		//archivo.append(this->systemPath);
		archivo.append(resultados[i]->d_name);
		Funcion(crypto,this->systemPath,archivo);
		archivo="";
	}
	//Liberar memoria
	for (i=0; i<numeroResultados; i++)
	{
		free (resultados[i]);
		resultados[i] = NULL;
	}
	free(resultados);
	resultados = NULL;


}


void AuthBusiness::encryptFile(BlowfishCrypto * crypto,string path,string file){
	string archivoIn="";
	archivoIn.append(path);
	archivoIn.append(file);

	string archivoOut="";
	archivoOut.append(path);
	archivoOut.append("encrypt");
	archivoOut.append(file);
	FILE *fe, *fs;
	unsigned char buffer[8];
	int bytesLeidos;

	fe = fopen(archivoIn.c_str(), "rb");
	fs = fopen(archivoOut.c_str(), "wb");
	while((bytesLeidos = fread(buffer, 1, 8, fe))){
		QWord msj(buffer,bytesLeidos);

		QWord en = crypto->encrypt(msj);

		fwrite(en.xl.byte, 1, 4, fs);
		fwrite(en.xr.byte, 1, 4, fs);
	}

	fclose(fe);
	fclose(fs);

}


void AuthBusiness::desencryptFile(BlowfishCrypto * crypto,string path,string file){
	string archivoIn="";
	archivoIn.append(path);
	archivoIn.append(file);

	string archivoOut="";
	archivoOut.append(path);
	archivoOut.append("encrypt");
	archivoOut.append(file);
	FILE *fe, *fs;
	unsigned char buffer[8];
	int bytesLeidos;

	fe = fopen(archivoIn.c_str(), "rb");
	fs = fopen(archivoOut.c_str(), "wb");
	while((bytesLeidos = fread(buffer, 1, 8, fe))){
		QWord msj(buffer,bytesLeidos);

		QWord en = crypto->desencrypt(msj);

		fwrite(en.xl.byte, 1, 4, fs);
		fwrite(en.xr.byte, 1, 4, fs);
	}

	fclose(fe);
	fclose(fs);

}




