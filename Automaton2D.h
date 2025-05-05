#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<time.h>

class Automaton2D {
	 
	int width, height;		//both variables describe amount of cells, not size in pixels!
	int** cells;
	int boundary;			//0 - absorb, 1 - periodic; 2 - bounce
	void execute_iteration(sf::Image &c);
	void draw(sf::Image& c);
	

public:
	Automaton2D(int w, int h);
	void load_initial_state(std::string file);
	void randomize(int num);
	void save_state_to_file();
	~Automaton2D();
	void display();
	void set_boundary_condition(int a);
};