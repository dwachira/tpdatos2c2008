/*
 * Controller.cpp
 *
 *  Created on: 05-jun-2008
 *      Author: gsenno
 */

#include "Controller.h"

#include <iostream>
#include "../../object/exceptions/RecursoInaccesibleException.h"
#include "../../object/exceptions/EntidadYaExistenteException.h"
#include "../../object/exceptions/EntidadInexistenteException.h"
#include "../../object/exceptions/ImagenFaltanteException.h"
#include "../../object/exceptions/EspacioInsuficienteException.h"

void Controller::agregarMensaje(std::string& filename) {
	try {
		mensajeManager->agregarMensaje(filename);
	} catch (RecursoInaccesibleException e) {
		std::cout<<"No se puede agregar "<<filename
		<<" verifique que esta escrito correctamente y que"
		<< "tiene permisos de lectura sobre él."<<std::endl;
	} catch (EntidadYaExistenteException e) {
		std::cout<<"El mensaje ya existe en el sistema. "<<std::endl;
	} catch (EspacioInsuficienteException e) {
		std::cout<<"No hay espacio suficiente para alojar el texto. Agregue mas imagenes."<<std::endl;
	}
}

void Controller::agregarDirectorio(std::string& path) {
	try {
		directorioManager->agregarDirectorio(path);
	} catch (RecursoInaccesibleException e) {
		std::cout<<"No se puede acceder al directorio."
		"Compruebe que lo haya escrito bien y que, ademas, tenga"
		"permisos de escritura sobre el mismo"<<std::endl;

	} catch (EntidadYaExistenteException e) {
		std::cout<<"El directorio ya existe en el sistema"<<std::endl;
	}
}

void Controller::removerDirectorio(std::string& path) {
	try {
		if (directorioManager->directorioEnUso(path)) {
			if(this->confirmar(REMOVERDIRECTORIO)) {
				mensajeManager->quitarMensajesEnDirectorio(path);
				directorioManager->removerDirectorio(path);
			}
		}
		else {
			directorioManager->removerDirectorio(path);
		}
	} catch (EntidadInexistenteException e) {
		std::cout<<"El directorio no existe en el sistema. "<<std::endl;
	}
}

void Controller::removerMensaje(std::string& filename) {
	try {
		mensajeManager->quitarMensaje(filename);
	} catch (EntidadInexistenteException e) {
		std::cout<<"El mensaje no existe."<<std::endl;
	}
}

void Controller::obtenerMensaje(std::string& filename, std::string& pathDestino) {
	try {
		mensajeManager->obtenerMensaje(filename,pathDestino);
	} catch (EntidadInexistenteException e) {
		std::cout<<"El mensaje solicitado no se encuentra en la base de datos. Compruebe el nombre"<<std::endl;
	} catch (RecursoInaccesibleException e) {
		std::cout<<"No se puede escribir en el destino."<<std::endl;
	} catch (ImagenFaltanteException e) {
		if(this->confirmar(OBTENERMSG)) {
			this->removerMensaje(filename);
		}
	}
}

void Controller::mostrarDirectorios() {
	list<string> directorios = directorioManager->getDirectorios();
	if (directorios.size() != 0) {
		for (list<string>::iterator it = directorios.begin(); it
				!= directorios.end(); it++) {
			std::cout<<*it<<std::endl;
		}
	} else {
		std::cout<<"No se ha agregado ningun directorio."<<std::endl;
	}
}

void Controller::mostrarMensajes() {
	list<string> mensajes = mensajeManager->getMensajes();
	if (mensajes.size() != 0) {
		for (list<string>::iterator it = mensajes.begin(); it != mensajes.end(); it++) {
			std::cout<<*it<<std::endl;
		}
	} else {
		std::cout<<"No se ha agregado ningun mensaje."<<std::endl;
	}
}

bool Controller::login(string& password) {
	loggedIn = authBusiness->login(password);
	if (loggedIn) {
		this->managerDao = new dao::ManagerDAO();
		this->trieDao = new dao::TrieDAO(*this->managerDao);
		this->trieDao->loadTrie(DIRECTORIOS);
		this->trieDao->loadTrie(MENSAJES);
		this->trieDao->loadTrie(IMAGENES);
		this->directorioManager = new DirectorioManager(*this->managerDao,*this->trieDao);
		this->mensajeManager
				= new MensajeManager(*this->managerDao,*this->directorioManager,*this->trieDao,password);
		std::cout<<"Acaba de ingresar al sístema."<<std::endl;
	} else {
		std::cout<<"Contraseña invalida."<<std::endl;
	}
	return loggedIn;
}

bool Controller::validatePassword(const std::string& password) {
	bool result = true;
	if ((password.size() < MIN_PASS_LENGTH)||(password.size() > MAX_PASS_LENGTH)) {
		std::cout<<"La longitud del password debe ser de 4 a 6 caracteres."<<std::endl;
		result = false;
	}
	return result;
}

void Controller::changePassword(std::string& oldPassword, std::string& newPassword) {
	if (validatePassword(newPassword)) {
		if (!authBusiness->changePass(oldPassword,newPassword))
			std::cout<<"El password ingresado como oldPassword no coincide con el almacenado."<<std::endl;
	}
}

bool Controller::confirmar(string pregunta) {
	char buffer[MAX_LINE];
	string opcion = "";
	string valido = "";
	while (valido.compare("s") != 0 && valido.compare("n") != 0) {
		std::cout<< pregunta;
		cin.getline(buffer, MAX_LINE);
		opcion.assign(buffer);
		if (opcion.compare("S") == 0 || opcion.compare("s") == 0) {
			valido.assign("s");
		} else if (opcion.compare("N") == 0 || opcion.compare("n") == 0) {
			valido.assign("n");
		}
	}
	if (valido.compare("s") == 0) {
		return true;
	} else {
		return false;
	}
}

Controller::~Controller() {
	if (loggedIn) {
		authBusiness->logout();
		delete mensajeManager;
		delete directorioManager;
		delete trieDao;
		delete managerDao;
	}
	delete authBusiness;
}
