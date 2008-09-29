/*
 * Directory.h
 *
 *  Created on: 07-jul-2008
 *      Author: gsenno
 */

#ifndef DIRECTORY_H_
#define DIRECTORY_H_

#include <string>
#include "../../util/date/Date.h"

namespace business {

class Directory {
private:
	long id;
	std::string path;
	util::Date lastModification;
public:
	Directory(std::string& path, util::Date& lastModification) : path(path), lastModification(lastModification) {}
	virtual ~Directory();

	long getId() const
	{
		return id;
	}

	void setId(long id)
	{
		this->id = id;
	}

	std::string getPath() const
    {
        return path;
    }

    util::Date getLastModificationDate() const
    {
        return lastModification;
    }

    void setLastModificationDate(util::Date& lastModification)
    {
        this->lastModification = lastModification;
    }

};

}

#endif /* DIRECTORY_H_ */
