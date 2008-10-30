/*
 * DirectorioManager.h
 *
 *  Created on: 07-jul-2008
 *      Author: gsenno
 */

#ifndef DIRECTORIOMANAGER_H_
#define DIRECTORIOMANAGER_H_
#include <string>
#include <list>
#include "../../object/Directorio.h"
#include "DirectorioIteradorImagenes.h"
#include "../../dao/manager/ManagerDAO.h"

namespace business {

class DirectorioManager {
private:

	dao::DirectorioDAO& directorioDAO;
	dao::ImagenDAO& imagenDAO;
	dao::ParticionDAO& particionDAO;
	void accederDirectorio(std::string path) const;
public:
	DirectorioManager(dao::ManagerDAO& manager) : directorioDAO(manager.getDirectorioDAO()),
		imagenDAO(manager.getImagenDAO()), particionDAO(manager.getParticionDAO()) {}

	/*@throws DirectoryAccessException */
	void agregarDirectorio(const std::string path) const;

	bool directorioEnUso(const Directorio& directory) const;

	void removerDirectorio(const long id) const;

	/*@throws DirectoryAccessException */
	DirectorioIteradorImagenes obtenerIteradorDeImagenes(Directorio& directorio) const;

	std::list<Directorio*> getDirectorios() const;

	void buscarImagenes(Directorio& directory) const;

	virtual ~DirectorioManager();
};

}

#endif /* DIRECTORIOMANAGER_H_ */
