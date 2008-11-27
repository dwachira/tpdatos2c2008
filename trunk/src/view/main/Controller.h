/*
 * Controller.h
 *
 *  Created on: 05-jun-2008
 *      Author: gsenno
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#define MAX_LINE 250
#define REMOVERDIRECTORIO "  El directorio que desea remover contiene imagenes que contienen texto guardado.\n  ¿Desea eliminarlo? (Se eliminaran los mensajes que en él estaban guardados)(s/n)."
#define OBTENERMSG "  No se ha podido obtener el mensaje deseado ya que hay un archivo faltante.\n  ¿Desea eliminar las otras partes del mensaje? (s/n)."
#define MIN_PASS_LENGTH 6
#define MAX_PASS_LENGTH 15

#include "../../business/directorios/DirectorioManager.h"
#include "../../business/mensajes/MensajeManager.h"
#include <string>
#include "../../dao/manager/ManagerDAO.h"
#include "../../business/auth/AuthBusiness.h"
#include "../../dao/TrieDAO.h"
using namespace business;
using namespace dao;

class Controller {
private:
	ManagerDAO* managerDao;
	TrieDAO* trieDao;
	DirectorioManager* directorioManager;
	MensajeManager* mensajeManager;
	AuthBusiness* authBusiness;
	bool loggedIn;
	bool confirmar(string pregunta);

public:

	Controller() : loggedIn(false) {
		authBusiness = new AuthBusiness(PACKAGE_DATA_DIR);
	}

	void agregarMensaje(std::string& filename);

	void agregarDirectorio(std::string& path);

	void removerDirectorio(std::string& path);

	void removerMensaje(std::string& filename);

	void obtenerMensaje(std::string& filename, std::string& pathDestino);

	void mostrarDirectorios();

	void mostrarMensajes();

	bool login(std::string& password);

	void changePassword(std::string& oldPassword, std::string& newPassword);

	bool validatePassword(const std::string& password);

	virtual ~Controller();
};

#endif /* CONTROLLER_H_ */
