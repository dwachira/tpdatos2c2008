#include "../DirectorioDAO.h"
#include "../ImagenDAO.h"
#include "../MensajeDAO.h"
#include "../ParticionDAO.h"

using namespace dao;
#ifndef MANAGERDAO_H_
#define MANAGERDAO_H_

namespace dao
{

class ManagerDAO
{
private:
	DirectorioDAO directorioDao;
	ImagenDAO imagenDAO;
	MensajeDAO mensajeDAO;
	ParticionDAO particionDAO;

public:
	ManagerDAO();
	DirectorioDAO* getDirectorioDAO() {
		return &directorioDao;
	}
	ImagenDAO* getImagenDAO() {
		return &imagenDAO;
	}
	MensajeDAO* getMensajeDAO() {
		return &mensajeDAO;
	}
	ParticionDAO* getParticionDAO() {
		return &particionDAO;
	}
	virtual ~ManagerDAO();
	//TagDAO * getTagDAO(){return tagDAO;}

};

}

#endif /*MANAGERDAO_H_*/
