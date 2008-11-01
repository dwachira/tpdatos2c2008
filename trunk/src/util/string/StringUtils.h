#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_

#include <stdio.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <algorithm>

namespace util
{
using namespace std;
class StringUtils
{
public:
	StringUtils();
	virtual ~StringUtils();
	static int split(string input, const string& delimiter, vector<string>& results, bool includeEmpties);
	static string charAt(string input, int pos);
	static bool isInteger(string input);
	static string uppercase(string input);
	static string toString(float input);
	static string toString(int input);
	static bool isXMLFile(string file);
	static unsigned int toUnsigInt(string input);
	static float toFloat(string input);
	static string& replaceAll(string& input, const string& from,  const string& to);
	static string trim(string input);
	static string toString(unsigned int input);
	static string toString(double input);
	static double concat(unsigned int input1, unsigned int input2);
	static double concat(unsigned int input1, unsigned int input2, unsigned int input3);
	static int compare(string s1, string s2);
};

}

#endif /*STRINGUTILS_H_*/
