/*
 * MD5Test.h
 *
 *  Created on: 03/11/2008
 *      Author: maxi
 */

#ifndef MD5TEST_H_
#define MD5TEST_H_


#include "../../../config/Config.h"
#include "../TestCase.h"
#include "../../../business/md5/md5wrapper.h"

class MD5Test : public TestCase {
public:
	MD5Test();
	virtual ~MD5Test();
	int test();
};

#endif /* MD5TEST_H_ */
