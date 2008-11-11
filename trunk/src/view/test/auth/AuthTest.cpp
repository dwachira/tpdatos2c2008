/*
 * AuthTest.cpp
 *
 *  Created on: 11/11/2008
 *      Author: maxi
 */

#include "AuthTest.h"

AuthTest::AuthTest() {
	// TODO Auto-generated constructor stub

}

AuthTest::~AuthTest() {
	// TODO Auto-generated destructor stub
}




int AuthTest::test() {

	AuthBusiness auth("/home/maxi/workspace/Stegno/system/");
	std::cout << "Se ingresara una contraseña incorrecta 'admin'"  << std::endl;
	if (!auth.login("upalala")){
		std::cout << "Contraseña incorrecta 'admin'"  << std::endl;
	}else{
		std::cout << "FALLO valido una contraseña incorrecta 'admin'"  << std::endl;
	}
	std::cout << "Se ingresara una contraseña correcta"  << std::endl;
	if (auth.login("admin")){
		std::cout << "Contraseña correcta"  << std::endl;
	}else{
		std::cout << "FALLO no valido una contraseña correcta"  << std::endl;
	}

	std::cout << "Se cambia el pass 'admin2'"  << std::endl;
	if (!auth.changePass("admin","admin2")){
		std::cout << "FALLO cambiando pass de admin a admin2"  << std::endl;
	}
	std::cout << "Se deslogea con pass admin2"  << std::endl;
	auth.logout();
	std::cout << "Se ingresara una contraseña correcta 'admin2'"  << std::endl;
	if (auth.login("admin2")){
		std::cout << "Contraseña correcta 'admin2'"  << std::endl;
	}else{
		std::cout << "FALLO no valido una contraseña correcta 'admin2'"  << std::endl;
	}
	std::cout << "Se cambia el pass 'admin'"  << std::endl;
	auth.changePass("admin2","admin");
	std::cout << "Se deslogea con pass admin"  << std::endl;
	auth.logout();


	return 0;

}
