/*
 * Date.cpp
 *
 *  Created on: 08-jul-2008
 *      Author: gsenno
 */

#include "Date.h"


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
	else {
		return false;
	}
}

std::ostream& operator<<(std::ostream& os, const util::Date& date) {
	std::stringstream stringBuff;
	stringBuff << date.getDay() << "/" << date.getMonth() << "/"
			   << date.getYear() << " " << date.getHour() << ":" << date.getMinute();
	os << stringBuff.str();
	return os;
}

Date::Date(unsigned int day, unsigned int month, unsigned int year,
		unsigned int hour, unsigned int minute) {
	this->minute = minute;
	this->hour = hour;
	this->day = day;
	this->month = month;
	this->year = year;
}

Date* Date::valueOf(unsigned int day, unsigned int month, unsigned int year,
		unsigned int hour, unsigned int minute)
{
	if ( (day < 32) && (month < 13) )
		return new Date(day,month,year,hour,minute);
	else
		return NULL;
}

double Date::concatFecha(unsigned int anio, unsigned int mes, unsigned int dia,
												unsigned int hora, unsigned int minutos){

	string s1, s2, s3, s4, s5;

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

	string output = s1 + s2 + s3 + s4 + s5;
	double result = atof(output.c_str());
	return result;
}


Date::~Date() {
	// TODO Auto-generated destructor stub
}

}
