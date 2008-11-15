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

bool compararDirectoriosPorPath(Directorio& iz, Directorio& der) {
	return (iz.getPath() < der.getPath());
}

string transformDirectorioToString(Directorio& dir) {
	return dir.getPath();
}

DirectorioIteradorImagenes DirectorioManager::obtenerIteradorDeImagenes(Directorio& directorio) const{
	if (EntradaSalidaManager::recursoEsAccesible(directorio.getPath()))
		return DirectorioIteradorImagenes(directorio);
	else
		throw RecursoInaccesibleException();
}

void DirectorioManager::agregarDirectorio(const std::string& path)
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
			throw RecursoInaccesibleException();
		}
	}
	else
		throw EntidadYaExistenteException();

}

void DirectorioManager::removerDirectorio(string& path) const
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
		throw EntidadInexistenteException();
}

bool DirectorioManager::directorioEnUso(string& path) const
{
	unsigned int dirId = trieDao.getIndice(DIRECTORIOS,path);
	//TODO:: FIX. UN DIRECTORIO QUE NO ESTA EN USO PUEDE TENER IMAGENES CON PARTICIONES.
	if (dirId != 0) {
		Directorio* directory = directorioDAO.getDirById(dirId);
		bool isBeingUsed = false;
		list<Imagen> imagenes = imagenDAO.getImgsByDirectorio(directory->getID());
		list<Imagen>::iterator it = imagenes.begin();
		while ( (it != imagenes.end()) && (!isBeingUsed) ) {
			const list<Particion>& particiones = particionDAO.getPartsByImg((*it).getID());
			if (particiones.size() > 0)
				isBeingUsed = true;
			it++;
		}
		delete directory;
		return isBeingUsed;
	}
	else
		throw EntidadInexistenteException();
}

bool DirectorioManager::agregarImagenEnDirectorio(Directorio& directorio, Imagen& imagen) const {
	StegoBusiness* stego = StegoFactory::newInstance(imagen.getNombre());
	struct stat fileStats;
	if (stego != NULL) {
		lstat(imagen.getNombre().data(),&fileStats);
		imagen.setID_Dir(directorio.getID());
		imagen.setEspacio_libre(stego->getFreeSpace());
		std::cout<<imagen.getEspacio_libre()<<std::endl;
		imagen.setProximo_bit_libre(stego->getFirstFreeBit());
		imagen.setTamanio(fileStats.st_size);
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
		Imagen imagen(entryName);
		imagen.setFechaUltimaModificacion(new Date());
		agregarImagenEnDirectorio(directorio,imagen);
	}
}

Imagen* DirectorioManager::buscarImagenMovida(const Imagen& imagen) const
{
	list<Directorio> directorios = directorioDAO.getDirsSortedByFechaModif();
	for(list<Directorio>::iterator it = directorios.begin(); it != directorios.end(); it++) {
		Directorio& directorio = *it;
		if (imagen.getFechaUltimaModificacion() < *getFechaModificacionActual(directorio)) {
			DirectorioIteradorImagenes iterador = obtenerIteradorDeImagenes(directorio);
			while (iterador.hasNext()) {
				string entryName = iterador.next();
				string hash = hasheador.getHashFromFile(entryName);
				if (imagen.getHash_value().compare(hash) == 0) {
					Imagen* nuevaImagen = new Imagen(imagen);
					nuevaImagen->setNombre(entryName);
					nuevaImagen->setID_Dir(directorio.getID());
					return nuevaImagen;
				}
			}
		}
	}
	return NULL;
}

std::list<string> business::DirectorioManager::getDirectorios() const
{
	list<Directorio> directorios = directorioDAO.getDirsSortedByFechaModif();
	directorios.sort(compararDirectoriosPorPath);
	list<string> nombres;
	nombres.resize(directorios.size());
	transform(directorios.begin(),directorios.end(),nombres.begin(),transformDirectorioToString);
	return nombres;
}

util::Date* DirectorioManager::getFechaModificacionActual(Directorio& directorio) const
{
	if (EntradaSalidaManager::recursoEsAccesible(directorio.getPath())) {
		struct stat dirStats;

		//VEO LA FECHA DE ULTIMA MODIFICACION DEL DIRECTORIO Y LA COMPARO CON LA ALMACENADA
		lstat(directorio.getPath().data(),&dirStats);
		struct tm* timeAux = localtime(&dirStats.st_mtim.tv_sec);
		Date* lastModification = Date::valueOf(timeAux->tm_mday, timeAux->tm_mon + 1,
				timeAux->tm_year + 1900, timeAux->tm_hour, timeAux->tm_min, timeAux->tm_sec);

		return lastModification;
	}
	return NULL;
}
void DirectorioManager::actualizarFechaDeModificacion(Directorio & directorio) const
{
		directorio.setFechaUltimaModificacion(getFechaModificacionActual(directorio));
		directorioDAO.update(directorio.getID(),directorio.getFechaUltimaModificacion());
}

DirectorioManager::~DirectorioManager() {
	// TODO Auto-generated destructor stub
}

}
