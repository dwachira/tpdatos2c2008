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

void DirectorioManager::accederDirectorio(std::string path) const {
	struct stat fileStats;
	if (lstat(path.data(),&fileStats) == -1) {
		if (errno == EACCES) {
			throw new DirectoryAccessException("No tienes permiso de lectura en el directorio");
		}
		else
			throw new DirectoryAccessException();
	}
}

DirectorioIteradorImagenes DirectorioManager::obtenerIteradorDeImagenes(Directorio& directorio) const{
	accederDirectorio(directorio.getPath());
	return DirectorioIteradorImagenes(directorio);
}

void DirectorioManager::agregarDirectorio(std::string path) const
{
	//TODO::Preguntar si el directorio ya existe en la base de datos.
	accederDirectorio(path);
	Directorio unDirectorio(path);
	directorioDAO.insert(unDirectorio);
	buscarImagenes(unDirectorio);		//ESCANEAR Directorio EN BUSCA DE IMAGENES

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
	struct stat dirStats;

	//VEO LA FECHA DE ULTIMA MODIFICACION DEL DIRECTORIO Y LA COMPARO CON LA ALMACENADA
	lstat(directorio.getPath().data(),&dirStats);
	struct tm* timeAux = gmtime(&dirStats.st_mtim.tv_sec);
	Date* lastModification = Date::valueOf(timeAux->tm_mday, timeAux->tm_mon + 1,
			timeAux->tm_year + 1900, timeAux->tm_hour, timeAux->tm_min);

	if ( directorio.getFechaUltimaModificacion() < *lastModification ) {
		DirectorioIteradorImagenes iterador = obtenerIteradorDeImagenes(directorio);
		struct stat fileStats;
		while (iterador.hasNext()) {
			string entryName = iterador.next();
			string fullFileName = directorio.getPath() + "/" + entryName;
			StegoBusiness* stego = StegoFactory::newInstance(fullFileName);
			if (stego != NULL) {
				lstat(fullFileName.data(),&fileStats);
				Imagen imagen(directorio.getID(),stego->getFreeSpace(),
							stego->getFirstFreeBit(),0,fileStats.st_size,fullFileName);
//				imagen.setTamanio(fileStats.st_size);
//				imagen.setNombre(fullFileName);
//				imagen.setID_Dir(directorio.getID());
//				imagen.setEspacio_libre(stego->getFreeSpace());
//				imagen.setProximo_bit_libre(stego->getFirstFreeBit());
				imagenDAO.insert(imagen);
				delete stego;
			}
		}
	directorio.setFechaUltimaModificacion(lastModification);
	//TODO::DAO.UPDATE DIRECTORIO !
	}
}

std::list<Directorio*> business::DirectorioManager::getDirectorios() const
{
	list<Directorio*> directorios;
//	for (int i = 1; i <= directorioDAO.getLastAssignedId(); i++)
	for (unsigned int i = 1; i <= object::Directorio::getLastAssignedId(); i++)
		directorios.push_back(directorioDAO.getDirById(i));
	return directorios;
}

DirectorioManager::~DirectorioManager() {
	// TODO Auto-generated destructor stub
}

}
