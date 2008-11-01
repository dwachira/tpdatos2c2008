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

class Controller {
private:
	business::DirectorioManager& directorioManager;
	business::MensajeManager& mensajeManager;
public:
	Controller(business::DirectorioManager& dirMan, business::MensajeManager& menMan) :
		directorioManager(dirMan), mensajeManager(menMan) {}

	void agregarMensaje(std::string filename);

	void agregarDirectorio(std::string path);

	bool directorioEnUso(std::string path);

	void removerDirectorio(std::string path);

	void removerMensaje(std::string filename);

	void obtenerMensaje(std::string filename, std::string pathDestino);

	virtual ~Controller();
};

#endif /* CONTROLLER_H_ */
