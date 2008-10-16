/*
 * Mensaje.h
 *
 *  Created on: 02/10/2008
 *      Author: andres
 */

#ifndef MENSAJE_H_
#define MENSAJE_H_


#include <string>
using namespace std;


class Mensaje{

	private:

		unsigned int ID;
		string nombre;
		unsigned int tamanio;
		int cant_partes;

	public:

		Mensaje();
		Mensaje(unsigned int newID, string newNombre, unsigned int newTamanio,
				int newCant): ID(newID),nombre(newNombre),tamanio(newTamanio),
														cant_partes(newCant){}
		~Mensaje();



		unsigned int getID() {return ID;}
		void setID(unsigned int ID) {this->ID = ID;}

		string getNombre() {return nombre;}
		void setNombre(string nombre) {this->nombre = nombre;}

		unsigned int getTamanio() {return tamanio;}
		void setTamanio(unsigned int tamanio) {this->tamanio = tamanio;}

		int getCant_partes() {return cant_partes;}
		void setCant_partes(int cant_partes) {this->cant_partes = cant_partes;}

};


#endif /* MENSAJE_H_ */
