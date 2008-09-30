/*
 * AVL.cpp
 *
 *  Created on: 25/09/2008
 *      Author: andres
 */

#include "AVL.h"

/*******************************************************
 * CONSTRUCTOR Y DESTRUCTOR
 *******************************************************/

AVL::AVL(){
	this->raiz = NULL;
	this->actual = NULL;
}

AVL::~AVL(){
	Borrar_SubArbol(this->raiz);
}


/*******************************************************
 * METODOS PUBLICOS
 *******************************************************/

bool AVL::EstaVacio(NodoAVL* N){
	return (N == NULL);
}

RegPagina AVL::ValorActual(){
	RegPagina* reg = new RegPagina(actual->dato.clave, actual->dato.offset);
	return *reg;
}

bool AVL::Insertar(const par_dato dat){

	NodoAVL* padre = NULL;
	actual = this->raiz;
	bool encontrado = false;

	// Busco el lugar donde se debe insertar el dato nuevo, y simultaneamente
	// verifico que no haya sido insertado previamente y este en el arbol.
	while (!EstaVacio(actual) && !encontrado){
		if (dat.clave == actual->dato.clave)
			encontrado = true;
		else{
			padre = actual;
			if (dat.clave < actual->dato.clave)
				actual = actual->izquierdo;
			else
				actual = actual->derecho;
		}
	}

	if (encontrado)		// las claves coincidieron --> el elemento esta en el arbol.
		return false;	// FALSE = el nuevo dato no se inserto.

	if (EstaVacio(padre))				// si el padre esta vacio, el nodo debe ser la
		this->raiz = new NodoAVL(dat);	// raiz, ya que cualquier otro nodo tiene un padre.
										// (pido asi porque no puedo verificar por el actual)
	else 		// hay que insertar el nodo nuevo.
		if (dat.clave < padre->dato.clave){				// la clave nueva es menor -->
			padre->izquierdo = new NodoAVL(dat,padre);	// el nodo se inserta a la IZQ
			Balancear(padre,IZQUIERDO);					// se rebalancea el arbol
		}
		else {											// la clave nueva es mayor -->
			padre->derecho = new NodoAVL(dat,padre);	// el nodo se inserta a la DER
			Balancear(padre,DERECHO);					// se rebalancea el arbol
		}

	return true;	// TRUE = el nuevo nodo fue insertado
}

bool AVL::Buscar(const double bus_clave){

	actual = this->raiz;
	bool encontrado = false;

	while (! EstaVacio(actual) && ! encontrado){
		if (bus_clave == actual->dato.clave)	// se encontro el dato, y 'actual'
			encontrado = true; 					// apunta al nodo con el puntero
		else
			if (bus_clave < actual->dato.clave)	// el dato no esta en el nodo, por
				actual = actual->izquierdo;		// lo tanto, sigo recorriendo el
			else								// arbol, comparando las claves
				actual = actual->derecho;		// y bifurcando a IZQ o a DER
	}

	if (encontrado)
		return true;
	else
		return false;
}

void AVL::ArmarArbol(std::vector<RegPagina> vector){

	par_dato dato;

	for(unsigned int i=0 ; i< vector.size(); i++){

		dato.clave = vector[i].getID();
		dato.offset= vector[i].getOffset();
		Insertar(dato);
	}
}


/*******************************************************
 * METODOS PRIVADOS
 *******************************************************/

void AVL::Borrar_SubArbol(NodoAVL* &N){
	if(N){
		Borrar_SubArbol(N->derecho);
		Borrar_SubArbol(N->izquierdo);
		delete N;
		N = NULL;
	}
}

void AVL::Balancear(NodoAVL* nodo, int rama){

	bool salir = false;

	// se debe recorrer el camino desde el nodo hasta la raiz, verificando los FE
	// (factor de equilibrio) de cada nodo por el que se pasa. De esta forma, en
	// aquellos que sea necesario, se aplica la rotacion correspondiente. Si se
	// llega a un nodo en el que las ramas ya estan balanceadas, no es necesario
	// seguir subiendo en el arbol ya que el FE de ese nodo ya es un valor valido.
	while(nodo && !salir){
		if(rama == IZQUIERDO)  nodo->FE--;	// la insercion fue a Izq, el FE disminuye
		else  				   nodo->FE++;	// la insercion fue a Der, el FE aumenta

		if (nodo->FE == 0)	// el nodo esta balanceado, es decir, sus dos subarboles
			salir = true;	// tienen alturas iguales o que difieren en uno como max.
		else{
			if(nodo->FE == -2){ // El subarbol Izq es mas grande -> Rotar a Derecha ...
				if(nodo->izquierdo->FE == 1) 	DobleDer(nodo);		// ... doble
				else 							SimpleDer(nodo);	// ... simple
				salir = true;
			}
			if(nodo->FE == 2){ // El subarbol Der es mas grande -> Rotar a Izquierda ...
				if(nodo->derecho->FE == -1) 	DobleIzq(nodo); 	// ... doble
				else 							SimpleIzq(nodo);	// ... simple
				salir = true;
      		}
		}
		// de no haber encontrado un nodo en el que el FE permanezca valido, se
		// debe seguir subiendo en el arbol, verificando los Factores de Eq.
		if(nodo->padre)
			if(nodo->padre->derecho == nodo) 	rama = DERECHO;
			else 								rama = IZQUIERDO;
		nodo = nodo->padre;
	}
}

void AVL::SimpleIzq(NodoAVL* nodo){
									// todos los "parentescos" son relativos al parametro
   NodoAVL* Padre = nodo->padre;
   NodoAVL* nodo_deseq = nodo;				// nodo desequilibrado, las alturas de sus subarboles
   NodoAVL* hijoDer = nodo_deseq->derecho;		// difieren en mas de un elemento. (FE = 2)
   NodoAVL* nieto = hijoDer->izquierdo;

   if(Padre)
      if(Padre->derecho == nodo_deseq) 	// si el Param es hijo Derecho del Padre, esa pos ahora
		  Padre->derecho = hijoDer;		// es el hijo derecho del Param (nieto der der de Padre)
      else 								// si el Param es el hijo Izquierdo de Padre, esa pos ahora
		  Padre->izquierdo = hijoDer;	// es el hijo derecho del Param (nieto izq der de Padre)
   else
	   raiz = hijoDer;			// si Padre == NULL, quiere decir que nodo es la raiz.
								// esa pos ahora es el hijo derecho de P.
   // se reconstruye el árbol
   nodo_deseq->derecho = nieto;				//    Padre			|   Padre
   hijoDer->izquierdo = nodo_deseq;	  // antes       >A			|    A<
   											//        >B		|	 >B
   // se reasignan los padres				//        C< 		|	 C<
   nodo_deseq->padre = hijoDer;				// 					|
   if(nieto) nieto->padre = nodo_deseq;		// *********************************
   hijoDer->padre = Padre;			// despues		 			|
   											//    Padre			|   Padre
   // se ajustan los valores de FE			//       >B			|    B<
   nodo_deseq->FE = 0;						//       A<			|   A<
   hijoDer->FE = 0;							//       >C 		|   >C
}

void AVL::SimpleDer(NodoAVL* nodo){
									// todos los "parentescos" son relativos al parametro
   NodoAVL* Padre = nodo->padre;
   NodoAVL* nodo_deseq = nodo;				// nodo desequilibrado, con FE = -2
   NodoAVL* hijoIzq = nodo_deseq->izquierdo;
   NodoAVL* nieto = hijoIzq->derecho;

   if(Padre)
       if(Padre->derecho == nodo_deseq) // si el hijo es Derecho, esa pos ahora es el
		   Padre->derecho = hijoIzq;	// nieto derecho - izquierdo
       else 							// si el hijo es Izquierdo, esa pos ahora es el
		   Padre->izquierdo = hijoIzq;	// nieto izquierdo - izquierdo
   else
	   raiz = hijoIzq;				// si Padre == NULL, quiere decir que nodo es la raiz.
									// esa pos ahora es el hijo izquierdo de P.
   // se reconstruye el árbol
   nodo_deseq->izquierdo = nieto;				//    Padre		|     Padre
   hijoIzq->derecho = nodo_deseq;		  // antes     A<		|       >A
   												//    B< 		|       B<
   // se reasignan los padres					//    >C		|       >C
   nodo_deseq->padre = hijoIzq;					//				|
   if(nieto) nieto->padre = nodo_deseq;			//*********************************
   hijoIzq->padre = Padre;			 	// despues				|
												//    Padre		|     Padre
   // se ajustan los valores de FE				//     B<		|       >B
   nodo_deseq->FE = 0;							//     >A		|        >A
   hijoIzq->FE = 0;								//     C<		|        C<
}

void AVL::DobleDer(NodoAVL* nodo){
									// todos los "parentescos" son relativos al parametro
   NodoAVL* Padre = nodo->padre;
   NodoAVL* nodo_deseq = nodo;
   NodoAVL* hijoIzq = nodo_deseq->izquierdo;
   NodoAVL* nieto = hijoIzq->derecho;
   NodoAVL* bisIzq = nieto->izquierdo;
   NodoAVL* bisDer = nieto->derecho;

   if(Padre)
     if(Padre->derecho == nodo) 			//      Padre		|      Padre
		 Padre->derecho = nieto;	  // antes         >A		|       A<
     else 									//         B<		|      B<
		 Padre->izquierdo = nieto;			//         >C		|      >C
   else 									//        D< >E		|     D< >E
	   raiz = nieto;						//					|
											//*************************************
   // se reconstruye el árbol		// despues					|
   hijoIzq->derecho = bisIzq;				//      Padre		|      Padre
   nodo_deseq->izquierdo = bisDer;			//        >C		|       C<
   nieto->izquierdo = hijoIzq;				//       B< >A		|     B< >A
   nieto->derecho = nodo_deseq;				//       >D E<		|     >D E<

   // se reasignan los padres
   nieto->padre = Padre;
   nodo_deseq->padre = hijoIzq->padre = nieto;
   if(bisIzq) bisIzq->padre = hijoIzq;
   if(bisDer) bisDer->padre = nodo_deseq;

   // se ajustan los valores de FE

   if (nieto->FE == -1){
	   hijoIzq->FE = 0;
	   nodo_deseq->FE = 1;
   }
   else if(nieto->FE == 0){
	   		hijoIzq->FE = 0;
	   		nodo_deseq->FE = 0;
   		}
		else if (nieto->FE == 1){
				hijoIzq->FE = -1;
				nodo_deseq->FE = 0;
			}
   nieto->FE = 0;
}

void AVL::DobleIzq(NodoAVL* nodo){
									// todos los "parentescos" son relativos al parametro
   NodoAVL* Padre = nodo->padre;
   NodoAVL* nodo_deseq = nodo;
   NodoAVL* hijoDer = nodo_deseq->derecho;
   NodoAVL* nieto = hijoDer->izquierdo;
   NodoAVL* bisIzq = nieto->izquierdo;
   NodoAVL* bisDer = nieto->derecho;

   if(Padre)
     if(Padre->derecho == nodo) 			//      Padre		|    Padre
		 Padre->derecho = nieto;	  // antes        >A		|      A<
     else 									//         >B		|      >B
		 Padre->izquierdo = nieto;			//         C<		|      R<
   else 									//       D< >E	 	|    D< >E
	   raiz = nieto;						//					|
											//***********************************
   // se reconstruye el árbol		// despues					|
   nodo_deseq->derecho = bisIzq;			//      Padre		|    Padre
   hijoDer->izquierdo = bisDer;				//         >C		|     C<
   nieto->izquierdo = nodo_deseq;			//        A< >B		|   A< >B
   nieto->derecho = hijoDer;				//        >D E<		|   >D E<
   											//					|
   // se reasignan los padres
   nieto->padre = Padre;
   nodo_deseq->padre = hijoDer->padre = nieto;
   if(bisIzq) bisIzq->padre = nodo_deseq;
   if(bisDer) bisDer->padre = hijoDer;

   // se ajustan los valores de FE

   if (nieto->FE == -1){
	   nodo_deseq->FE = 0;
	   hijoDer->FE = 1;
   }
   else if (nieto->FE == 0){
	   		nodo_deseq->FE = 0;
	   		hijoDer->FE = 0;
   		}
		else if (nieto->FE == 1){
				nodo_deseq->FE = -1;
				hijoDer->FE = 0;
			}
   nieto->FE = 0;
}
