/*
 * Directorio.cpp
 *
 *  Created on: 02/10/2008
 *      Author: andres
 */
#include "Directorio.h"

namespace object{

Directorio::Directorio(std::string newPath) : path(newPath) {
	this->fechaUltimaModificacion = util::Date::valueOf(0, 0, 0, 0, 0);
}

/*
Directorio::Directorio(unsigned int newID, string newPath, util::Date* newModif){

	this->ID = newID;
	this->path = newPath;
	this->ultimaModif = newModif;
}
*/
Directorio::~Directorio() {
	delete this->fechaUltimaModificacion;
}

}
