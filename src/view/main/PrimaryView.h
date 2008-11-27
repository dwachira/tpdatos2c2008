#ifndef PRIMARYVIEW_H_
#define PRIMARYVIEW_H_
#define MAX_LINE 250
#include "Controller.h"
#include <istream>
#include "UseCaseValidator.h"
#include "NcursesConsoleManager.h"

class PrimaryView
{
private:
	Controller controller;
	std::istream& input;
	UseCaseValidator aUseCaseValidator;
	bool loggedIn;
	bool wantsToQuit;
public:
	PrimaryView() : input(std::cin), aUseCaseValidator(controller) {
		loggedIn = false;
		wantsToQuit = false;
	}
	PrimaryView(std::istream& is) : input(is), aUseCaseValidator(controller) {}
	void start();
	void doNcursesLoggin();
	bool isWantsToQuit() {
		return wantsToQuit;
	}
	bool isLoggedIn() {
		return loggedIn;
	}
	virtual ~PrimaryView();
};

#endif /*PRIMARYVIEW_H_*/
