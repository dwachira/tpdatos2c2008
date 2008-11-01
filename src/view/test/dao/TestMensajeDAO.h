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

	private:
		void print(Mensaje* dir);
		void insertar(MensajeDAO* dao, Mensaje* dir);
		void borrar(MensajeDAO* dao, unsigned int id);
		void getById(MensajeDAO* dao, unsigned int id);
		void update(MensajeDAO* dao, unsigned int id, int newCant);

	public:
		TestMensajeDAO();
		~TestMensajeDAO();
		int test();
};

#endif /* TESTMENSAJEDAO_H_ */
