/*
 * ImagenDAO.cpp
 *
 *  Created on: 03/10/2008
 *      Author: andres
 */
#include "ImagenDAO.h"
#include <cstring>

namespace dao {


/*******************************************************
 * CONSTRUCTOR Y DESTRUCTOR
 *******************************************************/

ImagenDAO::ImagenDAO(){

	this->index_Prim = new Indice(__BASE_DIR__"/INDEX_IMG_Prim.idx", false);
	this->index_Espacio = new Indice(__BASE_DIR__"/INDEX_IMG_Espacio.idx", true);
	this->index_Directorio = new Indice(__BASE_DIR__"/INDEX_IMG_Directorio.idx", true);

	this->archivo = new StreamFijo(__BASE_DIR__"/STREAMFIJO_IMG.str", sizeof(REG_IMG));
	this->stream = new StreamVariable(__BASE_DIR__"/STREAM_IMG.str");
}

ImagenDAO::~ImagenDAO(){

	delete(this->index_Prim);
	delete(this->index_Espacio);
	delete(this->index_Directorio);

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

	this->index_Directorio->insertar((double) buffer->ID_Dir, offset_registro);
	this->index_Espacio->insertar((double) buffer->espacio_libre, offset_registro);

	free(buffer);
	return true;
}

void ImagenDAO::borrar(unsigned int id){

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
		REG_IMG* buffer = new REG_IMG();
		this->archivo->abrir(READ);
		this->archivo->leer(buffer, reg.getOffset());
		this->archivo->cerrar();

		//elimino del archivo de datos
		this->archivo->abrir(DELETE);
		this->archivo->borrar(reg.getOffset());
		this->archivo->cerrar();

		//doy de baja el registro de los indices
		this->index_Prim->eliminar((double) id);
		this->index_Espacio->eliminar((double) buffer->espacio_libre, reg.getOffset());
		this->index_Directorio->eliminar((double) buffer->ID_Dir, reg.getOffset());


		//elimino el nombre de la imagen del archivo de regs de long variable
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
	this->index_Directorio->eliminar((double) dirViejo, reg.getOffset());
	this->index_Directorio->insertar((double) newId_Dir, reg.getOffset());

	return true;
}

bool ImagenDAO::updateEspacioLibre(unsigned int ID, unsigned int newEspacioLibre){

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
	this->index_Espacio->eliminar((double) espacioViejo, reg.getOffset());
	this->index_Espacio->insertar((double) newEspacioLibre, reg.getOffset());

	return true;
}

bool ImagenDAO::updateHashValue(unsigned int ID, string newHashValue){

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
	REG_IMG* buffer = new REG_IMG();
	this->archivo->abrir(READ);
	this->archivo->leer(buffer, reg.getOffset());
	this->archivo->cerrar();

	//actualizo el campo a modificar y sobreescribo en el archivo
	memcpy(buffer->hash_value,newHashValue.data(),Imagen::hashSize);
	this->archivo->abrir(UPDATE);
	this->archivo->actualizar(buffer, reg.getOffset());
	this->archivo->cerrar();

	return true;
}

bool ImagenDAO::updateProxBitLibre(unsigned int ID, unsigned int newProxBitLibre){

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
	REG_IMG* buffer = new REG_IMG();
	this->archivo->abrir(READ);
	this->archivo->leer(buffer, reg.getOffset());
	this->archivo->cerrar();

	//actualizo el campo a modificar y sobreescribo en el archivo
	buffer->prox_bit_libre = newProxBitLibre;
	this->archivo->abrir(UPDATE);
	this->archivo->actualizar(buffer, reg.getOffset());
	this->archivo->cerrar();

	return true;
}

bool ImagenDAO::updateNombre(unsigned int ID, string newNombre){

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

bool ImagenDAO::updateFecha(unsigned int ID, util::Date newFecha){
	unsigned int anio = newFecha.getYear();
	unsigned int mes = newFecha.getMonth();
	unsigned int dia = newFecha.getDay();
	unsigned int hora = newFecha.getHour();
	unsigned int min = newFecha.getMinute();
	unsigned int sec = newFecha.getSecond();

	return updateFecha(ID, anio, mes, dia, hora, min, sec);
}

bool ImagenDAO::updateFecha(unsigned int ID, unsigned int anio, unsigned int mes,
				unsigned int dia, unsigned int hora, unsigned int min, unsigned int sec){

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
	REG_IMG* buffer = new REG_IMG();
	this->archivo->abrir(READ);
	this->archivo->leer(buffer, reg.getOffset());
	this->archivo->cerrar();

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

	return true;
}

Imagen ImagenDAO::getImgById(unsigned int newID){

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
		util::Date* fecha = util::Date::valueOf(0,0,0,0,0,0);
		Imagen img(0,0,0,0,0,0,"",fecha);
		return img;
	}

	RegPagina reg = candidata[i];

	REG_IMG* buffer = new REG_IMG();
	this->archivo->abrir(READ);
	this->archivo->leer(buffer, reg.getOffset());
	this->archivo->cerrar();

	string nombre = this->recuperarNombre(buffer->offset_nombre);
	util::Date* lastModification = util::Date::valueOf(buffer->dia,	buffer->mes,
							buffer->anio, buffer->hora, buffer->min, buffer->sec);
	Imagen img(buffer->ID, buffer->ID_Dir, buffer->espacio_libre, buffer->prox_bit_libre,
							buffer->hash_value, buffer->tamanio, nombre, lastModification);
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
		util::Date* lastModification = util::Date::valueOf(buffer->dia, buffer->mes,
									buffer->anio, buffer->hora, buffer->min, buffer->sec);
		Imagen img(buffer->ID, buffer->ID_Dir, buffer->espacio_libre, buffer->prox_bit_libre,
								buffer->hash_value, buffer->tamanio, nombre,lastModification);
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
			util::Date* lastModification = util::Date::valueOf(buffer->dia, buffer->mes,
									buffer->anio, buffer->hora, buffer->min, buffer->sec);
			Imagen img(buffer->ID, buffer->ID_Dir, buffer->espacio_libre, buffer->prox_bit_libre,
									buffer->hash_value, buffer->tamanio, nombre,lastModification);
			lista.push_back(img);
		}
	}

	this->archivo->cerrar();
	free(buffer);

	return lista;
}

vector<RegPagina> ImagenDAO::recorrer(){

	return (this->index_Prim->recorrerIndice());
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
	memcpy(buffer->hash_value,img.getHash_value().data(),Imagen::hashSize);
	buffer->tamanio = img.getTamanio();
	buffer->prox_bit_libre = img.getProximo_bit_libre();
	buffer->offset_nombre = offset_nombre;

	util::Date fechaUltimaModificacion = img.getFechaUltimaModificacion();
	buffer->anio = fechaUltimaModificacion.getYear();
	buffer->mes = fechaUltimaModificacion.getMonth();
	buffer->dia = fechaUltimaModificacion.getDay();
	buffer->hora = fechaUltimaModificacion.getHour();
	buffer->min = fechaUltimaModificacion.getMinute();
	buffer->sec = fechaUltimaModificacion.getSecond();

	return buffer;
}

}
