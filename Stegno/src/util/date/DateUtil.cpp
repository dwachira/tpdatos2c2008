#include "DateUtil.h"

namespace util
{

DateUtil::DateUtil()
{
}

DateUtil::~DateUtil()
{
}

//Fromato de date en string dd/mm/aaaa, formato date en in aaaammdd
int DateUtil::dateToInt(std::string date){
	std::stringstream ret;
	//Año
	ret << date.at(6);
	ret << date.at(7);
	ret << date.at(8);
	ret << date.at(9);
	//Mes
	ret << date.at(3);
	ret << date.at(4);
	//Dia
	ret << date.at(0);
	ret << date.at(1);
	return util::StringUtils::toUnsigInt(ret.str());
}

std::string DateUtil::intToDate(int date){
	std::stringstream ret;
	ret << date;
	std::string sDate = ret.str();
	std::stringstream result;
	//Dia
	result << sDate.at(6);
	result << sDate.at(7);
	result << "/";
	result << sDate.at(4);
	result << sDate.at(5);
	result << "/";
	result << sDate.at(0);
	result << sDate.at(1);
	result << sDate.at(2);
	result << sDate.at(3);
	return result.str();
}

}
