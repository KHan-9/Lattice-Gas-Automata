#pragma once

#include<iostream>
#include<fstream>
#include<string>
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<time.h>


class cell {
public:
	//int in;
	int in[4];
	//int out;
	int out[4];
	int mid[4];
	int C;
	int is_a_wall;

	cell() {
		/*in = 0;
		out = 0;*/
		for (int i = 0; i < 4; i++) {
			in[i] = out[i] = mid[i] =  0;
		}
		C = 0;
		is_a_wall = 0;
	}

	void solidify() {
		is_a_wall = 1;
	}

	void remove() {
		is_a_wall = 0;
	}
};
