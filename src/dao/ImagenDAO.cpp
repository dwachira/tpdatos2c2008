/*
 * ImagenDAO.cpp
 *
 *  Created on: 03/10/2008
 *      Author: andres
 */
#include "ImagenDAO.h"
#include <stdlib.h>

namespace dao {

/*******************************************************
 * CONSTRUCTOR Y DESTRUCTOR
 *******************************************************/

ImagenDAO::ImagenDAO(){

	this->index_Prim = new Indice(__BASE_DIR__"/INDEX_IMG_Prim.idx", false);
	this->index_Espacio = new Indice(__BASE_DIR__"/INDEX_IMG_Espacio.idx", true);
	this->index_Directorio = new Indice(__BASE_DIR__"/INDEX_IMG_Directorio.idx", true);

	this->arbol = new AVL();
	this->minID = 0;
	this->maxID = 0;

	this->archivo = new StreamFijo(__BASE_DIR__"/STREAMFIJO_IMG.str", sizeof(REG_IMG));
	this->stream = new StreamVariable(__BASE_DIR__"/STREAM_IMG.str");
}

ImagenDAO::~ImagenDAO(){

	delete(this->index_Prim);
	delete(this->index_Espacio);
	delete(this->index_Directorio);

	delete(this->arbol);

	delete(this->archivo);
	delete(this->stream);
}


/*******************************************************
 * METODOS PUBLICOS
 *******************************************************/

bool ImagenDAO::insert(Imagen& img){

	//almaceno el nombre en el stream que maneja registros de longitud
	//variable y recupero el offset de insercion
	unsigned long int offset_nombre = guardarNombre(img.getNombre());
	//si devuelve 0, indica que no pudo almacenar la informacion
	if(offset_nombre == 0)
		return false;

	//genero el 'struct' para almacenar los datos en el stream de registros
	//de longitud fija
	REG_IMG* buffer = aStruct(img, offset_nombre);

	bool open = this->archivo->abrir(WRITE);
	if(! open)
		return false;

	//almaceno el registro armado en base a la informacion de la clase en el
	//archivo que maneja registros de longitud fija
	unsigned long int offset_registro = this->archivo->escribir(buffer);
	this->archivo->cerrar();
	//si devolvio 0, indica que no pudo almacenar la informacion
	//******* NO ALMACENO EL REG. FIJO PERO SI PUDO ALMACENAR EL NOMBRE *******
	if(offset_registro == 0)
		return false;

	//inserto en los indices. se indica la clave referente a cada indice y
	//el offset de insercion de los datos en el archivo stream
	bool insertar = this->index_Prim->insertar((double) buffer->ID, offset_registro);
	//***** NO PUDO INSERTAR EN EL INDICE, PERO SI ALMACENO EN LOS STREAMS *****
	if(! insertar)
		return false;

	this->index_Directorio->insertar((double) buffer->ID_Dir, offset_registro);
	this->index_Espacio->insertar((double) buffer->espacio_libre, offset_registro);

	free(buffer);
	return true;
}

Imagen ImagenDAO::getImgById(unsigned int newID){

	//primero verifico con el arbol cargado en memoria. Si la clave buscada es
	//menor a la minima clave de ese arbol, o mayor a la maxima clave de ese
	//arbol, entonces tengo que cargar la pagina candidata a poseer la clave
	//que estoy buscando. Sino, sigo trabajando con el arbol que ya tengo cargado
	//sin tener que acceder al disco ni recorrer el archivo
	if((newID < this->minID) || (newID > this->maxID)){
		//obtengo la pag candidata y armo el arbol con la misma
		vector<RegPagina> candidata = this->index_Prim->getPaginaCandidata((double) newID);
		this->arbol->ArmarArbol(candidata);
		//actualizo los limites del arbol
		this->minID = candidata[0].getID();
		this->maxID = candidata[candidata.size()-1].getID();
	}

	bool buscar = arbol->Buscar((double) newID);

	if(!buscar){		//si no lo encontro, no existe en el indice
		Imagen img(0,0,0,0,0,"");
		return img;
	}

	RegPagina reg = this->arbol->ValorActual();

	REG_IMG* buffer = new REG_IMG();
	this->archivo->abrir(READ);
	this->archivo->leer(buffer, reg.getOffset());
	this->archivo->cerrar();

	string nombre = this->recuperarNombre(buffer->offset_nombre);
	Imagen img(buffer->ID, buffer->ID_Dir, buffer->espacio_libre,
								buffer->hash_value, buffer->tamanio, nombre);
	free(buffer);
	return img;
}

list<Imagen> ImagenDAO::getImgsByDirectorio(unsigned int newID_Dir){

	list<Imagen> lista;
	vector<RegPagina> resultados = this->index_Directorio->leerSecuencial((double) newID_Dir);

	REG_IMG* buffer = new REG_IMG();
	this->archivo->abrir(READ);
	for(unsigned int i=0; i<resultados.size(); i++){
		this->archivo->leer(buffer, resultados[i].getOffset());
		string nombre = this->recuperarNombre(buffer->offset_nombre);
		Imagen img(buffer->ID, buffer->ID_Dir, buffer->espacio_libre,
							buffer->hash_value, buffer->tamanio, nombre);
		lista.push_back(img);
	}

	this->archivo->cerrar();
	free(buffer);

	return lista;
}

list<Imagen> ImagenDAO::getImgsSortedByEspacioLibre(){

	list<Imagen> lista;
	vector<RegPagina> resultados = this->index_Espacio->recorrerIndice();

	REG_IMG* buffer = new REG_IMG();
	this->archivo->abrir(READ);

	//para que queden de mayor a menor arranco desde el final y voy al ppio
	for(unsigned int i=resultados.size()-1; i>=0; i--){
		this->archivo->leer(buffer, resultados[i].getOffset());
		if(buffer->espacio_libre > 0){
			string nombre = this->recuperarNombre(buffer->offset_nombre);
			Imagen img(buffer->ID, buffer->ID_Dir, buffer->espacio_libre,
								buffer->hash_value, buffer->tamanio, nombre);
			lista.push_back(img);
		}
	}

	this->archivo->cerrar();
	free(buffer);

	return lista;
}


/*******************************************************
 * METODOS PRIVADOS
 *******************************************************/

unsigned long int ImagenDAO::guardarNombre(string nombre){

	bool open = this->stream->abrir(WRITE);
	if(! open)
		return 0;

	unsigned long int offset = this->stream->escribir(nombre);
	this->stream->cerrar();

	return offset;
}

string ImagenDAO::recuperarNombre(unsigned long int offset){

	bool open = this->stream->abrir(READ);
	if(! open)
		return "";

	string nombre = this->stream->leer(offset);
	this->stream->cerrar();

	return nombre;
}

REG_IMG* ImagenDAO::aStruct(Imagen img, unsigned long int offset_nombre){

	REG_IMG* buffer = new REG_IMG();
	buffer->ID = img.getID();
	buffer->ID_Dir = img.getID_Dir();
	buffer->espacio_libre = img.getEspacio_libre();
	buffer->hash_value = img.getHash_value();
	buffer->tamanio = img.getTamanio();
	buffer->offset_nombre = offset_nombre;

	return buffer;
}

}
