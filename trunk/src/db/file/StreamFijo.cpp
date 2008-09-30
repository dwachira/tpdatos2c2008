/*
 * StreamFijo.cpp
 *
 *  Created on: 10/09/2008
 *      Author: andres
 */

#include "StreamFijo.h"

/*******************************************************
 * CONSTRUCTOR Y DESTRUCTOR
 *******************************************************/

StreamFijo::StreamFijo(const char* nombreFisico, unsigned int NEWsize_reg){

	this->cant_reg = 0;
	this->cant_vacios = 0;
	this->size_reg = NEWsize_reg;
	this->size_header = sizeof(cant_reg) + sizeof(cant_vacios) + sizeof(size_reg);
	this->modo = 0;

	this->nombre = (char*) malloc(strlen(nombreFisico) +1);		//el +1 va porque sino el
	strcpy(this->nombre, nombreFisico);							//nombre del archivo queda
																//re cualquiera...
												//no se porqué en el otro archivo no pasa

	//Lo abro primero como lectura para verificar si existe
	this->archivo.open(this->nombre, fstream::binary | fstream::in);
	if (! this->archivo){	//si el archivo no existe
		this->archivo.open(this->nombre, fstream::binary | fstream::out);
		this->archivo.write((char*) &this->cant_reg, sizeof(this->cant_reg));
		this->archivo.write((char*) &this->cant_vacios, sizeof(this->cant_vacios));
		this->archivo.write((char*) &this->size_reg, sizeof(this->size_reg));
		this->archivo.close();
	}
	else{
		this->archivo.seekg(0, ios_base::beg);
		this->archivo.read((char*) &this->cant_reg, sizeof(this->cant_reg));
		this->archivo.read((char*) &this->cant_vacios, sizeof(this->cant_vacios));
		this->archivo.read((char*) &this->size_reg, sizeof(this->size_reg));
		this->archivo.close();
	}
}

StreamFijo::~StreamFijo(){
	free(nombre);
	if (this->archivo.is_open())
		cerrar();
	free(this->archivo);
}


/*******************************************************
 * METODOS PUBLICOS
 *******************************************************/

bool StreamFijo::isOpen(){
	return (archivo.is_open());
}

bool StreamFijo::abrir(int newModo){

	if(this->modo != 0)		// si el archivo ya esta abierto, error
		return false;

	//recupero la cabecera aca, porque si abro para escritura no voy a poder
	//entonces abro el archivo para lectura y despues veo si lo cierro y abro
	//de nuevo o si puedo seguir en el mismo modo
	this->archivo.open(this->nombre, fstream::binary | fstream::in);
	if(! this->archivo.is_open())
		return false;
	leerAdmin();

	if(newModo == WRITE){					//si queria escritura, lo cierro y abro,
		this->archivo.close();				//y ya recupere la cabecera
		this->archivo.open(this->nombre, fstream::binary | ios::out | ios::app);
		if(! this->archivo.is_open())
			return false;
	}
		//si queria update, tambien se cerraba y se abria para lectoescritura
		//ahora no es asi para el update... porque primero tengo que leer el flag
		//del registro y verificar que no haya sido borrado, en ese caso puedo
		//actualizar. si estaba marcado como borrado no puedo hacer nada
		//por lo tanto la operacion arranca con el archivo en modo de lectura

	if(newModo == DELETE){				//si queria delete, tambien se cierra y se abre
		this->archivo.close();			//para el nuevo modo, que permite lectoescritura
		this->archivo.open(this->nombre, fstream::binary | fstream::out | fstream::in);
		if(! this->archivo.is_open())
			return false;
	}

	this->modo = newModo;
	return true;
}

void StreamFijo::cerrar(){

	this->modo = 0;							//si la cant de reg es >100 y el factor
	this->archivo.close();					//de carga es mayor al 50% (mas de la mitad
	grabarAdmin();							//de los registros estan vacios), reorganizo
	if(this->cant_reg > 100 && this->cant_vacios > 49){
		float factor_de_carga = this->cant_vacios / ((float) this->cant_reg);
		if(factor_de_carga > 0.5){		//se debe reorganizar el archivo
			char* nombreExtra = (char*) malloc (strlen(this->nombre)+5);
			strcpy(nombreExtra,this->nombre);
			strcat(nombreExtra,".temp");
											//abro el archivo para lectura y
			//this->archivo.open(this->nombre, fstream::binary | fstream::in);
			abrir(READ);
			fstream archivoTemp;			//uso un stream temporal para escritura
			archivoTemp.open(nombreExtra, fstream::binary | fstream::out);
			void* buffer = (void*) malloc(this->size_reg);

			unsigned long int NEWcant_reg = this->cant_reg - this->cant_vacios;
			unsigned long int NEWcant_vacios = 0;
			archivoTemp.write((char*) &NEWcant_reg, sizeof(NEWcant_reg));
			archivoTemp.write((char*) &NEWcant_vacios, sizeof(NEWcant_vacios));
			archivoTemp.write((char*) &this->size_reg, sizeof(size_reg));
			seek_beg();
			unsigned long int res = leerProximo(buffer);	//voy leyendo del archivo los regs
			while(res != 0){								//y los paso al stream temporal
				archivoTemp.write("1",1);			//almacenando tambien los flags
				archivoTemp.write((char*)buffer, this->size_reg);
				res = leerProximo(buffer);
			}

			free(buffer);
			this->modo = 0;
			this->archivo.close();				//hago el cambio... borro el archivo
			archivoTemp.close();				//viejo y lo reemplazo con el generado
			remove(this->nombre);				//en el que no hay espacios vacios
			rename(nombreExtra,this->nombre);
		}
	}
}

unsigned long int StreamFijo::escribir(void* buffer){

	if(this->modo != WRITE)	// si no se abrio para escritura, error
		return 0;

	this->archivo.seekp(0, ios_base::end);
	unsigned long int offset = this->archivo.tellp();	// recupero la posicion actual de escritura

	this->archivo.write("1",1);		//escritura del flag que indica registro ocupado
	this->archivo.write((char*)buffer, this->size_reg);		//escritura del registro

	this->cant_reg++;
	unsigned long int nro_reg = ((offset - this->size_header) / (this->size_reg+1)) + 1;
	return nro_reg;					//+1 para empezar desde uno y dejar el 0 para error
}									//this->size_reg+1 porque es el registro y el flag

bool StreamFijo::leer(void* buffer, unsigned long int nro_reg){

	if(this->modo != READ)	// si no se abrio para lectura, error
		return false;

	if(nro_reg < 1 || nro_reg > this->cant_reg)		//los registros van del 1 al N
		return false;								//con N la cantidad total

	unsigned long int offset = (nro_reg - 1) * (this->size_reg+1) + this->size_header;

	char aux;
	this->archivo.seekg(offset, ios_base::beg);
	this->archivo.read(&aux,1);						//leo el flag y si es '0', indica
	if(aux == '0')									//que el registro fue marcado como
		return false;								//vacio, entonces devuelvo error

	this->archivo.read((char*) buffer, this->size_reg);		//sino leo el registro

	return true;
}

bool StreamFijo::actualizar(void* buffer, unsigned long int nro_reg){

	if(this->modo != UPDATE)
		return false;

	if(nro_reg < 1 || nro_reg > this->cant_reg)
		return false;

	unsigned long int offset = (nro_reg - 1) * (this->size_reg+1) + this->size_header;

	char aux;
	this->archivo.seekg(offset, ios_base::beg);
	this->archivo.read(&aux,1);
	if(aux == '0')					//si el registro estaba marcado como vacio, no lo
		return false;				//puedo modificar
									//sino, cierro y abro para read-write asi puedo
	this->archivo.close();			//sobreescribir la informacion
	this->archivo.open(this->nombre, fstream::binary | fstream::out | fstream::in);
	this->archivo.seekp(offset, ios_base::beg);
	this->archivo.write("1",1);								//flag de ocupado
	this->archivo.write((char*)buffer, this->size_reg);		//registro

	this->archivo.close();
	this->archivo.open(this->nombre, fstream::binary | fstream::in);

	return true;
}

void StreamFijo::seek_beg(){
	this->archivo.seekg(this->size_header, ios_base::beg);
}

unsigned long int StreamFijo::leerProximo(void* buffer){

	if(this->modo != READ)
		return 0;

	unsigned long int offsetLeido = this->archivo.tellg();
	unsigned long int size = this->size_header + (this->size_reg+1) * this->cant_reg;
	if(offsetLeido >= size)
		return 0;			//estoy parado al final del archivo

	char aux;
	this->archivo.read(&aux,1);
	while(aux != '1'){									//si no es flag 'ocupado', muevo
		this->archivo.seekg(offsetLeido + this->size_reg + 1);
														//para adelante el tamaño de un
		offsetLeido = this->archivo.tellg();			//registro mas su flag de estado
		if(offsetLeido >= size)						//si me pase del limite indico
			return 0;									//que hubo error... sino
		this->archivo.read(&aux,1);						//vuelvo a leer y repito el ciclo
	}

	this->archivo.read((char*) buffer, this->size_reg);
	unsigned long int regLeido = ((offsetLeido - this->size_header) / (this->size_reg+1)) + 1;
	return regLeido;				//+1 para empezar desde uno y dejar el 0 para error
}									//this->size_reg+1 porque es el registro y el flag

bool StreamFijo::borrar(unsigned long int nro_reg){

	if(this->modo != DELETE)
		return false;

	if(nro_reg < 1 || nro_reg > this->cant_reg)
		return false;

	unsigned long int offset = (nro_reg - 1) * (this->size_reg+1) + this->size_header;
	this->archivo.seekp(offset, ios_base::beg);
	this->archivo.write("0",1);

	this->cant_vacios ++;
	return true;
}




unsigned long int StreamFijo::getPosLibre(){
	this->abrir(WRITE);
	this->archivo.seekp(0, ios_base::end);
	unsigned long int offset = this->archivo.tellp();
	this->cerrar();
	unsigned long int nro_reg = ((offset - this->size_header) / (this->size_reg+1)) + 1;
	return nro_reg;
}

unsigned long int StreamFijo::contarRegistros(){
	return (this->cant_reg - this->cant_vacios);
}


/*******************************************************
 * METODOS PRIVADOS
 *******************************************************/

void StreamFijo::grabarAdmin(){

	//debe abrirlo en READ_WRITE si o si para tener permiso de sobreescritura
	this->archivo.open(this->nombre, fstream::binary | fstream::in | fstream::out);

	this->archivo.seekg(0, ios_base::beg);
	//muevo al principio del archivo y actualizo la cabecera
	this->archivo.write((char*) &this->cant_reg, sizeof(this->cant_reg));
	this->archivo.write((char*) &this->cant_vacios, sizeof(this->cant_vacios));
	this->archivo.write((char*) &this->size_reg, sizeof(this->size_reg));

	this->archivo.close();
}

void StreamFijo::leerAdmin(){

	this->archivo.seekg(0, ios_base::beg);

	this->archivo.read((char*) &this->cant_reg, sizeof(this->cant_reg));
	this->archivo.read((char*) &this->cant_vacios, sizeof(this->cant_vacios));
	this->archivo.read((char*) &this->size_reg, sizeof(this->size_reg));

}
