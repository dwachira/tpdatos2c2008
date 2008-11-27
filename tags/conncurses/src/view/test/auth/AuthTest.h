/*
 * AuthTest.h
 *
 *  Created on: 11/11/2008
 *      Author: maxi
 */

#ifndef AUTHTEST_H_
#define AUTHTEST_H_

#include "../../../config/Config.h"
#include "../TestCase.h"
#include "../../../business/auth/AuthBusiness.h"

class AuthTest : public TestCase {
public:
	AuthTest();
	virtual ~AuthTest();
	int test();
};

#endif /* AUTHTEST_H_ */
