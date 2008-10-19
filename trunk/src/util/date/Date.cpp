/*
 * Date.cpp
 *
 *  Created on: 08-jul-2008
 *      Author: gsenno
 */

#include "Date.h"


namespace util
{

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

Date::~Date() {
	// TODO Auto-generated destructor stub
}

}
