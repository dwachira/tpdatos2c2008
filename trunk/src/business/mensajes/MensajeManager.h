/*
 * MensajeManager.h
 *
 *  Created on: 28-jul-2008
 *      Author: gsenno
 */

#ifndef MENSAJEMANAGER_H_
#define MENSAJEMANAGER_H_

#include <string>
#include "../../object/Mensaje.h"
#include "../../dao/manager/ManagerDAO.h"

class MensajeManager {
private:
	ManagerDAO& managerDAO;
public:
	MensajeManager(ManagerDAO& manager) : managerDAO(manager) {}
	void agregarMensaje(std::string filename);
	void quitarMensaje(std::string filename);
	void quitarMensaje(Mensaje& mensaje);
	void obtenerMensaje(std::string filename);
	virtual ~MensajeManager();
};

#endif /* MENSAJEMANAGER_H_ */
