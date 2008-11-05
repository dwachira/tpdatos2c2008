#include "PrimaryView.h"
#include "UseCaseValidator.h"

#include <iostream>
#include <string>
#include <map>
using namespace std;

PrimaryView::PrimaryView() {
}

void PrimaryView::start() {
	string command = "";
	char buffer[MAX_LINE];
	while (command.compare("quit") != 0) {
		cin.getline(buffer,MAX_LINE);
		command.assign(buffer);
		int posFirstSpace = command.find_first_of(" ");
		string action = command.substr(0, posFirstSpace);
		int posSecondSpace = command.find_first_of(" ",posFirstSpace+1);
		string firstParameter = command.substr(posFirstSpace+1,posSecondSpace-posFirstSpace-1);
		string secondParameter = command.substr(posSecondSpace+1);
		UseCaseValidator aUseCaseValidator(action, firstParameter,secondParameter);
		aUseCaseValidator.execute(controller);

	}
}

PrimaryView::~PrimaryView() {
}
