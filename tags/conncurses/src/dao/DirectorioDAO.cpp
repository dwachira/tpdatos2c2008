/*
 * DirectorioDAO.cpp
 *
 *  Created on: 14/10/2008
 *      Author: andres
 */
#include "DirectorioDAO.h"
#include <stdlib.h>

using namespace util;

namespace dao {

/*******************************************************
 * CONSTRUCTOR Y DESTRUCTOR
 *******************************************************/

DirectorioDAO::DirectorioDAO(){

	this->index_Prim = new Indice(PACKAGE_DATA_DIR"/INDEX_DIR_Prim.idx", false);
	this->index_FechaModif = new Indice(PACKAGE_DATA_DIR"/INDEX_DIR_FechaModif.idx", true);

	this->archivo = new StreamFijo(PACKAGE_DATA_DIR"/STREAMFIJO_DIR.str", sizeof(REG_DIR));
	this->stream = new StreamVariable(PACKAGE_DATA_DIR"/STREAM_DIR.str");
}

DirectorioDAO::~DirectorioDAO(){

	delete(this->index_Prim);
	delete(this->index_FechaModif);

	delete(this->archivo);
	delete(this->stream);
}


/*******************************************************
 * METODOS PUBLICOS
 *******************************************************/

bool DirectorioDAO::insert(Directorio& dir){

	//almaceno el nombre en el stream que maneja registros de longitud
	//variable y recupero el offset de insercion
	unsigned long int offset_path = guardarPath(dir.getPath());
	//si devuelve 0, indica que no pudo almacenar la informacion
	if(offset_path == 0)
		return false;

	//genero el 'struct' para almacenar los datos en el stream de registros
	//de longitud fija
	REG_DIR* buffer = aStruct(dir, offset_path);

	bool open = this->archivo->abrir(WRITE);
	//******* NO ALMACENO EL REG. FIJO PERO SI PUDO ALMACENAR EL NOMBRE *******
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

	//se arma la clave compuesta concatenando los valores de la fecha
	double claveCompuestaFecha = Date::concatFecha(buffer->anio, buffer->mes, buffer->dia,
									buffer->hora, buffer->min, buffer->sec);

	this->index_FechaModif->insertar(claveCompuestaFecha, offset_registro);

	free(buffer);
	return true;
}

void DirectorioDAO::borrar(unsigned int id){

	//obtengo la pag candidata
	vector<RegPagina> candidata = this->index_Prim->getPaginaCandidata((double) id);

	/********************************************/
	bool encontrado = false;
	unsigned int i = 0;							//TODO esto se deberia reemplazar
	while(!encontrado && i<candidata.size()){	//por una busqueda binaria
		if(candidata[i].getID() == id)			//para hacerlo mas eficiente
			encontrado = true;
		else{
			if(candidata[i].getID() > id)
				i = candidata.size();			//si el leido es mayor, me pase
			else								//y asigno el valor para que
				i++;							//salga como un error.
		}
	}
	/********************************************/

	if(encontrado){

		RegPagina reg = candidata[i];
		REG_DIR* buffer = new REG_DIR();
		this->archivo->abrir(READ);
		this->archivo->leer(buffer, reg.getOffset());
		this->archivo->cerrar();

		//elimino del archivo de datos
		this->archivo->abrir(DELETE);
		this->archivo->borrar(reg.getOffset());
		this->archivo->cerrar();

		//doy de baja el registro de los indices
		this->index_Prim->eliminar((double) id);

		//se arma la clave compuesta concatenando los valores de la fecha
		double claveCompuestaFecha = Date::concatFecha(buffer->anio, buffer->mes,
								buffer->dia, buffer->hora, buffer->min, buffer->sec);
		this->index_FechaModif->eliminar(claveCompuestaFecha, reg.getOffset());

		//elimino el nombre del directorio del archivo de regs de long variable
		this->stream->abrir(DELETE);
		this->stream->borrar(buffer->offset_path);
		this->stream->cerrar();
	}
}

void DirectorioDAO::borrar(Directorio& dir){

	unsigned int id = dir.getID();
	return borrar(id);

	//debo utilizar la otra funcion porque no puedo evitar tener que leer desde
	//el archivo la informacion. Esto es porque sino resultaria imposible dar
	//de baja el nombre del directorio del archivo de registros de longitud variable
	//Por ese motivo es que debe seguirse esta secuencia
}

bool DirectorioDAO::update(unsigned int ID, unsigned int anio, unsigned int mes,
				unsigned int dia, unsigned int hora, unsigned int min, unsigned int sec){

	//obtengo la pag candidata y busco el Id solicitado
	vector<RegPagina> candidata = this->index_Prim->getPaginaCandidata((double) ID);

	/********************************************/
	bool encontrado = false;
	unsigned int i = 0;							//TODO esto se deberia
	while(!encontrado && i<candidata.size()){	//reemplazar por una
		if(candidata[i].getID() == ID)			//busqueda binaria
			encontrado = true;					//para hacerlo mas eficiente
		else{
			if(candidata[i].getID() > ID)
				i = candidata.size();			//si el leido es mayor, me pase
			else								//y asigno el valor para que
				i++;							//salga como un error.
		}
	}
	/********************************************/

	if(!encontrado)
		return false;

	//si existe, recupero la informacion
	RegPagina reg = candidata[i];
	REG_DIR* buffer = new REG_DIR();
	this->archivo->abrir(READ);
	this->archivo->leer(buffer, reg.getOffset());
	this->archivo->cerrar();

	//recupero la fecha vieja para poder eliminar del indice
	unsigned int anioViejo = buffer->anio;
	unsigned int mesViejo = buffer->mes;
	unsigned int diaViejo = buffer->dia;
	unsigned int horaViejo = buffer->hora;
	unsigned int minViejo = buffer->min;
	unsigned int secViejo = buffer->sec;

	//actualizo los valores de la fecha de modificacion
	buffer->anio = anio;
	buffer->mes = mes;
	buffer->dia = dia;
	buffer->hora = hora;
	buffer->min = min;
	buffer->sec = sec;

	//y sobreescribo en el archivo
	this->archivo->abrir(UPDATE);
	this->archivo->actualizar(buffer, reg.getOffset());
	this->archivo->cerrar();

	//y elimino y inserto del indice pertinente
	//se arma la clave compuesta concatenando los valores de la fecha
	double claveCompuestaFecha = Date::concatFecha(anioViejo, mesViejo,
			diaViejo, horaViejo, minViejo, secViejo);
	//y se arma la clave nueva
	double newClaveCompuestaFecha = Date::concatFecha(anio, mes, dia, hora, min, sec);

	this->index_FechaModif->eliminar(claveCompuestaFecha, reg.getOffset());
	this->index_FechaModif->insertar(newClaveCompuestaFecha, reg.getOffset());

	return true;
}

bool DirectorioDAO::update(unsigned int ID, util::Date newFecha){

	unsigned int anio = newFecha.getYear();
	unsigned int mes = newFecha.getMonth();
	unsigned int dia = newFecha.getDay();
	unsigned int hora = newFecha.getHour();
	unsigned int min = newFecha.getMinute();
	unsigned int sec = newFecha.getSecond();

	return update(ID, anio, mes, dia, hora, min, sec);
}

Directorio* DirectorioDAO::getDirById(unsigned int newID){

	//obtengo la pag candidata y armo el arbol con la misma
	vector<RegPagina> candidata = this->index_Prim->getPaginaCandidata((double) newID);

	/********************************************/
	bool encontrado = false;
	unsigned int i = 0;							//TODO esto se deberia
	while(!encontrado && i<candidata.size()){	//reemplazar por una
		if(candidata[i].getID() == newID)		//busqueda binaria
			encontrado = true;
		else{
			if(candidata[i].getID() > newID)
				i = candidata.size();			//si el leido es mayor, me pase
			else								//y asigno el valor para que
				i++;							//salga como un error.
		}
	}
	/********************************************/

	if(!encontrado)
		return NULL;

	RegPagina reg = candidata[i];
	REG_DIR* buffer = new REG_DIR();
	this->archivo->abrir(READ);
	this->archivo->leer(buffer, reg.getOffset());
	this->archivo->cerrar();

	unsigned int id = buffer->ID;
	string nombre = this->recuperarPath(buffer->offset_path);
	Date* lastModification = Date::valueOf(buffer->dia,buffer->mes,buffer->anio,
												buffer->hora,buffer->min,buffer->sec);
	Directorio* dir = new Directorio(id,nombre,lastModification);
	return dir;
}

list<Directorio> DirectorioDAO::getDirsSortedByFechaModif(){

	list<Directorio> lista;
	vector<RegPagina> resultados = this->index_FechaModif->recorrerIndice();

	REG_DIR* buffer = new REG_DIR();
	this->archivo->abrir(READ);
	for(unsigned int i=0; i<resultados.size(); i++){
		this->archivo->leer(buffer, resultados[i].getOffset());
		unsigned int id = buffer->ID;
		string nombre = this->recuperarPath(buffer->offset_path);
		Date* lastModification = Date::valueOf(buffer->dia,buffer->mes,buffer->anio,
												buffer->hora,buffer->min,buffer->sec);
		Directorio* dir = new Directorio(id,nombre,lastModification);
		lista.push_back(*dir);
	}

	this->archivo->cerrar();
	free(buffer);

	return lista;
}

vector<RegPagina> DirectorioDAO::recorrer(){

	return (this->index_Prim->recorrerIndice());
}


/*******************************************************
 * METODOS PRIVADOS
 *******************************************************/

unsigned long int DirectorioDAO::guardarPath(string nombre){

	bool open = this->stream->abrir(WRITE);
	if(! open)
		return 0;

	unsigned long int offset = this->stream->escribir(nombre);
	this->stream->cerrar();

	return offset;
}

string DirectorioDAO::recuperarPath(unsigned long int offset){

	bool open = this->stream->abrir(READ);
	if(! open)
		return "";

	string nombre = this->stream->leer(offset);
	this->stream->cerrar();

	return nombre;
}

REG_DIR* DirectorioDAO::aStruct(const Directorio& dir, unsigned long int offset_path){

	REG_DIR* buffer = new REG_DIR();
	util::Date fechaUltimaModificacion = dir.getFechaUltimaModificacion();

	buffer->ID = dir.getID();
	buffer->anio = fechaUltimaModificacion.getYear();
	buffer->mes = fechaUltimaModificacion.getMonth();
	buffer->dia = fechaUltimaModificacion.getDay();
	buffer->hora = fechaUltimaModificacion.getHour();
	buffer->min = fechaUltimaModificacion.getMinute();
	buffer->sec = fechaUltimaModificacion.getSecond();
	buffer->offset_path = offset_path;

	return buffer;
}

}
