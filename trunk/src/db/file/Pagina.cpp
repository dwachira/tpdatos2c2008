/*
 * Pagina.cpp
 *
 *  Created on: 25/09/2008
 *      Author: andres
 */

#include "Pagina.h"

/*******************************************************
 * CONSTRUCTOR Y DESTRUCTOR
 *******************************************************/

Pagina::Pagina(){ }

Pagina::~Pagina(){ }


/*******************************************************
 * METODOS PUBLICOS
 *******************************************************/

bool Pagina::tieneSiguiente(){
	return (this->IDPagSig != 0);
}
bool Pagina::tieneAnterior(){
	return (this->IDPagAnt != 0);
}
bool Pagina::tieneLugar(){
	return (this->cantReg < MAX_REGS_PAGINA);
}

void Pagina::reset(){
	this->setIDPagina(0);
	this->setCantReg( 0);
	this->setIDPagSig(0);
	this->setIDPagAnt(0);
}

void Pagina::insertar(RegPagina newReg){

	if(tieneLugar()){

	if(this->cantReg == 0)
		this->registros[0] = newReg;
	else{
		bool encontrado = false;
		int pos = 0;
		while(!encontrado && pos < this->cantReg){
			if(newReg.getID() > this->registros[pos].getID())
				pos++;
			else
				encontrado = true;
		}
		if(!encontrado)			//el nuevo tiene clave mayor a todos los almacenados
			this->registros[this->cantReg] = newReg;	//entonces lo guardo al final
		else{
			// recorro de atras para adelante para hacer lugar para el nuevo elemento
			for(int i=this->cantReg -1 ; i>=pos ; i-- )
				this->registros[i+1] = this->registros[i];
			this->registros[pos] = newReg;
		}
	}
	this->cantReg++;

	}
}

void Pagina::eliminar(double ID){

	if(this->cantReg > 0){
		bool encontrado = false;
		int pos = 0;
		while(!encontrado && pos < this->cantReg){
			if(ID > this->registros[pos].getID())
				pos++;
			else
				encontrado = true;
		}

		//si 'encontrado', 'pos' indica la primer ocurrencia de la clave dada
		if(encontrado){
			int contador = 1;
			int pos2 = pos + 1;
			while(registros[pos2].getID() == ID && pos2 < this->cantReg){
				contador ++;
				pos2 ++;
			}

			//ahora contador indica la cantidad de veces que se repite la clave en la pagina
			if(pos2 == this->cantReg)	//significa que salio porque llego al final de la pag
				this->cantReg = pos;	//entonces solamente la recorto y listo....
			else{
				int origen = pos2;
				int destino = pos;
				while(origen < this->cantReg){
					double newID = registros[origen].getID();
					unsigned long int newOffset = registros[origen].getOffset();
					registros[destino].setID(newID);
					registros[destino].setOffset(newOffset);
					origen++;
					destino++;
				}
				this->cantReg = this->cantReg - contador;
			}
		}
	}
}
		//si 'encontrado', 'pos' indica la primer ocurrencia de la clave dada
/*		if(encontrado){
			//mientras siga leyendo la misma clave, voy reemplazando con el registro
			//que sigue en la lista de registros y moviendo del final al principio
			while(registros[pos].getID() == ID && pos < this->cantReg){
				int pos2 = pos;
				while( (pos2 + 1) < this->cantReg) {
					double newID = registros[pos2 + 1].getID();
					registros[pos2].setID(newID);
					unsigned long int newOffset = registros[pos2 + 1].getOffset();
					registros[pos2].setOffset(newOffset);
					pos2++;
				}
				this->cantReg--;

			}
		}*/

void Pagina::setIDPagina(unsigned long int newIDPagina){
	this->IDPagina = newIDPagina;
}
void Pagina::setIDPagSig(unsigned long int newIDPagSig){
	this->IDPagSig = newIDPagSig;
}
void Pagina::setIDPagAnt(unsigned long int newIDPagAnt){
	this->IDPagAnt = newIDPagAnt;
}
void Pagina::setCantReg(int newCantReg){
	this->cantReg = newCantReg;
}
unsigned long int Pagina::getIDPagina(){
	return this->IDPagina;
}
unsigned long int Pagina::getIDPagSig(){
	return this->IDPagSig;
}
unsigned long int Pagina::getIDPagAnt(){
	return this->IDPagAnt;
}
int Pagina::getCantReg(){
	return this->cantReg;
}
