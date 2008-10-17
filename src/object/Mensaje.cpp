/*
 * Mensaje.cpp
 *
 *  Created on: 02/10/2008
 *      Author: andres
 */
#include "Mensaje.h"

Mensaje::Mensaje(){

	this->ID = 0;
	this->nombre = "";
	this->tamanio = 0;
	this->cant_partes = -1;
}

Mensaje::Mensaje(const Mensaje &msj){

	this->ID = msj.ID;
	this->nombre = msj.nombre;
	this->tamanio = msj.tamanio;
	this->cant_partes = msj.cant_partes;
}

/*
Mensaje::Mensaje(unsigned int ID, string nombre, unsigned int tamanio, int cant_partes){

	this->ID = ID;
	this->nombre = nombre;
	this->tamanio = tamanio;
	this->cant_partes = cant_partes;
}
*/
Mensaje::~Mensaje(){

	this->ID = 0;
	this->nombre = "";
	this->tamanio = 0;
	this->cant_partes = -1;
}
