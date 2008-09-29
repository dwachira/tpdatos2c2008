#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "../../business/directories/DirectoryManager.h"
using namespace business;


int main(int argc, char* argv[]){

	DirectoryManager manager;
	manager.addDirectory("/home/gsenno/Documents");
	return 0;
}


