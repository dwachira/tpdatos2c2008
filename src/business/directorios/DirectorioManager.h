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
#include "../../object/Imagen.h"
#include "DirectorioIteradorImagenes.h"
#include "../../dao/manager/ManagerDAO.h"
#include "../../dao/TrieDAO.h"

namespace business {

class DirectorioManager {
private:

	dao::DirectorioDAO& directorioDAO;
	dao::ImagenDAO& imagenDAO;
	dao::ParticionDAO& particionDAO;
	dao::TrieDAO& trieDao;
	void accederDirectorio(std::string path) const;
public:
	DirectorioManager(dao::ManagerDAO& manager, dao::TrieDAO& trie) : directorioDAO(manager.getDirectorioDAO()),
		imagenDAO(manager.getImagenDAO()), particionDAO(manager.getParticionDAO()), trieDao(trie) {}

	/*@throws RecursoInaccesibleException, EntidadYaExistenteException */
	void agregarDirectorio(const std::string path);

	/**Agrega la imagen en la base de datos con todos los datos necesarios**/
	/**Devuelve true si pudo y false si no.**/
	bool agregarImagenEnDirectorio(Directorio& directorio, Imagen& filename) const;

	/*@throws RecursoInexistenteException*/
	bool directorioEnUso(std::string path) const;

	/*@throws DirectoryAccessException */
	void actualizarFechaDeModificacion(Directorio& directorio);

	/**
	 * Remueve el directorio y sus respectivas imagenes asociadas de la base de datos.
	 * @throws RecursoInaccesibleException
	 */
	void removerDirectorio(std::string path) const;

	DirectorioDAO& getDirectorioDao() const {
		return directorioDAO;
	}

	/*@throws RecursoInaccesibleException */
	DirectorioIteradorImagenes obtenerIteradorDeImagenes(Directorio& directorio) const;

	std::list<Directorio*> getDirectorios() const;

	void buscarImagenes(Directorio& directory) const;

	virtual ~DirectorioManager();
};

}

#endif /* DIRECTORIOMANAGER_H_ */
