/*
 * DirectorioIteradorImagenes.cpp
 *
 *  Created on: 07-ago-2008
 *      Author: gsenno
 */

#include "DirectorioIteradorImagenes.h"
#include "../../util/string/StringUtils.h"
using namespace std;

namespace business {

string DirectorioIteradorImagenes::BMP = "BMP";
string DirectorioIteradorImagenes::GIF = "GIF";
string DirectorioIteradorImagenes::JPG = "JPG";
string DirectorioIteradorImagenes::PNG = "PNG";

DirectorioIteradorImagenes::DirectorioIteradorImagenes(object::Directorio& dir) {
	osDir = opendir(dir.getPath().data());
	obtenerSiguienteImagen();
}

void DirectorioIteradorImagenes::obtenerSiguienteImagen() {
	while ( (nextDirEntry = readdir(osDir)) != NULL ) {
		string entryName(nextDirEntry->d_name);
		if (entryName.size() > EXTENSION_LENGTH) {
			string extension = util::StringUtils::uppercase(entryName.substr(entryName.size() - EXTENSION_LENGTH));
			if ( (extension.compare(BMP) == 0) || (extension.compare(GIF) == 0) || (extension.compare(JPG) == 0) || (extension.compare(PNG) == 0) )
				break;
		}
	}
}

string DirectorioIteradorImagenes::next()
{
	std::string entry(nextDirEntry->d_name);
	obtenerSiguienteImagen();
	return entry;
}

bool DirectorioIteradorImagenes::hasNext() const
{
	if (nextDirEntry == NULL)
		return false;
	return true;
}

DirectorioIteradorImagenes::~DirectorioIteradorImagenes() {
}

}
