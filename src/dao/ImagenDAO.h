/*
 * ImagenDAO.h
 *
 *  Created on: 03/10/2008
 *      Author: andres
 */

#ifndef IMAGENDAO_H_
#define IMAGENDAO_H_

#include "../db/file/StreamFijo.h"
#include "../db/file/StreamVariable.h"
#include "../db/file/Indice.h"
#include "../object/Imagen.h"
#include <stdlib.h>
#include <vector>
#include <list>
using namespace std;
using namespace object;

namespace dao {

typedef struct{
	unsigned int ID;
	unsigned int ID_Dir;
	unsigned long int offset_nombre;
	unsigned int prox_bit_libre;
	unsigned int tamanio;
	unsigned int espacio_libre;
	unsigned long int hash_value;
}REG_IMG;


class ImagenDAO{

	private:

		Indice*						index_Prim;
		Indice*						index_Espacio;
		Indice*						index_Directorio;

		StreamFijo*					archivo;
		StreamVariable*				stream;


		/*
		 * Se almcena el nombre de la imagen, que es la parte de longitud
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
		 * Transformacion a struct de una instancia de clase 'Imagen'
		 */
		REG_IMG* aStruct(Imagen img, unsigned long int offset_nombre);

	public:

		ImagenDAO();
		~ImagenDAO();

		/*
		 * Se inserta el nombre de la imagen en el archivo para regs de long
		 * variable. Luego, los demas datos, que son de long fija, junto con
		 * el offset del nombre se almacenan en el archivo para regs de long
		 * fija y se recupera su offset. Finalmente, para cada indice a usar
		 * se genera la clave que corresponde y se almacena con el offset
		 */
		bool insert(Imagen& img);

		/*
		 * A partir de un Codigo de Imagen, se da de baja la informacion del
		 * registro del archivo de datos, se da de baja de todos los indices,
		 * y se elimina el nombre de la misma del archivo de regs de long variable.
		 */
		void borrar(Imagen& img);
		void borrar(unsigned int id);

		/*
		 * Se pueden actualizar los campos EspacioLibre (por las modificaciones que
		 * sufre la imagen), Id de Directorio (por la posibilidad de que una imagen
		 * sea movida a otro directorio), el hashValue (que se modifica con cada
		 * cambio que sufre la imagen) y el nombre (en caso que sea renombrada)
		 */
		bool updateEspacioLibre(unsigned int ID, unsigned int newEspacioLibre);
		bool updateDirectorio(unsigned int ID, unsigned int newId_Dir);
		bool updateHashValue(unsigned int ID, unsigned long int newHashValue);
		bool updateProxBitLibre(unsigned int ID, unsigned int newProxBitLibre);
		bool updateNombre(unsigned int ID, string newNombre);

		/*
		 * Se accede al indice primario, ordenado por ID, y se recupera el
		 * offset del dato con el id solicitado. Luego se accede al archivo para
		 * regs de long fija y se recupera la informacion almacenada en ese
		 * offset. Por ultimo, se accede al archivo para regs de long variable
		 * y se recupera el nombre de la imagen, ultimo dato necesario.
		 */
		Imagen getImgById(unsigned int newID);

		/*
		 * Se obtienen todas las imagenes asociadas al directorio indicado
		 * por parametro y se devuelven en una lista.
		 */
		list<Imagen> getImgsByDirectorio(unsigned int newID_Dir);

		/*
		 * Se obtienen TODOS los registros del indice que tienen espacio libre
		 * ordenados en forma descendente. Se devuelven en una lista donde el
		 * primer elemento es el que tiene mas espacio libre.
		 */
		list<Imagen> getImgsSortedByEspacioLibre();


		/*
		 * funcinoalidad para la carga inicial del Trie correspondiente
		 */
		vector<RegPagina> recorrer();
};

}

#endif /* IMAGENDAO_H_ */
