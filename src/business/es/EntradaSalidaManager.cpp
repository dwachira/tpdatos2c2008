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

bool EntradaSalidaManager::esUnDirectorio(std::string path) {
	struct stat fileStats;
	if ((lstat(path.data(),&fileStats) != -1)&&(S_ISDIR(fileStats.st_mode)))
		return true;
	return false;
}

EntradaSalidaManager::~EntradaSalidaManager() {
	// TODO Auto-generated destructor stub
}
