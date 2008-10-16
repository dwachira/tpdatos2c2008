/*
 * Directorio.h
 *
 *  Created on: 02/10/2008
 *      Author: andres
 */

#ifndef DIRECTORIO_H_
#define DIRECTORIO_H_


#include <string>
using namespace std;


class Directorio{

	private:

		unsigned int ID;
		string path;
		unsigned int dia;
		unsigned int mes;
		unsigned int anio;
		unsigned int hora;
		unsigned int min;


	public:

		Directorio();
		Directorio(unsigned int newID,string newPath,unsigned int newDia,unsigned int newMes,
				unsigned int newAnio, unsigned int newHora,unsigned int newMin):
						ID(newID), path(newPath),dia(newDia),mes(newMes),anio(newAnio),
						hora(newHora),min(newMin){}
		~Directorio();



		unsigned int getID() {return ID;}
		void setID(unsigned int newID)	{this->ID = newID;}

		string getPath() {return path;}
		void setPath(string newPath) {this->path = newPath;}

		unsigned int getDia() {return dia;}
		void setDia(unsigned int newDia)	{this->dia = newDia;}

		unsigned int getMes() {return mes;}
		void setMes(unsigned int newMes)	{this->mes = newMes;}

		unsigned int getAnio() {return anio;}
		void setAnio(unsigned int newAnio)	{this->anio = newAnio;}

		unsigned int getHora() {return hora;}
		void setHora(unsigned int newHora)	{this->hora = newHora;}

		unsigned int getMin() {return min;}
		void setMin(unsigned int newMin)	{this->min = newMin;}
};


#endif /* DIRECTORIO_H_ */
