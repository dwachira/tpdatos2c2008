#ifndef MANAGER_H_
#define MANAGER_H_

#include "../../db/trie/Trie.h"
#include "../../dao/manager/ManagerDAO.h"
#include "../crypto/CryptoBusiness.h"
#include "../../config/Config.h"

namespace business
{
class ManagerBusiness
{
private:
	dao::ManagerDAO * managerDAO;
	Trie * nameFileTrie;
	CryptoBusiness * cryptoBusiness;
	
	
public:
	ManagerBusiness();
	virtual ~ManagerBusiness();
	CryptoBusiness * getCryptoBusiness(){return cryptoBusiness;}
	
};
}

#endif /*MANAGER_H_*/
