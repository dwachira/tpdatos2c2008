/*
 * NodoAVL.cpp
 *
 *  Created on: 25/09/2008
 *      Author: andres
 */

#include "NodoAVL.h"

/*******************************************************
 * CONSTRUCTOR Y DESTRUCTOR
 *******************************************************/

NodoAVL::NodoAVL(const par_dato dat){
	this->dato = dat;
	this->padre = NULL;
	this->izquierdo = NULL;
	this->derecho = NULL;
	this->FE = 0;
}

NodoAVL::NodoAVL(const par_dato dat, NodoAVL* pad){
	this->dato = dat;
	this->padre = pad;
	this->izquierdo = NULL;
	this->derecho = NULL;
	this->FE = 0;
}
