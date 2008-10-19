/*
 * TestAddDirectory.h
 *
 *  Created on: 27-jul-2008
 *      Author: gsenno
 */

#ifndef TESTADDDIRECTORY_H_
#define TESTADDDIRECTORY_H_

#include "TestCase.h"
#include "../../business/directorios/DirectorioManager.h"
#include "../../dao/manager/ManagerDAO.h"

class TestAddDirectory: public TestCase {
public:
	TestAddDirectory();
	int test();
	virtual ~TestAddDirectory();
};

#endif /* TESTADDDIRECTORY_H_ */
