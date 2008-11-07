/*
 * Indice.cpp
 *
 *  Created on: 26/09/2008
 *      Author: andres
 */
#include <iostream>
#include "Indice.h"
#include <string.h>
#include <stdlib.h>

/*******************************************************
 * CONSTRUCTOR Y DESTRUCTOR
 *******************************************************/

Indice::Indice(const char* nombreFisico, bool esIndiceSecundario){

	this->nombre = (char*) malloc(strlen(nombreFisico) +1);
	strcpy(this->nombre, nombreFisico);
	this->archivo = new StreamFijo(this->nombre, sizeof(Pagina));

	this->esIndiceSecundario = esIndiceSecundario;
	this->pActual = NULL;

	this->cantPaginas = this->archivo->contarRegistros();
	if(this->cantPaginas == 0)
		this->primeraPagina = 0;
	else{
		this->archivo->abrir(READ);
		this->archivo->seek_beg();
		Pagina pAux;
		unsigned long int id = this->archivo->leerProximo(&pAux);
		this->archivo->cerrar();
		this->primeraPagina = id;					//como no se si la primer pagina
		this->cargarPagina(this->primeraPagina);	//tiene ID=1, cargo esa y despues
		while(this->pActual->tieneAnterior())		//mientras tenga alguna antes, la
			this->cargarPagina(this->pActual->getIDPagAnt());	//cargo, hasta llegar
	}												//a la primera del archivo paginado

}

Indice::~Indice(){
	free(nombre);
	delete(this->pActual);
	delete(this->archivo);
}


/*******************************************************
 * METODOS PUBLICOS
 *******************************************************/

vector<RegPagina> Indice::getPaginaCandidata(double clave){

	vector<RegPagina> vector;

	if(this->cantPaginas > 0){	//si hay algo en el archivo

		//Localizo y cargo la pagina donde deberia estar el dato buscado
		int busqueda = this->buscarPagina(clave);
		//los resultados posibles son 0,1,3,4. y en los casos 1 y 3 se sabe
		//ciertamente que el registro no existe. En los otros casos, si el
		//registro existe, entonces esta en la pagina actual
		if(busqueda == 0 || busqueda == 4)
			vector = this->armarVector();
	}

	return vector;
}

void Indice::eliminar(double clave){

	if(this->cantPaginas > 0){	//si hay algo en el archivo

		int busqueda = this->buscarPagina(clave);
		bool existe = this->contieneDato(clave);


		while(existe){
			//mientras se determine que hay algun registro con la clave a borrar
			//en la pagina actual, se llama a borrar de esa pagina al registro y
			//se repite la operacion
			this->pActual->eliminar(clave);		//elimino todas las ocurrencias de
					//la pagina indicada como Actual y verifico su posterior estado

			if(this->pActual->getCantReg() == 0){

				Pagina pAntAux; bool hayAnt = this->pActual->tieneAnterior();
				Pagina pSigAux; bool haySig = this->pActual->tieneSiguiente();

				//si puedo recupero la siguiente y la anterior
				this->archivo->abrir(READ);
				if(hayAnt)	this->archivo->leer(&pAntAux, this->pActual->getIDPagAnt());
				if(haySig)	this->archivo->leer(&pSigAux, this->pActual->getIDPagSig());
				this->archivo->cerrar();

				if(hayAnt && haySig){
					//en este caso corrijo dos enlaces y actualizo en disco
					pAntAux.setIDPagSig(pSigAux.getIDPagina());
					pSigAux.setIDPagAnt(pAntAux.getIDPagina());
					this->archivo->abrir(UPDATE);
					this->archivo->actualizar(&pAntAux,pAntAux.getIDPagina());
					this->archivo->actualizar(&pSigAux,pSigAux.getIDPagina());
					this->archivo->cerrar();
					//borro la que quedo vacia y cargo la siguiente porque el
					//indicador quedo invalido al borrar la pagina vacia
					this->archivo->abrir(DELETE);
					this->archivo->borrar(this->pActual->getIDPagina());
					this->archivo->cerrar();
					this->cargarPagina(pSigAux.getIDPagina());
				}
				if(hayAnt && !haySig){
					//aca debo corregir solo el enlace de la anterior. (era ultima)
					pAntAux.setIDPagSig(0);	//no hay siguiente.
					this->archivo->abrir(UPDATE);
					this->archivo->actualizar(&pAntAux,pAntAux.getIDPagina());
					this->archivo->cerrar();
					//borro la que quedo vacia y cargo la anterior porque el
					//indicador quedo invalido al borrar la pagina vacia
					this->archivo->abrir(DELETE);
					this->archivo->borrar(this->pActual->getIDPagina());
					this->archivo->cerrar();
					this->cargarPagina(pAntAux.getIDPagina());
				}
				if(!hayAnt && haySig){
					//aca debo corregir solo el enlace de la siguiente. (era primera)
					pSigAux.setIDPagAnt(0);
					this->archivo->abrir(UPDATE);
					this->archivo->actualizar(&pSigAux,pSigAux.getIDPagina());
					this->archivo->cerrar();
					//borro la que quedo vacia y cargo la siguiente porque el
					//indicador quedo invalido al borrar la pagina vacia
					this->archivo->abrir(DELETE);
					this->archivo->borrar(this->pActual->getIDPagina());
					this->archivo->cerrar();
					this->primeraPagina = pSigAux.getIDPagina();
					this->cargarPagina(pSigAux.getIDPagina());
				}
				if(!hayAnt && !haySig){
					//este es el caso en que no quedan mas paginas en el indice.
					//borro la que quedo vacia y cargo la siguiente porque el
					//indicador quedo invalido al borrar la pagina vacia
					this->archivo->abrir(DELETE);
					this->archivo->borrar(this->pActual->getIDPagina());
					this->archivo->cerrar();
					this->pActual = NULL;
					this->cantPaginas = 0;
					this->primeraPagina = 0;
				}
			}
			else{	//sino, guardo la pagina a disco
				this->archivo->abrir(UPDATE);
				this->archivo->actualizar(this->pActual, this->pActual->getIDPagina());
				this->archivo->cerrar();
			}
			busqueda = this->buscarPagina(clave);
			existe = false;
			if(this->pActual != NULL)
				existe = this->contieneDato(clave);	//y verifico si hay ocurrencias de la
		}									//clave a borrar en las paginas subsiguiente
	}
}

void Indice::eliminar(double clave, unsigned int offset){

	if(this->cantPaginas > 0){
		vector<RegPagina> candidata = this->getPaginaCandidata(clave);
		if(candidata.size() > 0){
			bool claveBuscada = false;
			bool offsetBuscado = false;
			bool salir = false;
			int i = 0;
			//voy a recorrer la pagina candidata, que es la actual, buscando un registro
			//con la clave y el offset recibidos por parametro
			while(!salir && !claveBuscada && !offsetBuscado && i < this->pActual->getCantReg()){
				//salteo todos los registros del principio hasta la 1er ocurrencia de la clave
				while(!claveBuscada && i < this->pActual->getCantReg()){
					if(this->pActual->registros[i].getID() == clave)
						claveBuscada = true;		//la clave coincide, empieza la lista
					else
						i++;
				}
				//ahora recorro los de clave deseada buscando aquel con el offset deseado
				//si no encontro nada, va a salir porque el contador se excedio y va a terminar
				//la ejecucion de la funcion
				if(claveBuscada){
					while(claveBuscada && !offsetBuscado && i < this->pActual->getCantReg()){
						if(this->pActual->registros[i].getID() != clave)
							claveBuscada = false;		//termino la lista con claves deseadas y
						else{							//el offset deseado no estaba
							if(this->pActual->registros[i].getOffset() == offset)
								offsetBuscado = true;		//coincide el offset, encontre el registro
							else
								i++;
						}
					}
					if(!claveBuscada)		//es el caso en que recorrio toda la secuencia con las
						salir = true;		//claves deseadas, pero ningun registro tenia el offset
					else if(!offsetBuscado){			//es el caso que llego al final de la pagina
							claveBuscada = false;		//viendo las claves deseadas, pero no encontro
							i = 0;						//el offset. Entonces cargo la pag siguiente
							this->cargarPagina(this->pActual->getIDPagSig());
					}
				}
			}
			if(this->pActual->getCantReg() == 0){

				Pagina pAntAux; bool hayAnt = this->pActual->tieneAnterior();
				Pagina pSigAux; bool haySig = this->pActual->tieneSiguiente();

				//si puedo recupero la siguiente y la anterior
				this->archivo->abrir(READ);
				if(hayAnt)	this->archivo->leer(&pAntAux, this->pActual->getIDPagAnt());
				if(haySig)	this->archivo->leer(&pSigAux, this->pActual->getIDPagSig());
				this->archivo->cerrar();

				if(hayAnt && haySig){
					//en este caso corrijo dos enlaces y actualizo en disco
					pAntAux.setIDPagSig(pSigAux.getIDPagina());
					pSigAux.setIDPagAnt(pAntAux.getIDPagina());
					this->archivo->abrir(UPDATE);
					this->archivo->actualizar(&pAntAux,pAntAux.getIDPagina());
					this->archivo->actualizar(&pSigAux,pSigAux.getIDPagina());
					this->archivo->cerrar();
					//borro la que quedo vacia y cargo la siguiente porque el
					//indicador quedo invalido al borrar la pagina vacia
					this->archivo->abrir(DELETE);
					this->archivo->borrar(this->pActual->getIDPagina());
					this->archivo->cerrar();
					this->cargarPagina(pSigAux.getIDPagina());
				}
				if(hayAnt && !haySig){
					//aca debo corregir solo el enlace de la anterior. (era ultima)
					pAntAux.setIDPagSig(0);	//no hay siguiente.
					this->archivo->abrir(UPDATE);
					this->archivo->actualizar(&pAntAux,pAntAux.getIDPagina());
					this->archivo->cerrar();
					//borro la que quedo vacia y cargo la anterior porque el
					//indicador quedo invalido al borrar la pagina vacia
					this->archivo->abrir(DELETE);
					this->archivo->borrar(this->pActual->getIDPagina());
					this->archivo->cerrar();
					this->cargarPagina(pAntAux.getIDPagina());
				}
				if(!hayAnt && haySig){
					//aca debo corregir solo el enlace de la siguiente. (era primera)
					pSigAux.setIDPagAnt(0);
					this->archivo->abrir(UPDATE);
					this->archivo->actualizar(&pSigAux,pSigAux.getIDPagina());
					this->archivo->cerrar();
					//borro la que quedo vacia y cargo la siguiente porque el
					//indicador quedo invalido al borrar la pagina vacia
					this->archivo->abrir(DELETE);
					this->archivo->borrar(this->pActual->getIDPagina());
					this->archivo->cerrar();
					this->primeraPagina = pSigAux.getIDPagina();
					this->cargarPagina(pSigAux.getIDPagina());
				}
				if(!hayAnt && !haySig){
					//este es el caso en que no quedan mas paginas en el indice.
					//borro la que quedo vacia y cargo la siguiente porque el
					//indicador quedo invalido al borrar la pagina vacia
					this->archivo->abrir(DELETE);
					this->archivo->borrar(this->pActual->getIDPagina());
					this->archivo->cerrar();
					this->pActual = NULL;
					this->cantPaginas = 0;
				}
			}
			else{	//sino, guardo la pagina a disco
				this->archivo->abrir(UPDATE);
				this->archivo->actualizar(this->pActual, this->pActual->getIDPagina());
				this->archivo->cerrar();
			}
		}
	}
}

bool Indice::insertar(double clave, unsigned long int offset){

	Pagina pAux;
	pAux.reset();
	RegPagina newRegPag;
	newRegPag.setID(clave);
	newRegPag.setOffset(offset);

	// Si el Indice esta vacio agrego una pagina nueva en la primer posicion con el par clave-offset.
	// es decir, se crea una nueva pagina, y se guarda en el primer registro (1) del archivo paginado
	if( this->cantPaginas == 0 ) {
		pAux.insertar(newRegPag);	// inserta el clave-offset en la pagina pAux (nueva)
		pAux.setIDPagina(this->archivo->getPosLibre());		// setea ID segun posicion libre en el archivo
		this->archivo->abrir(WRITE);
		this->archivo->escribir(&pAux);						// inserta la pagina en el archivo en el lugar
		this->archivo->cerrar();							//que habia indicado 'getPosLibre', y la pagina
		this->primeraPagina = pAux.getIDPagina();			//1 ahora es el registro 1, donde se guardo la pag
		this->cantPaginas++;
		//como la paginaActual es null, creo una nueva pagina.
		this->pActual = new Pagina();
		//y copio la pagina nueva, con el registro que se inserta, a la pagina actual.
		*(this->pActual) = pAux;
		return true;
	}

	// ***************EL INDICE NO ESTA VACIO*******************

	//Localizo y cargo la pagina donde deberia ir el dato a insertar
	int busqueda = this->buscarPagina(clave);

	bool existe = false;
	if(busqueda == 0 || busqueda == 4)
		//en estos casos puede que exista un registro con la clave nueva, entonces llamo a la
		//funcion que recorre la pagina actual buscandolo.
		existe = this->contieneDato(clave);

	//si el dato a insertar existe en el indice y el mismo NO es secundario ==> no se debe insertar
	if( existe && !(this->esIndiceSecundario))
		return false;
	//si el dato no esta en el indice, o si esta pero el mismo es secundario, puedo insertar

	//entonces llegue aca sabiendo que puedo insertar en el indice; y 'busqueda' me indica
	//que puedo insertar en pActual(0 o 1) o que tengo que hacer split de pActual (3 o 4)

	if(busqueda == 0 || busqueda == 1){
		//en ambos casos se puede insertar en pActual directamente

		this->pActual->insertar(newRegPag);	//inserta el clave-offset en la pagina
		this->archivo->abrir(UPDATE);
		this->archivo->actualizar(this->pActual, this->pActual->getIDPagina());
		this->archivo->cerrar();
	}

	else if(busqueda == 3 || busqueda == 4){
		//en ambos casos hay que hacer split de pActual e insertar

		this->dividirPagina();
		//cargo la segunda generada de la division
		this->cargarPagina(this->pActual->getIDPagSig());

		if(clave < this->pActual->registros[0].getID()){
			//insertar en la primera de la division, entonces la cargo
			this->cargarPagina(this->pActual->getIDPagAnt());
		}//sino, insertar en la 2da, que ya esta cargada

		this->pActual->insertar(newRegPag);	//inserta el clave-offset en la pagina

		this->archivo->abrir(UPDATE);
		this->archivo->actualizar(this->pActual, this->pActual->getIDPagina());
		this->archivo->cerrar();
	}

	return true;
}

vector<RegPagina> Indice::leerSecuencial(double clave){

	vector<RegPagina> vector_final;
	vector_final.clear();
	vector_final.resize(0);

	vector<RegPagina> vec = this->getPaginaCandidata(clave);
	unsigned int i = 0;
	bool found = false;
	if(! vec.empty()){
		while(i < vec.size() && !found){		//con este ciclo me muevo desde el ppio
			if(vec[i].getID() == clave){		//de lo recuperado hasta la primer
				vector_final.push_back(vec[i]);	//ocurrencia del id que busco y lo meto
				found = true;
			}
			else
				i++;
		}

		if(! found) 	return vector_final;	//si found sigue false, no existe la clave
												//y se devuelve el vector vacio

		i++;
		while (i < vec.size() && found){		//con este ciclo voy leyendo los ids
			if(vec[i].getID() == clave){		//que se repiten para pasarlos al vector
				vector_final.push_back(vec[i]);
				i++;
			}
			else
				found = false;
		}

		while(found == true && this->pActual->tieneSiguiente()){
			//llegó al final de la pagina y el ultimo correspondia con el id buscado
			//x lo tanto, si existe pagina siguiente, la leo
			this->cargarPagina(this->pActual->getIDPagSig());
			vec = this->armarVector();

			i = 0;
			found = true;
			while (i < vec.size() && found){
				if(vec[i].getID() == clave){
					vector_final.push_back(vec[i]);
					i++;
				}
				else
					found = false;
			}
		}
	}

	return vector_final;

}

vector<RegPagina> Indice::recorrerIndice(){

	vector<RegPagina> vector_final = getPrimeraPagina();
	vector<RegPagina> vecTemp = getPaginaSiguiente();

	while(vecTemp.size() > 0){
		for(unsigned int i=0; i<vecTemp.size(); i++)
			vector_final.push_back(vecTemp[i]);

		vecTemp = getPaginaSiguiente();
	}

	return vector_final;
}


/*******************************************************
 * METODOS PRIVADOS
 *******************************************************/

vector<RegPagina> Indice::armarVector(){

	vector<RegPagina> vector;
	vector.clear();
	vector.resize(0);

	for(int i=0; i<this->pActual->getCantReg(); i++)
		vector.push_back(this->pActual->registros[i]);

	return vector;
}

void Indice::cargarPagina(unsigned long int ID_pagina){

	Pagina	pAux;

	this->archivo->abrir(READ);
	this->archivo->leer(&pAux,ID_pagina);
	this->archivo->cerrar();

	//si la paginaActual es null, creo una nueva pagina.
	if( this->pActual == NULL )
		this->pActual = new Pagina();
	//y copio la pagina levantada a la pagina actual.
	*(this->pActual) = pAux;

}

bool Indice::contieneDato(double clave){

	bool encontrado = false;
	int i = 0;
	while(!encontrado && i<this->pActual->getCantReg()){
		if(this->pActual->registros[i].getID() == clave)
			encontrado = true;
		else
			i++;
	}
	return encontrado;
	/*
	for(int i=0; i<this->pActual->getCantReg(); i++)
		vector.push_back(this->pActual->registros[i]);

	AVL* arbol = new AVL();
	vector<RegPagina> vector;
	vector.clear();
	vector.resize(0);
	vector = this->armarVector();
	arbol->ArmarArbol(vector);
	return (arbol->Buscar(clave));
	*//*
	bool encontrado = false;
	int	pos = 0;

	//recorro el vector de registros de la pagina Actual, buscando el deseado
	while( !encontrado && pos < this->pActual->getCantReg() ){
		if(clave > this->pActual->registros[pos].getID())
			pos++;
		else
			if(clave < this->pActual->registros[pos].getID())
				//quiere decir que me pase y no lo encontre, entonces asigno al contador
				pos = this->pActual->getCantReg();	//un valor maximo para salir del ciclo
			else
				encontrado = true;			//sino, son iguales y lo encontre
	}
	return encontrado;
	*/
}

int Indice::buscarPagina(double clave){

	if(this->cantPaginas == 0)
		return -2;

	else {
		//se trabajara a partir de la pagina actual y si no es posible,
		//se trabaja desde el principio del archivo
		unsigned long int IDPagina;
		if(this->pActual == NULL){
			IDPagina = this->primeraPagina;
			this->cargarPagina(IDPagina);
		}
		else
			IDPagina= this->pActual->getIDPagina();

		int busqueda;
		bool encontrado = false;
		while(!encontrado){
			busqueda = this->buscarTarget(clave);
			switch (busqueda) {
				case  0:	// se debe insertar en la que quedo como pActual
							encontrado = true;
						break;
				case  1: 	// se debe insertar en la que quedo como pActual y
							// se que no hay registro/s con la clave nueva
							encontrado = true;
						break;
				case  2:	// se debe mover hacia adelante
							IDPagina = this->pActual->getIDPagSig();
							this->cargarPagina(IDPagina);
						break;
				case -2:	// se debe mover hacia atras
							IDPagina = this->pActual->getIDPagAnt();
							this->cargarPagina(IDPagina);
						break;
				case  3:	// se debe hacer SPLIT de la que quedo como pActual y
							// se que no hay registro/s con la clave nueva
							encontrado = true;
						break;
				case  4:	// se debe hacer SPLIT de la que quedo como pActual
							encontrado = true;
						break;
			}
		}
		return busqueda;
	}
}

int Indice::buscarTarget(double clave){

	if(clave < this->pActual->registros[0].getID()){
		//la clave buscada es menor a la primera de la pagina actual
		if(this->pActual->tieneAnterior()){
			Pagina pAntAux;
			this->archivo->abrir(READ);
			this->archivo->leer(&pAntAux, this->pActual->getIDPagAnt());
			this->archivo->cerrar();
			if(clave > pAntAux.registros[pAntAux.getCantReg() -1].getID()){
				//el nuevo es menor al minimo de pActual y mayor al maximo de pAnterior
				//si estoy buscando, no existe
				if(pAntAux.tieneLugar()){
					this->cargarPagina(pAntAux.getIDPagina());
					return 1;	//si busco NO existe, y si inserto hay lugar
				}				//primero veo la anterior (optimizacion)
				else{
					if(this->pActual->tieneLugar())
						return 1;	//si busco NO existe, y si inserto hay lugar
					else
						return 3;	//si busco NO existe, y si inserto NO hay lugar
				}					//entonces debo hacer split de actual
			}
			else{
				//el nuevo es menor o igual al maximo de pAnterior
				if(clave >= pAntAux.registros[0].getID()){
					//ademas, es mayor o igual al minimo de pAnterior ==> ANTERIOR
					this->cargarPagina(pAntAux.getIDPagina());
					if(this->pActual->tieneLugar())
						return 0;		//si busco existe, y si inserto hay lugar
					else
						return 4;		//si busco existe, y si inserto NO hay lugar
				}						//entonces debo hacer split de actual
				else{
					//ademas, es menor al minimo de pAnterior
					return -2;
				}
			}
		}
		else{
			//el nuevo es menor al minimo de pActual y pActual no tiene anterior ==> ACTUAL
			if(this->pActual->tieneLugar())
				return 1;	//si busco NO existe, y si inserto hay lugar
			else
				return 3;	//si busco NO existe, y si inserto NO hay lugar
		}					//entonces debo hacer split de actual
	}
	else{ //el nuevo es mayor o igual al minimo de pActual
		if(clave > this->pActual->registros[this->pActual->getCantReg() -1].getID()){
			//el nuevo es mayor al maximo de pActual
			if(this->pActual->tieneSiguiente()){
				Pagina pSigAux;
				this->archivo->abrir(READ);
				this->archivo->leer(&pSigAux, this->pActual->getIDPagSig());
				this->archivo->cerrar();
				if(clave < pSigAux.registros[0].getID()){
					//el nuevo es mayor al maximo de pActual y menor al minimo de pSiguiente
					//(si estoy buscando, no existe)
					if(this->pActual->tieneLugar())
						return 1;	//si busco NO existe, y si inserto hay lugar
					else{
						if(pSigAux.tieneLugar()){
							this->cargarPagina(pSigAux.getIDPagina());
							return 1;	//si busco NO existe, y si inserto hay lugar
						}
						else
							return 3;	//si busco NO existe, y si inserto NO hay lugar
					}					//entonces debo hacer split de actual
				}
				else{
					//el nuevo es mayor o igual al minimo de pSiguiente
					if(clave > pSigAux.registros[pSigAux.getCantReg() -1].getID()){
						//ademas, es mayor al maximo de pSiguiente
						return 2;
					}
					else{
						//el nuevo es >= al min de pSiguiente y <= al max
						//de pSiguiente ==> SIGUIENTE (si busco puede existir)
						this->cargarPagina(pSigAux.getIDPagina());
						if(this->pActual->tieneLugar())
							return 0;		//si busco existe, y si inserto hay lugar
						else
							return 4;		//si busco existe, y si inserto NO hay lugar
					}						//entonces debo hacer split de actual
				}
			}
			else{ //el nuevo es mayor al maximo de pActual y pActual no tiene siguiente
				  // si estoy buscando, no existe; pero si quiero insertar ==> ACTUAL
				if(this->pActual->tieneLugar())
					return 1;	//si busco NO existe, y si inserto hay lugar
				else
					return 3;	//si busco NO existe, y si inserto NO hay lugar
			}					//entonces debo hacer split de actual
		}
		else{ //el nuevo es >= al min y <= al max de pActual ===> ACTUAL
			  // ademas, si estoy buscando la clave, si existe esta en pActual
			if(this->pActual->tieneLugar())
				return 0;		//si busco existe, y si inserto hay lugar
			else
				return 4;		//si busco existe, y si inserto NO hay lugar
		}						//entonces debo hacer split de actual
	}
}

void Indice::dividirPagina(){

	Pagina	pNueva, pSigNueva;
	pNueva.reset();

	//paso la segunda mitad de la pagina a una pagina nueva
	for(int i = MAX_REGS_PAGINA/2 ; i<=MAX_REGS_PAGINA-1 ; i++)
		pNueva.insertar(this->pActual->registros[i]);

	//la cantidad de registros de la que era Pagina actual bajo a la mitad... 'baja logica'
	this->pActual->setCantReg(MAX_REGS_PAGINA/2);
	//y la cantidad de la nueva pagina es la mitad de lo que tenia inicialmente la 'actual'
	pNueva.setCantReg(MAX_REGS_PAGINA/2);

	//Arreglo los enlaces
	pNueva.setIDPagSig(this->pActual->getIDPagSig());
	pNueva.setIDPagAnt(this->pActual->getIDPagina());

	// Obtengo el proximo numero de registro donde va a insertar a esa pagina nueva
	// que tambien va a ser el ID de la pagina
	pNueva.setIDPagina(this->archivo->getPosLibre());

	this->archivo->abrir(WRITE);
	this->archivo->escribir(&pNueva);
	this->archivo->cerrar();

	this->pActual->setIDPagSig(pNueva.getIDPagina());

	//se actualiza la pagina que ya era pagina, que estaba almacenada en el archivo
	this->archivo->abrir(UPDATE);
	this->archivo->actualizar(this->pActual, this->pActual->getIDPagina());
	this->archivo->cerrar();

	//Corrigo el enlace al anterior del siguiente a la nueva pagina insertada
	if( pNueva.getIDPagSig()!= 0 ){
		//primero leo la pagina a modificar
		this->archivo->abrir(READ);
		this->archivo->leer(&pSigNueva, pNueva.getIDPagSig());
		this->archivo->cerrar();

		//cambio el ID de la anterior
		pSigNueva.setIDPagAnt(pNueva.getIDPagina());

		//y ahora la updateo en el archivo
		this->archivo->abrir(UPDATE);
		this->archivo->actualizar(&pSigNueva, pSigNueva.getIDPagina());
		this->archivo->cerrar();
	}

	this->cantPaginas++;
}

vector<RegPagina> Indice::getPrimeraPagina(){

	vector<RegPagina> vector;
	vector.clear();
	vector.resize(0);

	if(this->cantPaginas != 0){
		this->cargarPagina(this->primeraPagina);
		vector = this->armarVector();
	}
	return vector;
}

vector<RegPagina> Indice::getPaginaSiguiente(){

	vector<RegPagina> vector;
	vector.clear();
	vector.resize(0);

	if(this->cantPaginas !=0){
		// si hay siguiente la levanto
		if(this->pActual->tieneSiguiente()){
			this->cargarPagina(this->pActual->getIDPagSig());
			vector = this->armarVector();
		}
	}
	return vector;
}

