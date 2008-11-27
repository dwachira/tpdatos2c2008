/*
 * TrieDAO.cpp
 *
 *  Created on: 21/10/2008
 *      Author: andres
 */
#include "TrieDAO.h"

namespace dao {

/*******************************************************
 * CONSTRUCTOR Y DESTRUCTOR
 *******************************************************/

TrieDAO::TrieDAO(ManagerDAO& managerDao) : daoManager(managerDao) {
	this->mensajes = new Trie();
	this->directorios = new Trie();
	this->imagenes = new Trie();
}

TrieDAO::~TrieDAO(){

	delete(this->directorios);
	delete(this->imagenes);
	delete(this->mensajes);
}


/*******************************************************
 * METODOS PUBLICOS
 *******************************************************/

void TrieDAO::loadTrie(int codigoTrie){

	vector<RegPagina> result;
	switch(codigoTrie){
		case IMAGENES: result = daoManager.getImagenDAO().recorrer();
					   break;
		case MENSAJES: result = daoManager.getMensajeDAO().recorrer();
					   break;
		case DIRECTORIOS: result = daoManager.getDirectorioDAO().recorrer();
						  break;
	}

	if(result.size() > 0){
		unsigned int i = 0;
		unsigned int id;
		while(i < result.size()){

			id = result[i].getID();
			string cadena;

			switch(codigoTrie){
				case IMAGENES: cadena = daoManager.getImagenDAO().getImgById(id).getNombre();
							   this->imagenes->insertCadena(cadena,id);
							   break;
				case MENSAJES: cadena = daoManager.getMensajeDAO().getMsjById(id).getNombre();
							   this->mensajes->insertCadena(cadena,id);
							   break;
				case DIRECTORIOS: cadena = daoManager.getDirectorioDAO().getDirById(id)->getPath();
							   this->directorios->insertCadena(cadena,id);
								  break;
			}

			i++;
		}

		//ahora, en ID quedo el ultimo id leido, o sea, el ultimo id asignado
		//lo seteo en la clase correspondiente para empezar a numerar desde ahi.
		switch(codigoTrie){
			case IMAGENES: object::Imagen::setIncrementalId(id);
						   break;
			case MENSAJES: object::Mensaje::setIncrementalId(id);
						   break;
			case DIRECTORIOS: object::Directorio::setIncrementalId(id);
						   break;
		}
	}
	else{	//si el vector estaba vacio, el indice estaba vacio, no habia registros
			//por lo tanto se asigna cero al contador estatico que genera los ids
		switch(codigoTrie){
			case IMAGENES: object::Imagen::setIncrementalId(0);
						   break;
			case MENSAJES: object::Mensaje::setIncrementalId(0);
						   break;
			case DIRECTORIOS: object::Directorio::setIncrementalId(0);
						   break;
		}
	}
}

bool TrieDAO::insertCadena(int codigoTrie, string cadena, unsigned int indice){

	Trie* trie;
	switch(codigoTrie){
		case IMAGENES: trie = this->imagenes;
					break;
		case MENSAJES: trie = this->mensajes;
					break;
		case DIRECTORIOS: trie = this->directorios;
					break;
	}

	return (trie->insertCadena(cadena, indice));

}

unsigned int TrieDAO::getIndice(int codigoTrie, string cadena){

	Trie* trie;
	switch(codigoTrie){
		case IMAGENES: trie = this->imagenes;
					break;
		case MENSAJES: trie = this->mensajes;
					break;
		case DIRECTORIOS: trie = this->directorios;
					break;
	}

	return (trie->getIndice(cadena));
}

bool TrieDAO::deleteCadena(int codigoTrie, string cadena){

	Trie* trie;
	switch(codigoTrie){
		case IMAGENES: trie = this->imagenes;
					break;
		case MENSAJES: trie = this->mensajes;
					break;
		case DIRECTORIOS: trie = this->directorios;
					break;
	}

	return (trie->deleteCadena(cadena));
}

}
