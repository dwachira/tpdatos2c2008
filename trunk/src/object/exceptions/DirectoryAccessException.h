/*
 * DirectoryAccessException.h
 *
 *  Created on: 04-ago-2008
 *      Author: gsenno
 */

#ifndef DIRECTORYACCESSEXCEPTION_H_
#define DIRECTORYACCESSEXCEPTION_H_

#include "Exception.h"
#include <string>

class DirectoryAccessException: public Exception {
public:
	DirectoryAccessException() {}
	DirectoryAccessException(std::string descripcion);
	virtual ~DirectoryAccessException();
};

#endif /* DIRECTORYACCESSEXCEPTION_H_ */
