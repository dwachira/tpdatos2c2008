/*
 * BlowfishTest.h
 *
 *  Created on: 17/10/2008
 *      Author: maxi
 */

#ifndef BLOWFISHTEST_H_
#define BLOWFISHTEST_H_

#include "../../../config/Config.h"
#include "../../../business/crypto/blowfish/BlowfishCrypto.h"
class BlowfishTest {
public:
	BlowfishTest();
	virtual ~BlowfishTest();
	int test();
};

#endif /* BLOWFISHTEST_H_ */
