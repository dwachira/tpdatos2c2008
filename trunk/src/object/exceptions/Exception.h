/*
 * Exception.h
 *
 *  Created on: 22-jul-2008
 *      Author: gsenno
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <string>

class Exception {
private:
	std::string description;
public:
	Exception();
	Exception(const std::string& description);
	virtual ~Exception();
};

#endif /* EXCEPTION_H_ */
