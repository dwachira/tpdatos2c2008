#ifndef USECASEVALIDATOR_H_
#define USECASEVALIDATOR_H_


#include <string>
#include <map>
#include "Controller.h"
using namespace std;

class UseCaseValidator
{
private:
	Controller& controller;
	bool login;
	void loadMap(map<string, int> &useCases);

public:
	UseCaseValidator(Controller& aController);
	void execute(string action, string firstParameter,string secondParameter);
	virtual ~UseCaseValidator();
};

#endif /*USECASEVALIDATOR_H_*/
