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

void Controller::agregarMensaje(std::string filename) {
	try {
		mensajeManager.agregarMensaje(filename);
	}catch (RecursoInaccesibleException e) {
		std::cout<<"No se puede agregar "<<filename
			<<" verifique que esta escrito correctamente y que"
					<< "tiene permisos de lectura sobre él."<<std::endl;
	}
}

void Controller::agregarDirectorio(std::string path) {
	try {
			directorioManager.agregarDirectorio(path);
		}catch (RecursoInaccesibleException e) {
			//MOSTRAR MENSAJE ADECUADO
		}catch (EntidadYaExistenteException e) {
			//MOSTRAR MENSAJE ADECUADO
		}
}

bool Controller::directorioEnUso(std::string path) {
	try {
		if (directorioManager.directorioEnUso(path)) {
			//PREGUNTAR AL USUARIO QUE DESEA HACER EN ESTE CASO
		}
	} catch (EntidadInexistenteException e) {
		//MOSTRAS MENSAJE ADECUADO
	}
	return true;
}

void Controller::removerDirectorio(std::string path) {
	try {
		if (directorioManager.directorioEnUso(path)) {
			//PREGUNTAR AL USUARIO QUE DESEA HACER EN ESTE CASO
		}
	} catch (EntidadInexistenteException e) {
		//MOSTRAS MENSAJE ADECUADO
	}
}

void Controller::removerMensaje(std::string filename) {

}

void Controller::obtenerMensaje(std::string filename, std::string pathDestino) {

}


Controller::~Controller() {
	// TODO Auto-generated destructor stub
}
