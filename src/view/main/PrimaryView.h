#ifndef PRIMARYVIEW_H_
#define PRIMARYVIEW_H_
#define MAX_LINE 250
#include "Controller.h"

class PrimaryView
{
private:
	Controller controller;
public:
	PrimaryView();

	void start();

	virtual ~PrimaryView();
};

#endif /*PRIMARYVIEW_H_*/
