/*
 * TrieDAO.h
 *
 *  Created on: 21/10/2008
 *      Author: andres
 */

#ifndef TRIEDAO_H_
#define TRIEDAO_H_

#define IMAGENES 7
#define MENSAJES 8
#define DIRECTORIOS 9

#include "../db/trie/Trie.h"
#include "./manager/ManagerDAO.h"
#include "../db/file/RegPagina.h"
#include <vector>

using namespace std;

namespace dao{


class TrieDAO{

	private:

		ManagerDAO&			daoManager;
		Trie* 				directorios;		//path de los directorios del sistema
		Trie*				imagenes;			//nombre de las imagenes utilizadas
		Trie* 				mensajes;			//nombre de los mensajes ocultos

	public:

		TrieDAO(ManagerDAO& managerDao);
		~TrieDAO();

		/*
		 * Se procede a cargar desde el archivo secuencial, de registros de longitud
		 * variable, todas las cadenas con sus correspondientes indices. Segun el
		 * codigoTrie ingresado se define el caso a trabajar.
		 * Ademas, se recupera el ultimo Id generado para el caso indicado, y se
		 * setea como inicio del contador usado para generar codigos.
		 * Si la lectura del indice resulta nula, indica que no habia registros
		 * almacenados, y por esa razon, no se carga nada al Trie y el contador
		 * se inicializa con 0.
		 */
		void loadTrie(int codigoTrie);

		/*
		 * Se inserta en el Trie especificado por codigoTrie, la cadena con su
		 * correspondiente indice o ID.
		 */
		bool insertCadena(int codigoTrie, string cadena, unsigned int indice);

		/*
		 * Se recupera, para una cadena determinada, en un trie en particular,
		 * su indice o ID.
		 */
		unsigned int getIndice(int codigoTrie, string cadena);

		/*
		 * Se da de baja una cadena del trie especificado.
		 */
		bool deleteCadena(int codigoTrie, string cadena);

};

}

#endif /* TRIEDAO_H_ */
