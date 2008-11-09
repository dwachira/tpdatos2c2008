/*
 * ImagenFaltanteException.h
 *
 *  Created on: 08/11/2008
 *      Author: gsenno
 */

#ifndef IMAGENFALTANTEEXCEPTION_H_
#define IMAGENFALTANTEEXCEPTION_H_

#include "Exception.h"

class ImagenFaltanteException: public Exception {
public:
	ImagenFaltanteException();
	virtual ~ImagenFaltanteException();
};

#endif /* IMAGENFALTANTEEXCEPTION_H_ */
