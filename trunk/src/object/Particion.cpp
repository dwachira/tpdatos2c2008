/*
 * Particion.cpp
 *
 *  Created on: 02/10/2008
 *      Author: andres
 */
#include "Particion.h"

namespace object{

Particion::Particion(){

	this->ID_Img = 0;
	this->ID_Txt = 0;
	this->posicion = 0;
	this->bit_inicio = 0;
	this->longitud = 0;
	this->libre = true;
}

Particion::Particion(const Particion &part){

	this->ID_Img = part.ID_Img;
	this->ID_Txt = part.ID_Txt;
	this->posicion = part.posicion;
	this->bit_inicio = part.bit_inicio;
	this->longitud = part.longitud;
	this->libre = part.libre;
}

/*
Particion::Particion(unsigned int ID_Img, unsigned int ID_Txt, unsigned int posicion,
		unsigned int bit_inicio, unsigned int longitud, bool libre){

	this->ID_Img = ID_Img;
	this->ID_Txt = ID_Txt;
	this->posicion = posicion;
	this->bit_inicio = bit_inicio;
	this->longitud = longitud;
	this->libre = libre;
}
*/
Particion::~Particion(){

	this->ID_Img = 0;
	this->ID_Txt = 0;
	this->posicion = 0;
	this->bit_inicio = 0;
	this->longitud = 0;
	this->libre = true;
}

}
