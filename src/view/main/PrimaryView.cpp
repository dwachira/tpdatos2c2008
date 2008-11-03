#include "PrimaryView.h"
#include "UseCaseValidator.h"

#include <iostream.h>
#include <string>
#include <map>
using namespace std;

PrimaryView::PrimaryView() {
}

void PrimaryView::start() {
	string command = "";

	while (command.compare("quit") != 0) {
		cin >> command;
		int pos = command.find_first_of(" ");
		string action = command.substr(0, pos);
		string parameter = command.substr(pos+1);
		UseCaseValidator aUseCaseValidator(action, parameter);
		aUseCaseValidator.execute();

	}
}

PrimaryView::~PrimaryView() {
}
