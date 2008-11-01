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
#include "../util/string/StringUtils.h"
#include <vector>
#include <list>
using namespace std;
using namespace object;

namespace dao {

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

	private:

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
		REG_DIR* aStruct(const Directorio& dir, unsigned long int offset_path);

	public:

		DirectorioDAO();
		~DirectorioDAO();

		/*
		 * Se inserta el path del directorio en el archivo para regs de long
		 * variable. Luego, los demas datos, que son de long fija, junto con
		 * el offset del path se almacenan en el archivo para regs de long
		 * fija y se recupera su offset. Finalmente, para cada indice a usar
		 * se genera la clave que corresponde y se almacena con el offset
		 */
		bool insert(Directorio& dir);

		/*
		 * A partir de un Codigo de Directorio, se da de baja la informacion del
		 * registro del archivo de datos, se da de baja de todos los indices,
		 * y se elimina el nombre del mismo del archivo de regs de long variable.
		 */
		void borrar(Directorio& dir);
		void borrar(unsigned int id);

		/*
		 * Se utiliza para actualizar la ultima fecha de modificacion de un
		 * directorio. Los demas campos permanecen inalterables.
		 */
		bool update(unsigned int ID, util::Date newFecha);
		bool update(unsigned int ID, unsigned int anio, unsigned int mes,
						unsigned int dia, unsigned int hora, unsigned int min);

		/*
		 * Se accede al indice primario, ordenado por ID, y se recupera el
		 * offset del dato con el id solicitado. Luego se accede al archivo para
		 * regs de long fija y se recupera la informacion almacenada en ese
		 * offset. Por ultimo, se accede al archivo para regs de long variable
		 * y se recupera el path del directorio, ultimo dato necesario.
		 */
		Directorio* getDirById(unsigned int newID);

		/*
		 * Se devuelve una lista con todos los directorios cargados en el sistema
		 * Se devuelven ordenados por Fecha de Ultima Modificacion, por si tiene
		 * alguna utilidad (de menor a mayor, o sea, los ultimos modificados al final)
		 */
		list<Directorio> getDirsSortedByFechaModif();

//podria hacerse un get por fecha de moficacion, pero no le veo sentido aun


		/*
		 * funcinoalidad para la carga inicial del Trie correspondiente
		 */
		vector<RegPagina> recorrer();
};

}

#endif /* DIRECTORIODAO_H_ */
