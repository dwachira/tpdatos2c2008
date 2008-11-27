#include "ManagerBusiness.h"

namespace business
{

ManagerBusiness::ManagerBusiness(){

	this->managerDAO = new dao::ManagerDAO();
	this->nameFileTrie = new Trie();
	this->blowfishCrypto  = new BlowfishCrypto();
}

ManagerBusiness::~ManagerBusiness()
{

	delete this->nameFileTrie;
	delete this->blowfishCrypto;
	delete this->managerDAO;

}


}

