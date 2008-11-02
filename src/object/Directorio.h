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
using namespace std;

namespace object{

class Directorio{

	private:

		unsigned int ID;
		string path;
		util::Date* fechaUltimaModificacion;

		static unsigned int incrementalId;

		unsigned int getNewId(){return ++incrementalId;}


	public:

		Directorio(string newPath);
		Directorio(string newPath, util::Date* ultimaModificacion);
		Directorio(unsigned int newId, string newPath, util::Date* ultimaModificacion);
		~Directorio();

		static unsigned int getLastAssignedId(){return incrementalId;}
		static void setIncrementalId(unsigned int newId){incrementalId = newId;}

		unsigned int getID() const {return ID;}
		void setID(unsigned int newID)	{this->ID = newID;}

		string getPath() const {return path;}
		void setPath(string newPath) {this->path = newPath;}

	    util::Date getFechaUltimaModificacion() const {return *fechaUltimaModificacion;}
	    void setFechaUltimaModificacion(util::Date* ultimaModificacion)
							{this->fechaUltimaModificacion = ultimaModificacion;}
};

}

#endif /* DIRECTORIO_H_ */
