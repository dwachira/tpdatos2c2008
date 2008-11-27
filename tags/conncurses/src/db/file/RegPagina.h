/*
 * RegPagina.h
 *
 *  Created on: 25/09/2008
 *      Author: andres
 */

#ifndef REGPAGINA_H_
#define REGPAGINA_H_

/*
 * Esta clase representa un registro interno a una Pagina
 * del Archivo Paginado del Indice.
 * Es decir, el archivo tiene N paginas; cada una con M
 * registros, instancias de esta clase.
 */

class RegPagina{

	private:

		double ID;
		unsigned long int offset;

	public:

		RegPagina();
		RegPagina(double newID, unsigned long int newOffset);
		~RegPagina();

		void setID(double newID);
		void setOffset(unsigned long int newOffset);
		double getID() const;
		unsigned long int getOffset() const;
};


#endif /* REGPAGINA_H_ */
