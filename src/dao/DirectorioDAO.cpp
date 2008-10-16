/*
 * DirectorioDAO.cpp
 *
 *  Created on: 14/10/2008
 *      Author: andres
 */
#include "DirectorioDAO.h"

/*******************************************************
 * CONSTRUCTOR Y DESTRUCTOR
 *******************************************************/

DirectorioDAO::DirectorioDAO(string baseDir){

	this->index_Prim = new Indice((baseDir+"/INDEX_DIR_Prim.idx").c_str(), false);
	this->index_FechaModif = new Indice((baseDir+"/INDEX_DIR_FechaModif.idx").c_str(), true);

	this->archivo = new StreamFijo((baseDir+"/STREAMFIJO_DIR.str").c_str(), sizeof(REG_DIR));
	this->stream = new StreamVariable((baseDir+"/STREAM_DIR.str").c_str());
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

bool DirectorioDAO::insert(Directorio dir){

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

	double claveCompuestaFecha;
/*
 * FALTA ARMAR LA CLAVE COMPUESTA EN BASE A LAS PARTES DE LOS ATRIBUTOS DE LA FECHA
 */
	this->index_FechaModif->insertar(claveCompuestaFecha, offset_registro);

	free(buffer);
	return true;
}

Directorio DirectorioDAO::getDirById(unsigned int newID){

	//obtengo la pag candidata y armo el arbol con la misma
	vector<RegPagina> candidata = this->index_Prim->getPaginaCandidata((double) newID);
	bool encontrado = false;
	unsigned int i = 0;
	while(!encontrado && i<candidata.size()){
		if(candidata[i].getID() == newID)
			encontrado = true;
		else{
			if(candidata[i].getID() > newID)
				i = candidata.size();			//si el leido es mayor, me pase
			else								//y asigno el valor para que
				i++;							//salga como un error.
		}
	}

	if(!encontrado){
		Directorio dir(0,"",0,0,0,0,0);
		return dir;
	}

	RegPagina reg = candidata[i];
	REG_DIR* buffer = new REG_DIR();
	this->archivo->abrir(READ);
	this->archivo->leer(buffer, reg.getOffset());
	this->archivo->cerrar();

	string nombre = this->recuperarPath(buffer->offset_path);
	Directorio dir(buffer->ID,nombre,buffer->dia,buffer->mes,buffer->anio,
			buffer->hora,buffer->min);
	return dir;
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

REG_DIR* DirectorioDAO::aStruct(Directorio dir, unsigned long int offset_path){

	REG_DIR* buffer = new REG_DIR();
	buffer->ID = dir.getID();
	buffer->anio = dir.getAnio();
	buffer->mes = dir.getMes();
	buffer->dia = dir.getDia();
	buffer->hora = dir.getHora();
	buffer->min = dir.getMin();
	buffer->offset_path = offset_path;

	return buffer;
}
