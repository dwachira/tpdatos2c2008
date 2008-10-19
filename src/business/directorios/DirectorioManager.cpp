/*
 * DirectorIOManager.cpp
 *
 *  Created on: 07-jul-2008
 *      Author: gsenno
 */

#include "DirectorioManager.h"
#include "../../dao/DirectorioDAO.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <iostream>

using namespace business;
using namespace dao;
using namespace util;
using namespace std;

namespace business {

string DirectorioManager::GIF = "gif";
string DirectorioManager::JPG = "jpg";
string DirectorioManager::PNG = "png";


void DirectorioManager::agregarDirectorio(std::string path) const
{

	//if (PersistenceService.getDirectorioByPath(path) == NULL) {
		struct stat fileStats;
		int errorCode;
		if ((errorCode = lstat(path.data(),&fileStats)) != 0) {
	//		error = new Error("DirectorioManager","addDirectorio",errorCode);
	//		return;
		}
		else {
			//CREAR Directorio CON FECHA DE ULTIMA MODIFICACION
			struct tm* timeAux = gmtime(&fileStats.st_mtim.tv_sec);
			Date* lastModification = Date::valueOf(timeAux->tm_mday, timeAux->tm_mon + 1,
					timeAux->tm_year + 1900, timeAux->tm_hour, timeAux->tm_min);
			Directorio unDirectorio(path,*lastModification);
			unDirectorio.setID(5);
			managerDAO.getDirectorioDAO()->insert(unDirectorio);
			buscarImagenes(unDirectorio);		//ESCANEAR Directorio EN BUSCA DE IMAGENES
			delete lastModification;

		}
	//}
}

void DirectorioManager::removerDirectorio(long  id) const
{
	DirectorioDAO* dao = managerDAO.getDirectorioDAO();
	Directorio* directorio = dao->getDirById(id);
	if (directorio != NULL) {
		//dao.remove(Directorio);
	}
}

bool DirectorioManager::directorioEnUso(const Directorio & directory) const
{
//	bool isBeingUsed = false;
//	list<Image>& images = PersistenceService.getImagesByDirectorio(directory);
//	list<Image>::iterator it = images.begin();
//	while ( (it != images.end()) && (!isBeingUsed) ) {
//		list<Partition>& particiones = PersistenceService.getPartitionsByImage(*it);
//		if (particiones.size() > 0)
//			isBeingUsed = true;
//	}
//	return isBeingUsed;
}

void DirectorioManager::buscarImagenes(const Directorio& directorio) const
{
	struct dirent* dirEntry;
	DIR* osDir = opendir(directorio.getPath().data());
	struct stat fileStats;
	while ( (dirEntry = readdir(osDir)) != NULL ) {
		string entryName(dirEntry->d_name);
		if (entryName.size() > EXTENSION_LENGTH) {
			string extension = entryName.substr(entryName.size() - EXTENSION_LENGTH);
			if ( (extension.compare("rar") == 0) || (extension.compare(JPG) == 0) || (extension.compare(PNG) == 0) ) {
				string fullFileName = directorio.getPath() + "/" + entryName;
				lstat(fullFileName.data(),&fileStats);
				Imagen imagen;
				imagen.setTamanio(fileStats.st_size);
				imagen.setNombre(fullFileName);
				imagen.setID_Dir(directorio.getID());
				managerDAO.getImagenDAO()->insert(imagen);
//				Stego.setFreeSpace(anImage);
//				PersistenceManager.save(anImage);
			}
		}

	}
}

std::list<Directorio*> business::DirectorioManager::getDirectorios() const
{
	list<Directorio*> directorios;
	directorios.push_front(managerDAO.getDirectorioDAO()->getDirById(5));
	return directorios;
}

DirectorioManager::~DirectorioManager() {
	// TODO Auto-generated destructor stub
}

}
