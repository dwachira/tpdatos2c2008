/*
 * RecursoInaccesibleException.h
 *
 *  Created on: 05-jun-2008
 *      Author: gsenno
 */

#ifndef RECURSOINACCESIBLEEXCEPTION_H_
#define RECURSOINACCESIBLEEXCEPTION_H_

#include "Exception.h"

class RecursoInaccesibleException: public Exception {
public:
	RecursoInaccesibleException();
	virtual ~RecursoInaccesibleException();
};

#endif /* RECURSOINACCESIBLEEXCEPTION_H_ */
