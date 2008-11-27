#include "PrimaryView.h"
#include <string.h>
#include <iosfwd>
using namespace std;
int main(int argc, char* argv[]){

	if (argc == 3) {
		if (memcmp(argv[1],"-d",2) == 0) {
			char buffer[250];
			std::stringstream action;
			PrimaryView mainApp(action);
			memset(buffer,'\0',250);
			ifstream input(argv[2], fstream::in);
			if (!input.fail()) {
				while (memcmp(buffer,"quit",4) != 0) {
					input.getline(buffer,250);
					action << buffer << "\nquit\n";
					cout<<"  Ejecutando comando: "<<buffer<<std::endl;
					mainApp.start();
					cout<<std::endl;
					char c;
					cin.get(c);
				}
			}
		}
	}
	else {
		PrimaryView mainApp;
		std::string pass("");
		while ((!mainApp.isLoggedIn())&&(!mainApp.isWantsToQuit())) {
			pass = mainApp.doNcursesLoggin();
		}
		if (mainApp.isLoggedIn()) {
			mainApp.executeLogin(pass);
			mainApp.start();
		}
	}

}
