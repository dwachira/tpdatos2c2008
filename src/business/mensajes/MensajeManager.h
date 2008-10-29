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
#include "../compressor/CompressorBusiness.h"

namespace business {

class MensajeManager {
private:
	ImagenDAO& imagenDao;
	ParticionDAO& particionDao;
	MensajeDAO& mensajeDao;
	CompressorBusiness compressor;
	static std::string TMP_COMPRESSED_FILE_NAME;
public:
	MensajeManager(ManagerDAO& manager) : imagenDao(manager.getImagenDAO()), particionDao(manager.getParticionDAO()),
											mensajeDao(manager.getMensajeDAO()) {}
	void agregarMensaje(std::string filename);
	void quitarMensaje(std::string filename);
	void quitarMensaje(Mensaje& mensaje);
	void obtenerMensaje(std::string filename, std::string destino);
	virtual ~MensajeManager();
};
}
#endif /* MENSAJEMANAGER_H_ */
