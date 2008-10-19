/*
 * TestAddDirectory.cpp
 *
 *  Created on: 27-jul-2008
 *      Author: gsenno
 */

#include "TestAddDirectory.h"
#include <cassert>
#include <list>

TestAddDirectory::TestAddDirectory() {
	// TODO Auto-generated constructor stub

}

int TestAddDirectory::test() {
	dao::ManagerDAO managerDAO;
	business::DirectorioManager manager(managerDAO);
	manager.agregarDirectorio("/home/gsenno/Documents");
	list<Directorio*> directorios = manager.getDirectorios();
	assert(directorios.front()->getPath() == "/home/gsenno/Documents");
	std::list<Imagen> imagenes = managerDAO.getImagenDAO()->getImgsByDirectorio(5);
	for (std::list<Imagen>::iterator it = imagenes.begin(); it != imagenes.end(); it++)
		std::cout<<(*it).getNombre()<<std::endl;
}

TestAddDirectory::~TestAddDirectory() {
	// TODO Auto-generated destructor stub
}
