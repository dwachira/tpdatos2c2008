/*
 * EspacioInsuficienteException.h
 *
 *  Created on: 05-jun-2008
 *      Author: gsenno
 */

#ifndef ESPACIOINSUFICIENTEEXCEPTION_H_
#define ESPACIOINSUFICIENTEEXCEPTION_H_

#include "Exception.h"

class EspacioInsuficienteException: public Exception {
public:
	EspacioInsuficienteException();
	virtual ~EspacioInsuficienteException();
};

#endif /* ESPACIOINSUFICIENTEEXCEPTION_H_ */
