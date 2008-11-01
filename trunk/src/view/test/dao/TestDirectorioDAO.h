/*
 * TestDirectorioDAO.h
 *
 *  Created on: 31/10/2008
 *      Author: andres
 */

#ifndef TESTDIRECTORIODAO_H_
#define TESTDIRECTORIODAO_H_

#include "../../../object/Directorio.h"
#include "../../../dao/DirectorioDAO.h"
#include "../../../util/string/StringUtils.h"
#include "../TestCase.h"
#include <list>

using namespace std;
using namespace dao;
using namespace object;
using namespace util;

class TestDirectorioDAO : public TestCase{

	private:
		void print(Directorio* dir);
		void insertar(DirectorioDAO* dao, Directorio* dir);
		void borrar(DirectorioDAO* dao, unsigned int id);
		void update(DirectorioDAO* dao, unsigned int id, Date* newFecha);
		void getById(DirectorioDAO* dao, unsigned int id);

	public:
		TestDirectorioDAO();
		~TestDirectorioDAO();
		int test();
		int test2();
};

#endif /* TESTDIRECTORIODAO_H_ */
