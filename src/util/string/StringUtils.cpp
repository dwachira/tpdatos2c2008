#include "StringUtils.h"

namespace util
{using namespace std;

StringUtils::StringUtils()
{
}

StringUtils::~StringUtils()
{
}

int StringUtils::split(string input, const string& delimiter,
		vector<string>& results, bool includeEmpties) {

	int iPos = 0;
	int newPos = -1;
	int sizeS2 = (int)delimiter.size();
	int isize = (int)input.size();

	if ((isize == 0)||(sizeS2 == 0)) {
		return 0;
	}

	vector<int> positions;

	newPos = input.find(delimiter, 0);

	if (newPos < 0) {
		results.push_back(input);
		return 0;
	}

	int numFound = 0;

	while (newPos >= iPos ) {
		numFound++;
		positions.push_back(newPos);
		iPos = newPos;
		newPos = input.find(delimiter, iPos+sizeS2);
	}

	if (numFound == 0) {
		return 0;
	}

	for (int i=0; i <= (int)positions.size(); ++i ) {
		string s("");
		if (i == 0) {
			s = input.substr(i, positions[i]);
		}
		int offset = positions[i-1] + sizeS2;
		if (offset < isize ) {
			if (i == (int)positions.size() ) {
				s = input.substr(offset);
			} else if (i > 0) {
				s = input.substr(positions[i-1] + sizeS2, positions[i]
				                                                    - positions[i-1]- sizeS2 );
			}
		}
		if (includeEmpties || (s.size() > 0)) {
			results.push_back(s);
		}
	}
	return numFound;

}

string StringUtils::charAt(string input, int pos) {
	stringstream ret;
	ret << input.at(pos);
	return ret.str();
}

bool StringUtils::isInteger(string input) {
	std::istringstream inpStream(input);
	int inpValue = 0;
	if (inpStream >> inpValue) {
		return true;
	} else {
		return false;
	}
}

string StringUtils::uppercase(string input){


	for (int j=0; j< (int)input.length(); j++)
	{
		input[j]=toupper(input[j]);
	}
	return input;
}

string StringUtils::toString(float input){
	stringstream converter;
	string ret;
	converter << input;
	converter >> ret;
	return ret;
}

string StringUtils::toString(int input){
	stringstream converter;
	string ret;
	converter << input;
	converter >> ret;
	return ret;
}

bool StringUtils::isXMLFile(string file){
	vector<string> vec;
	StringUtils::split(file,".",vec,false);
	if (vec.size() != 2){
		return false;
	} else {
		string extencion = vec[1];
		if (strcasecmp("xml",extencion.c_str()) != 0){
			return false;
		}
	}
	return true;
}

unsigned int StringUtils::toUnsigInt(string input){
	return atoi(input.c_str());;
}

float StringUtils::toFloat(string input){
	return atof(input.c_str());
}

string& StringUtils::replaceAll(string& input, const string& from,  const string& to) {
	size_t lookHere = 0;
	size_t foundHere;
	while((foundHere = input.find(from, lookHere)) != string::npos) {
		input.replace(foundHere, from.size(), to);
		lookHere = foundHere + to.size();
	}
	return input;
}

string StringUtils::trim(string input){


	bool isSpace = false;
	int primerChar = 0;
	int ultimoSpace = 0;
	for (int j=0; j< (int)input.length(); j++)
	{
		if (input[j] != ' ' || isSpace){
			isSpace = true;
			ultimoSpace = 0;
		}
		if (input[j] == ' '){
			ultimoSpace = j;
			if (!isSpace)
				primerChar++;
		}
	}
	unsigned int fin = input.length();
	if (primerChar > 0)
			input = input.substr(primerChar, fin - primerChar -1);
	if (ultimoSpace > 0)
		input = input.substr(0, ultimoSpace);
	return input;
}

string StringUtils::toString(unsigned int input){
	stringstream converter;
	string ret;
	converter << input;
	converter >> ret;
	return ret;
}


double StringUtils::concat(unsigned int input1, unsigned int input2){

	string s1 = toString(input1);
	string s2 = toString(input2);

	string output = s1 + s2;

	double result = atof(output.c_str());

	return result;
}

double StringUtils::concat(unsigned int input1, unsigned int input2, unsigned int input3){

	string s1 = toString(input1);
	string s2 = toString(input2);
	string s3 = toString(input3);

	string output = s1 + s2 + s3;

	double result = atof(output.c_str());

	return result;
}

int StringUtils::compare(string s1, string s2)
{
  const int l1 = s1.size();
  const int l2 = s2.size();
  const int lmin = l1 < l2 ? l1 : l2;
  const char *c1 = s1.data();
  const char *c2 = s2.data();
  int l = 0;

  while (l < lmin && *c1 == *c2) {
    c1++;
    c2++;
    l++;
  }

  if (l == l2)
    return 1;
  else return 0;
}
}
