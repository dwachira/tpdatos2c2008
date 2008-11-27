/*
 * MD5Test.cpp
 *
 *  Created on: 03/11/2008
 *      Author: maxi
 */

#include "MD5Test.h"

MD5Test::MD5Test() {
	// TODO Auto-generated constructor stub

}

MD5Test::~MD5Test() {

}


int MD5Test::test() {
	md5wrapper md5;

	// create a hash from a string
	std::string hash1 = md5.getHashFromString(" %$#/&%/(&(&%$:[]*�+�{-.,^^`~�");

	// create a hash from a file
	std::string hash2 = md5.getHashFromFile("/home/maxi/in.txt");

	std::cout << "Hash de la cadena: " << hash1 << endl;
	std::cout << "Hash del archivo: " << hash2 << endl;
	return 0;
}
