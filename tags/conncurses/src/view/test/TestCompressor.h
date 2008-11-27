#ifndef TESTCOMPRESSOR_H_
#define TESTCOMPRESSOR_H_

#include "TestCase.h"
#include "../../business/compressor/CompressorBusiness.h"


class TestCompressor: public TestCase {
public:
	TestCompressor();
	virtual ~TestCompressor();
	int test();
	
};
#endif /*TESTCOMPRESSOR_H_*/
