/*
 * EntidadInexistenteException.h
 *
 *  Created on: 05-jun-2008
 *      Author: gsenno
 */

#ifndef ENTIDADINEXISTENTEEXCEPTION_H_
#define ENTIDADINEXISTENTEEXCEPTION_H_

#include "Exception.h"

class EntidadInexistenteException: public Exception {
public:
	EntidadInexistenteException();
	virtual ~EntidadInexistenteException();
};

#endif /* ENTIDADINEXISTENTEEXCEPTION_H_ */
