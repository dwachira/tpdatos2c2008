/*
 * TestMensajeDAO.cpp
 *
 *  Created on: 01/11/2008
 *      Author: andres
 */
#include "TestMensajeDAO.h"

TestMensajeDAO::TestMensajeDAO(){

}

TestMensajeDAO::~TestMensajeDAO(){

}

int TestMensajeDAO::test(){

	MensajeDAO* mDAO = new MensajeDAO();

	Mensaje* msj1 = new Mensaje("el primero", 3260, 1);
	this->print(msj1);
	Mensaje* msj2 = new Mensaje("vale 2", 120368, 56);
	this->print(msj2);
	Mensaje* msj3 = new Mensaje("triplete", 333, 3);
	this->print(msj3);
	Mensaje* msj4 = new Mensaje("cuadro msj", 3260, 56);
	this->print(msj4);
	Mensaje* msj5 = new Mensaje("quinto mensaje", 332,3);
	this->print(msj5);
	Mensaje* msj6 = new Mensaje("sexto y ultimo", 3260, 42);
	this->print(msj6);

	this->insertar(mDAO,msj1);
	this->insertar(mDAO,msj2);
	this->insertar(mDAO,msj3);
	this->insertar(mDAO,msj4);
	this->insertar(mDAO,msj5);
	this->insertar(mDAO,msj6);

	this->getById(mDAO,4);
	this->getById(mDAO,0);
	this->getById(mDAO,1);
	this->getById(mDAO,7);
	this->getById(mDAO,2);
	this->getById(mDAO,6);
	this->getById(mDAO,3);
	this->getById(mDAO,5);

	std::cout << "" << std::endl;
	this->borrar(mDAO,2);
	this->getById(mDAO,2);
	std::cout << "OJO. Debia dar NULL porque en el paso anterior se elimino." << std::endl;
	std::cout << "" << std::endl;

	this->getById(mDAO,6);
	this->update(mDAO,6,1);
	this->getById(mDAO,6);

	this->update(mDAO,2,15);

	std::cout << "" << std::endl;
	std::cout << "- - - That's All Folks! - - -" << std::endl;
	return 0;
}

/*************************************************************************************/

void TestMensajeDAO::print(Mensaje* msj){
	std::cout << "**ID=" << msj->getID()
			  << "--Nombre=" << msj->getNombre()
			  << "--Tamanio=" << msj->getTamanio()
			  << "--Cant.Partes=" << msj->getCant_partes() << std::endl;
}

void TestMensajeDAO::getById(MensajeDAO* dao, unsigned int id){
	Mensaje buffer = dao->getMsjById(id);
	std::cout << "**** getById -" << id << "-" << std::endl;
	if(buffer.getID() != 0)	//el ID 0 indica informacion invalida
		print(&buffer);
	else
		std::cout << "**** NULL" << std::endl;
}

void TestMensajeDAO::insertar(MensajeDAO* dao, Mensaje* msj){
	bool result = dao->insert(*msj);
	std::cout << "** insert Msj -" << msj->getID() << "- = " << result << std::endl;
}

void TestMensajeDAO::borrar(MensajeDAO* dao, unsigned int id){
	dao->borrar(id);
	std::cout << "** Se elimino el Mensaje -" << id << "-" << std::endl;
}

void TestMensajeDAO::update(MensajeDAO* dao, unsigned int id, int newCant){
	bool result = dao->update(id,newCant);
	std::cout << "** Update [" << newCant << "] del Mensaje -" << id << "- = " << result << std::endl;
}
