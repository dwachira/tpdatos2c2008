/*
 * Controller.h
 *
 *  Created on: 05-jun-2008
 *      Author: gsenno
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "../../business/directorios/DirectorioManager.h"
#include "../../business/mensajes/MensajeManager.h"
#include <string>
#include "../../dao/manager/ManagerDAO.h"
using namespace dao;

class Controller {
private:
	dao::ManagerDAO managerDao;
	dao::TrieDAO trieDao;
	business::DirectorioManager directorioManager;
	business::MensajeManager mensajeManager;
public:
	Controller() : trieDao(managerDao), directorioManager(managerDao,trieDao), mensajeManager(managerDao,directorioManager,trieDao) {
		trieDao.loadTrie(DIRECTORIOS);
		trieDao.loadTrie(MENSAJES);
		trieDao.loadTrie(IMAGENES);
	}

	void agregarMensaje(std::string filename);

	void agregarDirectorio(std::string& path);

	bool directorioEnUso(std::string path);

	void removerDirectorio(std::string path);

	void removerMensaje(std::string filename);

	void obtenerMensaje(std::string filename, std::string pathDestino);

	void mostrarDirectorios();

	virtual ~Controller();
};

#endif /* CONTROLLER_H_ */
