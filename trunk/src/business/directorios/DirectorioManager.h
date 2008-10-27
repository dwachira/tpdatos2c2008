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
#include "../../dao/manager/ManagerDAO.h"

namespace business {

class DirectorioManager {
private:
	static std::string GIF;
	static std::string JPG;
	static std::string PNG;
	static std::string BMP;
	static const int EXTENSION_LENGTH = 3;

	dao::DirectorioDAO& directorioDAO;
	dao::ImagenDAO& imagenDAO;
	dao::ParticionDAO& particionDAO;

public:
	DirectorioManager(dao::ManagerDAO& manager) : directorioDAO(manager.getDirectorioDAO()),
		imagenDAO(manager.getImagenDAO()), particionDAO(manager.getParticionDAO()) {}
	void agregarDirectorio(const std::string path) const;
	bool directorioEnUso(const Directorio& directory) const;
	void removerDirectorio(const long id) const;
	std::list<Directorio*> getDirectorios() const;
	void buscarImagenes(Directorio& directory) const;
	virtual ~DirectorioManager();
};

}

#endif /* DIRECTORIOMANAGER_H_ */
