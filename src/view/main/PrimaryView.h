#ifndef PRIMARYVIEW_H_
#define PRIMARYVIEW_H_
#define MAX_LINE 250
#include "Controller.h"
#include <istream>
#include "UseCaseValidator.h"

class PrimaryView
{
private:
	Controller controller;
	std::istream& input;
	UseCaseValidator aUseCaseValidator;
public:
	PrimaryView() : input(std::cin), aUseCaseValidator(controller) {}
	PrimaryView(std::istream& is) : input(is), aUseCaseValidator(controller) {}
	void start();

	virtual ~PrimaryView();
};

#endif /*PRIMARYVIEW_H_*/
