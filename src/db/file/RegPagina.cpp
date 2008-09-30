/*
 * RegPagina.cpp
 *
 *  Created on: 25/09/2008
 *      Author: andres
 */

#include "RegPagina.h"

/*******************************************************
 * CONSTRUCTOR Y DESTRUCTOR
 *******************************************************/

RegPagina::RegPagina(){}

RegPagina::RegPagina(double newID, unsigned long int newOffset) {
	this->ID = newID;
	this->offset = newOffset;
}

RegPagina::~RegPagina() { }


/*******************************************************
 * METODOS PUBLICOS
 *******************************************************/

void RegPagina::setID(double newID){
	this->ID = newID;
}
void RegPagina::setOffset(unsigned long int newOffset){
	this->offset = newOffset;
}

double RegPagina::getID(){
	return this->ID;
}
unsigned long int RegPagina::getOffset(){
	return this->offset;
}
