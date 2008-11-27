#ifndef MANAGER_H_
#define MANAGER_H_

#include "../../db/trie/Trie.h"
#include "../../dao/manager/ManagerDAO.h"
#include "../crypto/blowfish/BlowfishCrypto.h"
#include "../../config/Config.h"

namespace business
{
class ManagerBusiness
{
private:
	dao::ManagerDAO * managerDAO;
	Trie * nameFileTrie;
	BlowfishCrypto * blowfishCrypto;


public:
	ManagerBusiness();
	virtual ~ManagerBusiness();
	BlowfishCrypto * getBlowfishCrypto(){return blowfishCrypto;}

};
}

#endif /*MANAGER_H_*/
