/*
 * AuthBusiness.h
 *
 *  Created on: 30/10/2008
 *      Author: maxi
 */

#ifndef AUTHBUSINESS_H_
#define AUTHBUSINESS_H_

#define PATH_ENCRYPT "encrypt/"
#define FILE_PASS_HASH "passHash.hsh"

#include "../../config/Config.h"
#include <dirent.h>
#include <stdlib.h>
#include "../crypto/blowfish/BlowfishCrypto.h"
#include "../../business/md5/md5wrapper.h"
#include <sstream>

class AuthBusiness {
private:
	string pass;
	string systemPath;
	static void desencryptFile(BlowfishCrypto * crypto,string path,string file);
	static void encryptFile(BlowfishCrypto * crypto,string path,string file);
	/** Ejecutar la funcion pasada por paramentro, con los archivos pertenecientes al path*/
	void executeFunctionInPath(string path,BlowfishCrypto * crypto,void (* Funcion)(BlowfishCrypto * crypto,string,string));
	bool checkPass(string pass);
	void deleteInPath(string pathDelete);
public:
	AuthBusiness(string systemPath);
	virtual ~AuthBusiness();
	/** Retornar true, si se puedo logear el usuario*/
	bool login(string pass);
	/** Retornar true. si se pudo cambiar el pass*/
	bool changePass(string oldPass,string newPass);
	/** Retornar true, si se pudo deslogear correctamente el usuario*/
	bool logout();

};

#endif /* AUTHBUSINESS_H_ */
