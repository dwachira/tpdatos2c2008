/*
 * StreamVariable.cpp
 *
 *  Created on: 10/09/2008
 *      Author: andres
 */

#include "StreamVariable.h"
#include <string.h>
#include <stdlib.h>

/*******************************************************
 * CONSTRUCTOR Y DESTRUCTOR
 *******************************************************/

StreamVariable::StreamVariable(const char* nombreFisico){
	this->modo = 0;
	this->lastPos = 0;
	this->listaLoaded = false;
	this->nombre = (char*) malloc(strlen(nombreFisico));
	strcpy(this->nombre, nombreFisico);

	char* nombreExtra = (char*) malloc (strlen(nombreFisico)+4);
	strcpy(nombreExtra,this->nombre);
	strcat(nombreExtra,".dat");
	this->archivoVacios = new StreamFijo(nombreExtra,sizeof(Reg_Vacio));
}

StreamVariable::~StreamVariable(){
	free(this->nombre);
//	delete(this->listaVacios);
	delete(this->archivoVacios);
	if(this->archivo.is_open())
		cerrar();
}


/*******************************************************
 * METODOS PUBLICOS
 *******************************************************/

bool StreamVariable::isOpen(){
	return(archivo.is_open());
}

bool StreamVariable::abrir(int newModo){

	if(this->archivo.is_open())
		return false;		//el archivo ya estaba abierto

	switch(newModo){
		case READ:
		case DELETE:	this->archivo.open(this->nombre, fstream::binary | fstream::in);
					break;
		case WRITE: 	this->archivo.open(this->nombre, fstream::binary | fstream::out | fstream::app);
					break;
		default:	return false;
	}

	if(! this->archivo.is_open())
		return false;				//el archivo no se pudo abrir por algun motivo

	this->modo = newModo;
	this->archivo.seekp(0, ios_base::end);
	this->lastPos = this->archivo.tellp();

	//si abro para escritura o borrado, cargo a memoria el archivo de registros vacios
	if((this->modo == WRITE || this->modo == DELETE) && !listaLoaded){
		this->listaVacios.clear();					//reseteo la lista
		this->archivoVacios->abrir(READ);				//abro el archivo y muevo
		this->archivoVacios->seek_beg();				//al principio del mismo
		Reg_Vacio* buffer= new Reg_Vacio();
		unsigned long int leo = archivoVacios->leerProximo(buffer);
		while(leo != 0){								//leo secuencialmente el
			this->listaVacios.push_back(*buffer);		//archivo hasta el final
			leo = archivoVacios->leerProximo(buffer);	//almacenando en la lista
		}												//los registros leidos
		this->archivoVacios->cerrar();
		this->actualizarVacios = false;
		this->listaLoaded = true;
	}
	return true;
}

void StreamVariable::cerrar(){

	this->modo = 0;
	this->archivo.close();
	if(this->actualizarVacios){
//		delete(this->archivoVacios);
		char* nombreExtra = (char*) malloc (strlen(this->nombre)+4);
		strcpy(nombreExtra,this->nombre);
		strcat(nombreExtra,".dat");
		remove(nombreExtra);
		this->archivoVacios = new StreamFijo(nombreExtra,sizeof(Reg_Vacio));
		this->archivoVacios->abrir(WRITE);
		list<Reg_Vacio>::iterator it;
		for(it = this->listaVacios.begin(); it != this->listaVacios.end(); it++)
			this->archivoVacios->escribir(&*it);
		this->archivoVacios->cerrar();
		this->actualizarVacios = false;
		this->listaLoaded = false;
	}
}

unsigned long int StreamVariable::escribir(string registro){

	if(this->modo != WRITE)		// si no se abrio para escritura, error
		return 0;

	char reg[registro.size()];
	strcpy(reg,registro.c_str());

	unsigned long int posicion;
	bool encontrado = false;
	if(this->listaVacios.size() > 0){
		list<Reg_Vacio>::iterator it = this->listaVacios.begin();
		while(!encontrado && it != this->listaVacios.end()){
			if(it->size == (strlen(reg) + 4)){	//si el espacio alcanza para el registro
				encontrado = true;				//mas los datos administrativos, voy a
				posicion = it->offset;			//almacenar el nuevo registro ahi
				this->listaVacios.erase(it);
				this->actualizarVacios = true;
			}
			else if(it->size > (strlen(reg) + 4 + 5)){		//el +5 es para que el
				encontrado = true;							//espacio que queda libre
				posicion = it->offset;						//cuando almaceno el nuevo
				unsigned int newSize = it->size - (strlen(reg) + 4);
				unsigned int newOffset = it->offset + (strlen(reg) + 4);
				it->size = newSize;							//alcanze para un registro
				it->offset = newOffset;						//y no quede espacio libre
				this->actualizarVacios = true;				//en el que no entra ni la
			}												//parte administrativa de
			else											//un nuevo registro..
				it++;				//sino sigo recorriendo la lista
		}
	}

	if(encontrado){
		this->archivo.close();	//si encontre una posicion, cierro y abro en read-write
		this->archivo.open(this->nombre, fstream::binary | fstream::in | fstream::out);
		this->archivo.seekp(posicion, ios_base::beg);	// y muevo a la posicion
	}											//recuperada de la lista de libres
	else{
		this->archivo.seekp(0, ios_base::end);			//o al final del archivo
		this->lastPos = this->lastPos + strlen(reg) + 2;
	}

	unsigned long int pos = this->archivo.tellp();

	this->archivo.write("@",1);					//escritura del 'escape'
	this->archivo.write("1",1);					//escritura del flag de OCUPADO
	this->archivo.write(reg, strlen(reg));		//escritura del registro
	this->archivo.write("@",1);					//escritura del 'escape'
	this->archivo.write("|",1);					//escritura del caracter de corte

	if(encontrado){					//si habia cerrado y abierto para sobreescribir,
		this->archivo.close();		//vuelvo a cerrar y abrir para dejar como estaba
		this->archivo.open(this->nombre, fstream::binary | fstream::out | fstream::app);
	}

	return pos+1;			//devuelvo la posicion en que se almaceno el registro
}							//el +1 permite devolver 0 como codigo de error

string StreamVariable::leer(unsigned long int offset){

	offset = offset - 1;	//resto el 1 que se habia agregado para poder devolver error

	if(this->modo != READ)			// si no se abrio para lectura, error
		return "";

	if(offset >= this->lastPos-1)	//lastPos-1 es el ultimo caracter escrito
		return "";					//que es de corte. si se busca esa posicion
									//no sirve, y si busca posterior, no existe
	this->archivo.seekg(offset, ios_base::beg);
	string buffer = "";
	char aux;
	char aux2;

	this->archivo.read(&aux, 1);
	this->archivo.read(&aux2,1);
	if(aux!='@' || (aux=='@' && aux2!='1'))		//si no habia caracter de escape, o
		return "";				//habia pero el flag indicaba registro borrado, error


	//si llego aca es porque estaba el caracter de escape indicando que seguia el flag
	//y el mismo indicaba que el registro era valido, asi que tengo que recuperar el
	//registro indicado... (aux = @ y aux2 = 1)
	bool fin = false;
	this->archivo.read(&aux, 1);			//leo el primer caracter en 'aux'
	while(! fin){
		while(aux != '@'){					//mientras el caracter leido no sea el de
			buffer = buffer + aux;			//escape sigo leyendo el archivo y pasando
			this->archivo.read(&aux, 1);	//al registro que voy a devolver
		}
		this->archivo.read(&aux2,1);
		if(aux2 == '|')				//si el caracter que sigue al de escape es el de
			fin = true;				//corte, marco el final y salgo sin guardarlos
		else{						//sino, tengo que guardar los dos y seguir leyendo
			buffer = buffer + aux;		//paso el primer caracter (el '@')
			buffer = buffer + aux2;		//paso el segundo, que NO era un '|'
			this->archivo.read(&aux,1);		//leo un nuevo caracter y repito el ciclo
		}
	}

	return buffer;
}

void StreamVariable::seek_beg(){
	this->archivo.seekg(0, ios_base::beg);
}

unsigned long int StreamVariable::leerProximo(string* registro){

	*registro = "";

	if(this->modo != READ)			// si no se abrio para lectura, error
		return 0;

	unsigned long int offsetLeido = this->archivo.tellg();
	if(offsetLeido >= this->lastPos-1)
		return 0;

	char aux;
	char aux2;

	this->archivo.read(&aux, 1);
	this->archivo.read(&aux2, 1);
	while((aux!='@' || (aux=='@' && aux2!='1')) && (this->archivo.tellg() < this->lastPos-1)){
		if(aux2 == '@'){					//si el segundo caracter es un '@', podria
			aux = aux2;						//ser un caracter de escape, entonces lo
			this->archivo.read(&aux2, 1);	//paso a 'aux' y leo el caracter siguiente
		}
		else{								//sino quiere decir que ninguno de los dos
			this->archivo.read(&aux, 1);	//caracteres leidos es info administrativa
			this->archivo.read(&aux2, 1);	//y vuelvo a leer dos caracteres
		}
	}

	offsetLeido = this->archivo.tellg();
	if(offsetLeido >= this->lastPos-1)	//si salio del ciclo porque quedo
		return 0;									//el puntero fuera de rango, error

	//llegado este punto, aux es un caracter de escape y aux2 un flag 'reg_ocupado' y
	//el puntero quedo indicando el inicio del registro en una posicion valida
	bool fin = false;
	this->archivo.read(&aux, 1);			//leo el primer caracter en 'aux'
	while(! fin){
		while(aux != '@'){					//mientras el caracter leido no sea el de
			*registro = *registro + aux;	//escape sigo leyendo el archivo y pasando
			this->archivo.read(&aux, 1);	//al registro que voy a devolver
		}
		this->archivo.read(&aux2,1);
		if(aux2 == '|')				//si el caracter que sigue al de escape es el de
			fin = true;				//corte, marco el final y salgo sin guardarlos
		else{						//sino, tengo que guardar los dos y seguir leyendo
			*registro = *registro + aux;	//paso el primer caracter (el '@')
			*registro = *registro + aux2;	//paso el segundo, que NO era un '|'
			this->archivo.read(&aux,1);		//leo un nuevo caracter y repito el ciclo
		}
	}

	return offsetLeido + 1;		//devuelvo la posicion en que se leyo el registro
}								//el +1 permite devolver 0 como codigo de error

bool StreamVariable::borrar(unsigned long int offset){

	offset = offset - 1;	//resto el 1 que se habia agregado para poder devolver error

	if(this->modo != DELETE)	// si no se abrio para borrado, error
		return false;

	if(offset >= this->lastPos-1)	//lastPos-1 es el ultimo caracter escrito
		return false;				//que es de corte. si se busca esa posicion
									//no sirve, y si busca posterior, no existe

	this->archivo.seekg(offset, ios_base::beg);
	char aux;
	char aux2;

	this->archivo.read(&aux,1);
	this->archivo.read(&aux2,1);
	if(aux!='@' || (aux=='@' && aux2!='1'))		//si no habia caracter de escape, o
		return false;			//habia pero el flag indicaba registro borrado, error

	//si llego aca, el registro empezaba correctamente y tengo que verificar el
	//tamaño del registro, por eso primero lo recorro para conocer su tamaño
	unsigned int count = 2;			// =1 y no =0 porque ya considero los flags leidos
	bool fin = false;
	this->archivo.read(&aux, 1);			//leo el primer caracter en 'aux'
	while(! fin){
		while(aux != '@'){					//mientras el caracter leido no sea el de
			count++;						//escape sigo leyendo el archivo y contando
			this->archivo.read(&aux, 1);
		}
		this->archivo.read(&aux2,1);
		if(aux2 == '|')				//si el caracter que sigue al de escape es el de
			fin = true;				//corte, marco el final y salgo
		else{						//sino, tengo que contar los dos y seguir leyendo
			count++;					//cuento el primer caracter (el '@')
			count++;					//cuento el segundo (que NO era un '|')
			this->archivo.read(&aux,1);		//leo un nuevo caracter y repito el ciclo
		}
	}

	bool proxEsValido;
	unsigned long int nextReg = this->archivo.tellg();
	if(nextReg >= this->lastPos-1)			//si me pase del limite, el siguiente
		proxEsValido = false;				//es invalido
	else{
		this->archivo.read(&aux,1);			//si no me pase, verifico la cabecera
		this->archivo.read(&aux2,1);		//del registro que sigue, si indica que
		if(aux == '@' && aux2 == '0')		//fue borrado los anexo
			proxEsValido = true;
		else
			proxEsValido = false;
	}

	if(! proxEsValido){
		Reg_Vacio* newReg = new Reg_Vacio();
		newReg->offset = offset;
		newReg->size = count + 2;		//+2 por el '@' y el '|' del final que no conte
		this->listaVacios.push_back(*newReg);
		this->actualizarVacios = true;
	}
	else{
		list<Reg_Vacio>::iterator it = this->listaVacios.begin();
		bool encontrado = false;
		while(!encontrado && it != this->listaVacios.end()){
			if(it->offset == nextReg){
				encontrado = true;
				it->offset = offset;
				it->size = it->size + count + 2;
				this->actualizarVacios = true;
			}
			else
				it++;
		}
	}

	//y ahora cierro el archivo y lo abro en modo read-write para actualizar el flag
	//del registro; y x ultimo, vuelvo a abrir en el modo que estaba...
	this->archivo.close();
	this->archivo.open(this->nombre, fstream::binary | fstream::in | fstream::out);

	this->archivo.seekp(offset+1, ios_base::beg);	//+1 para saltear el caracter de escape
	this->archivo.write("0",1);		//se marca el registro como "libre"

	this->archivo.close();
	this->archivo.open(this->nombre, fstream::binary | fstream::in);

	return true;
}
