/*
 * DirectorIOManager.cpp
 *
 *  Created on: 07-jul-2008
 *      Author: gsenno
 */

#include "DirectorioManager.h"
#include "../../dao/DirectorioDAO.h"
#include "../stego/StegoBusiness.h"
#include "../stego/StegoFactory.h"
#include "../../object/exceptions/DirectoryAccessException.h"
#include "../../util/string/StringUtils.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <iostream>

using namespace business;
using namespace dao;
using namespace util;
using namespace std;

namespace business {

string DirectorioManager::BMP = "BMP";
string DirectorioManager::GIF = "GIF";
string DirectorioManager::JPG = "JPG";
string DirectorioManager::PNG = "PNG";


void DirectorioManager::agregarDirectorio(std::string path) const
{
	//TODO::Preguntar si el directorio ya existe en la base de datos.
	struct stat fileStats;
	if (lstat(path.data(),&fileStats) == -1) {
		if (errno == EACCES) {
			throw new DirectoryAccessException("No tienes permiso de lectura en el directorio");
		}
		else
			throw new DirectoryAccessException();
	}
	else {
		Directorio unDirectorio(path);
		directorioDAO.insert(unDirectorio);
		buscarImagenes(unDirectorio);		//ESCANEAR Directorio EN BUSCA DE IMAGENES
	}
}

void DirectorioManager::removerDirectorio(long  id) const
{
	Directorio* directorio = directorioDAO.getDirById(id);
	if (directorio != NULL) {
		//dao.remove(Directorio);
	}
}

bool DirectorioManager::directorioEnUso(const Directorio & directory) const
{
	bool isBeingUsed = false;
	list<Imagen> imagenes = imagenDAO.getImgsByDirectorio(directory.getID());
	list<Imagen>::iterator it = imagenes.begin();
	while ( (it != imagenes.end()) && (!isBeingUsed) ) {
		const list<Particion>& particiones = particionDAO.getPartsByImg((*it).getID());
		if (particiones.size() > 0)
			isBeingUsed = true;
	}
	return isBeingUsed;
}

void DirectorioManager::buscarImagenes(Directorio& directorio) const
{
	struct dirent* dirEntry;
	DIR* osDir = opendir(directorio.getPath().data());
	if (osDir == NULL)
		return; //TODO:: VER QUE HACER EN ESTE CASO
	struct stat dirStats;

	//VEO LA FECHA DE ULTIMA MODIFICACION DEL DIRECTORIO Y LA COMPARO CON LA ALMACENADA
	lstat(directorio.getPath().data(),&dirStats);
	struct tm* timeAux = gmtime(&dirStats.st_mtim.tv_sec);
	Date* lastModification = Date::valueOf(timeAux->tm_mday, timeAux->tm_mon + 1,
			timeAux->tm_year + 1900, timeAux->tm_hour, timeAux->tm_min);

	if ( directorio.getFechaUltimaModificacion() < *lastModification ) {
		struct stat fileStats;
		while ( (dirEntry = readdir(osDir)) != NULL ) {
			string entryName(dirEntry->d_name);
			if (entryName.size() > EXTENSION_LENGTH) {
				string extension = StringUtils::uppercase(entryName.substr(entryName.size() - EXTENSION_LENGTH));
				if ( (extension.compare(BMP) == 0) || (extension.compare(GIF) == 0) || (extension.compare(JPG) == 0) || (extension.compare(PNG) == 0) ) {
					string fullFileName = directorio.getPath() + "/" + entryName;
					StegoBusiness* stego = StegoFactory::newInstance(fullFileName);
					if (stego != NULL) {
						lstat(fullFileName.data(),&fileStats);
						Imagen imagen;
						imagen.setTamanio(fileStats.st_size);
						imagen.setNombre(fullFileName);
						imagen.setID_Dir(directorio.getID());
						imagen.setEspacio_libre(stego->getFreeSpace());
						imagen.setProximo_bit_libre(stego->getFirstFreeBit());
						imagenDAO.insert(imagen);
						delete stego;
					}
				}
			}

		}
		directorio.setFechaUltimaModificacion(lastModification);
		//TODO::DAO.UPDATE DIRECTORIO !
	}
}

std::list<Directorio*> business::DirectorioManager::getDirectorios() const
{
	list<Directorio*> directorios;
	for (int i = 1; i <= directorioDAO.getLastAssignedId(); i++)
		directorios.push_back(directorioDAO.getDirById(i));
	return directorios;
}

DirectorioManager::~DirectorioManager() {
	// TODO Auto-generated destructor stub
}

}
