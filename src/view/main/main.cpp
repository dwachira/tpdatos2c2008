#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../test/blowfish/BlowfishTest.h"
#include "../test/TestAddDirectory.h"
#include "../test/TestAgregarYRecuperarMensaje.h"
#include "../test/TestCompressor.h"
#include <iostream>
#include <vector>
#include <fstream>

int main(int argc, char* argv[]){

	std::vector<TestCase*> tests;
	tests.push_back(new TestAddDirectory());
	tests.push_back(new TestAgregarYRecuperarMensaje());

	for (int i = 0; i < sizeof(tests) ; i++)
		tests[i]->test();

/*	BlowfishTest bt;
	bt.test();*/
	return 0;
}


