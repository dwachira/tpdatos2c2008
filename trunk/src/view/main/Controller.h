/*
 * Controller.h
 *
 *  Created on: 05-jun-2008
 *      Author: gsenno
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#define MAX_LINE 250
#define REMOVERDIRECTORIO "El directorio que desea remover contiene imagenes que contienen texto guardado.\n¿Desea eliminarlo? (s/n)."
#define OBTENERMSG "No se ah podido obtener el mensaje deseado ya que hay un archivo faltante.\n¿Desea eliminar las otras partes del mensaje? (s/n)."

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
	
	bool confirmar(string pregunta);
	
public:
	Controller() : trieDao(managerDao), directorioManager(managerDao,trieDao), mensajeManager(managerDao,directorioManager,trieDao) {
		trieDao.loadTrie(DIRECTORIOS);
		trieDao.loadTrie(MENSAJES);
		trieDao.loadTrie(IMAGENES);
	}

	void agregarMensaje(std::string& filename);

	void agregarDirectorio(std::string& path);

	bool directorioEnUso(std::string& path);

	void removerDirectorio(std::string& path);

	void removerMensaje(std::string& filename);

	void obtenerMensaje(std::string& filename, std::string& pathDestino);

	void mostrarDirectorios();

	void mostrarMensajes();
	
	bool login(std::string& password);

	virtual ~Controller();
};

#endif /* CONTROLLER_H_ */
