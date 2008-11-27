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
#include "../../dao/TrieDAO.h"
#include "../directorios/DirectorioManager.h"
#include "../compressor/CompressorBusiness.h"
#include "../md5/md5wrapper.h"

namespace business {

class MensajeManager {
private:
	dao::ImagenDAO& imagenDao;
	dao::ParticionDAO& particionDao;
	MensajeDAO& mensajeDao;
	DirectorioManager& directorioManager;
	dao::TrieDAO& trieDao;
	md5wrapper hasheador;
	string userPass;
	static std::string TMP_COMPRESSED_FILE_NAME;
	static char* testbuff;

public:
	MensajeManager(dao::ManagerDAO& manager, DirectorioManager& directorioMan, dao::TrieDAO& trie, std::string pass) : imagenDao(manager.getImagenDAO()), particionDao(manager.getParticionDAO()),
											mensajeDao(manager.getMensajeDAO()),
											directorioManager(directorioMan), trieDao(trie), userPass(pass){}
	/*@throw EspacioInsuficienteException, RecursoInaccesibleException*/
	void agregarMensaje(std::string filename);
	void quitarMensaje(const std::string& filename);
	void quitarMensajesEnDirectorio(std::string& dirpath);
	void quitarMensaje(Mensaje& mensaje);
	void obtenerMensaje(std::string filename, std::string destino);
	std::list<std::string> getMensajes();
	virtual ~MensajeManager();
};
}
#endif /* MENSAJEMANAGER_H_ */
