/*
 * TestDirectorioDAO.cpp
 *
 *  Created on: 31/10/2008
 *      Author: andres
 */
#include "TestDirectorioDAO.h"

TestDirectorioDAO::TestDirectorioDAO(){

}

TestDirectorioDAO::~TestDirectorioDAO(){

}

int TestDirectorioDAO::test(){

	DirectorioDAO* dDAO = new DirectorioDAO();

	Date* date1 = Date::valueOf(13,3,87,13,33,33);
	Directorio* dir1 = new Directorio("andres/directorio1", date1);
	this->print(dir1);
	Date* date2 = Date::valueOf(26,1,89,0,55,55);
	Directorio* dir2 = new Directorio("home/agus/dir2", date2);
	this->print(dir2);
	Date* date3 = Date::valueOf(23,8,62,23,10,10);
	Directorio* dir3 = new Directorio("el+viejo/64/fechas", date3);
	this->print(dir3);
	Date* date4 = Date::valueOf(13,3,87,13,33,33);
	Directorio* dir4 = new Directorio("los 5 unos", date4);
	this->print(dir4);
	Date* date5 = Date::valueOf(26,1,89,0,55,55);
	Directorio* dir5 = new Directorio("Cambio de hora/igual que el 1ro", date5);
	this->print(dir5);
	Date* date6 = Date::valueOf(23,9,62,23,10,10);
	Directorio* dir6 = new Directorio("Mismo-Horario-3ro", date6);
	this->print(dir6);

	this->insertar(dDAO,dir5);
	this->insertar(dDAO,dir1);
	this->insertar(dDAO,dir3);
	this->insertar(dDAO,dir6);
	this->insertar(dDAO,dir2);
	this->insertar(dDAO,dir4);

	this->getById(dDAO,4);
	this->getById(dDAO,2);
	this->getById(dDAO,1);
	this->getById(dDAO,0);
	this->getById(dDAO,5);
	this->getById(dDAO,3);
	this->getById(dDAO,6);
	this->getById(dDAO,7);

	std::cout << "" << std::endl;
	this->borrar(dDAO,2);
	this->getById(dDAO,2);
	std::cout << "OJO. Debia dar NULL porque en el paso anterior se elimino." << std::endl;

	std::cout << "" << std::endl;
	std::cout << "Recorrido de la lista ordenada por Fecha de Modificacion" << std::endl;
	list<Directorio> lista = dDAO->getDirsSortedByFechaModif();
	list<Directorio>::iterator it;
	for(it=lista.begin();it!=lista.end();it++){
		this->print(&(*it));
	}

	std::cout << "" << std::endl;
	Date* newFecha = Date::valueOf(27,12,92,13,33,33);
	this->update(dDAO,2,newFecha);
	std::cout << "OJO. Debia dar Falso porque se quiso updatear algo eliminado" << std::endl;
	std::cout << "" << std::endl;
	this->getById(dDAO,1);
	this->update(dDAO,1,newFecha);
	std::cout << "OJO. Recordar que se modifico la Fecha del Dir1 en el paso anterior." << std::endl;
	this->getById(dDAO,1);

	std::cout << "" << std::endl;
	std::cout << "El Gran Finali" << std::endl;
	this->getById(dDAO,4);
	this->getById(dDAO,2);
	this->getById(dDAO,1);
	this->getById(dDAO,0);
	this->getById(dDAO,5);
	this->getById(dDAO,3);
	this->getById(dDAO,6);
	this->getById(dDAO,7);

	std::cout << "" << std::endl;
	std::cout << "Recorrido de la lista ordenada por Fecha de Modificacion" << std::endl;
	list<Directorio> lista2 = dDAO->getDirsSortedByFechaModif();
	list<Directorio>::iterator it2;
	for(it2=lista2.begin();it2!=lista2.end();it2++){
		this->print(&(*it2));
	}


	std::cout << "- - - That's All Folks! - - -" << std::endl;
	return 0;
}

int TestDirectorioDAO::test2(){

	DirectorioDAO* dDAO = new DirectorioDAO();

/*	this->update(dDAO, 4, Date::valueOf(1,1,99,23,33));
	this->getById(dDAO,4);
	std::cout << "" << std::endl;
	std::cout << "Recorrido de la lista ordenada por Fecha de Modificacion" << std::endl;
	list<Directorio> lista = dDAO->getDirsSortedByFechaModif();
	list<Directorio>::iterator it;
	for(it=lista.begin();it!=lista.end();it++){
		this->print(&(*it));
	}
*/

	std::cout << "The Beggining..." << std::endl;
	for(int i=0; i<10000; i++){

		Date* date = Date::valueOf(13,3,87,13,33,33);
		Directorio* dir = new Directorio("andres", date);
		this->print(dir);
		this->insertar(dDAO,dir);
	}

	this->getById(dDAO,3578);
	this->borrar(dDAO,3578);
//	Date* newDate = Date::valueOf(26,01,99,13,59);
//	this->update(dDAO,3578,newDate);
	this->getById(dDAO,3578);

	std::cout << "- - - That's All Folks! - - -" << std::endl;
	return 0;
}

/*************************************************************************************/

void TestDirectorioDAO::print(Directorio* dir){
	unsigned int anio = dir->getFechaUltimaModificacion().getYear();
	unsigned int mes = dir->getFechaUltimaModificacion().getMonth();
	unsigned int dia = dir->getFechaUltimaModificacion().getDay();
	unsigned int hora = dir->getFechaUltimaModificacion().getHour();
	unsigned int minuto = dir->getFechaUltimaModificacion().getMinute();
	unsigned int segundo = dir->getFechaUltimaModificacion().getSecond();

	//se arma la clave compuesta concatenando los valores de la fecha
	double fechaCompuesta = Date::concatFecha(anio, mes, dia, hora, minuto, segundo);

	std::cout << "** info= " << dir->getID() << "--" << dir->getPath() << "--" <<
	dia << "/" << mes << "/" << anio << " - " << hora << ":" << minuto << ":" << segundo
	<< "--" << fechaCompuesta << std::endl;
}

void TestDirectorioDAO::getById(DirectorioDAO* dao, unsigned int id){
	Directorio* buffer = dao->getDirById(id);
	std::cout << "**** getById -" << id << "-" << std::endl;
	if(buffer != NULL)
		print(buffer);
	else
		std::cout << "**** NULL" << std::endl;
}

void TestDirectorioDAO::insertar(DirectorioDAO* dao, Directorio* dir){
	bool result = dao->insert(*dir);
	std::cout << "** insert Dir -" << dir->getID() << "- = " << result << std::endl;
}

void TestDirectorioDAO::borrar(DirectorioDAO* dao, unsigned int id){
	dao->borrar(id);
	std::cout << "** Se elimino el Directorio -" << id << "-" << std::endl;
}

void TestDirectorioDAO::update(DirectorioDAO* dao, unsigned int id, Date* newFecha){
	bool result = dao->update(id, *newFecha);
	std::cout << "** Update del Directorio -" << id << "- = " << result << std::endl;
}
