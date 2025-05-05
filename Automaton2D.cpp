#include"Automaton2D.h"

Automaton2D::Automaton2D(int w, int h) {
	width = w;
	height = h;
	boundary = 0;

	cells = new int* [h];
	for (int i = 0; i < h; i++) {
		cells[i] = new int[w];
		for (int j = 0; j < w; j++) {
			cells[i][j] = 0;
		}
	}
}

Automaton2D::~Automaton2D() {

	for (int i = 0; i < height; i++) {
		delete[] cells[i];
	}

	delete[] cells;
}

void Automaton2D::display(){
	int h_pixels = height * 3;
	int w_pixels = width * 3;
	sf::RenderWindow window(sf::VideoMode(w_pixels,  h_pixels), "Game of Life");

	sf::Image canvas;
	sf::Texture texture;
	sf::Sprite sprite;

	canvas.create(w_pixels, h_pixels);

	for (int i = 0; i < h_pixels; i += 5) {
		for (int j = 0; j < w_pixels; j++) {
			canvas.setPixel(j, i, sf::Color(50, 50, 50));
			if (i + 4 >= h_pixels)break;
			canvas.setPixel(j, i + 4, sf::Color(50, 50, 50));

		}
	}

	for (int i = 0; i < w_pixels; i += 5) {
		for (int j = 0; j < h_pixels; j++) {
			canvas.setPixel(i, j, sf::Color(50, 50, 50));
			if (j + 4 >= h_pixels)break;
			canvas.setPixel(i, j + 4, sf::Color(50, 50, 50));

		}
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (cells[i][j] == 1) {
				int scaled_i = i * 5;
				int scaled_j = j * 5;
				for (int k = 1; k < 4; k++) {
					for (int l = 1; l < 4; l++){
						canvas.setPixel(scaled_j + k, scaled_i + l, sf::Color(255, 50, 50));
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

			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				execute_iteration(canvas);
				texture.loadFromImage(canvas);
				sprite.setTexture(texture);
				window.draw(sprite);
				window.display();

			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				save_state_to_file();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				std::cout << "File: ";
				std::string s;
				std::cin >> s;
				load_initial_state(s);
				draw(canvas);
				texture.loadFromImage(canvas);
				sprite.setTexture(texture);
				window.draw(sprite);
				window.display();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
				int c;
				std::cout << "Number of cells: ";
				std::cin >> c;
				randomize(c);
				draw(canvas);
				texture.loadFromImage(canvas);
				sprite.setTexture(texture);
				window.draw(sprite);
				window.display();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
				int c;
				std::cout << "Boundary condition:\n1.Absorb\n2.Periodic\n3.Bounce\n";
				std::cin >> c;
				set_boundary_condition(c);
			}

		}
	}
}

void Automaton2D::load_initial_state(std::string file) {
	std::ifstream inp(file);
	int x, y;
	while (true) {
		inp >> x >> y;
		cells[x][y] = 1;
		if (inp.eof())
			break;
	}
	inp.close();

}

void Automaton2D::execute_iteration(sf::Image& c) {
	for (int i = 0; i < height; i++) {	
		for (int j = 0; j < width; j++) {
			int nominator = 0;
			for (int k = -1; k < 2; k++) {
				for (int l = -1; l <2; l++) {
					if (l == 0 && k == 0 )
						continue;

					

					if (boundary == 0) {
						if (i + k < 0 || i + k >= height || j + l < 0 || j + l >= width)
							continue;

						if (abs(cells[i + k][j + l]) == 1)
							nominator++;
						else;
					}
					else if (boundary == 1) {
						int per_k = k;
						int per_l = l;
						if (i + k < 0)per_k += height;
						if (i + k >= height)per_k -= height;
						if (j + l < 0)per_l += width;
						if (j + l >= width)per_l -= width;

						if (abs(cells[i + per_k][j + per_l]) == 1)
							nominator++;

					}
					else if (boundary == 2) {
						int per_k = k;
						int per_l = l;
						if (i + k < 0)per_k += 1;
						if (i + k >= height)per_k -= 1;
						if (j + l < 0)per_l += 1;
						if (j + l >= width)per_l -= 1;

						if (abs(cells[i + per_k][j + per_l]) == 1)
							nominator++;
					}
						
				}
			}
			if (nominator == 3 && cells[i][j] == 0)
				cells[i][j] = 2;
			if ((nominator > 3 || nominator < 2) && cells[i][j] == 1)
				cells[i][j] = -1;
		}
	}

	draw(c);


	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (cells[i][j] == 2)cells[i][j] = 1;
			if (cells[i][j] == -1)cells[i][j] = 0;
		}
	}
	
}

void Automaton2D::draw(sf::Image& c) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
				int scaled_i = i * 5;
				int scaled_j = j * 5;
				for (int k = 1; k < 4; k++) {
					for (int l = 1; l < 4; l++) {
						if (cells[i][j] == 2)c.setPixel(scaled_j + k, scaled_i + l, sf::Color(255, 50, 50));
						if (cells[i][j] == -1)c.setPixel(scaled_j + k, scaled_i + l, sf::Color(0, 0, 0));
					}
				}
			

		}
	}
}

void Automaton2D::randomize(int num) {
	srand(time(NULL));

	for (int i = 0; i < num; i++) {
		cells[rand() % height][rand() % width] = 1;
	}

}

void Automaton2D::save_state_to_file() {
	std::ofstream out("gof_state.txt");
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (cells[i][j] == 1) {
				out << i << " " << j << std::endl;
			}
		}
	}
	out.close();
	std::cout << "Saved" << std::endl;

}

void Automaton2D::set_boundary_condition(int a) {
	if (a < 0 || a > 2)
		boundary = 0;
	else 
		boundary = a;
}


