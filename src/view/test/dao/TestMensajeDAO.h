/*
 * TestMensajeDAO.h
 *
 *  Created on: 01/11/2008
 *      Author: andres
 */

#ifndef TESTMENSAJEDAO_H_
#define TESTMENSAJEDAO_H_

#include "../../../object/Mensaje.h"
#include "../../../dao/MensajeDAO.h"
#include "../../../util/string/StringUtils.h"
#include "../TestCase.h"
#include <list>

using namespace std;
using namespace dao;
using namespace object;
using namespace util;

class TestMensajeDAO : public TestCase{

	public:
		TestMensajeDAO();
		~TestMensajeDAO();
		int test();

		static void print(Mensaje* dir);
		static void insertar(MensajeDAO* dao, Mensaje* dir);
		static void borrar(MensajeDAO* dao, unsigned int id);
		static void getById(MensajeDAO* dao, unsigned int id);
		static void update(MensajeDAO* dao, unsigned int id, int newCant);
};

#endif /* TESTMENSAJEDAO_H_ */
