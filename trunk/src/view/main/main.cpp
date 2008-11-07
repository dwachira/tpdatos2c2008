#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "PrimaryView.h"
#include "../test/md5/MD5Test.h"
#include "../test/blowfish/BlowfishTest.h"
#include "../test/TestAddDirectory.h"
#include "../test/TestAgregarYRecuperarMensaje.h"
#include "../test/TestCompressor.h"
#include "../test/dao/TestDirectorioDAO.h"
#include "../test/dao/TestImagenDAO.h"
#include "../test/dao/TestMensajeDAO.h"
#include "../test/dao/TestParticionDAO.h"
#include "../test/dao/TestTrieDAO.h"
#include <iostream>
#include <vector>
#include <fstream>

int main(int argc, char* argv[]){

	PrimaryView mainApp;
	mainApp.start();

//	std::vector<TestCase*> tests;
//	tests.push_back(new TestAgregarYRecuperarMensaje());
//	tests.push_back(new TestAddDirectory());
//	tests.push_back(new TestDirectorioDAO());
//	tests.push_back(new TestCompressor());

//	for (unsigned int i = 0; i < tests.size() ; i++)
//		tests[i]->test();


/*	TestDirectorioDAO* TestDir = new TestDirectorioDAO();
	TestDir->test();			//no correr los dos en simultaneo. La idea es
	//TestDir->test2();			//correr el primero, y despues el 2do

	TestImagenDAO* TestImg = new TestImagenDAO();
	TestImg->test();

	TestMensajeDAO* TestMsj = new TestMensajeDAO();
	TestMsj->test();

	TestParticionDAO* TestPart = new TestParticionDAO();
	TestPart->test();*/

/*	TestTrieDAO* TestTrie = new TestTrieDAO();
	TestTrie->test();
	//TestTrie->test2();
	//TestTrie->test3();*/

	/*MD5Test md5Test;
	md5Test.test();*/
	return 0;
}
