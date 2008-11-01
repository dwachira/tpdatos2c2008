/*
 * ParticionDAO.h
 *
 *  Created on: 14/10/2008
 *      Author: andres
 */

#ifndef PARTICIONDAO_H_
#define PARTICIONDAO_H_

#include "../db/file/StreamFijo.h"
#include "../db/file/Indice.h"
#include "../db/file/AVL.h"
#include "../object/Particion.h"
#include "../util/string/StringUtils.h"
#include <vector>
#include <list>
#include <stdlib.h>
using namespace std;
using namespace util;
using namespace object;

namespace dao {

typedef struct{
	unsigned int ID_Img;
	unsigned int ID_Txt;
	unsigned int posicion;		//...de la particion dentro del mensaje completo
	unsigned int bit_inicio;	//...posicion de inicio de la particion en la imagen
	unsigned int longitud;		//...de la particion (espacio que ocupa en la imagen)
	bool libre;
}REG_PART;


class ParticionDAO{

	private:

		Indice*						index_Prim;		//compuesto por ID-IMG-POSICION
		Indice*						index_Img;
		Indice*						index_Txt;
		Indice*						index_Libres;	//compuesto por estado libre + longitud

		StreamFijo*					archivo;

		/*
		 * Transformacion a struct de una instancia de clase 'Particion'
		 */
		REG_PART* aStruct(Particion part);

	public:

		ParticionDAO();
		~ParticionDAO();

		/*
		 * Se insertan los datos, que son de long fija, en el archivoy se
		 * recupera su offset. Luego, para cada indice a usar
		 * se genera la clave que corresponde y se almacena con el offset
		 */
		bool insert(Particion part);

		/*
		 * Dada una relacion entre Imagen - Texto - Posicion, se elimina de
		 * los indices pertinentes y del archivo de datos.
		 * Si se posee el objeto Particion no sera necesario recuperar cierta
		 * informacion desde el archivo que posibilite el proceso de baja de
		 * alguno de los indices utilizados.
		 */
		void borrar(Particion part);
		void borrar(unsigned int img, unsigned int txt, unsigned int pos);

		/*
		 * Dada una operacion de Baja de una Particion, se marca como libre.
		 * Como requisito, el registro indicado debe estar ocupado
		 */
		bool liberar(unsigned int img, unsigned int txt, unsigned int pos);

		/*
		 * Se accede al indice primario, ordenado por IDImg + IDTxt + Pos, y se
		 * recupera el offset del dato con la clave solicitada. Luego se accede
		 * al archivo se recupera la informacion almacenada en ese offset.
		 */
		Particion getPartByImgTxtPos(unsigned int newImg, unsigned int newTxt, unsigned int newPos);

		/*
		 * Se obtienen todas las particiones asociadas al codigo de Imagen
		 * indicado por parametro y se devuelven en una lista.
		 */
		list<Particion> getPartsByImg(unsigned int newImg);

		/*
		 * Se obtienen todas las particiones asociadas al codigo de Mensaje
		 * indicado por parametro y se devuelven en una lista.
		 */
		list<Particion> getPartsByTxt(unsigned int newTxt);
};

}

#endif /* PARTICIONDAO_H_ */
