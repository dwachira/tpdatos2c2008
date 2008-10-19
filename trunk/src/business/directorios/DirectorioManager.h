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
	static const int EXTENSION_LENGTH = 3;

	dao::ManagerDAO& managerDAO;
public:
	DirectorioManager(dao::ManagerDAO& manager) : managerDAO(manager) {}
	void agregarDirectorio(const std::string path) const;
	bool directorioEnUso(const Directorio& directory) const;
	void removerDirectorio(const long id) const;
	std::list<Directorio*> getDirectorios() const;
	void buscarImagenes(const Directorio& directory) const;
	virtual ~DirectorioManager();
};

}

#endif /* DIRECTORIOMANAGER_H_ */
