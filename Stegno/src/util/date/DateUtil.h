#ifndef DATEUTIL_H_
#define DATEUTIL_H_

#include <string>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include "../string/StringUtils.h"
namespace util
{

class DateUtil
{
public:
	DateUtil();
	virtual ~DateUtil();
	static int dateToInt(std::string date);
	static std::string intToDate(int date);
};

}

#endif /*DATEUTIL_H_*/
