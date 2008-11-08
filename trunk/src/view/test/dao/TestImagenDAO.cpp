/*
 * TestImagenDAO.cpp
 *
 *  Created on: 31/10/2008
 *      Author: andres
 */
#include "TestImagenDAO.h"

TestImagenDAO::TestImagenDAO(){

}

TestImagenDAO::~TestImagenDAO(){

}

int TestImagenDAO::test(){

	ImagenDAO* iDAO = new ImagenDAO();

	//ID_DIR - ESPACIO_LIBRE - PROX_BIT_LIBRE - HASHVALUE - TAMANIO - NOMBRE
	util::Date* fecha1 = util::Date::valueOf(13,02,87,23,33);
	Imagen* img1 = new Imagen(2, 3457, 221, "RiverCampeon2008", 5679, "La primera.jpg",fecha1);
	this->print(img1);
	util::Date* fecha2 = util::Date::valueOf(22,12,29,22,0);
	Imagen* img2 = new Imagen(1, 210, 15, "RiverCampeon2004", 1200, "Vale2.png",fecha2);
	this->print(img2);
	util::Date* fecha3 = util::Date::valueOf(30,03,99,3,3);
	Imagen* img3 = new Imagen(7, 3368, 220, "RiverCampeon2002", 3398, "Foto Numero3.bmp",fecha3);
	this->print(img3);
	util::Date* fecha4 = util::Date::valueOf(4,1,44,1,4);
	Imagen* img4 = new Imagen(4, 210, 351, "RiverCampeon2000", 334, "Se va la cuarta.gif",fecha4);
	this->print(img4);
	util::Date* fecha5 = util::Date::valueOf(5,5,95,5,55);
	Imagen* img5 = new Imagen(7, 1, 0, "RiverCampeon1999", 2783, "Quinta-llena.jpg",fecha5);
	this->print(img5);
	util::Date* fecha6 = util::Date::valueOf(16,6,66,16,6);
	Imagen* img6 = new Imagen(4, 3368, 996, "RiverCampeon1997", 12276, "Ultima66.bmp",fecha6);
	this->print(img6);

	this->insertar(iDAO,img3);
	this->insertar(iDAO,img6);
	this->insertar(iDAO,img1);
	this->insertar(iDAO,img4);
	this->insertar(iDAO,img2);
	this->insertar(iDAO,img5);

	this->getById(iDAO, 4);
	this->getById(iDAO, 0);
	this->getById(iDAO, 1);
	this->getById(iDAO, 7);
	this->getById(iDAO, 2);
	this->getById(iDAO, 6);
	this->getById(iDAO, 3);
	this->getById(iDAO, 5);

	std::cout << "" << std::endl;
	this->borrar(iDAO,3);
	this->getById(iDAO,3);
	std::cout << "OJO. Debia dar NULL porque en el paso anterior se elimino." << std::endl;
	std::cout << "" << std::endl;

	this->getById(iDAO, 0);
	this->getById(iDAO, 1);
	this->getById(iDAO, 2);
	this->getById(iDAO, 3);
	this->getById(iDAO, 4);
	this->getById(iDAO, 5);
	this->getById(iDAO, 6);
	this->getById(iDAO, 7);
	this->getImgsByDir(iDAO,0);
	this->getImgsByDir(iDAO,1);
	this->getImgsByDir(iDAO,2);
	this->getImgsByDir(iDAO,3);
	this->getImgsByDir(iDAO,4);
	this->getImgsByDir(iDAO,5);
	this->getImgsByDir(iDAO,6);
	this->getImgsByDir(iDAO,7);


	std::cout << "" << std::endl;
	std::cout << "Voy a updatear el nombre de la Imagen 2 [nueva_imagen DOS.png], cuyos datos son los siguientes:" << std::endl;
	this->getById(iDAO,2);
	this->updateNombre(iDAO,3,"nueva_imagen DOS.png");
	this->updateNombre(iDAO,2,"nueva_imagen DOS.png");
	this->getById(iDAO,2);
	std::cout << "" << std::endl;
	std::cout << "Voy a updatear el espacio libre de la imagen 1 [0], cuyos datos son los siguientes:" << std::endl;
	this->getById(iDAO,1);
	this->updateEspacio(iDAO,3,0);
	this->updateEspacio(iDAO,1,0);
	this->getById(iDAO,1);
	std::cout << "" << std::endl;
	std::cout << "Voy a updatear el directorio de la imagen 1 [7], cuyos datos son los siguientes:" << std::endl;
	this->getById(iDAO,1);
	this->updateDirectorio(iDAO,3,7);
	this->updateDirectorio(iDAO,1,7);
	this->getById(iDAO,1);
	std::cout << "" << std::endl;
	std::cout << "Voy a updatear el hash de la Imagen 2 [35218864], cuyos datos son los siguientes:" << std::endl;
	this->getById(iDAO,2);
	this->updateHash(iDAO,3,"BocaBanderasNegr");
	this->updateHash(iDAO,2,"La12Abandona1rTp");
	this->getById(iDAO,2);
	std::cout << "" << std::endl;
	std::cout << "Voy a updatear el prox bit libre de la Imagen 5 [129], cuyos datos son los siguientes:" << std::endl;
	this->getById(iDAO,5);
	this->updateProxBit(iDAO,3,129);
	this->updateProxBit(iDAO,5,129);
	this->getById(iDAO,5);
	std::cout << "" << std::endl;
	std::cout << "Voy a updatear la fecha de la Imagen 1 [1/1/1 1:1], cuyos datos son los siguientes:" << std::endl;
	this->getById(iDAO,1);
	util::Date* newFecha = util::Date::valueOf(1,1,1,1,1);
	this->updateFecha(iDAO,3,newFecha);
	this->updateFecha(iDAO,1,newFecha);
	this->getById(iDAO,1);

	std::cout << "" << std::endl;
	std::cout << "El Gran Finali" << std::endl;
	this->getById(iDAO, 0);
	this->getById(iDAO, 1);
	this->getById(iDAO, 2);
	this->getById(iDAO, 3);
	this->getById(iDAO, 4);
	this->getById(iDAO, 5);
	this->getById(iDAO, 6);
	this->getById(iDAO, 7);
	this->getImgsByDir(iDAO,0);
	this->getImgsByDir(iDAO,1);
	this->getImgsByDir(iDAO,2);
	this->getImgsByDir(iDAO,3);
	this->getImgsByDir(iDAO,4);
	this->getImgsByDir(iDAO,5);
	this->getImgsByDir(iDAO,6);
	this->getImgsByDir(iDAO,7);


	std::cout << "" << std::endl;
	std::cout << "Recorrido de la lista ordenada por Espacio Libre" << std::endl;
	list<Imagen> lista = iDAO->getImgsSortedByEspacioLibre();
	list<Imagen>::iterator it;
	for(it=lista.begin();it!=lista.end();it++){
		this->print(&(*it));
	}

	std::cout << "" << std::endl;
	std::cout << "- - - That's All Folks! - - -" << std::endl;
	return 0;
}

/*************************************************************************************/

void TestImagenDAO::print(Imagen* img){

	unsigned int anio = img->getFechaUltimaModificacion().getYear();
	unsigned int mes = img->getFechaUltimaModificacion().getMonth();
	unsigned int dia = img->getFechaUltimaModificacion().getDay();
	unsigned int hora = img->getFechaUltimaModificacion().getHour();
	unsigned int minuto = img->getFechaUltimaModificacion().getMinute();

	//se arma la clave compuesta concatenando los valores de la fecha
	double fechaCompuesta = Date::concatFecha(anio, mes, dia, hora, minuto);

	std::cout << "**ID=" << img->getID() <<
				 "-ID_DIR=" << img->getID_Dir() <<
				 "-LIBRE=" << img->getEspacio_libre() <<
				 "-BIT_LIBRE=" << img->getProximo_bit_libre() <<
				 "-HASH=" << img->getHash_value() <<
				 "-TAMANIO=" << img->getTamanio() <<
				 "-NOMBRE=" << img->getNombre() << "..." <<
				 dia<<"/"<<mes<<"/"<<anio<<"-"<<hora<<":"<<minuto<<
				 "..."<< fechaCompuesta << std::endl;
}

void TestImagenDAO::insertar(ImagenDAO* dao, Imagen* img){
	bool result = dao->insert(*img);
	std::cout << "** insert Img -" << img->getID() << "- = " << result << std::endl;
}

void TestImagenDAO::getById(ImagenDAO* dao, unsigned int id){
	Imagen buffer = dao->getImgById(id);
	std::cout << "**** getById -" << id << "-" << std::endl;
	if(buffer.getID() != 0)	//el ID 0 indica informacion invalida
		print(&buffer);
	else
		std::cout << "**** NULO" << std::endl;
}

void TestImagenDAO::borrar(ImagenDAO* dao, unsigned int id){
	dao->borrar(id);
	std::cout << "** Se elimino la Imagen -" << id << "-" << std::endl;
}

void TestImagenDAO::getImgsByDir(ImagenDAO* dao, unsigned int id){

	std::cout << "" << std::endl;
	std::cout << "getImagenesbyDirectorio " << id << std::endl;
	list<Imagen> lista = dao->getImgsByDirectorio(id);
	list<Imagen>::iterator it;
	for(it=lista.begin();it!=lista.end();it++){
		print(&(*it));
	}
}

void TestImagenDAO::updateEspacio(ImagenDAO* dao, unsigned int id, unsigned int newEspacio){
	bool result = dao->updateEspacioLibre(id, newEspacio);
	std::cout << "** Update [espacio] de la Imagen -" << id << "- = " << result << std::endl;
}

void TestImagenDAO::updateDirectorio(ImagenDAO* dao, unsigned int id, unsigned int newDir){
	bool result = dao->updateDirectorio(id, newDir);
	std::cout << "** Update [directorio] de la Imagen -" << id << "- = " << result << std::endl;
}

void TestImagenDAO::updateHash(ImagenDAO* dao, unsigned int id, string newHash){
	bool result = dao->updateHashValue(id, newHash);
	std::cout << "** Update [hash] de la Imagen -" << id << "- = " << result << std::endl;
}

void TestImagenDAO::updateProxBit(ImagenDAO* dao, unsigned int id, unsigned int newProxBit){
	bool result = dao->updateProxBitLibre(id, newProxBit);
	std::cout << "** Update [proxbitlibre] de la Imagen -" << id << "- = " << result << std::endl;
}

void TestImagenDAO::updateNombre(ImagenDAO* dao, unsigned int id, string newNombre){
	bool result = dao->updateNombre(id, newNombre);
	std::cout << "** Update [nombre] de la Imagen -" << id << "- = " << result << std::endl;
}

void TestImagenDAO::updateFecha(ImagenDAO* dao, unsigned int id, util::Date* newFecha){
	bool result = dao->updateFecha(id,*newFecha);
	std::cout << "** Update [fecha] de la Imagen -" << id << "- = " << result << std::endl;
}
