/*
 * AVL.h
 *
 *  Created on: 25/09/2008
 *      Author: andres
 */

#ifndef AVL_H_
#define AVL_H_


#include "NodoAVL.h"
#include "RegPagina.h"
#include <vector>


class AVL{

	private:

		enum 			{IZQUIERDO,DERECHO};
		NodoAVL* 		raiz;
		NodoAVL* 		actual;

		/*
		 * Se ejecuta recursivamente, y borra los
		 * subarboles (DER e IZQ) del nodo recibido
		 * como parametro, y borra dicho nodo.
		 */
		void Borrar_SubArbol(NodoAVL* &nodo);

		/*
		 * El nodo que se recibe es el padre del insertado,
		 * y se indica en que rama se realizo la insercion.
		 * Se debe recorrer desde el nuevo nodo hasta la raiz,
		 * para que el arbol quede AVL
		 */
		void Balancear(NodoAVL* nodo, int rama);

		/*
		 * Rotaciones de un subarbol, para asegurar que
		 * quede como un AVL siempre
		 */
		void SimpleIzq(NodoAVL* nodo);
		void SimpleDer(NodoAVL* nodo);
		void DobleIzq(NodoAVL* nodo);
		void DobleDer(NodoAVL* nodo);


/**************/
	public:
/**************/

		AVL();
		~AVL();

		bool EstaVacio(NodoAVL* nodo);

		/*
		 * Se intenta insertar el dato pasado como parametro.
		 * Si el dato ya estuviera, se devuelve false
		 * (no hay insercion). Si se puede insertar, se inserta,
		 * se rebalancea el arbol y se devuelve true.
		 */
		bool Insertar(const par_dato dat);

		/*
		 * True si se encuentra la clave buscada y false en caso
		 * contrario. Si se encuentra, 'actual' queda apuntando al
		 * nodo que tiene el puntero; y para obtener el dato se debe
		 * utilizar la primitiva "ValorActual"
		 */
		bool Buscar(const double bus_clave);

		/*
		 * Pasa los datos recuperados del indexado, en un vector
		 * a un arbol balanceado para realizar la busqueda
		 */
		void ArmarArbol(std::vector<RegPagina> vector);

		/*
		 * Se obtiene el dato, cuyo puntero esta almacenado
		 * en el nodo 'actual' del arbol.
		 */
		RegPagina ValorActual();

};


#endif /* AVL_H_ */
