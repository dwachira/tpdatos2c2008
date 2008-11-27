/*
 * TestAddDirectory.cpp
 *
 *  Created on: 27-jul-2008
 *      Author: gsenno
 */

#include "TestAddDirectory.h"
#include <list>
#include <string>
using namespace std;

TestAddDirectory::TestAddDirectory() {
	// TODO Auto-generated constructor stub

}

int TestAddDirectory::test() {
	dao::ManagerDAO managerDAO;
	dao::TrieDAO trieDao(managerDAO);
	business::DirectorioManager manager(managerDAO,trieDao);
	string expectedDir(__TEST_DIR__"");

	//Unit under test
	manager.agregarDirectorio(expectedDir);

	list<string> directorios = manager.getDirectorios();
	if (expectedDir.compare(directorios.front()) != 0) {
		fail("El directorio almacenado no coincide con el ingresado");
	}

	std::list<Imagen> imagenes = managerDAO.getImagenDAO().
		getImgsByDirectorio(trieDao.getIndice(DIRECTORIOS,expectedDir));

	for (std::list<Imagen>::iterator it = imagenes.begin(); it != imagenes.end(); it++)
		std::cout<<(*it).getNombre()<<std::endl;
	return 0;
}

TestAddDirectory::~TestAddDirectory() {
	// TODO Auto-generated destructor stub
}
