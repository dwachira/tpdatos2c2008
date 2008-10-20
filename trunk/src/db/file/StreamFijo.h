/*
 * StreamFijo.h
 *
 *  Created on: 10/09/2008
 *      Author: andres
 */

#ifndef STREAMFIJO_H_
#define STREAMFIJO_H_


/*
 * CABECERA DE DATOS ADMINISTRATIVOS:
 * **********************************
 * 			cantidadRegistros
 * 			cantidadRegistrosVacios
 * 			tamanioRegistro
 *
 * N° de Registro = (offset - size_header) / (size_reg+1) + 1
 * Offset = (N° de Reg - 1) * (size_reg+1) + size_header
 *
 * Posicion 0 --> Cabecera Administrativa
 * Los registros van del 1 al N; con 'N' la
 * cantidad total de registros almacenados
 */


#include <fstream>

#define READ 1
#define WRITE 2
#define DELETE 3
#define UPDATE 4


class StreamFijo{

	private:

		std::fstream				archivo;
		unsigned long int 	cant_reg;			//cant de registros almacenados
		unsigned long int 	cant_vacios;		//cant que se marcaron como vacios
		unsigned int 		size_reg;			//tamaño sin considerar el flag de estado
		unsigned int 		size_header;
		char*				nombre;
		int 				modo;	//0: cerrado
									//1: lectura
									//2: escritura (append)
									//3: borrado
									//4: actualizacion

		void grabarAdmin();		//el archivo esta cerrado, opera y lo cierra
		void leerAdmin();		//invocada unicamente cuando se abre el archivo


	public:

		StreamFijo(const char* nombreFisico, unsigned int NEWsize_reg);
		~StreamFijo();

		bool isOpen();

		/*
		 * Abre el archivo en el modo indicado (Read, Write, Update o Delete)
		 * y devuelve TRUE. Ante un error devuelve FALSE.
		 * Diseñado tal que la apertura para escritura esta considerada como
		 * append. Si se desea realizar cambios, se debe utilizar el modo Update
		 * Se recupera la cabecera administrativa del archivo.
		 */
		bool abrir(int newModo);

		/*
		 * Cierra el archivo, actualizando previamente la cabecera de informacion
		 * administrativa en el archivo. Ademas se verifica el factor de carga del
		 * archivo y si la cantidad de registros marcados como eliminados es muy
		 * alta en relacion a la cantidad total, se reorganiza el archivo.
		 * Debe estar abierto (Precondicion)
		 */
		void cerrar();

		/*
		 * Escribe el registro recibido por parametro si el modo de apertura
		 * del archivo lo permite. Siempre escribe al final del archivo y
		 * devuelve el nro de registro asignado. Si no pudo, devuelve 0
		 */
		unsigned long int escribir(void* buffer);

		/*
		 * Sobreescribe el registro almacenado en la posicion indicada con el
		 * nuevo registro. Debe abrirse como UPDATE y devuelve TRUE o FALSE
		 * segun la operacion haya sido exitosa o no.
		 * El numero de registro debe ser entre 1 y N, con N la cantidad total
		 * de registros almacenados en el archivo.
		 */
		bool actualizar(void* buffer, unsigned long int numero_reg);

		/*
		 * Lee el registro almacenado en la posicion indicada.
		 * El numero de registro debe ser entre 1 y N, con N la cantidad total
		 * de registros almacenados en el archivo.
		 * Devuelve FALSE si el archivo estaba cerrado o no permitia realizar
		 * lecturas o el registro habia sido dado de baja; y TRUE si pudo leer
		 */
		bool leer(void* buffer, unsigned long int numero_reg);

		/*
		 * Mueve el get-pointer al principio del archivo. Se debe usar antes
		 * de la primer invocacion a 'leerProximo'
		 */
		void seek_beg();

		/*
		 * Lee en forma secuencial. Es necesario ubicarse al principio del archivo
		 * con la primitiva 'seek_beg' antes de iniciar la lectura secuencial.
		 * Devuelve el numero de registro leido y 0 si la operacion fallo.
		 */
		unsigned long int leerProximo(void* buffer);

		/*
		 * Marca como borrado el registro indicado por parametro y devuelve TRUE
		 * si pudo completar la operacion y FALSE en caso contrario
		 */
		bool borrar(unsigned long int numero_reg);



		/*
		 * Funcion utilizada por el Indice. Indica en que posicion se va a
		 * almacenar el proximo registro recibido para insercion.
		 */
		unsigned long int getPosLibre();

		/*
		 * Funcion utilizada por el Indice. Indica cuantos registros se encuentran
		 * ocupados dentro del archivo; es decir, cuantas paginas tiene el Archivo
		 * Paginado que se utiliza dentro del Indice.
		 */
		unsigned long int contarRegistros();
};


#endif /* STREAMFIJO_H_ */
