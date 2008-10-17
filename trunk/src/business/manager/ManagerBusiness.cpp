#include "ManagerBusiness.h"

namespace business
{

ManagerBusiness::ManagerBusiness(){

	this->managerDAO = new dao::ManagerDAO();
	this->nameFileTrie = new Trie();
	this->cryptoBusiness  = new BlowfishCrypto();
}

ManagerBusiness::~ManagerBusiness()
{

	delete this->nameFileTrie;
	delete this->cryptoBusiness;
	delete this->managerDAO;

}


}

