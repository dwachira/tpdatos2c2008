/*
 * MensajeDAO.cpp
 *
 *  Created on: 14/10/2008
 *      Author: andres
 */
#include "MensajeDAO.h"

namespace dao {

/*******************************************************
 * CONSTRUCTOR Y DESTRUCTOR
 *******************************************************/

MensajeDAO::MensajeDAO(){

	this->index_Prim = new Indice(__BASE_DIR__"/INDEX_MSJ_Prim.idx", false);
	this->index_Tamanio = new Indice(__BASE_DIR__"/INDEX_MSJ_Tamanio.idx", true);

	this->archivo = new StreamFijo(__BASE_DIR__"/STREAMFIJO_MSJ.str", sizeof(REG_MSJ));
	this->stream = new StreamVariable(__BASE_DIR__"/STREAM_MSJ.str");
}

MensajeDAO::~MensajeDAO(){

	delete(this->index_Prim);
	delete(this->index_Tamanio);

	delete(this->archivo);
	delete(this->stream);
}


/*******************************************************
 * METODOS PUBLICOS
 *******************************************************/

bool MensajeDAO::insert(Mensaje& msj){

	//almaceno el nombre en el stream que maneja registros de longitud
	//variable y recupero el offset de insercion
	unsigned long int offset_nombre = guardarNombre(msj.getNombre());
	//si devuelve 0, indica que no pudo almacenar la informacion
	if(offset_nombre == 0)
		return false;

	//genero el 'struct' para almacenar los datos en el stream de registros
	//de longitud fija
	REG_MSJ* buffer = aStruct(msj, offset_nombre);

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

	this->index_Tamanio->insertar((double) buffer->tamanio, offset_registro);


	free(buffer);
	return true;
}

void MensajeDAO::borrar(unsigned int id){

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

		//recupero la informacion almacenada, requerido para poder dar de baja un indice
		REG_MSJ* buffer = new REG_MSJ();
		this->archivo->abrir(READ);
		this->archivo->leer(buffer, reg.getOffset());
		this->archivo->cerrar();

		//elimino del archivo de datos
		this->archivo->abrir(DELETE);
		this->archivo->borrar(reg.getOffset());
		this->archivo->cerrar();

		//doy de baja el registro de los indices
		this->index_Prim->eliminar((double) id);
		this->index_Tamanio->eliminar((double) buffer->tamanio,reg.getOffset());

		//elimino el nombre del mensaje del archivo de regs de long variable
		this->stream->borrar(buffer->offset_nombre);
	}
}

void MensajeDAO::borrar(Mensaje& msj){

	unsigned int id = msj.getID();
	return borrar(id);

	//debo utilizar la otra funcion porque no puedo evitar tener que leer desde
	//el archivo la informacion. Esto es porque sino resultaria imposible dar
	//de baja el nombre del mensaje del archivo de registros de longitud variable
	//Por ese motivo es que debe seguirse esta secuencia
}

bool MensajeDAO::update(unsigned int ID, int newCantPartes){

	//obtengo la pag candidata
	vector<RegPagina> candidata = this->index_Prim->getPaginaCandidata((double) ID);

	/********************************************/
	bool encontrado = false;
	unsigned int i = 0;							//TODO esto se deberia reemplazar
	while(!encontrado && i<candidata.size()){	//por una busqueda binaria
		if(candidata[i].getID() == ID)			//para hacerlo mas eficiente
			encontrado = true;
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

	RegPagina reg = candidata[i];
	REG_MSJ* buffer = new REG_MSJ();
	this->archivo->abrir(READ);
	this->archivo->leer(buffer, reg.getOffset());
	this->archivo->cerrar();

	//actualizo los valores que se van a modificar y sobreescribo el archivo
	buffer->cant_partes = newCantPartes;
	this->archivo->abrir(UPDATE);
	this->archivo->actualizar(buffer, reg.getOffset());
	this->archivo->cerrar();

	return true;
}

Mensaje MensajeDAO::getMsjById(unsigned int newID){

	//obtengo la pag candidata
	vector<RegPagina> candidata = this->index_Prim->getPaginaCandidata((double) newID);

	/********************************************/
	bool encontrado = false;
	unsigned int i = 0;							//TODO esto se deberia reemplazar
	while(!encontrado && i<candidata.size()){	//por una busqueda binaria
		if(candidata[i].getID() == newID)		//para hacerlo mas eficiente
			encontrado = true;
		else{
			if(candidata[i].getID() > newID)
				i = candidata.size();			//si el leido es mayor, me pase
			else								//y asigno el valor para que
				i++;							//salga como un error.
		}
	}
	/********************************************/

	if(!encontrado){		//si no lo encontro, no existe en el indice
		Mensaje msj(0,"",0,0);
		return msj;
	}

	RegPagina reg = candidata[i];

	REG_MSJ* buffer = new REG_MSJ();
	this->archivo->abrir(READ);
	this->archivo->leer(buffer, reg.getOffset());
	this->archivo->cerrar();

	string nombre = this->recuperarNombre(buffer->offset_nombre);
	Mensaje msj(buffer->ID,nombre,buffer->tamanio,buffer->cant_partes);

	free(buffer);
	return msj;
}

vector<RegPagina> MensajeDAO::recorrer(){

	return (this->index_Prim->recorrerIndice());
}


/*******************************************************
 * METODOS PRIVADOS
 *******************************************************/

unsigned long int MensajeDAO::guardarNombre(string nombre){

	bool open = this->stream->abrir(WRITE);
	if(! open)
		return 0;

	unsigned long int offset = this->stream->escribir(nombre);
	this->stream->cerrar();

	return offset;
}

string MensajeDAO::recuperarNombre(unsigned long int offset){

	bool open = this->stream->abrir(READ);
	if(! open)
		return "";

	string nombre = this->stream->leer(offset);
	this->stream->cerrar();

	return nombre;
}

REG_MSJ* MensajeDAO::aStruct(Mensaje msj, unsigned long int offset_nombre){

	REG_MSJ* buffer = new REG_MSJ();
	buffer->ID = msj.getID();
	buffer->cant_partes = msj.getCant_partes();
	buffer->tamanio = msj.getTamanio();
	buffer->offset_nombre = offset_nombre;

	return buffer;
}

}
