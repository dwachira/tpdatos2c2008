#include "PrimaryView.h"

#include <iostream>
#include <string>
#include <map>
using namespace std;

void PrimaryView::start() {
	string command = "";

	char buffer[MAX_LINE];
	printf(" \033[2J");
	while (command.compare("quit") != 0) {
		cout << "herodotus> ";
		if (!loggedIn) {
			NcursesConsoleManager ncursesConsole;
			command = ncursesConsole.readUserCommand();
		} else {
			input.getline(buffer, MAX_LINE);
			command.assign(buffer);
		}
		int posFirstSpace = command.find_first_of(" ");
		string action = command.substr(0, posFirstSpace);
		int posSecondSpace = command.find_first_of(" ", posFirstSpace+1);
		string firstParameter = command.substr(posFirstSpace+1, posSecondSpace
				-posFirstSpace-1);
		string secondParameter = command.substr(posSecondSpace+1);
		aUseCaseValidator.execute(action, firstParameter,secondParameter);
		loggedIn = aUseCaseValidator.isLoggedIn();
		
	}
}

std::string PrimaryView::doNcursesLoggin() {
	NcursesConsoleManager ncursesConsole;
	string command = "";
	ncursesConsole.write("Bienvenido al inicio de sesion en Herodotus. Ingrese \"login <password>\" para comensar.\n");
	ncursesConsole.write("herodotus> ");
	command = ncursesConsole.readUserCommand();
	if (command.compare("quit") == 0) {
		wantsToQuit = true;
		return;
	}
	else {
		int posFirstSpace = command.find_first_of(" ");
		string action = command.substr(0, posFirstSpace);
		int posSecondSpace = command.find_first_of(" ", posFirstSpace+1);
		string firstParameter = command.substr(posFirstSpace+1, posSecondSpace
				-posFirstSpace-1);
		string secondParameter = command.substr(posSecondSpace+1);
		if (action.compare("login") == 0) {
			if (controller.getAuthBusiness()->checkPass(firstParameter)) {
				loggedIn = true;
				return firstParameter;
			}
			else
				ncursesConsole.write("  Contraseña invalida.\n");
		}
	}
}

PrimaryView::~PrimaryView() {
}
