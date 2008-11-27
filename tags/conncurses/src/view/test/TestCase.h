/*
 * TestCase.h
 *
 *  Created on: 27-jul-2008
 *      Author: gsenno
 */

#ifndef TESTCASE_H_
#define TESTCASE_H_

#include <string>
#include <iostream>
#include <cstdlib>

class TestCase {
public:
	TestCase();
	int fail(std::string reason) {
		std::cout<<reason<<std::endl;
		exit(-1);
	}
	virtual int test() = 0;
	virtual ~TestCase();
};

#endif /* TESTCASE_H_ */
