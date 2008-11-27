/*
 * Pagina.h
 *
 *  Created on: 25/09/2008
 *      Author: andres
 */

#ifndef PAGINA_H_
#define PAGINA_H_


/*
 * Esta clase representa un registro del Archivo Paginado del Indice.
 * Es decir, el archivo tiene N paginas; cada una de ellas es una
 * instancia de esta clase.
 */

#ifndef MAX_REGS_PAGINA
#define MAX_REGS_PAGINA 100		//cantidad de RegPagina por Pagina
#endif							//CREO QUE DEBE SER PAR PARA QUE NO HAYA PROBLEMAS

#include "RegPagina.h"


class Pagina{

	private:

		unsigned long int		IDPagina;
		unsigned long int		IDPagSig;
		unsigned long int		IDPagAnt;
		int			    		cantReg;


/**************/
	public:
/**************/

		RegPagina registros[MAX_REGS_PAGINA];

		Pagina();
		~Pagina();

		bool tieneSiguiente();
		bool tieneAnterior();
		bool tieneLugar();

		void reset();

		/*
		 * Inserta el registro recibido por parametro de forma ordenada
		 * dentro de la pagina; siguiendo el orden de las claves.
		 */
		void insertar(RegPagina newReg);

		/*
		 * Elimina de la pagina todos los registros que hacen referencia al
		 * dato cuyo ID es recibido por parametro.
		 */
		void eliminar(double ID);
		bool eliminar(double ID, unsigned long int offset);

		void setIDPagina(unsigned long int newIDPagina);
		void setIDPagSig(unsigned long int newIDPagSig);
		void setIDPagAnt(unsigned long int newIDPagAnt);
		void setCantReg(int newCantReg);

		unsigned long int getIDPagina();
		unsigned long int getIDPagSig();
		unsigned long int getIDPagAnt();
		int getCantReg();

};


#endif /* PAGINA_H_ */
