#ifndef CRYPTOBUSINESS_H_
#define CRYPTOBUSINESS_H_

#include "../../config/Config.h"

using namespace std;
class CryptoBusiness {
public:
	CryptoBusiness();
	virtual ~CryptoBusiness();
	virtual void inicializar(string password) = 0;
	virtual string encrypt(string data) = 0;
	virtual string desencrypt(string password, string data) = 0;
};

#endif /* CRYPTOBUSINESS_H_ */
