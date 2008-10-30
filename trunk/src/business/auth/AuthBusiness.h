/*
 * AuthBusiness.h
 *
 *  Created on: 30/10/2008
 *      Author: maxi
 */

#ifndef AUTHBUSINESS_H_
#define AUTHBUSINESS_H_

#include "../../config/Config.h"

class AuthBusiness {
private:
	string pass;
	string systemPath;
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
