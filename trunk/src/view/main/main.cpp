#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../test/blowfish/BlowfishTest.h"
#include "../test/TestAddDirectory.h"
#include "../test/TestAgregarYRecuperarMensaje.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]){

	TestCase* tests[] = {new TestAgregarYRecuperarMensaje() };
	for (int i = 0; i < 1 ; i++)
		tests[i]->test();

/*	BlowfishTest bt;
	bt.test();*/
	return 0;
}


