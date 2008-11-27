#include "Trie.h"

  string Trie::getSubstring(string s){
       return s.substr(1,s.length() - 1);
  }

  Trie::Trie(){

  }

  Trie::~Trie(){
  }

  /** Insertar una cadena al Trie, y retornar si tuvo exito*/
  bool Trie::insertCadena( string cadena , unsigned int indice ){
       Nodo * inicio = getInicio(cadena[0]);
       if (insertCadenaSubLevel(inicio,cadena,indice)){
          cantCadena++;
          return true;
       }else
            return false;

  }



  Nodo* Trie::getInicio(char caracter){
	  //Recorrer todas las raices, hasta encontrar la correspondiente al caracter
	  for(Collection::iterator it = raices.begin(); it != raices.end(); it++){
		  if ((*it)->getCaracter() == caracter){
			  return (*it);
		  }
	  }
	  //Si no encuentra el nodo, lo crea y lo agrega a la raiz
	  Nodo * nodoRaiz = new Nodo(caracter,0);
	  raices.insert(nodoRaiz);
	  return nodoRaiz;
  }

  /** Insertar una cadena al Trie, y retornar si tuvo exito*/
  bool Trie::insertCadenaSubLevel(Nodo* nodo, string cadena , unsigned int indice ){
       string subCadena = getSubstring(cadena);
       /* Insertar caracter en el nodo correspondiente y actualizar el parametro
        * nodo con el hijo donde se hizo la insersecion */
       bool inserto = this->insertCaracterInHijo(&nodo,subCadena[0]);

       //Si no inserto, es el final de la cadena, y el indice no es 0 entonces ya estaba el termino
       if (!inserto && subCadena.length() == 0 && nodo->getIndice() > 0)
             return false;
       else
           if (subCadena.length() > 1){
              return insertCadenaSubLevel(nodo, subCadena, indice);
           }else{
                  nodo->setIndice(indice);
                  return true;
           }
  }


  /** Insertar una cadena en el hijo correspondiente, y retornar si tuvo exito*/
  bool Trie::insertCaracterInHijo(Nodo** nodo, char carac ){
       Nodo * nodoHijo = (*nodo)->getNodoHijoByCaracter(carac);
       if (nodoHijo == NULL){
          nodoHijo = new Nodo(carac,0);
          (*nodo)->addHijo(nodoHijo);
          (*nodo) = nodoHijo;

          return true;
       }else{
          (*nodo) = nodoHijo;
          return false;
       }

  }


  /** Indicar si existe la cadena. Si existe retornar true, si no false*/
  bool Trie::existCadena( string cadena ){
	  //Obtener el indice de la cadena, si es mayor a 0 retornar true
	 return (getIndice(cadena) > 0);
  }

  /** Retornar el indice de la cadena, si no existe retornar 0*/
  unsigned int Trie::getIndice( string cadena ){
	  //Obtener el nodo raiz del primer caracter de la cadena
	  Nodo * nodo = this->getInicio(cadena[0]);
	  //Obtener el substring de la cadena
	  string subCadena = getSubstring(cadena);
	  //Recorrer toda la cadena, hasta que no haya hijos o se acabe
	  while(nodo != NULL && subCadena.length() > 0){
		  	nodo = nodo->getNodoHijoByCaracter(subCadena[0]);
		  	subCadena = getSubstring(subCadena);
	  }
	  //Si termino con un nodo, y recorrio toda la cadena retornar el indice
	  if (nodo != NULL &&  subCadena.length() == 0)
		  return nodo->getIndice();
	  return 0;
  }


  /** Eliminar la cadena del Trie si existe retornar true, sino false*/
  bool Trie::deleteCadena( string cadena ){
	  //Obtener el nodo raiz del primer caracter de la cadena
	  Nodo * nodo = this->getInicio(cadena[0]);
	  //Obtener el substring de la cadena
	  string subCadena = getSubstring(cadena);
	  return deleteSubCadena(nodo,subCadena);
  }

  /** Eliminar la subcadena del nodo, si debe eliminar retornar true, sino false*/
   bool Trie::deleteSubCadena(Nodo * nodo, string subCadena ){
	  bool eliminar = false;
	   //Obtener el nodo hijo correspondiente al primer caracter de la subcadena
 	  Nodo * nodoHijo = nodo->getNodoHijoByCaracter(subCadena[0]);
 	  //Si existe hijo y la cadena no termino, eliminar la subcadena
 	  if (nodoHijo != NULL && subCadena.length() > 0){
 		  subCadena = getSubstring(subCadena);
 		  eliminar = deleteSubCadena(nodoHijo,subCadena);
  	  }

 	  if (eliminar ){
 		 //Elimino hijo del nodo
 		  nodo->deleteNodoHijo(nodoHijo);
 		  if (nodo->cantHijos() == 0 ){
 			 return true;
 		  }
 		  return false;
 	  }
 	  //Si llego al final de la subCadena, y tiene indice mayor a 0 retornar true
 	  if (subCadena.length() == 0 && nodo->getIndice() > 0){
 		  //Si la cantidad de hijos es 0 eliminar el nodo, si no setear el indice en 0
 	  	  if (nodo->cantHijos() == 0){
 	  		  return true;
 	  	  }else{
 	  		  nodo->setIndice(0);
 	  	  }
 	  }
 	  return false;
   }


   /** Imprimir el trie por pantalla*/
   void  Trie::printTrie(){
	   //Recorrer todas las raices e imprimir cada una
	   for(Collection::iterator it = raices.begin(); it != raices.end(); it++){
		   if ((*it)->cantHijos() > 0){
			   printNodo((*it));
		   }
	   }

   }

 /** Imprimir un nodopor pantalla*/
  void Trie::printNodo(Nodo * nodo ){
	  //Si el nodo no es null, imprimir el caracter por pantalla
	  if (nodo != NULL){
		  // printf("posicion:  %i ,  caracter: %c , hijos: %i \r\n",i,nodo->getCaracter(),nodo->hijos.size());
	     // printf("%c : %d \n",nodo->getCaracter(),nodo->cantHijos());
		  printf("%c",nodo->getCaracter());
	      //Si el indice es mayor a cero, termino la palabra impimir fin e indice
	      if (nodo->getIndice() > 0)
	    	  printf(" // i: %d \n",nodo->getIndice());
	      nodo->executeInHijos(&(printNodo));
	  }


  }
