#ifndef USECASEVALIDATOR_H_
#define USECASEVALIDATOR_H_


#include <string>
#include <map>
using namespace std;

class UseCaseValidator
{
private:
	string action;
	string parameter;

	void loadMap(map<string, int> &useCases);
	
public:
	UseCaseValidator(string action, string parameter);
	void execute();
	virtual ~UseCaseValidator();
};

#endif /*USECASEVALIDATOR_H_*/
