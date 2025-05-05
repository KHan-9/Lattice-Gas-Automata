#pragma once
#include"Cell.h"

class CelluarAutomata {
	int width, height;		//both variables describe amount of cells, not size in pixels!
	cell** cells;

public:
	CelluarAutomata(int, int);
	void display();
	void fill(int units, int up, int down, int left, int right);
	void setup();
	void execute_iteration();
};
