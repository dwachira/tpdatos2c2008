/*
 * DirectoryManager.h
 *
 *  Created on: 07-jul-2008
 *      Author: gsenno
 */

#ifndef DIRECTORYMANAGER_H_
#define DIRECTORYMANAGER_H_

#include <string>
#include "Directory.h"

namespace business {

class DirectoryManager {
private:
	void findImages(const Directory& directory) const;
public:
	DirectoryManager();
	void addDirectory(std::string path) const;
	virtual ~DirectoryManager();
};

}

#endif /* DIRECTORYMANAGER_H_ */
