/*
 * TestImagenDAO.h
 *
 *  Created on: 31/10/2008
 *      Author: andres
 */

#ifndef TESTIMAGENDAO_H_
#define TESTIMAGENDAO_H_

#include "../../../object/Imagen.h"
#include "../../../dao/ImagenDAO.h"
#include "../../../util/string/StringUtils.h"
#include "../TestCase.h"
#include <list>

using namespace std;
using namespace dao;
using namespace object;
using namespace util;

class TestImagenDAO : public TestCase{

	private:
		void print(Imagen* img);
		void insertar(ImagenDAO* dao, Imagen* img);
		void getById(ImagenDAO* dao, unsigned int id);
		void borrar(ImagenDAO* dao, unsigned int id);
		void getImgsByDir(ImagenDAO* dao, unsigned int id);

		void updateEspacio(ImagenDAO* dao, unsigned int id, unsigned int newEspacio);
		void updateDirectorio(ImagenDAO* dao, unsigned int id, unsigned int newDir);
		void updateHash(ImagenDAO* dao, unsigned int id, unsigned long int newHash);
		void updateProxBit(ImagenDAO* dao, unsigned int id, unsigned int newProxBit);
		void updateNombre(ImagenDAO* dao, unsigned int id, string newNombre);

	public:
		TestImagenDAO();
		~TestImagenDAO();
		int test();

};

#endif /* TESTIMAGENDAO_H_ */
