#ifndef USECASEVALIDATOR_H_
#define USECASEVALIDATOR_H_


#include <string>
#include <map>
#include "Controller.h"
using namespace std;

class UseCaseValidator
{
private:
	string action;
	string firstParameter;
	string secondParameter;

	void loadMap(map<string, int> &useCases);

public:
	UseCaseValidator(string action, string firstParameter,string secondParameter);
	void execute(Controller& controller);
	virtual ~UseCaseValidator();
};

#endif /*USECASEVALIDATOR_H_*/
