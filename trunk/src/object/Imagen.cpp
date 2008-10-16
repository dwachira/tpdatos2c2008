/*
 * Imagen.cpp
 *
 *  Created on: 02/10/2008
 *      Author: andres
 */
#include "Imagen.h"

Imagen::Imagen() {

	this->ID = 0;
	this->ID_Dir = 0;
	this->nombre = "";
	this->tamanio = 0;
	this->espacio_libre = 0;
	this->hash_value = 0;
}
/*
Imagen::Imagen(unsigned int ID, unsigned int ID_Dir, string nombre, unsigned int tamanio,
		unsigned int espacio_libre, unsigned long int hash_value){

	this->ID = ID;
	this->ID_Dir = ID_Dir;
	this->nombre = nombre;
	this->tamanio = tamanio;
	this->espacio_libre = espacio_libre;
	this->hash_value = hash_value;
}
*/
Imagen::~Imagen() {

	this->ID = 0;
	this->ID_Dir = 0;
	this->nombre = "";
	this->tamanio = 0;
	this->espacio_libre = 0;
	this->hash_value = 0;
}
