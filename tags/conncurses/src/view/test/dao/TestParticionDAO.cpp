/*
 * TestParticionDAO.cpp
 *
 *  Created on: 01/11/2008
 *      Author: andres
 */
#include "TestParticionDAO.h"

TestParticionDAO::TestParticionDAO(){

}

TestParticionDAO::~TestParticionDAO(){

}

int TestParticionDAO::test(){

	ParticionDAO* pDAO = new ParticionDAO();

	//ID_Img - ID_Txt - Posicion - Bit_inicio - Longitud - Libre
	Particion* part1 = new Particion(1,1,3,20,335,true);
	this->print(part1);
	Particion* part2 = new Particion(1,2,4,98,3210,false);
	this->print(part2);
	Particion* part3 = new Particion(2,3,1,11,335,true);
	this->print(part3);
	Particion* part4 = new Particion(1,2,1,999,3210,true);
	this->print(part4);
	Particion* part5 = new Particion(3,4,2,35,1507,false);
	this->print(part5);
	Particion* part6 = new Particion(3,3,2,623,335,true);
	this->print(part6);

	this->insertar(pDAO,part1);
	this->insertar(pDAO,part2);
	this->insertar(pDAO,part3);
	this->insertar(pDAO,part4);
	this->insertar(pDAO,part5);
	this->insertar(pDAO,part6);

	for(unsigned int i=0; i<5; i++){
		for(unsigned int j=0; j<5; j++){
			for(unsigned int k=0; k<5; k++){
				this->getById(pDAO,i,j,k);
			}
		}
	}

	std::cout << "" << std::endl;
	for(unsigned int img=0;img<5;img++)
		this->getByImg(pDAO,img);
	std::cout << "" << std::endl;
	for(unsigned int txt=0;txt<5;txt++)
		this->getByTxt(pDAO,txt);


	std::cout << "" << std::endl;
	this->borrar(pDAO, 1,1,3);
	this->getById(pDAO,1,1,3);
	std::cout << "OJO. Debia dar NULL porque en el paso anterior se elimino." << std::endl;
	std::cout << "" << std::endl;

	for(unsigned int i=0; i<5; i++){
		for(unsigned int j=0; j<5; j++){
			for(unsigned int k=0; k<5; k++){
				this->getById(pDAO,i,j,k);
			}
		}
	}

	std::cout << "" << std::endl;
	this->getById(pDAO,1,2,4);
	this->liberar(pDAO,1,2,4);
	this->getById(pDAO,1,2,4);
	this->liberar(pDAO,1,1,3);
	std::cout << "OJO. Debia dar NULL porque se habia eliminado." << std::endl;

	std::cout << "" << std::endl;
	for(unsigned int img=0;img<5;img++)
		this->getByImg(pDAO,img);
	std::cout << "" << std::endl;
	for(unsigned int txt=0;txt<5;txt++)
		this->getByTxt(pDAO,txt);


	std::cout << "" << std::endl;
	std::cout << "- - - That's All Folks! - - -" << std::endl;
	return 0;
}

/*************************************************************************************/

void TestParticionDAO::print(Particion* part){

	std::cout << "**ID_IMG=" << part->getID_Img() <<
				 "-ID_TXT=" << part->getID_Txt() <<
				 "-POSIC=" << part->getPosicion() <<
				 "-BIT_INICIO=" << part->getBit_inicio() <<
				 "-LONG=" << part->getLongitud() <<
				 "-LIBRE=" << part->isLibre() << std::endl;
}

void TestParticionDAO::insertar(ParticionDAO* dao, Particion* part){
	bool result = dao->insert(*part);
	std::cout << "** insert Part -" << part->getID_Img() << "." << part->getID_Txt()
						<< "." << part->getPosicion() << "- = " << result << std::endl;
}

void TestParticionDAO::getById(ParticionDAO* dao, unsigned int IMG, unsigned int TXT, unsigned int POS){
	Particion buffer = dao->getPartByImgTxtPos(IMG,TXT,POS);
	std::cout << "**** getById -" << IMG << "." << TXT << "." << POS << "-" << std::endl;
	if(buffer.getID_Img() == 0 && buffer.getID_Txt() == 0 && buffer.getPosicion() == 0)
		std::cout << "**** NULL" << std::endl;		//el ID 0 indica informacion invalida
	else
		print(&buffer);
}

void TestParticionDAO::borrar(ParticionDAO* dao, unsigned int IMG, unsigned int TXT, unsigned int POS){
	dao->borrar(IMG,TXT,POS);
	std::cout << "** Se elimino la Particion -" << IMG << "." << TXT << "." << POS << "-" << std::endl;
}

void TestParticionDAO::liberar(ParticionDAO* dao, unsigned int IMG, unsigned int TXT, unsigned int POS){
	bool result = dao->liberar(IMG,TXT,POS);
	std::cout << "** liberar Part -" << IMG << "." << TXT << "." << POS << "- = " << result << std::endl;
}

void TestParticionDAO::getByImg(ParticionDAO* dao, unsigned int IMG){

	std::cout << "" << std::endl;
	std::cout << "getParticionesByImagen " << IMG << std::endl;
	list<Particion> lista = dao->getPartsByImg(IMG);
	list<Particion>::iterator it;
	for(it=lista.begin();it!=lista.end();it++){
		this->print(&(*it));
	}
}

void TestParticionDAO::getByTxt(ParticionDAO* dao, unsigned int TXT){

	std::cout << "" << std::endl;
	std::cout << "getParticionesByTexto " << TXT << std::endl;
	list<Particion> lista = dao->getPartsByTxt(TXT);
	list<Particion>::iterator it;
	for(it=lista.begin();it!=lista.end();it++){
		this->print(&(*it));
	}
}
