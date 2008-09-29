/*
 * DirectoryManager.cpp
 *
 *  Created on: 07-jul-2008
 *      Author: gsenno
 */

#include "DirectoryManager.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <iostream>
using namespace business;
using namespace util;

namespace business {

DirectoryManager::DirectoryManager() {
	// TODO Auto-generated constructor stub

}

void DirectoryManager::addDirectory(std::string path) const
{
	//VERIFICAR SI EL DIRECTORIO EXISTE
	struct stat fileStats;
	Directory* aDirectory;
	int errorCode;
	if ((errorCode = lstat(path.data(),&fileStats)) != 0) {
//		error = new Error("DirectoryManager","addDirectory",errorCode);
//		return;
	}
	else {
		//CREAR DIRECTORIO CON FECHA DE ULTIMA MODIFICACION
		struct tm* timeAux = gmtime(&fileStats.st_mtim.tv_sec);
		Date* lastModification = Date::valueOf(timeAux->tm_mday, timeAux->tm_mon,
				timeAux->tm_year, timeAux->tm_hour, timeAux->tm_min);
		aDirectory = new Directory(path,*lastModification);
		findImages(*aDirectory);
		delete lastModification;
		//PERSISTIR DIRECTORIO
		//ESCANEAR DIRECTORIO EN BUSCA DE IMAGENES

	}
}

void DirectoryManager::findImages(const Directory& directory) const
{
	struct dirent* dirEntry;
	DIR* osDir = opendir(directory.getPath().data());
	while ( (dirEntry = readdir(osDir)) != NULL )
		if ( memcmp(dirEntry->d_name + strlen(dirEntry->d_name) - 3,"rar",3) == 0 )
			std::cout<< "es un rar" << std::endl;
}

DirectoryManager::~DirectoryManager() {
	// TODO Auto-generated destructor stub
}

}
