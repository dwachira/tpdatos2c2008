#include "NcursesConsoleManager.h"
#include <cstdlib>
#include <cstdio>
#include <string>
#include <locale.h>

NcursesConsoleManager::NcursesConsoleManager()
{
	term = initscr();
	setlocale(LC_ALL, "");
	cbreak();
}

std::string NcursesConsoleManager::readLine() {
	char buffer[1];
	int key = 0;
	bool echoenabled = true;
	std::string line("");
	while ((char)key != '\n') {
		key = getch();
		sprintf(buffer,"%c",key);
		if (key == 127) {
			int posX, posY;
			getyx(term,posX,posY);
			if (echoenabled) {
				move(posX,posY-3);
				delch();
				delch();
				delch();
			}
		}
		else {
			if (memcmp(buffer,"\n",1) != 0)
				line.append(buffer);
		}
	}
	return line;
}

void NcursesConsoleManager::write(char* c_string) {
	printw(c_string);
}
void NcursesConsoleManager::write(std::string str) {
	printw(str.c_str());
}
std::string NcursesConsoleManager::readLineWithNoEcho() {
	noecho();
	return readLine();
}
std::string NcursesConsoleManager::readLineWithEcho() {
	echo();
	return readLine();	
}
std::string NcursesConsoleManager::readUserCommand() {
	char buffer[1];
	int key = 0;
	bool echoenabled = true;
	std::string command("");
	while ((char)key != '\n') {
		key = getch();
		sprintf(buffer,"%c",key);
		if (key == 127) {
			int posX, posY;
			getyx(term,posX,posY);
			if (echoenabled) {
				move(posX,posY-3);
				delch();
				delch();
				delch();
			}
			if (command.compare("login ") == 0)
				move(posX,posY-1);
			command = command.substr(0,command.size()-1);
		}
		else {
			if (memcmp(buffer,"\n",1) != 0)
				command.append(buffer);
		}
		if ((command.size() > 5)&&(command.substr(0,6).compare("login ") == 0)) {
			noecho();
			echoenabled = false;
		}
		else {
			echo();
			echoenabled = true;
		}
	}
	write("\n");
	write("\n");
	echo();
	return command;
}

NcursesConsoleManager::~NcursesConsoleManager()
{
	nocbreak();
	endwin();
}
