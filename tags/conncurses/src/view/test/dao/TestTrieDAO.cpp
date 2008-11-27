/*
 * TestTrieDAO.cpp
 *
 *  Created on: 02/11/2008
 *      Author: andres
 */
#include "TestTrieDAO.h"

TestTrieDAO::TestTrieDAO(){

}

TestTrieDAO::~TestTrieDAO(){

}

//DIRECTORIOS
int TestTrieDAO::test(){

	ManagerDAO* manager = new ManagerDAO();
	DirectorioDAO dDAO = manager->getDirectorioDAO();
	bool result;
	unsigned int id;
	TrieDAO* trie = new TrieDAO(*manager);

	Date* date1 = Date::valueOf(13,3,87,13,33,33);string uno = "andres/directorio1";
	Directorio* dir1 = new Directorio(uno, date1);TestDirectorioDAO::print(dir1);
	Date* date2 = Date::valueOf(26,1,89,0,55,55);string dos = "home/agus/dir2";
	Directorio* dir2 = new Directorio(dos, date2);TestDirectorioDAO::print(dir2);
	Date* date3 = Date::valueOf(23,8,62,23,10,10);string tres = "el+viejo/64/fechas";
	Directorio* dir3 = new Directorio(tres, date3);TestDirectorioDAO::print(dir3);
	Date* date4 = Date::valueOf(13,3,87,13,33,33);string cuatro = "los 5 unos";
	Directorio* dir4 = new Directorio(cuatro, date4);TestDirectorioDAO::print(dir4);
	Date* date5 = Date::valueOf(26,1,89,0,55,55);string cinco = "Cambio de hora/igual que el 1ro";
	Directorio* dir5 = new Directorio(cinco, date5);TestDirectorioDAO::print(dir5);
	Date* date6 = Date::valueOf(23,9,62,23,10,10);string seis = "Mismo-Horario-3ro";
	Directorio* dir6 = new Directorio(seis, date6);TestDirectorioDAO::print(dir6);

	TestDirectorioDAO::insertar(&dDAO,dir1);
	TestDirectorioDAO::insertar(&dDAO,dir2);
	TestDirectorioDAO::insertar(&dDAO,dir3);
	TestDirectorioDAO::insertar(&dDAO,dir4);
	TestDirectorioDAO::insertar(&dDAO,dir5);
	TestDirectorioDAO::insertar(&dDAO,dir6);

	//A CONTINUACION CORRER CON OPCION 1 O 2. DEJAR ALGUNA DE LAS DOS COMENTADA

	//OPCION 1: ACA SE INSERTAN UNO A UNO EN EL TRIE TODAS LAS CADENAS
	result = trie->insertCadena(DIRECTORIOS,uno,dir1->getID());
	std::cout << "Insert " << dir1->getID() << "= " << result << std::endl;
	result = trie->insertCadena(DIRECTORIOS,dos,dir2->getID());
	std::cout << "Insert " << dir2->getID() << "= " << result << std::endl;
	result = trie->insertCadena(DIRECTORIOS,tres,dir3->getID());
	std::cout << "Insert " << dir3->getID() << "= " << result << std::endl;
	result = trie->insertCadena(DIRECTORIOS,cuatro,dir4->getID());
	std::cout << "Insert " << dir4->getID() << "= " << result << std::endl;
	result = trie->insertCadena(DIRECTORIOS,cinco,dir5->getID());
	std::cout << "Insert " << dir5->getID() << "= " << result << std::endl;
	result = trie->insertCadena(DIRECTORIOS,seis,dir6->getID());
	std::cout << "Insert " << dir6->getID() << "= " << result << std::endl;
	//OPCION 2: ACA SE RECORRE EL ARCHIVO Y SE INSERTAN TODAS LAS CADENAS EN EL TRIE
//	trie->loadTrie(DIRECTORIOS);

	id = trie->getIndice(DIRECTORIOS,uno);
	TestDirectorioDAO::getById(&dDAO,id);
	id = trie->getIndice(DIRECTORIOS,dos);
	TestDirectorioDAO::getById(&dDAO,id);
	id = trie->getIndice(DIRECTORIOS,tres);
	TestDirectorioDAO::getById(&dDAO,id);
	id = trie->getIndice(DIRECTORIOS,cuatro);
	TestDirectorioDAO::getById(&dDAO,id);
	id = trie->getIndice(DIRECTORIOS,cinco);
	TestDirectorioDAO::getById(&dDAO,id);
	id = trie->getIndice(DIRECTORIOS,seis);
	TestDirectorioDAO::getById(&dDAO,id);


	std::cout << "" << std::endl;
	TestDirectorioDAO::getById(&dDAO,dir2->getID());
	TestDirectorioDAO::borrar(&dDAO,dir2->getID());
	TestDirectorioDAO::getById(&dDAO,dir2->getID());
	std::cout << "OJO. Debia dar NULL porque en el paso anterior se elimino." << std::endl;
	result = trie->deleteCadena(DIRECTORIOS,dir2->getPath());
	std::cout << "Borrado " << dir2->getID() << " - " << dir2->getPath() << " = " << result << std::endl;
	id = trie->getIndice(DIRECTORIOS,dir2->getPath());
	std::cout << "getIndice del Borrado (debe ser 0) = " << id << std::endl;


	std::cout << "- - - That's All Folks! - - -" << std::endl;
	return 0;
}

//IMAGENES
int TestTrieDAO::test2(){

	ManagerDAO* manager = new ManagerDAO();
	ImagenDAO iDAO = manager->getImagenDAO();
	bool result;
	unsigned int id;
	TrieDAO* trie = new TrieDAO(*manager);

	//ID_DIR - ESPACIO_LIBRE - PROX_BIT_LIBRE - HASHVALUE - TAMANIO - NOMBRE
	string uno = "La primera.jpg";
	util::Date* fecha1 = util::Date::valueOf(13,02,87,23,33,33);
	Imagen* img1 = new Imagen(2, 3457, 221, "RiverCampeon2008", 5679, "La primera.jpg",fecha1);
	TestImagenDAO::print(img1);
	string dos = "Vale2.png";
	util::Date* fecha2 = util::Date::valueOf(22,12,29,22,0,0);
	Imagen* img2 = new Imagen(1, 210, 15, "RiverCampeon2004", 1200, "Vale2.png",fecha2);
		TestImagenDAO::print(img2);
	string tres = "Foto Numero3.bmp";
	util::Date* fecha3 = util::Date::valueOf(30,03,99,3,3,3);
	Imagen* img3 = new Imagen(7, 3368, 220, "RiverCampeon2002", 3398, "Foto Numero3.bmp",fecha3);
		TestImagenDAO::print(img3);
	string cuatro = "Se va la cuarta.gif";
	util::Date* fecha4 = util::Date::valueOf(4,1,44,1,4,4);
	Imagen* img4 = new Imagen(4, 210, 351, "RiverCampeon2000", 334, "Se va la cuarta.gif",fecha4);
		TestImagenDAO::print(img4);
	string cinco = "Quinta-llena.jpg";
	util::Date* fecha5 = util::Date::valueOf(5,5,95,5,55,55);
	Imagen* img5 = new Imagen(7, 1, 0, "RiverCampeon1999", 2783, "Quinta-llena.jpg",fecha5);
		TestImagenDAO::print(img5);
	string seis = "Ultima66.bmp";
	util::Date* fecha6 = util::Date::valueOf(16,6,66,16,6,6);
	Imagen* img6 = new Imagen(4, 3368, 996, "RiverCampeon1997", 12276, "Ultima66.bmp",fecha6);
		TestImagenDAO::print(img6);

	TestImagenDAO::insertar(&iDAO,img3);
	TestImagenDAO::insertar(&iDAO,img6);
	TestImagenDAO::insertar(&iDAO,img1);
	TestImagenDAO::insertar(&iDAO,img4);
	TestImagenDAO::insertar(&iDAO,img2);
	TestImagenDAO::insertar(&iDAO,img5);


	//A CONTINUACION CORRER CON OPCION 1 O 2. DEJAR ALGUNA DE LAS DOS COMENTADA

	//OPCION 1: ACA SE INSERTAN UNO A UNO EN EL TRIE TODAS LAS CADENAS
	result = trie->insertCadena(IMAGENES,uno,img1->getID());
	std::cout << "Insert " << img1->getID() << "= " << result << std::endl;
	result = trie->insertCadena(IMAGENES,dos,img2->getID());
	std::cout << "Insert " << img2->getID() << "= " << result << std::endl;
	result = trie->insertCadena(IMAGENES,tres,img3->getID());
	std::cout << "Insert " << img3->getID() << "= " << result << std::endl;
	result = trie->insertCadena(IMAGENES,cuatro,img4->getID());
	std::cout << "Insert " << img4->getID() << "= " << result << std::endl;
	result = trie->insertCadena(IMAGENES,cinco,img5->getID());
	std::cout << "Insert " << img5->getID() << "= " << result << std::endl;
	result = trie->insertCadena(IMAGENES,seis,img6->getID());
	std::cout << "Insert " << img6->getID() << "= " << result << std::endl;
	//OPCION 2: ACA SE RECORRE EL ARCHIVO Y SE INSERTAN TODAS LAS CADENAS EN EL TRIE
//	trie->loadTrie(IMAGENES);

	id = trie->getIndice(IMAGENES,uno);
	TestImagenDAO::getById(&iDAO,id);
	id = trie->getIndice(IMAGENES,dos);
	TestImagenDAO::getById(&iDAO,id);
	id = trie->getIndice(IMAGENES,tres);
	TestImagenDAO::getById(&iDAO,id);
	id = trie->getIndice(IMAGENES,cuatro);
	TestImagenDAO::getById(&iDAO,id);
	id = trie->getIndice(IMAGENES,cinco);
	TestImagenDAO::getById(&iDAO,id);
	id = trie->getIndice(IMAGENES,seis);
	TestImagenDAO::getById(&iDAO,id);


	std::cout << "" << std::endl;
	TestImagenDAO::getById(&iDAO,img3->getID());
	TestImagenDAO::borrar(&iDAO,img3->getID());
	TestImagenDAO::getById(&iDAO,img3->getID());
	std::cout << "OJO. Debia dar NULL porque en el paso anterior se elimino." << std::endl;

	result = trie->deleteCadena(IMAGENES,img3->getNombre());
	std::cout << "Borrado " << img3->getID() << " - " << img3->getNombre() << " = " << result << std::endl;
	id = trie->getIndice(IMAGENES,img3->getNombre());
	std::cout << "getIndice del Borrado (debe ser 0) = " << id << std::endl;


	std::cout << "- - - That's All Folks! - - -" << std::endl;
	return 0;
}

//MENSAJES
int TestTrieDAO::test3(){

	ManagerDAO* manager = new ManagerDAO();
	MensajeDAO mDAO = manager->getMensajeDAO();
	bool result;
	unsigned int id;
	TrieDAO* trie = new TrieDAO(*manager);

	string uno = "el primero";
	Mensaje* msj1 = new Mensaje(uno, 3260, 1);
	TestMensajeDAO::print(msj1);
	string dos = "vale 2";
	Mensaje* msj2 = new Mensaje(dos, 120368, 56);
	TestMensajeDAO::print(msj2);
	string tres = "triplete";
	Mensaje* msj3 = new Mensaje(tres, 333, 3);
	TestMensajeDAO::print(msj3);
	string cuatro = "cuadro msj";
	Mensaje* msj4 = new Mensaje(cuatro, 3260, 56);
	TestMensajeDAO::print(msj4);
	string cinco = "quinto mensaje";
	Mensaje* msj5 = new Mensaje(cinco, 332,3);
	TestMensajeDAO::print(msj5);
	string seis = "sexto y ultimo";
	Mensaje* msj6 = new Mensaje(seis, 3260, 42);
	TestMensajeDAO::print(msj6);

	TestMensajeDAO::insertar(&mDAO,msj1);
	TestMensajeDAO::insertar(&mDAO,msj2);
	TestMensajeDAO::insertar(&mDAO,msj3);
	TestMensajeDAO::insertar(&mDAO,msj4);
	TestMensajeDAO::insertar(&mDAO,msj5);
	TestMensajeDAO::insertar(&mDAO,msj6);


	//A CONTINUACION CORRER CON OPCION 1 O 2. DEJAR ALGUNA DE LAS DOS COMENTADA

	//OPCION 1: ACA SE INSERTAN UNO A UNO EN EL TRIE TODAS LAS CADENAS
	result = trie->insertCadena(MENSAJES,uno,msj1->getID());
	std::cout << "Insert " << msj1->getID() << "= " << result << std::endl;
	result = trie->insertCadena(MENSAJES,dos,msj2->getID());
	std::cout << "Insert " << msj2->getID() << "= " << result << std::endl;
	result = trie->insertCadena(MENSAJES,tres,msj3->getID());
	std::cout << "Insert " << msj3->getID() << "= " << result << std::endl;
	result = trie->insertCadena(MENSAJES,cuatro,msj4->getID());
	std::cout << "Insert " << msj4->getID() << "= " << result << std::endl;
	result = trie->insertCadena(MENSAJES,cinco,msj5->getID());
	std::cout << "Insert " << msj5->getID() << "= " << result << std::endl;
	result = trie->insertCadena(MENSAJES,seis,msj6->getID());
	std::cout << "Insert " << msj6->getID() << "= " << result << std::endl;
	//OPCION 2: ACA SE RECORRE EL ARCHIVO Y SE INSERTAN TODAS LAS CADENAS EN EL TRIE
//	trie->loadTrie(MENSAJES);

	id = trie->getIndice(MENSAJES,uno);
	TestMensajeDAO::getById(&mDAO,id);
	id = trie->getIndice(MENSAJES,dos);
	TestMensajeDAO::getById(&mDAO,id);
	id = trie->getIndice(MENSAJES,tres);
	TestMensajeDAO::getById(&mDAO,id);
	id = trie->getIndice(MENSAJES,cuatro);
	TestMensajeDAO::getById(&mDAO,id);
	id = trie->getIndice(MENSAJES,cinco);
	TestMensajeDAO::getById(&mDAO,id);
	id = trie->getIndice(MENSAJES,seis);
	TestMensajeDAO::getById(&mDAO,id);


	std::cout << "" << std::endl;
	TestMensajeDAO::getById(&mDAO,msj2->getID());
	TestMensajeDAO::borrar(&mDAO,msj2->getID());
	TestMensajeDAO::getById(&mDAO,msj2->getID());
	std::cout << "OJO. Debia dar NULL porque en el paso anterior se elimino." << std::endl;

	result = trie->deleteCadena(MENSAJES,msj2->getNombre());
	std::cout << "Borrado " << msj2->getID() << " - " << msj2->getNombre() << " = " << result << std::endl;
	id = trie->getIndice(MENSAJES,msj2->getNombre());
	std::cout << "getIndice del Borrado (debe ser 0) = " << id << std::endl;

	std::cout << "- - - That's All Folks! - - -" << std::endl;
	return 0;
}
