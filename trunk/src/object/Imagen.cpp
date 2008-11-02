/*
 * Imagen.cpp
 *
 *  Created on: 02/10/2008
 *      Author: andres
 */
#include "Imagen.h"

namespace object{

unsigned int Imagen::incrementalId = 0;		//si lo borro, no compila

Imagen::Imagen(const Imagen &img){

	this->ID = img.ID;
	this->ID_Dir = img.ID_Dir;
	this->nombre = img.nombre;
	this->tamanio = img.tamanio;
	this->espacio_libre = img.espacio_libre;
	this->proximo_bit_libre = img.proximo_bit_libre;
	this->hash_value = img.hash_value;
}

Imagen::Imagen(unsigned int pID_Dir,unsigned int pEspacio_Libre,unsigned int pProximo_Bit_Libre,
		unsigned long int pHash_Value,unsigned int pTamanio,string pNombre) :
				ID_Dir(pID_Dir),espacio_libre(pEspacio_Libre),proximo_bit_libre (pProximo_Bit_Libre),
				hash_value(pHash_Value),tamanio(pTamanio),nombre(pNombre){
	this->ID = getNewId();
}

Imagen::Imagen(unsigned int pID, unsigned int pID_Dir, unsigned int pEspacio_Libre,
		unsigned int pProximo_Bit_Libre, unsigned long int pHash_Value, unsigned int pTamanio, string pNombre) :
				ID(pID),ID_Dir(pID_Dir),espacio_libre(pEspacio_Libre),proximo_bit_libre (pProximo_Bit_Libre),
				hash_value(pHash_Value),tamanio(pTamanio),nombre(pNombre){}

Imagen::~Imagen() {

	this->ID = 0;
	this->ID_Dir = 0;
	this->nombre = "";
	this->tamanio = 0;
	this->espacio_libre = 0;
	this->hash_value = 0;
	this->proximo_bit_libre = 0;
}

bool operator== (const Imagen& left, const Imagen& right) {
	if ((left.getNombre().compare(right.getNombre())) == 0)
		return true;
	return false;
}

}
