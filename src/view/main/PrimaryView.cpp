#include "PrimaryView.h"
#include "UseCaseValidator.h"

#include <iostream>
#include <string>
#include <map>
using namespace std;

PrimaryView::PrimaryView() {
}

bool PrimaryView::loginMenu() {
	string opcion = "";
	string valido = "";
	while(valido.compare("s") == 0 || valido.compare("n") == 0)
	{
		std::cout<<"¿Desea loguearse? (S/N).";
		cin.getline(opcion, MAX_LINE);
		if(opcion.compare("S") == 0 || opcion.compare("s") == 0) {
			valido.assign("s");
		}
		else if(opcion.compare("N") == 0 || opcion.compare("n") == 0){
			valido.assign("n");
		}
	}
	if(valido.compare("s") == 0){
		//login
		return true;
	}
	else{
		std::cout<<"\nAplicación cerrada.";
		return false;
	}
}

void PrimaryView::start() {
	string command = "";
	char buffer[MAX_LINE];
	while (command.compare("quit") != 0) {
		cin.getline(buffer, MAX_LINE);
		command.assign(buffer);
		int posFirstSpace = command.find_first_of(" ");
		string action = command.substr(0, posFirstSpace);
		int posSecondSpace = command.find_first_of(" ", posFirstSpace+1);
		string firstParameter = command.substr(posFirstSpace+1, posSecondSpace
				-posFirstSpace-1);
		string secondParameter = command.substr(posSecondSpace+1);
		UseCaseValidator aUseCaseValidator(action, firstParameter,
				secondParameter);
		aUseCaseValidator.execute(controller);

	}
}

PrimaryView::~PrimaryView() {
}
