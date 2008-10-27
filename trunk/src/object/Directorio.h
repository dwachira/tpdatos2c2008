/*
 * Directorio.h
 *
 *  Created on: 02/10/2008
 *      Author: andres
 */

#ifndef DIRECTORIO_H_
#define DIRECTORIO_H_


#include <string>
#include "../util/date/Date.h"

namespace object{

class Directorio{

	private:

		unsigned int ID;
		std::string path;
		util::Date* fechaUltimaModificacion;


	public:

		Directorio(std::string newPath);
		Directorio(std::string newPath, util::Date* ultimaModificacion): path(newPath),
			fechaUltimaModificacion(ultimaModificacion) {}
		~Directorio();

		unsigned int getID() const {return ID;}
		void setID(unsigned int newID)	{this->ID = newID;}

		std::string getPath() const {return path;}
		void setPath(std::string newPath) {this->path = newPath;}

	    util::Date getFechaUltimaModificacion() const
							{return *fechaUltimaModificacion;}
	    void setFechaUltimaModificacion(util::Date* ultimaModificacion)
							{this->fechaUltimaModificacion = ultimaModificacion;}
};

}

#endif /* DIRECTORIO_H_ */
