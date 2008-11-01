/*
 * TestParticionDAO.h
 *
 *  Created on: 01/11/2008
 *      Author: andres
 */

#ifndef TESTPARTICIONDAO_H_
#define TESTPARTICIONDAO_H_

#include "../../../object/Particion.h"
#include "../../../dao/ParticionDAO.h"
#include "../../../util/string/StringUtils.h"
#include "../TestCase.h"
#include <list>

using namespace std;
using namespace dao;
using namespace object;
using namespace util;

class TestParticionDAO : public TestCase{

	private:
		void print(Particion* img);
		void insertar(ParticionDAO* dao, Particion* img);
		void getById(ParticionDAO* dao, unsigned int IMG, unsigned int TXT, unsigned int POS);
		void borrar(ParticionDAO* dao, unsigned int IMG, unsigned int TXT, unsigned int POS);
		void liberar(ParticionDAO* dao, unsigned int IMG, unsigned int TXT, unsigned int POS);
		void getByImg(ParticionDAO* dao, unsigned int IMG);
		void getByTxt(ParticionDAO* dao, unsigned int TXT);

	public:
		TestParticionDAO();
		~TestParticionDAO();
		int test();
};

#endif /* TESTPARTICIONDAO_H_ */
