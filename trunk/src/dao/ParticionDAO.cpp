/*
 * ParticionDAO.cpp
 *
 *  Created on: 14/10/2008
 *      Author: andres
 */
#include "ParticionDAO.h"
#include <stdlib.h>

namespace dao {

/*******************************************************
 * CONSTRUCTOR Y DESTRUCTOR
 *******************************************************/

ParticionDAO::ParticionDAO(){

	this->index_Prim = new Indice(__BASE_DIR__"/INDEX_PART_Prim.idx", false);
	this->index_Img = new Indice(__BASE_DIR__"/INDEX_PART_Img.idx", true);
	this->index_Txt = new Indice(__BASE_DIR__"/INDEX_PART_Txt.idx", true);
	this->index_Libres = new Indice(__BASE_DIR__"/INDEX_PART_Lib.idx", true);

	this->archivo = new StreamFijo(__BASE_DIR__"/STREAMFIJO_PART.str", sizeof(REG_PART));
}

ParticionDAO::~ParticionDAO(){

	delete(this->index_Prim);
	delete(this->index_Img);
	delete(this->index_Txt);

	delete(this->archivo);
}


/*******************************************************
 * METODOS PUBLICOS
 *******************************************************/

bool ParticionDAO::insert(Particion part){

	//genero el 'struct' para almacenar los datos en el stream de registros
	//de longitud fija
	REG_PART* buffer = aStruct(part);

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
	double claveCompuestaPrim = StringUtils::concat
							(buffer->ID_Img,buffer->ID_Txt,buffer->posicion);

	bool insertar = this->index_Prim->insertar(claveCompuestaPrim, offset_registro);
	//***** NO PUDO INSERTAR EN EL INDICE, PERO SI ALMACENO EN LOS STREAMS *****
	if(! insertar)
		return false;

	this->index_Img->insertar((double) buffer->ID_Img, offset_registro);
	this->index_Txt->insertar((double) buffer->ID_Txt, offset_registro);

	//si la particion esta marcada como libre, se agrega en el indice que indica
	//las posiciones libres que se pueden usar mas adelante
	if(buffer->libre){
		double claveCompuestaLibres = StringUtils::concat
											(buffer->posicion,buffer->longitud);

		this->index_Libres->insertar(claveCompuestaLibres, offset_registro);
	}

	free(buffer);
	return true;
}

Particion ParticionDAO::getPartByImgTxtPos(unsigned int newImg, unsigned int newTxt, unsigned int newPos){

	double claveBuscada = StringUtils::concat(newImg,newTxt,newPos);

	//primero verifico con el arbol cargado en memoria. Si la clave buscada es
	//menor a la minima clave de ese arbol, o mayor a la maxima clave de ese
	//arbol, entonces tengo que cargar la pagina candidata a poseer la clave
	//que estoy buscando. Sino, sigo trabajando con el arbol que ya tengo cargado
	//sin tener que acceder al disco ni recorrer el archivo
	if((claveBuscada < this->minID) || (claveBuscada > this->maxID)){
		//obtengo la pag candidata y armo el arbol con la misma
		vector<RegPagina> candidata = this->index_Prim->getPaginaCandidata(claveBuscada);
		this->arbol->ArmarArbol(candidata);
		//actualizo los limites del arbol
		this->minID = candidata[0].getID();
		this->maxID = candidata[candidata.size()-1].getID();
	}

	bool buscar = arbol->Buscar(claveBuscada);

	if(!buscar){		//si no lo encontro, no existe en el indice
		Particion partic(0,0,0,0,0,false);
		return partic;
	}

	RegPagina reg = this->arbol->ValorActual();

	REG_PART* buffer = new REG_PART();
	this->archivo->abrir(READ);
	this->archivo->leer(buffer, reg.getOffset());
	this->archivo->cerrar();

	Particion partic(buffer->ID_Img,buffer->ID_Txt,buffer->posicion,
						buffer->bit_inicio,buffer->longitud,buffer->libre);
	free(buffer);
	return partic;
}

list<Particion> ParticionDAO::getPartsByImg(unsigned int newImg){

	list<Particion> lista;
	vector<RegPagina> resultados = this->index_Img->leerSecuencial((double) newImg);

	REG_PART* buffer = new REG_PART();
	this->archivo->abrir(READ);
	for(unsigned int i=0; i<resultados.size(); i++){
		this->archivo->leer(buffer, resultados[i].getOffset());
		Particion partic(buffer->ID_Img,buffer->ID_Txt,buffer->posicion,
								buffer->bit_inicio,buffer->longitud,buffer->libre);
		lista.push_back(partic);
	}

	this->archivo->cerrar();
	free(buffer);

	return lista;
}

list<Particion> ParticionDAO::getPartsByTxt(unsigned int newTxt){

	list<Particion> lista;
	vector<RegPagina> resultados = this->index_Txt->leerSecuencial((double) newTxt);

	REG_PART* buffer = new REG_PART();
	this->archivo->abrir(READ);
	for(unsigned int i=0; i<resultados.size(); i++){
		this->archivo->leer(buffer, resultados[i].getOffset());
		Particion partic(buffer->ID_Img,buffer->ID_Txt,buffer->posicion,
								buffer->bit_inicio,buffer->longitud,buffer->libre);
		lista.push_back(partic);
	}

	this->archivo->cerrar();
	free(buffer);

	return lista;
}


/*******************************************************
 * METODOS PRIVADOS
 *******************************************************/


REG_PART* ParticionDAO::aStruct(Particion part){

	REG_PART* buffer = new REG_PART();
	buffer->ID_Img = part.getID_Img();
	buffer->ID_Txt = part.getID_Txt();
	buffer->bit_inicio = part.getBit_inicio();
	buffer->libre = part.isLibre();
	buffer->longitud = part.getLongitud();
	buffer->posicion = part.getPosicion();

	return buffer;
}

}
