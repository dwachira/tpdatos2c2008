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
#include <vector>
#include <list>
using namespace std;

namespace dao {

typedef struct{
	unsigned int ID_Img;
	unsigned int ID_Txt;
	unsigned int posicion;
	unsigned int bit_inicio;
	unsigned int longitud;
	bool libre;
}REG_PART;


class ParticionDAO{

	private:

		Indice*						index_Prim;		//compuesto por ID-IMG-POSICION
		Indice*						index_Img;
		Indice*						index_Txt;
		Indice*						index_Libres;	//compuesto por estado libre + longitud

		StreamFijo*					archivo;

		AVL*						arbol;
		unsigned int				minID;
		unsigned int				maxID;

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
