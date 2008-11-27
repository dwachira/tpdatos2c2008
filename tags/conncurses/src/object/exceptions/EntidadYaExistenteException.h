/*
 * EntidadYaExistenteException.h
 *
 *  Created on: 05-jun-2008
 *      Author: gsenno
 */

#ifndef ENTIDADYAEXISTENTEEXCEPTION_H_
#define ENTIDADYAEXISTENTEEXCEPTION_H_

#include "Exception.h"

class EntidadYaExistenteException: public Exception {
public:
	EntidadYaExistenteException();
	virtual ~EntidadYaExistenteException();
};

#endif /* ENTIDADYAEXISTENTEEXCEPTION_H_ */
