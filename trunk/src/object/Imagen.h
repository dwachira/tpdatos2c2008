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

namespace object{

class Imagen{

	private:

		unsigned int ID;
		unsigned int ID_Dir;
		unsigned int espacio_libre;
		unsigned int proximo_bit_libre;
		unsigned long int hash_value;
		unsigned int tamanio;
		string nombre;

		static std::string GIF;
		static std::string JPG;
		static std::string PNG;
		static std::string BMP;


	public:

		Imagen();
		Imagen(const Imagen &img);
		Imagen(unsigned int pID, unsigned int pID_Dir, unsigned int pEspacio_Libre, unsigned int pProximo_Bit_Libre,
				unsigned long int pHash_Value, unsigned int pTamanio, string pNombre):
							ID(pID),ID_Dir(pID_Dir), espacio_libre(pEspacio_Libre), proximo_bit_libre (pProximo_Bit_Libre),
							hash_value(pHash_Value),tamanio(pTamanio),nombre(pNombre){}
		~Imagen();

		unsigned int getProximo_bit_libre() const
		{
			return proximo_bit_libre;
		}

		void setProximo_bit_libre(unsigned int proximo_bit_libre)
		{
			this->proximo_bit_libre = proximo_bit_libre;
		}

		unsigned int getID() const {return ID;}
		void setID(unsigned int ID) {this->ID = ID;}

		unsigned int getID_Dir() const {return ID_Dir;}
		void setID_Dir(unsigned int ID_Dir) {this->ID_Dir = ID_Dir;}

		string getNombre() const {return nombre;}
		void setNombre(string nombre) {this->nombre = nombre;}

		unsigned int getTamanio() const {return tamanio;}
		void setTamanio(unsigned int tamanio) {this->tamanio = tamanio;}

		unsigned int getEspacio_libre() const {return espacio_libre;}
		void setEspacio_libre(unsigned int espacio_libre) {this->espacio_libre = espacio_libre;}

		unsigned long int getHash_value() const {return hash_value;}
		void setHash_value(unsigned long int hash_value) {this->hash_value = hash_value;}

};

}

#endif /* IMAGEN_H_ */
