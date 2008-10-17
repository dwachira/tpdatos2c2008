/*
 * Directorio.cpp
 *
 *  Created on: 02/10/2008
 *      Author: andres
 */
#include "Directorio.h"

Directorio::Directorio(){

	this->ID = 0;
	this->path = "";
	this->dia = 0;
	this->mes = 0;
	this->anio = 0;
	this->hora = 0;
	this->min = 0;
}

Directorio::Directorio(const Directorio &dir){

	this->ID = dir.ID;
	this->path = dir.path;
	this->dia = dir.dia;
	this->mes = dir.mes;
	this->anio = dir.anio;
	this->hora = dir.hora;
	this->min = dir.min;
}

/*
Directorio::Directorio(unsigned int newID, string newPath, util::Date* newModif){

	this->ID = newID;
	this->path = newPath;
	this->ultimaModif = newModif;
}
*/
Directorio::~Directorio() {

	this->ID = 0;
	this->path = "";
	this->dia = 0;
	this->mes = 0;
	this->anio = 0;
	this->hora = 0;
	this->min = 0;
}
