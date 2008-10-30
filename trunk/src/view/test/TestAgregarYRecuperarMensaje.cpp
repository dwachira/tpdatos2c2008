/*
 * TestAgregarYRecuperarMensaje.cpp
 *
 *  Created on: 03-ago-2008
 *      Author: gsenno
 */

#include "TestAgregarYRecuperarMensaje.h"
#include "../../dao/manager/ManagerDAO.h"
#include "../../business/directorios/DirectorioManager.h"
#include "../../business/mensajes/MensajeManager.h"

TestAgregarYRecuperarMensaje::TestAgregarYRecuperarMensaje() {
	// TODO Auto-generated constructor stub

}

int TestAgregarYRecuperarMensaje::test() {
	dao::ManagerDAO managerDAO;
	business::DirectorioManager directoryManager(managerDAO);
	business::MensajeManager mensajeManager(managerDAO);
	directoryManager.agregarDirectorio(__TEST_DIR__"");

	mensajeManager.agregarMensaje(__TEST_DIR__"/prueba_entrada.txt");
	mensajeManager.obtenerMensaje(__TEST_DIR__"/prueba_entrada.txt",__TEST_DIR__"/prueba_salida.txt");
	return 0;
}

TestAgregarYRecuperarMensaje::~TestAgregarYRecuperarMensaje() {
	// TODO Auto-generated destructor stub
}
