/*
 * EntradaSalidaManager.h
 *
 *  Created on: 08-ago-2008
 *      Author: gsenno
 */

#ifndef ENTRADASALIDAMANAGER_H_
#define ENTRADASALIDAMANAGER_H_

#include <string>

class EntradaSalidaManager {
public:
	EntradaSalidaManager();
	static bool recursoEsAccesible(std::string path);
	static bool esUnDirectorio(std::string path);
	virtual ~EntradaSalidaManager();
};

#endif /* ENTRADASALIDAMANAGER_H_ */
