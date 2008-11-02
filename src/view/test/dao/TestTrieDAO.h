/*
 * TestTrieDAO.h
 *
 *  Created on: 02/11/2008
 *      Author: andres
 *
 *      ATENCION
 *      ********
 *
 * En esta clase se incluyeron lo que son todos los testeos
 * de los demas elementos DAO (Directorio, Mensaje e Imagen)
 * De este modo se utilizan como instrucciones y se "anexa"
 * el uso del Trie en los 3 casos para verificar su funcionamiento
 *
 */

#ifndef TESTTRIEDAO_H_
#define TESTTRIEDAO_H_

#include "../TestCase.h"
#include "TestDirectorioDAO.h"
#include "TestImagenDAO.h"
#include "TestMensajeDAO.h"
#include "../../../dao/TrieDAO.h"
#include "../../../dao/manager/ManagerDAO.h"
#include "../../../util/string/StringUtils.h"
#include <list>

using namespace std;
using namespace dao;
using namespace object;
using namespace util;

class TestTrieDAO : public TestCase{

	private:

	public:
		TestTrieDAO();
		~TestTrieDAO();
		int test();		//Directorios
		int test2();	//Imagenes
		int test3();	//Mensajes

};


#endif /* TESTTRIEDAO_H_ */
