/*
 * Directorio.cpp
 *
 *  Created on: 02/10/2008
 *      Author: andres
 */
#include "Directorio.h"

namespace object{

unsigned int Directorio::incrementalId = 0;

Directorio::Directorio(string newPath) : path(newPath) {
	this->fechaUltimaModificacion = util::Date::valueOf(0, 0, 0, 0, 0);
	this->ID = getNewId();
}

Directorio::Directorio(std::string newPath, util::Date* ultimaModificacion) :
					path(newPath), fechaUltimaModificacion(ultimaModificacion) {
	this->ID = getNewId();
}

Directorio::Directorio(unsigned int newId, string newPath, util::Date* ultimaModificacion):
				ID(newId),path(newPath),fechaUltimaModificacion(ultimaModificacion){}

Directorio::~Directorio() {
	delete this->fechaUltimaModificacion;
}

}
