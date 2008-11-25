#include "UseCaseValidator.h"
#include "Controller.h"
#include "../../business/directorios/DirectorioManager.h"
#include "../../business/mensajes/MensajeManager.h"

using namespace business;

UseCaseValidator::UseCaseValidator(Controller& aController) : controller(aController) {
	login = false;
}

void UseCaseValidator::execute(string action, string firstParameter,
		string secondParameter) {
	map<string, int> useCases;
	map<string, int>::iterator it;
	loadMap(useCases);

	it = useCases.find(action);
	int call;
	if (it == useCases.end())
		call = -1;
	else
		call = it->second;

	if (login == true || call == 10 || call == 9) {

		switch (call) {
		case 1: {
			controller.agregarDirectorio(firstParameter);
			break;
		}
		case 2: {
			controller.removerDirectorio(firstParameter);
			break;
		}
		case 3: {
			controller.agregarMensaje(firstParameter);
			break;
		}
		case 4: {
			controller.removerMensaje(firstParameter);
			break;
		}
		case 5: {
			controller.obtenerMensaje(firstParameter, secondParameter);
			break;
		}
		case 6: {
			controller.mostrarMensajes();
			break;
		}
		case 7: {
			controller.mostrarDirectorios();
			break;
		}
		case 8: {
			controller.changePassword(firstParameter, secondParameter);
			break;
		}
		case 9: {
			break;
		}
		case 10: {
			if (controller.login(firstParameter)) {
				login = true;
				if(firstParameter.compare(DEFAULTPASSWORD) == 0){
					string newPassword = "";
					char buffer[MAX_LINE];
					std::cout<<"Se está logueando con la contraseña por defecto, "
						"ingrese su nuevo password: "<<std::endl;
					cin.getline(buffer, MAX_LINE);
					newPassword.assign(buffer);
					controller.changePassword(firstParameter, newPassword);
				}
			}
			break;
		}
		case 11: {
			std::cout<<"Los comandos posibles son: "<<std::endl;
			std::cout<<"\naddDirectory <directorio>"<<std::endl;
			std::cout<<"removeDirectory <directorio>"<<std::endl;
			std::cout<<"addFile <nombre_archivo>"<<std::endl;
			std::cout<<"removeFile <nombre_archivo>"<<std::endl;
			std::cout<<"getFile <nombre_archivo> <ruta_destino>"<<std::endl;
			std::cout<<"showFiles"<<std::endl;
			std::cout<<"showDirectories"<<std::endl;
			std::cout<<"changePassword <old_password> <new_password>"<<std::endl;
			std::cout<<"quit"<<std::endl;
			std::cout<<"login"<<std::endl;
			std::cout<<"help"<<std::endl;
			break;
		}
		default: {
			std::cout<<"ingresa \"help\" o \"-h\" para ayuda."<<std::endl;
		}
		}
	}
	else{
		std::cout<<"Hasta no loguearse no podrá utilizar el sístema. ('ĺogin' para loguearse)"<<std::endl;
	}

}

void UseCaseValidator::loadMap(map<string, int> &useCases) {
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

UseCaseValidator::~UseCaseValidator() {
}
