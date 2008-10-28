/*
 * MensajeDAO.h
 *
 *  Created on: 14/10/2008
 *      Author: andres
 */

#ifndef MENSAJEDAO_H_
#define MENSAJEDAO_H_

#include "../db/file/StreamFijo.h"
#include "../db/file/StreamVariable.h"
#include "../db/file/Indice.h"
#include "../db/file/AVL.h"

#include "../object/Mensaje.h"
#include <vector>
using namespace std;
using namespace object;

namespace dao {

typedef struct{
	unsigned int ID;
	unsigned long int offset_nombre;
	unsigned int tamanio;
	int cant_partes;
}REG_MSJ;


class MensajeDAO{

	private:

		Indice*						index_Prim;
		Indice*						index_Tamanio;

		StreamFijo*					archivo;
		StreamVariable*				stream;

		AVL*						arbol;
		unsigned int				minID;
		unsigned int				maxID;

		/*
		 * Se almcena el nombre del mensaje, que es la parte de longitud
		 * variable, en un archivo aparte y se recupera el offset
		 */
		unsigned long int guardarNombre(string nombre);

		/*
		 * A partir del offset recuperado con la funcion anterior, y que fue
		 * almacenado en el indice junto con los demas datos de longitud fija,
		 * se puede recuperar del stream de regs de long variable, el nombre
		 */
		string recuperarNombre(unsigned long int offset);

		/*
		 * Transformacion a struct de una instancia de clase 'Mensaje'
		 */
		REG_MSJ* aStruct(Mensaje msj, unsigned long int offset_nombre);

	public:

		MensajeDAO();
		~MensajeDAO();

		/*
		 * Se inserta el nombre del mensaje en el archivo para regs de long
		 * variable. Luego, los demas datos, que son de long fija, junto con
		 * el offset del nombre se almacenan en el archivo para regs de long
		 * fija y se recupera su offset. Finalmente, para cada indice a usar
		 * se genera la clave que corresponde y se almacena con el offset
		 */
		bool insert(Mensaje& msj);

		/*
		 * A partir de un Codigo de Mensaje, se da de baja la informacion del
		 * registro del archivo de datos, se da de baja de todos los indices,
		 * y se elimina el nombre del mismo del archivo de regs de long variable.
		 */
		void borrar(Mensaje& msj);
		void borrar(unsigned int id);

		/*
		 * Se accede al indice primario, ordenado por ID, y se recupera el
		 * offset del dato con el id solicitado. Luego se accede al archivo para
		 * regs de long fija y se recupera la informacion almacenada en ese
		 * offset. Por ultimo, se accede al archivo para regs de long variable
		 * y se recupera el nombre del mensaje, ultimo dato necesario.
		 */
		Mensaje getMsjById(unsigned int newID);

//podria usarse un indice por tamanio de mensaje. aun no le encuentro utilidad
};

}

#endif /* MENSAJEDAO_H_ */
