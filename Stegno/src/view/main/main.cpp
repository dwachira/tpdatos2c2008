#ifdef HAVE_CONFIG_H
#include <config.h>

#endif
#include <iostream>
using std::cout;


int main(int argc, char* argv[]){


int i = 1;
char *p = (char *) &i;
if ( p[0] == 1 )
	cout << "Little Endian\n";
else
	cout << "Big Endian\n";

	return 0;
}


