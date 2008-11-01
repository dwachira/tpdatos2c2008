#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../test/blowfish/BlowfishTest.h"
#include "../test/TestAddDirectory.h"
#include "../test/TestAgregarYRecuperarMensaje.h"
#include "../test/TestCompressor.h"
#include "../test/dao/TestDirectorioDAO.h"
#include "../test/dao/TestImagenDAO.h"
#include "../test/dao/TestMensajeDAO.h"
#include "../test/dao/TestParticionDAO.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]){
	std::cout << "Elegir un test en src/view/main :P" << std::endl;
/*	TestCase* tests[] = {new TestAgregarYRecuperarMensaje() };
	for (int i = 0; i < 1 ; i++)
		tests[i]->test();
*/
/*	BlowfishTest bt;
	bt.test();
*/
//	TestDirectorioDAO* TestDir = new TestDirectorioDAO();
//	TestDir->test();			//no correr los dos en simultaneo. La idea es
//	TestDir->test2();			//correr el primero, y despues el 2do

//	TestImagenDAO* TestImg = new TestImagenDAO();
//	TestImg->test();

//	TestMensajeDAO* TestMsj = new TestMensajeDAO();
//	TestMsj->test();

//	TestParticionDAO* TestPart = new TestParticionDAO();
//	TestPart->test();


	return 0;
}


