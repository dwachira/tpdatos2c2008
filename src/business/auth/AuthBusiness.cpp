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
	//Checkear si es el pass es el correcto
	if (!checkPass(pass)){
		return false;
	}
	string pathEncrypt = "";
	pathEncrypt.append(this->systemPath);
	pathEncrypt.append(PATH_ENCRYPT);
	BlowfishCrypto * crypto = new BlowfishCrypto();
	crypto->inicializar(pass);
	executeFunctionInPath(pathEncrypt,crypto,desencryptFile);
	delete crypto;
	this->pass = pass;
	return true;
}

/** Retornar true. si se pudo cambiar el pass*/
bool  AuthBusiness::changePass(string oldPass,string newPass){
	//Checkear si es el pass es el correcto
	if (!checkPass(oldPass)){
			return false;
	}
	this->pass = newPass;
	//Guardar el nuevo pass en disco
	string archivoHash = "";
	archivoHash.append(this->systemPath);
	archivoHash.append(FILE_PASS_HASH);
	//Eliminar el archivo de hash
	remove (archivoHash.c_str());
	FILE *fs;
	//Abrir el arhivo de hash y leer el hash del pass
	fs = fopen(archivoHash.c_str(), "wb");
	md5wrapper md5;
	std::string passUser = md5.getHashFromString(newPass);
	fwrite(passUser.c_str(), 1, 32, fs);
	fclose(fs);
	return true;
}

/** Retornar true, si se pudo deslogear correctamente el usuario*/
bool  AuthBusiness::logout(){
	//Elminar los archivos encriptados,para encriptar los archivos con las modificaciones
	string pathEncrypt = "";
	pathEncrypt.append(this->systemPath);
	pathEncrypt.append(PATH_ENCRYPT);
	deleteInPath(pathEncrypt);
	//Encriptar la estructura administrativa
	BlowfishCrypto * crypto = new BlowfishCrypto();
	crypto->inicializar(this->pass);
	executeFunctionInPath(this->systemPath,crypto,encryptFile);
	delete crypto;
	//Eliminar los archivos desencriptados
	deleteInPath(this->systemPath);
	return true;
}


void AuthBusiness::executeFunctionInPath(string path,BlowfishCrypto * crypto, void (* Funcion)(BlowfishCrypto * crypto,string,string)){
	string archivo="";

	struct dirent **resultados = NULL;
	int numeroResultados;
	//Scanear el path
	numeroResultados = scandir (path.c_str(), &resultados, NULL, NULL);
	int i;
	for (i=0; i<numeroResultados; i++){
		//Aplicar la funcion a los archivos, excepto al del hash del pass
		string filename(resultados[i]->d_name);
		if (filename.compare(FILE_PASS_HASH) && filename.compare("..") && filename.compare(".") && filename.compare("encrypt") && filename.compare(".svn")){
			archivo.append(filename);
			Funcion(crypto,path,archivo);
		}
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


void AuthBusiness::deleteInPath(string pathDelete){
	string archivo="";

	struct dirent **resultados = NULL;
	int numeroResultados;
	//Scanear el path
	numeroResultados = scandir (pathDelete.c_str(), &resultados, NULL, NULL);
	int i;
	for (i=0; i<numeroResultados; i++){
		//Eliminar los archivos, excepto al del hash del pass
		string filename(resultados[i]->d_name);
		if (filename.compare(FILE_PASS_HASH) && filename.compare("..")  && filename.compare(".") && filename.compare("encrypt") && filename.compare(".svn")){
			archivo.append(pathDelete);
			archivo.append(filename);
			remove (archivo.c_str());
		}
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
	archivoOut.append(PATH_ENCRYPT);
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
	//archivoIn.append(PATH_ENCRYPT);
	archivoIn.append(file);

	string archivoOut="";
	archivoOut.append(path);
	archivoOut.append("../");
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


bool AuthBusiness::checkPass(string pass){
	string archivoHash = "";
	archivoHash.append(this->systemPath);
	archivoHash.append(FILE_PASS_HASH);
	FILE *fe;
	unsigned char buffer[32];
	//Abrir el arhivo de hash y leer el hash del pass
	fe = fopen(archivoHash.c_str(), "rb");
	fread(buffer, 1, 32, fe);
	fclose(fe);
	stringstream ss;
	ss << buffer;
	string passSystem(ss.str());
	// Hashear el pass del usuario
	md5wrapper md5;
	std::string passUser = md5.getHashFromString(pass);
	//Comparar el pass del usuario con el del sistema, si da 0 es por que SON IGUALES
	if (passSystem.compare(passUser)){
		return false;
	}
	return true;
}


