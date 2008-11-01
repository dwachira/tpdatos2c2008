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

namespace object{

class Mensaje{

	private:

		unsigned int ID;
		string nombre;
		unsigned int tamanio;
		int cant_partes;

		static unsigned int incrementalId;

		unsigned int getNewId(){return ++incrementalId;}

	public:

		Mensaje(const Mensaje &msj);
		Mensaje(string newNombre, unsigned int newTamanio,int newCant);
		Mensaje(unsigned int newId,string newNombre,unsigned int newTamanio,int newCant);
		~Mensaje();

		static unsigned int getLastAssignedId(){return incrementalId;}

		unsigned int getID() {return ID;}
		void setID(unsigned int ID) {this->ID = ID;}

		string getNombre() {return nombre;}
		void setNombre(string nombre) {this->nombre = nombre;}

		unsigned int getTamanio() {return tamanio;}
		void setTamanio(unsigned int tamanio) {this->tamanio = tamanio;}

		int getCant_partes() {return cant_partes;}
		void setCant_partes(int cant_partes) {this->cant_partes = cant_partes;}

};

}

#endif /* MENSAJE_H_ */
