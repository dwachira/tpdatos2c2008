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

	this->index_Prim = new Indice("INDEX_MSJ_Prim.idx", false);
	this->index_Tamanio = new Indice("INDEX_MSJ_Tamanio.idx", true);

	this->archivo = new StreamFijo("STREAMFIJO_MSJ.str", sizeof(REG_MSJ));
	this->stream = new StreamVariable("STREAM_MSJ.str");

	this->arbol = new AVL();
}

MensajeDAO::~MensajeDAO(){

	delete(this->index_Prim);
	delete(this->index_Tamanio);

	delete(this->archivo);
	delete(this->stream);

	delete(this->arbol);
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

	//si lo que inserte iba dentro de la pagina que se mantiene en buffer, la
	//vuelvo a cargar despues de la insercion.
	if((msj.getID() >= this->minID) && (msj.getID() <= this->maxID)){
		//obtengo la pag candidata y armo el arbol con la misma
		vector<RegPagina> candidata = this->index_Prim->getPaginaCandidata((double) msj.getID());
		this->arbol->ArmarArbol(candidata);
		//actualizo los limites del arbol
		this->minID = candidata[0].getID();
		this->maxID = candidata[candidata.size()-1].getID();
	}

	free(buffer);
	return true;
}

void MensajeDAO::borrar(unsigned int id){

	//primero verifico con el arbol cargado en memoria. Si la clave buscada es
	//menor a la minima clave de ese arbol, o mayor a la maxima clave de ese
	//arbol, entonces tengo que cargar la pagina candidata a poseer la clave
	//que estoy buscando. Sino, sigo trabajando con el arbol que ya tengo cargado
	//sin tener que acceder al disco ni recorrer el archivo
	if((id < this->minID) || (id > this->maxID)){
		//obtengo la pag candidata y armo el arbol con la misma
		vector<RegPagina> candidata = this->index_Prim->getPaginaCandidata((double) id);
		this->arbol->ArmarArbol(candidata);
		//actualizo los limites del arbol
		this->minID = candidata[0].getID();
		this->maxID = candidata[candidata.size()-1].getID();
	}

	if(arbol->Buscar((double) id)){

		RegPagina reg = this->arbol->ValorActual();

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
		this->index_Tamanio->eliminar((double) buffer->tamanio);

		//cargo la nueva pagina del indice, ya que sufrio modificaciones
		vector<RegPagina> candidata = this->index_Prim->getPaginaCandidata(id);
		this->arbol->ArmarArbol(candidata);
		//actualizo los limites del arbol
		this->minID = candidata[0].getID();
		this->maxID = candidata[candidata.size()-1].getID();

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

	//primero verifico con el arbol cargado en memoria. Si la clave buscada es
	//menor a la minima clave de ese arbol, o mayor a la maxima clave de ese
	//arbol, entonces tengo que cargar la pagina candidata a poseer la clave
	//que estoy buscando. Sino, sigo trabajando con el arbol que ya tengo cargado
	//sin tener que acceder al disco ni recorrer el archivo
	if((ID < this->minID) || (ID > this->maxID)){
		//obtengo la pag candidata y armo el arbol con la misma
		vector<RegPagina> candidata = this->index_Prim->getPaginaCandidata((double) ID);
		this->arbol->ArmarArbol(candidata);
		//actualizo los limites del arbol
		this->minID = candidata[0].getID();
		this->maxID = candidata[candidata.size()-1].getID();
	}

	if(! arbol->Buscar((double) ID))
		return false;

	//recupero la informacion almacenada
	RegPagina reg = this->arbol->ValorActual();
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
		Mensaje msj(0,"",0,0);
		return msj;
	}

	RegPagina reg = this->arbol->ValorActual();

	REG_MSJ* buffer = new REG_MSJ();
	this->archivo->abrir(READ);
	this->archivo->leer(buffer, reg.getOffset());
	this->archivo->cerrar();

	string nombre = this->recuperarNombre(buffer->offset_nombre);
	Mensaje msj(buffer->ID,nombre,buffer->tamanio,buffer->cant_partes);

	free(buffer);
	return msj;
}


/*******************************************************
 * METODOS PRIVADOS
 *******************************************************/
/*
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
*/
REG_MSJ* MensajeDAO::aStruct(Mensaje msj, unsigned long int offset_nombre){

	REG_MSJ* buffer = new REG_MSJ();
	buffer->ID = msj.getID();
	buffer->cant_partes = msj.getCant_partes();
	buffer->tamanio = msj.getTamanio();
	buffer->offset_nombre = offset_nombre;

	return buffer;
}

}
