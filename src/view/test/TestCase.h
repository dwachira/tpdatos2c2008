/*
 * TestCase.h
 *
 *  Created on: 27-jul-2008
 *      Author: gsenno
 */

#ifndef TESTCASE_H_
#define TESTCASE_H_

class TestCase {
public:
	TestCase();
	virtual int test() = 0;
	virtual ~TestCase();
};

#endif /* TESTCASE_H_ */
