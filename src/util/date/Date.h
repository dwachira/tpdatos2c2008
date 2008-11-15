/*
 * Date.h
 *
 *  Created on: 08-jul-2008
 *      Author: gsenno
 */
#ifndef NULL
#define NULL	0
#endif
#ifndef DATE_H_
#define DATE_H_

#include <sstream>
#include <ostream>
#include "../string/StringUtils.h"

namespace util
{

class Date {
private:
	unsigned int second;
	unsigned int minute;
	unsigned int hour;
	unsigned int day;
	unsigned int month;
	unsigned int year;
	Date(unsigned int day, unsigned int month,
			unsigned int year, unsigned int hour, unsigned int minute, unsigned int second);
public:
	static Date* valueOf(unsigned int day, unsigned int month, unsigned int year,
			unsigned int hour, unsigned int minute, unsigned int second);

	static double concatFecha(unsigned int anio, unsigned int mes, unsigned int dia,
							unsigned int hora, unsigned int minutos, unsigned int segundos);

	Date();

	Date(const Date& anotherDate) {
		this->second = anotherDate.second;
		this->minute = anotherDate.minute;
		this->hour = anotherDate.hour;
		this->day = anotherDate.day;
		this->month = anotherDate.month;
		this->year = anotherDate.year;
	}

	virtual ~Date();

	unsigned int getDay() const
    {
        return day;
    }

    unsigned int getMonth() const
    {
        return month;
    }

    unsigned int getYear() const
    {
        return year;
    }

    unsigned int getHour() const
	{
		return hour;
	}

    unsigned int getMinute() const
	{
		return minute;
	}

	unsigned int getSecond() const
    {
        return second;
    }

    friend bool operator< (const Date& left, const Date& right);

    friend std::ostream& operator<<(std::ostream& os, const util::Date& date);

};

}


#endif /* DATE_H_ */
