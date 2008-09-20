#ifndef __NODO_H__
#define __NODO_H__

#include<iostream>
#include<set>
using namespace std;



class Nodo{
	typedef  set<Nodo *> Collection;

private:
        char            caracter;
        unsigned int    indice;
        Collection     hijos;
        static int comparePointerNodo(Nodo * nodoLeft,Nodo* nodoRight);

public:
        Nodo();
        Nodo(char carac):caracter(carac),indice(0){};
        Nodo(char carac,unsigned int index):caracter(carac),indice(index){}
        ~Nodo();

        void addHijo(Nodo* nodo){hijos.insert(nodo);}
        char getCaracter()const{return caracter;}
        unsigned int getIndice(){return indice;}

        void setCaracter(char carac){this->caracter = carac;}
        void setIndice(unsigned int index){indice = index;}

        Nodo * getNodoHijoByCaracter(char caracter);
        void deleteNodoHijo(Nodo * nodoHijo);

        unsigned int cantHijos();

        /**Ejecutar el puntero a funcion del parametro, en todos los nodos hijos*/
        void executeInHijos(void (*funcion)(Nodo *));

        /** Definir el operador menor, para que inserte ordenado*/
        bool operator<(const Nodo& rightNodo )const{
        	return (this->getCaracter() < rightNodo.getCaracter());
        }
        /** Definir el operador igual, para que inserte ordenado*/
        bool operator==(const Nodo& rightNodo )const{
            return (this->getCaracter() == rightNodo.getCaracter());
        }


};

















#endif
