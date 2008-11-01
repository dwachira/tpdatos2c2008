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

TrieDAO::~TrieDAO(){

}


/*******************************************************
 * METODOS PUBLICOS
 *******************************************************/

void TrieDAO::loadTrie(int codigoTrie){

	DAO dao;
	Trie* trie;
	switch(codigoTrie){
		case IMAGENES: dao = daoManager->getImagenDAO();
					   trie = & (this->imagenes);
					   break;
		case MENSAJES: dao = daoManager->getMensajeDAO();
					   trie = & (this->mensajes);
					   break;
		case DIRECTORIOS: dao = daoManager->getDirectorioDAO();
						  trie = & (this->directorios);
						  break;
	}

	dao.openStream();
	string buffer;
	unsigned long int offset = dao.leerProximo(&buffer);
	while(offset != 0){
		trie->insertCadena(buffer, offset);
		buffer.clear();
		offset = dao.leerProximo(&buffer);
	}
	dao.closeStream();
}

bool TrieDAO::insertCadena(int codigoTrie, string cadena, unsigned int indice){

	Trie* trie;
	switch(codigoTrie){
		case IMAGENES: trie = & (this->imagenes);
					break;
		case MENSAJES: trie = & (this->mensajes);
					break;
		case DIRECTORIOS: trie = & (this->directorios);
					break;
	}

	return (trie->insertCadena(cadena, indice));

}

unsigned int TrieDAO::getIndice(int codigoTrie, string cadena){

	Trie* trie;
	switch(codigoTrie){
		case IMAGENES: trie = & (this->imagenes);
					break;
		case MENSAJES: trie = & (this->mensajes);
					break;
		case DIRECTORIOS: trie = & (this->directorios);
					break;
	}

	return (trie->getIndice(cadena));
}

bool TrieDAO::deleteCadena(int codigoTrie, string cadena){

	Trie* trie;
	switch(codigoTrie){
		case IMAGENES: trie = & (this->imagenes);
					break;
		case MENSAJES: trie = & (this->mensajes);
					break;
		case DIRECTORIOS: trie = & (this->directorios);
					break;
	}

	return (trie->deleteCadena(cadena));
}

}
