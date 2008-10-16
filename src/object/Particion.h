/*
 * Particion.h
 *
 *  Created on: 02/10/2008
 *      Author: andres
 */

#ifndef PARTICION_H_
#define PARTICION_H_


using namespace std;


class Particion{

	private:
		unsigned int ID_Img;
		unsigned int ID_Txt;
		unsigned int posicion;
		unsigned int bit_inicio;
		unsigned int longitud;
		bool libre;

	public:

		Particion();
		Particion(unsigned int newID_Img,unsigned int newID_Txt,unsigned int newPosicion,
				unsigned int newBit_inicio,unsigned int newLongitud,bool newLibre):
						ID_Img(newID_Img),ID_Txt(newID_Txt),posicion(newPosicion),
						bit_inicio(newBit_inicio),longitud(newLongitud),libre(newLibre){}
		~Particion();



		unsigned int getID_Img() {return ID_Img;}
		void setID_Img(unsigned int ID_Img) {this->ID_Img = ID_Img;}

		unsigned int getID_Txt() {return ID_Txt;}
		void setID_Txt(unsigned int ID_Txt) {this->ID_Txt = ID_Txt;}

		unsigned int getPosicion() {return posicion;}
		void setPosicion(unsigned int posicion) {this->posicion = posicion;}

		unsigned int getBit_inicio() {return bit_inicio;}
		void setBit_inicio(unsigned int bit_inicio) {this->bit_inicio = bit_inicio;}

		unsigned int getLongitud() {return longitud;}
		void setLongitud(unsigned int longitud) {this->longitud = longitud;}

		bool isLibre() {return libre;}
		void liberar(bool libre) {this->libre = false;}
		void ocupar(bool libre) {this->libre = true;}

};


#endif /* PARTICION_H_ */
