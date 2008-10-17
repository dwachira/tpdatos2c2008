/*
 * Imagen.h
 *
 *  Created on: 02/10/2008
 *      Author: andres
 */

#ifndef IMAGEN_H_
#define IMAGEN_H_


#include <string>
using namespace std;


class Imagen{

	private:

		unsigned int ID;
		unsigned int ID_Dir;
		unsigned int espacio_libre;
		unsigned long int hash_value;
		unsigned int tamanio;
		string nombre;

	public:

		Imagen();
		Imagen(const Imagen &img);
		Imagen(unsigned int pID, unsigned int pID_Dir, unsigned int pEspacio_Libre,
				unsigned long int pHash_Value, unsigned int pTamanio, string pNombre):
							ID(pID),ID_Dir(pID_Dir),espacio_libre(pEspacio_Libre),
							hash_value(pHash_Value),tamanio(pTamanio),nombre(pNombre){}
		~Imagen();



		unsigned int getID() {return ID;}
		void setID(unsigned int ID) {this->ID = ID;}

		unsigned int getID_Dir() {return ID_Dir;}
		void setID_Dir(unsigned int ID_Dir) {this->ID_Dir = ID_Dir;}

	    string getNombre() {return nombre;}
	    void setNombre(string nombre) {this->nombre = nombre;}

		unsigned int getTamanio() {return tamanio;}
		void setTamanio(unsigned int tamanio) {this->tamanio = tamanio;}

		unsigned int getEspacio_libre() {return espacio_libre;}
		void setEspacio_libre(unsigned int espacio_libre) {this->espacio_libre = espacio_libre;}

		unsigned long int getHash_value() {return hash_value;}
		void setHash_value(unsigned long int hash_value) {this->hash_value = hash_value;}

};


#endif /* IMAGEN_H_ */
