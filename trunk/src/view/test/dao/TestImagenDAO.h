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

	public:
		TestImagenDAO();
		~TestImagenDAO();
		int test();

		static void print(Imagen* img);
		static void insertar(ImagenDAO* dao, Imagen* img);
		static void getById(ImagenDAO* dao, unsigned int id);
		static void borrar(ImagenDAO* dao, unsigned int id);
		static void getImgsByDir(ImagenDAO* dao, unsigned int id);

		static void updateEspacio(ImagenDAO* dao, unsigned int id, unsigned int newEspacio);
		static void updateDirectorio(ImagenDAO* dao, unsigned int id, unsigned int newDir);
		static void updateHash(ImagenDAO* dao, unsigned int id, unsigned long int newHash);
		static void updateProxBit(ImagenDAO* dao, unsigned int id, unsigned int newProxBit);
		static void updateNombre(ImagenDAO* dao, unsigned int id, string newNombre);
};

#endif /* TESTIMAGENDAO_H_ */
