/*
 * DirectorioIteradorImagenes.h
 *	Esta clase recibe un Directorio e itera por todas las entradas devolviendo
 *	las que sean imagenes con las llamadas a next().
 *
 *  Created on: 07-ago-2008
 *      Author: gsenno
 */

#ifndef DIRECTORIOITERADORIMAGENES_H_
#define DIRECTORIOITERADORIMAGENES_H_

#include "../../object/Directorio.h"
#include <dirent.h>
#include <string>

namespace business {

class DirectorioIteradorImagenes {
private:
	DIR* osDir;
	struct dirent* nextDirEntry;
	std::string directoryPath;
	static std::string GIF;
	static std::string JPG;
	static std::string PNG;
	static std::string BMP;
	static const unsigned int EXTENSION_LENGTH = 3;
	void obtenerSiguienteImagen();
public:
	DirectorioIteradorImagenes(object::Directorio& dir);
	std::string next();
	bool hasNext() const;
	virtual ~DirectorioIteradorImagenes();
};
}
#endif /* DIRECTORIOITERADORIMAGENES_H_ */
