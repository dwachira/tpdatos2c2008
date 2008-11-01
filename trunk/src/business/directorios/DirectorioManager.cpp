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
#include "../../object/exceptions/RecursoInaccesibleException.h"
#include "../../object/exceptions/EntidadYaExistenteException.h"
#include "../../object/exceptions/EntidadInexistenteException.h"
#include "../es/EntradaSalidaManager.h"
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

DirectorioIteradorImagenes DirectorioManager::obtenerIteradorDeImagenes(Directorio& directorio) const{
	if (EntradaSalidaManager::recursoEsAccesible(directorio.getPath()))
		return DirectorioIteradorImagenes(directorio);
	else
		throw new RecursoInaccesibleException();
}

void DirectorioManager::agregarDirectorio(string path)
{

	if (trieDao.getIndice(DIRECTORIOS,path) == 0) {
		if (EntradaSalidaManager::recursoEsAccesible(path)) {
			Directorio unDirectorio(path);
			actualizarFechaDeModificacion(unDirectorio);
			directorioDAO.insert(unDirectorio);
			trieDao.insertCadena(DIRECTORIOS,path,unDirectorio.getID());
			buscarImagenes(unDirectorio);
		}
		else {
			throw new RecursoInaccesibleException();
		}
	}
	else
		throw new EntidadYaExistenteException();

}

void DirectorioManager::removerDirectorio(string  path) const
{
	unsigned int dirId = trieDao.getIndice(DIRECTORIOS,path);

	if ( dirId != 0 ) {
		list<Imagen> imagenesEnDir = imagenDAO.getImgsByDirectorio(dirId);
		for (list<Imagen>::iterator it = imagenesEnDir.begin(); it != imagenesEnDir.end(); it++) {
			Imagen& imagen = *it;
			list<Particion> particionesPorImagen = particionDAO.getPartsByImg(imagen.getID());
			for (list<Particion>::iterator it2 = particionesPorImagen.begin(); it2 != particionesPorImagen.end(); it2++) {
				particionDAO.borrar(*it2);
			}
			imagenDAO.borrar(imagen);
			trieDao.deleteCadena(IMAGENES,imagen.getNombre());
		}
		directorioDAO.borrar(dirId);
		trieDao.deleteCadena(DIRECTORIOS,path);
	}
	else
		throw new EntidadInexistenteException();
}

bool DirectorioManager::directorioEnUso(string path) const
{
	unsigned int dirId = trieDao.getIndice(DIRECTORIOS,path);

	if (dirId != 0) {
		Directorio* directory = directorioDAO.getDirById(dirId);
		bool isBeingUsed = false;
		list<Imagen> imagenes = imagenDAO.getImgsByDirectorio(directory->getID());
		list<Imagen>::iterator it = imagenes.begin();
		while ( (it != imagenes.end()) && (!isBeingUsed) ) {
			const list<Particion>& particiones = particionDAO.getPartsByImg((*it).getID());
			if (particiones.size() > 0)
				isBeingUsed = true;
		}
		delete directory;
		return isBeingUsed;
	}
	else
		throw new EntidadInexistenteException();
}

bool DirectorioManager::agregarImagenEnDirectorio(Directorio& directorio, string filename) const {
	StegoBusiness* stego = StegoFactory::newInstance(filename);
	struct stat fileStats;
	if (stego != NULL) {
		lstat(filename.data(),&fileStats);
		Imagen imagen(directorio.getID(),stego->getFreeSpace(),stego->getFirstFreeBit(),
				0,fileStats.st_size,filename);
		imagenDAO.insert(imagen);
		trieDao.insertCadena(IMAGENES,imagen.getNombre(),imagen.getID());
		delete stego;
		return true;
	}
	else
		return false;
}

void DirectorioManager::buscarImagenes(Directorio& directorio) const
{
	DirectorioIteradorImagenes iterador = obtenerIteradorDeImagenes(directorio);

	while (iterador.hasNext()) {
		string entryName = iterador.next();
		string fullFileName = directorio.getPath() + "/" + entryName;
		agregarImagenEnDirectorio(directorio,fullFileName);
	}
}

std::list<Directorio*> business::DirectorioManager::getDirectorios() const
{
	list<Directorio*> directorios;
	for (unsigned int i = 1; i <= object::Directorio::getLastAssignedId(); i++)
		directorios.push_back(directorioDAO.getDirById(i));
	return directorios;
}

void DirectorioManager::actualizarFechaDeModificacion(Directorio & directorio)
{
	if (EntradaSalidaManager::recursoEsAccesible(directorio.getPath())) {
		struct stat dirStats;

		//VEO LA FECHA DE ULTIMA MODIFICACION DEL DIRECTORIO Y LA COMPARO CON LA ALMACENADA
		lstat(directorio.getPath().data(),&dirStats);
		struct tm* timeAux = gmtime(&dirStats.st_mtim.tv_sec);
		Date* lastModification = Date::valueOf(timeAux->tm_mday, timeAux->tm_mon + 1,
				timeAux->tm_year + 1900, timeAux->tm_hour, timeAux->tm_min);
		directorio.setFechaUltimaModificacion(lastModification);
		directorioDAO.update(directorio.getID(),directorio.getFechaUltimaModificacion());
	}
}

DirectorioManager::~DirectorioManager() {
	// TODO Auto-generated destructor stub
}

}
