/*
 * DirectorioDAO.h
 *
 *  Created on: 14/10/2008
 *      Author: andres
 */

#ifndef DIRECTORIODAO_H_
#define DIRECTORIODAO_H_


#include "../db/file/StreamFijo.h"
#include "../db/file/StreamVariable.h"
#include "../db/file/Indice.h"

#include "../object/Directorio.h"
#include <vector>
using namespace std;


typedef struct{
	unsigned int ID;
	unsigned long int offset_path;
	unsigned int anio;
	unsigned int mes;
	unsigned int dia;
	unsigned int hora;
	unsigned int min;
}REG_DIR;


class DirectorioDAO{

	public:

		Indice*						index_Prim;
		Indice*						index_FechaModif;

		StreamFijo*					archivo;
		StreamVariable*				stream;

		/*
		 * Se almcena el path del directorio, que es la parte de longitud
		 * variable, en un archivo aparte y se recupera el offset
		 */
		unsigned long int guardarPath(string nombre);

		/*
		 * A partir del offset recuperado con la funcion anterior, y que fue
		 * almacenado en el indice junto con los demas datos de longitud fija,
		 * se puede recuperar del stream de regs de long variable, el path
		 */
		string recuperarPath(unsigned long int offset);

		/*
		 * Transformacion a struct de una instancia de clase 'Directorio'
		 */
		REG_DIR* aStruct(Directorio dir, unsigned long int offset_path);

	private:

		DirectorioDAO(string baseDir);
		~DirectorioDAO();

		/*
		 * Se inserta el path del directorio en el archivo para regs de long
		 * variable. Luego, los demas datos, que son de long fija, junto con
		 * el offset del path se almacenan en el archivo para regs de long
		 * fija y se recupera su offset. Finalmente, para cada indice a usar
		 * se genera la clave que corresponde y se almacena con el offset
		 */
		bool insert(Directorio dir);

		/*
		 * Se accede al indice primario, ordenado por ID, y se recupera el
		 * offset del dato con el id solicitado. Luego se accede al archivo para
		 * regs de long fija y se recupera la informacion almacenada en ese
		 * offset. Por ultimo, se accede al archivo para regs de long variable
		 * y se recupera el path del directorio, ultimo dato necesario.
		 */
		Directorio getDirById(unsigned int newID);

//podria hacerse un get por fecha de moficacion, pero no le veo sentido aun
};


#endif /* DIRECTORIODAO_H_ */
