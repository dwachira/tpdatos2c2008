#include "UseCaseValidator.h"
#include "Controller.h"
#include "../../business/directorios/DirectorioManager.h"
#include "../../business/mensajes/MensajeManager.h"

using namespace business;

UseCaseValidator::UseCaseValidator(string action, string parameter)
{
	this->action = action;
	this->parameter = parameter;
}

void UseCaseValidator::execute(){
	map<string, int> useCases;
	map<string, int>::iterator it;
	loadMap(useCases);
	
	it = useCases.find(action);
	int call;
	if(it == useCases.end())
		call = -1;
	else call = it->second;
	
	ManagerDAO managerDao;
	TrieDAO trieDao(managerDao);
	DirectorioManager directorioManager(managerDao,trieDao);
	MensajeManager mensajeManager(managerDao,directorioManager,trieDao);
	Controller controller(directorioManager,mensajeManager);
	
	switch(call){
		case 1:{
			controller.agregarDirectorio(parameter);
			break;
		}
		case 2:{
			controller.removerDirectorio(parameter);
			break;
		}
		case 3:{
			controller.agregarMensaje(parameter);
			break;
		}
		case 4:{
			controller.removerMensaje(parameter);
			break;
		}
		case 5:{
			//controller.obtenerMensaje(parameter);
			break;
		}
		case 6:{
			//controller.showFiles(parameter);
			break;
		}
		case 7:{
			//controller.showDirectories(parameter);
			break;
		}
		case 8:{
			//controller.changePassword(parameter);
			break;
		}
		case 9:{
			break;
		}
		case 10:{
			break;
		}
		case 11:{
			std::cout<<"Comandos posibles: "<<std::endl;
			std::cout<<"addDirectory <directorio>"<<std::endl;
			std::cout<<"removeDirectory <directorio>"<<std::endl;
			std::cout<<"addFile <nombre_archivo>"<<std::endl;
			std::cout<<"getFile <nombre_archivo> <ruta_destino>"<<std::endl;
			std::cout<<"showFiles"<<std::endl;
			std::cout<<"showDirectories"<<std::endl;
			std::cout<<"changePassword <old_password> <new_password>"<<std::endl;
			std::cout<<"quit"<<std::endl;
			std::cout<<"login"<<std::endl;
			std::cout<<"help"<<std::endl;
			break;
		}
		default:{
			
		}
	}
	
	
	
}

void UseCaseValidator::loadMap(map<string, int> &useCases){
	useCases["addDirectory"] = 1;
	useCases["removeDirectory"] = 2;
	useCases["addFile"] = 3;
	useCases["removeFile"] = 4;
	useCases["getFile"] = 5;
	useCases["showFiles"] = 6;
	useCases["showDirectories"] = 7;
	useCases["changePassword"] = 8;
	useCases["quit"] = 9;
	useCases["login"] = 10;
	useCases["-h"] = 11;
	useCases["help"] = 11;
	return;
}

UseCaseValidator::~UseCaseValidator()
{
}
