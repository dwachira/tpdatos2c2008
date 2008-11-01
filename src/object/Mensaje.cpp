/*
 * Mensaje.cpp
 *
 *  Created on: 02/10/2008
 *      Author: andres
 */
#include "Mensaje.h"

namespace object{

unsigned int Mensaje::incrementalId = 0;

Mensaje::Mensaje(const Mensaje &msj){

	this->ID = msj.ID;
	this->nombre = msj.nombre;
	this->tamanio = msj.tamanio;
	this->cant_partes = msj.cant_partes;
}

Mensaje::Mensaje(string newNombre, unsigned int newTamanio, int newCant):
				nombre(newNombre), tamanio(newTamanio), cant_partes(newCant){
	this->ID = getNewId();
}

Mensaje::Mensaje(unsigned int newId, string newNombre, unsigned int newTamanio, int newCant):
				ID(newId), nombre(newNombre), tamanio(newTamanio), cant_partes(newCant){}

Mensaje::~Mensaje(){

	this->ID = 0;
	this->nombre = "";
	this->tamanio = 0;
	this->cant_partes = -1;
}

}
