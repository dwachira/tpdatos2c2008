/*
 * NodoAVL.h
 *
 *  Created on: 25/09/2008
 *      Author: andres
 */

#ifndef NODOAVL_H_
#define NODOAVL_H_


#include <iostream>

struct par_dato{
	double clave;
	unsigned long int offset;
};


class NodoAVL{

/**************/
	public:
/**************/

		NodoAVL(const par_dato dat);
   		NodoAVL(const par_dato dat, NodoAVL* pad);

  		par_dato dato;
  		int FE; 	// Factor de Equilibrio (-1, 0, 1)
   					// FE = altura subárbol derecho - altura subárbol izquierdo

   		NodoAVL* izquierdo;
   		NodoAVL* derecho;
   		NodoAVL* padre;

   		friend class AVL;

};


#endif /* NODOAVL_H_ */
