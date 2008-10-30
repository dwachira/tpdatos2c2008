/*
 * ImagenDAO.cpp
 *
 *  Created on: 03/10/2008
 *      Author: andres
 */
#include "ImagenDAO.h"
#include <stdlib.h>

namespace dao {

int ImagenDAO::incrementalId = 0;

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

	img.setID(getNewId());

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

	//si lo que inserte iba dentro de la pagina que se mantiene en buffer, la
	//vuelvo a cargar despues de la insercion.
	if((img.getID() >= this->minID) && (img.getID() <= this->maxID)){
		//obtengo la pag candidata y armo el arbol con la misma
		vector<RegPagina> candidata = this->index_Prim->getPaginaCandidata((double) img.getID());
		this->arbol->ArmarArbol(candidata);
		//actualizo los limites del arbol
		this->minID = candidata[0].getID();
		this->maxID = candidata[candidata.size()-1].getID();
	}

	free(buffer);
	return true;
}

void ImagenDAO::borrar(unsigned int id){

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
		REG_IMG* buffer = new REG_IMG();
		this->archivo->abrir(READ);
		this->archivo->leer(buffer, reg.getOffset());
		this->archivo->cerrar();

		//elimino del archivo de datos
		this->archivo->abrir(DELETE);
		this->archivo->borrar(reg.getOffset());
		this->archivo->cerrar();

		//cargo la nueva pagina del indice, ya que sufrio modificaciones
		vector<RegPagina> candidata = this->index_Prim->getPaginaCandidata(id);
		this->arbol->ArmarArbol(candidata);
		//actualizo los limites del arbol
		this->minID = candidata[0].getID();
		this->maxID = candidata[candidata.size()-1].getID();

		//doy de baja el registro de los indices
		this->index_Prim->eliminar((double) id);
		this->index_Espacio->eliminar((double) buffer->espacio_libre);
		this->index_Directorio->eliminar((double) buffer->ID_Dir);

		//elimino el nombre de la iamgen del archivo de regs de long variable
		this->stream->borrar(buffer->offset_nombre);
	}
}

void ImagenDAO::borrar(Imagen& img){

	unsigned int id = img.getID();

	return borrar(id);

	//debo utilizar la otra funcion porque no puedo evitar tener que leer desde
	//el archivo la informacion. Esto es porque sino resultaria imposible dar
	//de baja el nombre de la imagen del archivo de registros de longitud variable
	//Por ese motivo es que debe seguirse esta secuencia
}

bool ImagenDAO::updateDirectorio(unsigned int ID, unsigned int newId_Dir){

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

	//si encontro el dato, lo recupero para actualizarlo
	RegPagina reg = this->arbol->ValorActual();
	REG_IMG* buffer = new REG_IMG();
	this->archivo->abrir(READ);
	this->archivo->leer(buffer, reg.getOffset());
	this->archivo->cerrar();

	//recupero el dato viejo para poder dar de baja en el indice
	unsigned int dirViejo = buffer->ID_Dir;

	//actualizo el campo a modificar y sobreescribo en el archivo
	buffer->ID_Dir = newId_Dir;
	this->archivo->abrir(UPDATE);
	this->archivo->actualizar(buffer, reg.getOffset());
	this->archivo->cerrar();

	//finalmente, doy de baja e inserto del indice correspondiente
	this->index_Directorio->eliminar((double) dirViejo);
	this->index_Directorio->insertar((double) newId_Dir, reg.getOffset());

	return true;
}

bool ImagenDAO::updateEspacioLibre(unsigned int ID, unsigned int newEspacioLibre){

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

	//si encontro el dato, lo recupero para actualizarlo
	RegPagina reg = this->arbol->ValorActual();
	REG_IMG* buffer = new REG_IMG();
	this->archivo->abrir(READ);
	this->archivo->leer(buffer, reg.getOffset());
	this->archivo->cerrar();

	//recupero el dato viejo para poder dar de baja en el indice
	unsigned int espacioViejo = buffer->espacio_libre;

	//actualizo el campo a modificar y sobreescribo en el archivo
	buffer->espacio_libre = newEspacioLibre;
	this->archivo->abrir(UPDATE);
	this->archivo->actualizar(buffer, reg.getOffset());
	this->archivo->cerrar();

	//finalmente, doy de baja e inserto del indice correspondiente
	this->index_Espacio->eliminar((double) espacioViejo);
	this->index_Espacio->insertar((double) newEspacioLibre, reg.getOffset());

	return true;
}

bool ImagenDAO::updateHashValue(unsigned int ID, unsigned long int newHashValue){

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

	//si encontro el dato, lo recupero para actualizarlo
	RegPagina reg = this->arbol->ValorActual();
	REG_IMG* buffer = new REG_IMG();
	this->archivo->abrir(READ);
	this->archivo->leer(buffer, reg.getOffset());
	this->archivo->cerrar();

	//actualizo el campo a modificar y sobreescribo en el archivo
	buffer->hash_value = newHashValue;
	this->archivo->abrir(UPDATE);
	this->archivo->actualizar(buffer, reg.getOffset());
	this->archivo->cerrar();

	return true;
}

bool ImagenDAO::updateNombre(unsigned int ID, string newNombre){

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

	//si encontro el dato, lo recupero para actualizarlo
	RegPagina reg = this->arbol->ValorActual();
	REG_IMG* buffer = new REG_IMG();
	this->archivo->abrir(READ);
	this->archivo->leer(buffer, reg.getOffset());
	this->archivo->cerrar();

	//elimino el nombre del archivo de registros de longitud variable
	this->stream->abrir(DELETE);
	this->stream->borrar(buffer->offset_nombre);
	this->stream->cerrar();

	//inserto el nuevo nombre
	unsigned long int newOffset = guardarNombre(newNombre);

	//actualizo el valor en el registro y sobreescribo en el archivo
	buffer->offset_nombre = newOffset;
	this->archivo->abrir(UPDATE);
	this->archivo->actualizar(buffer, reg.getOffset());
	this->archivo->cerrar();

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
		Imagen img(0,0,0,0,0,0,"");
		return img;
	}

	RegPagina reg = this->arbol->ValorActual();

	REG_IMG* buffer = new REG_IMG();
	this->archivo->abrir(READ);
	this->archivo->leer(buffer, reg.getOffset());
	this->archivo->cerrar();

	string nombre = this->recuperarNombre(buffer->offset_nombre);
	Imagen img(buffer->ID, buffer->ID_Dir, buffer->espacio_libre,0,
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
		Imagen img(buffer->ID, buffer->ID_Dir, buffer->espacio_libre,0,
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
	for(int i=resultados.size()-1; i>=0; i--){
		this->archivo->leer(buffer, resultados[i].getOffset());
		if(buffer->espacio_libre > 0){
			string nombre = this->recuperarNombre(buffer->offset_nombre);
			Imagen img(buffer->ID, buffer->ID_Dir, buffer->espacio_libre,0,
								buffer->hash_value, buffer->tamanio, nombre);
			lista.push_back(img);
		}
	}

	this->archivo->cerrar();
	free(buffer);

	return lista;
}
/*
void ImagenDAO::openStream(){

	this->stream->abrir(READ);
	this->stream->seek_beg();
}

unsigned long int ImagenDAO::leerProximo(string* cadena){

	return this->stream->leerProximo(cadena);
}

void ImagenDAO::closeStream(){

	this->stream->cerrar();
}
*/

/*******************************************************
 * METODOS PRIVADOS
 *******************************************************/
/*
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
*/
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
