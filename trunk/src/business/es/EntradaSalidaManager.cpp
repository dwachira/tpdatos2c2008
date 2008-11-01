/*
 * EntradaSalidaManager.cpp
 *
 *  Created on: 08-ago-2008
 *      Author: gsenno
 */

#include "EntradaSalidaManager.h"
#include <sys/stat.h>
#include <sys/types.h>

EntradaSalidaManager::EntradaSalidaManager() {
	// TODO Auto-generated constructor stub

}

bool EntradaSalidaManager::recursoEsAccesible(std::string path) {
	struct stat fileStats;
	if (lstat(path.data(),&fileStats) == -1)
		return false;
	return true;
}

EntradaSalidaManager::~EntradaSalidaManager() {
	// TODO Auto-generated destructor stub
}
