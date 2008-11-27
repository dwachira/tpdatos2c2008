/*
 * Indice.h
 *
 *  Created on: 26/09/2008
 *      Author: andres
 */

#ifndef INDICE_H_
#define INDICE_H_


#include "StreamFijo.h"
#include "Pagina.h"
#include "AVL.h"
#include <vector>
using namespace std;


class Indice{

	private:

		StreamFijo*				archivo;
		char*					nombre;
		bool					esIndiceSecundario;
		Pagina*					pActual;
		unsigned long int		cantPaginas;
		unsigned long int		primeraPagina;	// ID de la 1er Pagina

		/*
		 * se genera un vector con los elementos de la Pagina Actual
		 */
		vector<RegPagina> armarVector();

		/*
		 * carga la pagina correspondiente a un numero de registro válido,
		 * la deja como 'pActual' y actualiza todos los datos referentes
		 * a dicha pagina, como los ids de paginas sig y ant
		 */
		void cargarPagina( unsigned long int ID_pagina );

		/*
		 * divide la pagina actual, que debe estar llena, en dos paginas a media capacidad
		 * la operacion es similar a un Split
		 */
		void dividirPagina();


		/*
		 * Busca si existe el dato dentro de la pagina indicada como actual
		 */
		bool contieneDato(double clave);

		/*
		 * Localiza y carga la pagina candidata para insertar la clave deseada,
		 * Casos Posibles:
		 * 		Devuelve  0 si hay que insertar el dato en la que quedo como pagina Actual.
		 * 		Devuelve  1 si hay que insertar el dato en la que quedo como pagina Actual
		 * 					y se que el registro no existe.
		 * 		Devuelve  3 si hay que hacer SPLIT de la que quedo como Actual para insertar
		 * 					y se que el registro no existe.
		 * 		Devuelve  4 si hay que hacer SPLIT de la que quedo como Actual para insertar.
		 * 		Devuelve -2 si no hay paginas en el indice.
		 */
		int buscarPagina(double clave);

		/*
		 * Se encarga de buscar donde debe insertarse un par clave-offset, o donde
		 * se encontraria si ya fue insertado. En caso de encontrar una pagina
		 * candidata, carga dicha pagina como actual. Puede devolver:
		 *  	 0 -> si busco existe y si quiero insertar puedo en la pagina que quedo Actual
		 *  	 1 -> si busco NO existe y si quiero insertar puedo en la pagina que quedo Actual
		 *  	 2 -> buscar o insertar despues de la pSiguiente
		 * 		-2 -> buscar o insertar antes de la pAnterior
		 * 		 3 -> si busco NO existe y si quiero insertar hay que hacer SPLIT
		 * 			  de la que quedo como actual
		 * 		 4 -> si busco existe y si quiero insertar hay que hacer SPLIT
		 * 			  de la que quedo como actual
		 */
		int buscarTarget(double clave);

		/*
		 * se obtiene la primera pagina del indice. Usado cuando se quiere
		 * recuperar todo el indice de forma ordenada
		 */
		vector<RegPagina> getPrimeraPagina();

		/*
		 * se obtiene la pagina siguiente a la actual. Usado cuando se quiere
		 * recuperar todo el indice de forma ordenada
		 */
		vector<RegPagina> getPaginaSiguiente();

		/*
		 * Se utiliza para resolver los conlictos despues de eliminar un registro cuando
		 * queda una pagina vacia; motivo por el cual debe darse de baja.
		 */
		void acomodarPunteros();

/**************/
	public:
/**************/

		Indice(const char* nombreFisico, bool esIndiceSecundario);
		~Indice();

		/*
		 * Devuelve un vector con los registros de la pagina del archivo en la que
		 * es posible que se encuentre el dato buscado (si existe se encuentra ahi)
		 */
		vector<RegPagina> getPaginaCandidata(double clave);

		/*
		 * Elimina del indice todas las entradas referidas a la clave recibida
		 */
		void eliminar(double clave);

		/*
		 * Elimina de un indice secundario una unica ocurrencia de una clave.
		 * Se recupera el offset leyendo como si se recuperara el registro, y
		 * luego se va comparando todos los registros con la clave deseada contra
		 * el offset recuperado. Aquel que posea ese offset, que ademas sera
		 * uno solo, se da de baja.
		 */
		void eliminar(double clave, unsigned int offset);

		/*
		 * Inserta el dato en el indice manteniendo el orden
		 */
		bool insertar(double clave, unsigned long int offset);

		/*
		 * Dada una clave en un indice secundario, devuelve los registros
		 * que refieren a dicha clave. Precondicion: el Indice sea secundario
		 */
		vector<RegPagina> leerSecuencial(double clave);

		/*
		 * se obtiene la totalidad del indice...
		 * se recuperan todos los pares clave-offset
		 * en el orden establecido por el indice
		 */
		vector<RegPagina> recorrerIndice();

};


#endif /* INDICE_H_ */
