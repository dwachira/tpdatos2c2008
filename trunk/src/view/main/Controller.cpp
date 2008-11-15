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

void Controller::agregarMensaje(std::string& filename) {
	try {
		mensajeManager.agregarMensaje(filename);
	}catch (RecursoInaccesibleException e) {
		std::cout<<"No se puede agregar "<<filename
			<<" verifique que esta escrito correctamente y que"
					<< "tiene permisos de lectura sobre él."<<std::endl;
	} catch (EntidadYaExistenteException e) {
		std::cout<<"El mensaje ya existe en el sistema. "<<std::endl;
	}
}

void Controller::agregarDirectorio(std::string& path) {
	try {
			directorioManager.agregarDirectorio(path);
		}catch (RecursoInaccesibleException e) {
			std::cout<<"No se puede acceder al directorio."
					"Compruebe que lo haya escrito bien y que, ademas, tenga"
					"permisos de escritura sobre el mismo"<<std::endl;

		}catch (EntidadYaExistenteException e) {
			std::cout<<"El directorio ya existe en el sistema"<<std::endl;
		}
}

bool Controller::directorioEnUso(std::string& path) {
	try {
		if (directorioManager.directorioEnUso(path)) {
			//PREGUNTAR AL USUARIO QUE DESEA HACER EN ESTE CASO
		}
	} catch (EntidadInexistenteException e) {
		//MOSTRAS MENSAJE ADECUADO
	}
	return true;
}

void Controller::removerDirectorio(std::string& path) {
	try {
		if (directorioManager.directorioEnUso(path)) {
			//PREGUNTAR AL USUARIO QUE DESEA HACER EN ESTE CASO
		}
		else
			directorioManager.removerDirectorio(path);
	} catch (EntidadInexistenteException e) {
		std::cout<<"El directorio no existe en el sistema. "<<std::endl;
	}
}

void Controller::removerMensaje(std::string& filename) {
	try {
		mensajeManager.quitarMensaje(filename);
	} catch (EntidadInexistenteException e) {
		std::cout<<"El mensaje no existe."<<std::endl;
	}
}

void Controller::obtenerMensaje(std::string& filename, std::string& pathDestino) {
	try {
		mensajeManager.obtenerMensaje(filename,pathDestino);
	}catch (EntidadInexistenteException e) {
		std::cout<<"El mensaje solicitado no se encuentra en la base de datos. Compruebe el nombre"<<std::endl;
	}catch (RecursoInaccesibleException e) {
		std::cout<<"No se puede escribir en el destino."<<std::endl;
	}catch (ImagenFaltanteException e) {
		//TODO::PREGUNTAR SI QUIERE ELIMINAR EL MENSAJE DEL SISTEMA.
	}
}

void Controller::mostrarDirectorios() {
	list<string> directorios = directorioManager.getDirectorios();
	//TODO::DARLE UN FORMATO MAS MEJOR :P
	for(list<string>::iterator it = directorios.begin(); it != directorios.end(); it++) {
		std::cout<<*it<<std::endl;
	}
}

void Controller::mostrarMensajes() {
	list<string> mensajes = mensajeManager.getMensajes();
	//TODO::DARLE UN FORMATO MAS MEJOR :P
	for(list<string>::iterator it = mensajes.begin(); it != mensajes.end(); it++) {
		std::cout<<*it<<std::endl;
	}
}

Controller::~Controller() {
	// TODO Auto-generated destructor stub
}
