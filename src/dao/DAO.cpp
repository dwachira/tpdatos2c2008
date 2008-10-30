/*
 * DAO.cpp
 *
 *  Created on: 30/10/2008
 *      Author: andres
 */
#include "DAO.h"

namespace dao{

void DAO::openStream(){

	this->stream->abrir(READ);
	this->stream->seek_beg();
}

unsigned long int DAO::leerProximo(string* cadena){

	return this->stream->leerProximo(cadena);
}

void DAO::closeStream(){

	this->stream->cerrar();
}



unsigned long int DAO::guardarNombre(string nombre){

	bool open = this->stream->abrir(WRITE);
	if(! open)
		return 0;

	unsigned long int offset = this->stream->escribir(nombre);
	this->stream->cerrar();

	return offset;
}

string DAO::recuperarNombre(unsigned long int offset){

	bool open = this->stream->abrir(READ);
	if(! open)
		return "";

	string nombre = this->stream->leer(offset);
	this->stream->cerrar();

	return nombre;
}

}
