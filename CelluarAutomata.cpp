#include "CelluarAutomata.h"

CelluarAutomata::CelluarAutomata(int w, int h) {
	width = w;
	height = h;

	cells = new cell* [h];
	for (int i = 0; i < h; i++) {
		cells[i] = new cell[w];
	}
}

void CelluarAutomata::fill(int units, int up, int down, int left, int right) {

	srand(time(NULL));
	for (int i = 0; i < units; i++) {
		int x = up + rand() % (up - down);
		int y =  left + rand() % (right - left);
		if (cells[x][y].out > 0) {
			i--;
			continue;
		}
		int vel = rand() % 4;
		if(vel == 0)cells[x][y].out += 1;
		if (vel == 1)cells[x][y].out += 2;
		if (vel == 2)cells[x][y].out += 4;
		if (vel == 3)cells[x][y].out += 8;
		
	}
}

void CelluarAutomata::setup() {
	for (int i = 0; i < width; i++) {
		cells[0][i].solidify();
		cells[height - 1][i].solidify();
	}

	for (int i = 0; i < height; i++) {
		cells[i][0].solidify();
		cells[i][width-1].solidify();
		cells[i][width / 4].solidify();
	}
}

void CelluarAutomata::execute_iteration() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int mod = cells[i][j].out % 2;

			if (mod)cells[i+1][j].in += 1;
			cells[i][j].out /=  2;
			mod = cells[i][j].out % 2;
			if (mod)cells[i][j - 1].in += 2;
			cells[i][j].out /= 2;
			mod = cells[i][j].out % 2;
			if (mod)cells[i-1][j].in += 4;
			cells[i][j].out /= 2;
			mod = cells[i][j].out % 2;
			if (mod)cells[i][j + 1].in += 8;
			
		}
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (cells[i][j].is_a_wall == 1) {
				cells[i][j].out = 0;
				if(cells[i][j].in  % 2 == 1)cells[i][j].out += 4;
				cells[i][j].in /= 2;
				if (cells[i][j].in % 2 == 1)cells[i][j].out += 8;
				cells[i][j].in /= 2;
				if (cells[i][j].in % 2 == 1)cells[i][j].out += 2;
				cells[i][j].in /= 2;
				if (cells[i][j].in % 2 == 1)cells[i][j].out += 1;
				cells[i][j].in = 0;
			}
			else {
				if (cells[i][j].in == 10)cells[i][j].in = 5;
				else if (cells[i][j].in == 5)cells[i][j].in = 10;
				cells[i][j].out = cells[i][j].in;
				cells[i][j].in = 0;
			}

		}
	}
}

void CelluarAutomata::display() {
	int h_pixels = height * 3;
	int w_pixels = width * 3;
	sf::RenderWindow window(sf::VideoMode(w_pixels, h_pixels), "LGA");

	sf::Image canvas;
	sf::Texture texture;
	sf::Sprite sprite;

	canvas.create(w_pixels, h_pixels);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (cells[i][j].out > 0) {
				int scaled_i = i * 3;
				int scaled_j = j * 3;
				for (int k = 1; k < 2; k++) {
					for (int l = 1; l < 2; l++) {
						canvas.setPixel(scaled_j + k, scaled_i + l, sf::Color(255, 50, 50));
					}
				}
			}
			if(cells[i][j].is_a_wall == 1) {
				int scaled_i = i * 3;
				int scaled_j = j * 3;
				for (int k = -1; k < 2; k++) {
					for (int l = -1; l < 2; l++) {
						if (scaled_i + k < 0 || scaled_i + k >= h_pixels || scaled_i + l < 0 || scaled_i + l >= w_pixels)continue;
						canvas.setPixel(scaled_j + k, scaled_i + l, sf::Color(100, 100, 100));
					}
				}
			}

		}
	}
	texture.loadFromImage(canvas);
	sprite.setTexture(texture);
	window.clear();
	window.draw(sprite);
	window.display();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				execute_iteration();
				//Te same mechanizmy wyœwietlania danych, co powy¿ej
				canvas.create(w_pixels, h_pixels);
				for (int i = 0; i < height; i++) {
					for (int j = 0; j < width; j++) {
						if (cells[i][j].out > 0) {
							int scaled_i = i * 3;
							int scaled_j = j * 3;
							for (int k = 1; k < 2; k++) {
								for (int l = 1; l < 2; l++) {
									canvas.setPixel(scaled_j + k, scaled_i + l, sf::Color(255, 50, 50));
								}
							}
						}
						if (cells[i][j].is_a_wall == 1) {
							int scaled_i = i * 3;
							int scaled_j = j * 3;
							for (int k = -1; k < 2; k++) {
								for (int l = -1; l < 2; l++) {
									if (scaled_i + k < 0 || scaled_i + k >= h_pixels || scaled_i + l < 0 || scaled_i + l >= w_pixels)continue;
									canvas.setPixel(scaled_j + k, scaled_i + l, sf::Color(100, 100, 100));
								}
							}
						}

					}
				}
				texture.loadFromImage(canvas);
				sprite.setTexture(texture);
				window.clear();
				window.draw(sprite);
				window.display();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				for (int i = 80; i <= 120; i++) {
					cells[i][width / 4].remove();
				}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)){
				int balance = 0;
				for (int i = 0; i < height; i++) 
					for (int j = 0; j < width; j++) {
						int tmp = cells[i][j].out;
						if (tmp % 2 == 1)balance ++;
						tmp /= 2;
						if (tmp % 2 == 1)balance++;
						tmp /= 2;
						if (tmp % 2 == 1)balance++;
						tmp /= 2;
						if (tmp % 2 == 1)balance++;
					}
				std::cout << balance << std::endl;
				}
			

		}

	}
}
