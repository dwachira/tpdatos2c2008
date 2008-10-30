/*
 * DAO.h
 *
 *  Created on: 30/10/2008
 *      Author: andres
 */

#ifndef DAO_H_
#define DAO_H_


/* esta clase se utiliza para proveer
 * una generalizacion al manejo de los
 * daos desde la clase trieDAO.
 */


#include "../db/file/StreamVariable.h"
#include <string>
using namespace std;

namespace dao{

class DAO{

	protected:

		StreamVariable*			stream;

		/*
		 * Se almcena el nombre o path, que es la parte de longitud
		 * variable, en un archivo aparte y se recupera el offset
		 */
		unsigned long int guardarNombre(string nombre);

		/*
		 * A partir del offset recuperado con la funcion anterior, y que fue
		 * almacenado en el indice junto con los demas datos de longitud fija,
		 * se puede recuperar del stream de regs de long variable, el nombre/path
		 */
		string recuperarNombre(unsigned long int offset);


	public:

		void openStream();
		unsigned long int leerProximo(string* cadena);
		void closeStream();

};

}

#endif /* DAO_H_ */
