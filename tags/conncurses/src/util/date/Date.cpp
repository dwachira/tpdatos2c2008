/*
 * Date.cpp
 *
 *  Created on: 08-jul-2008
 *      Author: gsenno
 */

#include "Date.h"
#include <time.h>

namespace util
{

bool operator< (const Date& left, const Date& right) {
	if (left.getYear() < right.getYear()) {
		return true;
	}
	else if (left.getYear() == right.getYear()) {
		if (left.getMonth() < right.getMonth()) {
			return true;
		}
		else if (left.getMonth() == right.getMonth()) {
			if (left.getDay() < right.getDay()) {
				return true;
			}
			else if (left.getDay() == right.getDay()) {
				if (left.getHour() < right.getHour()) {
					return true;
				}
				else if (left.getHour() < right.getHour()) {
					return true;
				}
				else if (left.getHour() == right.getHour()) {
					if (left.getMinute() < right.getMinute()) {
						return true;
					}
					else if(left.getMinute() == right.getMinute()){
						if(left.getSecond() < right.getSecond()){
							return true;
						}
						else{
							return false;
						}
					}
					else
					{
						return false;
					}
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

std::ostream& operator<<(std::ostream& os, const util::Date& date) {
	std::stringstream stringBuff;
	stringBuff << date.getDay() << "/" << date.getMonth() << "/" << date.getYear()
		<< " " << date.getHour() << ":" << date.getMinute() << ":" << date.getSecond();
	os << stringBuff.str();
	return os;
}

Date::Date() {
	time_t tiempo = time(NULL);
	struct tm* timeAux = localtime(&tiempo);
	this->second = timeAux->tm_sec;
	this->day = timeAux->tm_mday;
	this->hour = timeAux->tm_hour;
	this->minute = timeAux->tm_min;
	this->month = timeAux->tm_mon + 1;
	this->year = timeAux->tm_year + 1900;
}

Date::Date(unsigned int day, unsigned int month, unsigned int year,
		unsigned int hour, unsigned int minute, unsigned int second) {
	this->second = second;
	this->minute = minute;
	this->hour = hour;
	this->day = day;
	this->month = month;
	this->year = year;
}

Date* Date::valueOf(unsigned int day, unsigned int month, unsigned int year,
		unsigned int hour, unsigned int minute, unsigned int second)
{
	if ( (day < 32) && (month < 13) )
		return new Date(day,month,year,hour,minute,second);
	else
		return NULL;
}

double Date::concatFecha(unsigned int anio, unsigned int mes, unsigned int dia,
						unsigned int hora, unsigned int minutos, unsigned int segundos){

	string s1, s2, s3, s4, s5, s6;

	if(segundos == 0)
			s6 == "00";
		else
			if(segundos < 10)
				s6 = "0" + StringUtils::toString(segundos);
			else
				s6 = StringUtils::toString(segundos);

	if(minutos == 0)
		s5 == "00";
	else
		if(minutos < 10)
			s5 = "0" + StringUtils::toString(minutos);
		else
			s5 = StringUtils::toString(minutos);

	if(hora == 0)
		s4 = "00";
	else
		if(hora < 10)
			s4 = "0" + StringUtils::toString(hora);
		else
			s4 = StringUtils::toString(hora);

	if(dia < 10)
		s3 = "0" + StringUtils::toString(dia);
	else
		s3 = StringUtils::toString(dia);

	if(mes < 10)
		s2 = "0" + StringUtils::toString(mes);
	else
		s2 = StringUtils::toString(mes);

	if(anio == 0)
		s1 = "00";
	else
		if(anio < 10)
			s1 = "0" + StringUtils::toString(anio);
		else
			s1 = StringUtils::toString(anio);

	string output = s1 + s2 + s3 + s4 + s5 + s6;
	double result = atof(output.c_str());
	return result;
}


Date::~Date() {
	// TODO Auto-generated destructor stub
}

}
