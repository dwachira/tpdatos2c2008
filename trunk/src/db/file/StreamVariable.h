/*
 * StreamVariable.h
 *
 *  Created on: 10/09/2008
 *      Author: andres
 */

#ifndef STREAMVARIABLE_H_
#define STREAMVARIABLE_H_


#include <fstream>
#include "StreamFijo.h"
#include <list>
using namespace std;

#define READ 1
#define WRITE 2
#define DELETE 3


typedef struct{
	unsigned long int offset;
	unsigned int size;
}Reg_Vacio;


class StreamVariable {

	private:

		fstream				archivo;
		StreamFijo*			archivoVacios;
		list<Reg_Vacio>		listaVacios;
		bool				listaLoaded;
		bool				actualizarVacios;
		char*				nombre;
		unsigned long int	lastPos;		//1ra pos libre (final del archivo)
		int 				modo;	//0: cerrado
									//1: lectura
									//2: escritura (append)
									//3: borrado

	public:

		StreamVariable(const char* nombreFisico);
		~StreamVariable();

		/*
		 * Indica si el archivo esta abierto ;)
		 * Precondicion: el archivo debe estar abierto
		 */
		bool isOpen();

		/*
		 * Abre el archivo en el modo indicado, Read, Write o Delete.
		 * Devuelve 0 si pudo y devuelve -1 en caso de un error.
		 */
		bool abrir(int modo);

		/*
		 * Cierra el archivo y baja a disco la lista de espacios libres.
		 */
		void cerrar();

		/*
		 * Primeramente verifica la lista de espacios vacios. Si hubiera alguno
		 * que sirva, almacena el registro ahi. De lo contrario, escribe el
		 * registro recibido por parametro al final del archivo y escribe a
		 * continuacion el caracter de corte. Devuelve el offset en
		 * que fue insertado el registro (+1) si la operacion tuvo exito
		 * o 0 si el archivo no permitia escribir o estaba cerrado.
		 * El +1 se utiliza para poder devolver 0 en caso de error, ya
		 * que la primitiva devuelve un valor sin signo.
		 */
		unsigned long int escribir(string registro);

		/*
		 * Lee el registro almacenado en la posicion indicada (si el modo de
		 * apertura lo permite) procesando el caracter de corte que indica el
		 * final del mismo. Devuelve "" si el archivo estaba cerrado o no
		 * permitia realizar lecturas; y la cadena leida si fue exitosa.
		 * Permite leer desde el offset 1.
		 */
		string leer(unsigned long int offset);

		/*
		 * Mueve el get-pointer a la posicion inicial del archivo
		 * para poder realizar lecturas secuenciales.
		 */
		void seek_beg();

		/*
		 * Lee en forma secuencial. Recibe como parametro un puntero a un string
		 * y devuelve en el mismo el registro leido. Como resultado indica el
		 * offset del registro que se leyo o 0 si hubo algun error.
		 * Previamente debe invocarse 'seek_beg' para posicionar el puntero.
		 */
		unsigned long int leerProximo(string* registro);

		/*
		 * Agrega el registro cuyo offset es recibido como parametro a la lista
		 * de Registros Vacios.
		 */
		bool borrar(unsigned long int offset);

};


#endif /* STREAMVARIABLE_H_ */
