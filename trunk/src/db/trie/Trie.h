#ifndef __TRIE_H__
#define __TRIE_H__

#include "Nodo.h"
#include<string>


#define RAICES 27

using namespace std;




class Trie{
private:
        Nodo raices[RAICES];
        unsigned int    cantCadena;
        
        Nodo* getInicio(char caracter);
        static string getSubstring(string s);
        bool insertCadenaSubLevel(Nodo* nodo, string cadena , unsigned int indice );
        bool insertCaracterInHijo(Nodo** nodo, char carac );
        /** Eliminar la cadena del Trie si existe retornar True, sino false*/
        bool deleteSubCadena(Nodo * nodo, string subCadena );
        static void printNodo(Nodo * nodo );

public:
        Trie();
        ~Trie();

        /** Insertar una cadena al Trie, y retornar si tuvo exito*/
        bool insertCadena( string cadena , unsigned int indice );

        /** Retornar true, si el Trie esta vacio*/
        bool empty(){return (this->cantCadena == 0);}

        /** Indicar si existe la cadena. Si existe retornar true, si no false*/
        bool existCadena( string cadena );
        
        /*- Retornar el indice de la cadena, si no existe retornar 0*/
        unsigned int getIndice( string cadena );

       
        /** Eliminar la cadena del Trie si existe retornar True, sino false*/
        bool deleteCadena( string cadena );
        
        /** Imprimir el trie por pantalla*/
        void  printTrie();


};

















#endif
