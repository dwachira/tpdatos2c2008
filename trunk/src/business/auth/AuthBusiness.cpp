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
	// TODO Auto-generated destructor stub
}

/** Retornar true, si se puedo logear el usuario*/
bool AuthBusiness::login(string pass){

	return true;
}

/** Retornar true. si se pudo cambiar el pass*/
bool  AuthBusiness::changePass(string oldPass,string newPass){
	return true;
}

/** Retornar true, si se pudo deslogear correctamente el usuario*/
bool  AuthBusiness::logout(){
	return true;
}
