#include "Nodo.h"


 Nodo::Nodo(){

 }
 Nodo::~Nodo(){
	 for(Collection::iterator it = hijos.begin(); it != hijos.end(); it++){
	 		hijos.erase(it);
	 		delete (*it);

	 }
 }

 Nodo * Nodo::getNodoHijoByCaracter(char caracter){
	 for(Collection::iterator it = hijos.begin(); it != hijos.end(); it++){
			if ((*it)->getCaracter() == caracter){
				return (*it);
			}
	 }
	 return NULL;
 }



 void Nodo::deleteNodoHijo(Nodo * nodoHijo){
	 for(Collection::iterator it = hijos.begin(); it != hijos.end(); it++){
		 if ((*it)->getCaracter() == nodoHijo->getCaracter()){
					hijos.erase(it);
					delete (*it);
		 }
	 }
 }

 void Nodo::executeInHijos(void (*funcion)(Nodo *)){
	 for(Collection::iterator it = hijos.begin(); it != hijos.end(); it++)
		 funcion((*it));
 }


unsigned int Nodo::cantHijos(){
      return hijos.size();
}

int Nodo::comparePointerNodo(Nodo * nodoLeft,Nodo* nodoRight){
	if ((*nodoLeft) == (*nodoRight))
		return 0;
	if ((*nodoLeft) < (*nodoRight))
		return -1;
	return 1;
}




