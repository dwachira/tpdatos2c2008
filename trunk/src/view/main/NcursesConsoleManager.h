#ifndef NCURSESCONSOLEMANAGER_H_
#define NCURSESCONSOLEMANAGER_H_

#include <ncurses.h>
#include <string>

class NcursesConsoleManager
{
private:
	WINDOW * term;
public:
	NcursesConsoleManager();
	std::string readLine();
	void write(char* c_string);
	void write(std::string str);
	std::string readLineWithNoEcho();
	std::string readLineWithEcho();
	std::string readUserCommand();
	virtual ~NcursesConsoleManager();
};

#endif /*NCURSESCONSOLEMANAGER_H_*/
