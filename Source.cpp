#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main() {


	srand((unsigned int)time(NULL));
	float road_len = 33600;
	float _width = 1280;
	float _height = 720;
	bool paused = false;

	int hlines = 240;
	int vlines = 5;

	float line_height = _height / hlines / 2;

	float speed = 0.2;
	float max_speed = 7.2;
	float distance = hlines;

	int road_separation = hlines / 6;
	int light_lines = hlines / 12;
	int trees_num = 20;

	std::vector<float> curves{ 0 };
	std::vector<float> hills{ 0 }; // must be higher than -7

	int curve_len = 600;
	int hill_len = 700;

	for (int i = 1; i < road_len / curve_len; i++) {
		int new_curve = curves[i - 1] + rand() % 15 - 7;
		if (abs(new_curve) < 19)
			curves.push_back(new_curve);
		else
			curves.push_back(19);
	}

	for (int i = 1; i < road_len / hill_len; i++) {
		int new_hill = hills[i - 1] + rand() % 11 - 5;
		if (new_hill < 5 && new_hill > -5)
			hills.push_back(new_hill);
		else
			hills.push_back(7);
	}

	std::vector<sf::Sprite> trees(trees_num);
	std::vector<long double> trees_scale(trees_num);

	float car_x = 450;
	int car_rot = 0;

	float other_pos_init = static_cast<float>(hlines) - 90.0;

	std::vector<float> cars_max_spd{ 7.0, 6.2, 5.6, 5.0, 4.0 };

	int cars_num = cars_max_spd.size();

	std::vector<float> cars_pos{};
	std::vector<float> cars_spd{};
	std::vector<float> cars_scale(cars_num);

	for (int t = cars_num - 1; t >= 0; t--) {
		cars_pos.push_back(other_pos_init + 100 * t);
		cars_spd.push_back(0.0);
	}

	bool finished = false;

	float height = 1.1;

	sf::Texture tex;
	tex.loadFromFile("tree.png");
	tex.setSmooth(false);

	sf::Sprite tree;
	tree.setTexture(tex);

	sf::Texture car_tex;
	car_tex.loadFromFile("car.png");
	sf::Sprite car;
	car.setTexture(car_tex);
	car.setTextureRect(sf::IntRect(200, 174, 40, 28));
	car.setScale(sf::Vector2f(6, 6));
	car.setPosition(sf::Vector2f(_width / 2 - 120, _height - 180));

	sf::Texture other_tex;
	other_tex.loadFromFile("enm.png");

	sf::Texture bg02tex;
	bg02tex.loadFromFile("Background02.png");
	sf::Sprite bg1;
	bg1.setTexture(bg02tex);
	bg1.setScale(_width / bg1.getTexture()->getSize().x, _height * 0.5 / bg1.getTexture()->getSize().y);

	sf::Texture bgMnt02tex;
	bgMnt02tex.loadFromFile("BackgroundMuntain02.png");
	sf::Sprite bg2;
	bg2.setTexture(bgMnt02tex);
	bg2.setScale(_width / bg2.getTexture()->getSize().x, _height * 0.6 / bg2.getTexture()->getSize().y);

	sf::Sprite bg2l;
	bg2l.setTexture(bgMnt02tex);
	bg2l.setScale(_width / bg2.getTexture()->getSize().x, _height * 0.6 / bg2.getTexture()->getSize().y);
	bg2l.setPosition(-_width, 0);

	sf::Sprite bg2r;
	bg2l.setTexture(bgMnt02tex);
	bg2l.setScale(_width / bg2.getTexture()->getSize().x, _height * 0.6 / bg2.getTexture()->getSize().y);
	bg2l.setPosition(_width, 0);

	float bg_pos = 0;

	std::vector<sf::Sprite> other(cars_num);
	for (int i = 0; i < cars_num; i++) {
		other[i].setTexture(other_tex);
		other[i].setTextureRect(sf::IntRect(200, 174, 40, 28));
		other[i].setScale(sf::Vector2f(6, 6));
		other[i].setPosition(sf::Vector2f(_width, 0));
	}
	int car_rect_y = 174;

	for (int i = 0; i < trees_num; i++) {
		trees[i] = tree;
	}

	bool last_dark = false;

	sf::RenderWindow window(sf::VideoMode(_width, _height), "OutFun");

	sf::RectangleShape rs;
	sf::RectangleShape road;
	sf::RectangleShape road_paint(sf::Vector2f(10, line_height));

	sf::Text text;
	sf::Font font;
	font.loadFromFile("PressStart2P.ttf");
	text.setFont(font);
	text.setPosition(sf::Vector2f(16, 16));
	text.setString("Speed:");

	sf::Text text_pos;
	text_pos.setFont(font);
	text_pos.setPosition(sf::Vector2f(_width - 346, 16));
	text_pos.setString("Position: null");

	sf::Text text_dist;
	text_dist.setFont(font);
	text_dist.setPosition(sf::Vector2f(16, 64));
	text_dist.setString("Distance:");

	sf::Text text_congrats;
	text_congrats.setFont(font);
	text_congrats.setPosition(sf::Vector2f(_width / 2 - 306, 128));
	text_congrats.setCharacterSize(46);
	text_congrats.setString("Race finished!");

	sf::RectangleShape spd_meter(sf::Vector2f(20, 20));
	spd_meter.setPosition(sf::Vector2f(196, 19));
	sf::RectangleShape spd_meter_max(sf::Vector2f(20, 20));
	spd_meter_max.setPosition(sf::Vector2f(max_speed * 30 + 196, 19));
	spd_meter_max.setFillColor(sf::Color(200, 200, 200, 255));

	sf::RectangleShape dist_meter(sf::Vector2f(20, 20));
	dist_meter.setPosition(sf::Vector2f(290, 67));

	sf::RectangleShape dist_meter_max(sf::Vector2f(20, 20));
	dist_meter_max.setPosition(sf::Vector2f(412, 67));
	dist_meter_max.setFillColor(sf::Color(200, 200, 200, 255));
	
	window.setFramerateLimit(60);

	sf::Text diff_menu;
	diff_menu.setFont(font);
	diff_menu.setPosition(sf::Vector2f(_width / 2 - 446, _height / 2));
	diff_menu.setFillColor(sf::Color::Black);
	diff_menu.setString("Select difficulty:  1  2  3  Inf");

	sf::Text title;
	title.setFont(font);
	title.setPosition(sf::Vector2f(_width / 2 - 196, _height / 4));
	title.setFillColor(sf::Color::Black);
	title.setString("OutFun");
	title.setCharacterSize(56);

	sf::Text picker;
	picker.setFont(font);
	picker.setFillColor(sf::Color::Black);
	picker.setPosition(sf::Vector2f(_width / 2 + 211, _height / 2));
	picker.setString(">");

	int picker_pos = 1;
	float difficulty = 1;

	bool infinite = false;
	bool started = false;
	
	sf::Clock clock;
	float lastTime = 0;

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
			{
				paused = paused ? false : true;
			}

			if (started) {
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W && speed < max_speed)
				{
					speed += 0.05;

				}
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S && speed >= 0.05)
				{
					speed -= 0.05;
				}


				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D && car_x < 555)
				{
					car_x += 4.9 * (speed + 0.1);
					if (car_rot < 10)
						car_rot = 10;
					if (car_rot < 21)
						car_rot += 5;
				}

				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A && car_x > -555)
				{
					car_x -= 4.9 * (speed + 0.1);
					if (car_rot > -10)
						car_rot = -10;
					if (car_rot > -21)
						car_rot -= 5;
				}
			}
			if (!started) {
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D)
				{
					picker_pos += 1;
					picker_pos %= 4;
				}

				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A)
				{
					picker_pos -= 1;
					picker_pos = (picker_pos + 4) % 4;
				}

				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
				{
					if (picker_pos == 0) difficulty = 0.7;
					if (picker_pos == 2) difficulty = 1.2;
					if (picker_pos == 3) infinite = true;
					started = true;
				}

				picker.setPosition(sf::Vector2f(_width / 2 + 121 + 90 * picker_pos, _height / 2));
			}
		}



		if (!started) {
			window.clear(sf::Color(255, 255, 255, 255));
			window.draw(diff_menu);
			window.draw(title);
			window.draw(picker);
			window.display();
		}

		if (!paused && started) {
			window.clear(sf::Color(122, 178, 255, 255));

			window.draw(bg1);
			
			bg2.setPosition(bg_pos + _width / 2, 0);
			bg2l.setPosition(bg_pos - _width / 2, 0);
			bg2r.setPosition(bg_pos + 1.5 * _width, 0);

			window.draw(bg2);
			window.draw(bg2l);
			window.draw(bg2r);

			if (abs(car_x) > 520 && speed >= max_speed / 2) speed -= 0.07;
			spd_meter.setSize(sf::Vector2f(speed * 30, 20));
			if (!infinite)
				dist_meter.setSize(sf::Vector2f((distance - hlines) / road_len * 122, 20));

			int car_rect_x = 200 + 40 * (car_rot / 9);
			car.setTextureRect(sf::IntRect(car_rect_x, car_rect_y, 40, 28));
			car_rect_y = 174;
			if (car_rot > 0) car_rot -= 1;
			if (car_rot < 0) car_rot += 1;

			float road_delta = 0;
			float road_dd = 0;
			float road_ddd = 0;
			distance += speed;

			int current_hill = 0;
			float line_number = hlines;
			float hill_d = 0;
			float hill_dd = 0;
			float hill_ddd = 0;

			int trees_counter = 0;

			float min_line_y = _height;
			float last_y = _height;

			if (speed < max_speed - 0.1) {
				speed += 0.02;
			}

			int pos = cars_num + 1;

			for (int i = 0; i < cars_num; i++) {
				if (cars_spd[i] < cars_max_spd[i] && cars_spd[i] < max_speed * 0.95)
					cars_spd[i] += 0.03;
				cars_pos[i] += cars_spd[i] * difficulty;
				if (abs(cars_pos[i] - distance + hlines - 28 * cars_scale[i]) < 3 && ((i % 2 == 0 && car_x + 40 > 0) || (i % 2 && car_x - 40 <= 0)))
					speed *= 0.8;
				if (cars_pos[i] < distance - hlines)
					pos -= 1;
				float delta_dist = distance - cars_pos[i];
				cars_scale[i] = 22 * (delta_dist / _height);
				float delta_y = delta_dist - 28 * cars_scale[i];
				if (delta_y > 0) {
					float other_y = _height / 2 + delta_y;
					other[i].setScale(sf::Vector2f(cars_scale[i], cars_scale[i]));
					other[i].setPosition(sf::Vector2f(_width, other_y));
				}
				else
					other[i].setPosition(sf::Vector2f(_width, -1000));
			}

			text_pos.setString("Position: " + std::to_string(pos));

			if (distance - hlines >= road_len && !infinite) {
				if (speed >= 0.07)
					speed -= 0.07;
				window.draw(text_congrats);
				finished = true;
			}

			if (distance > 1000000) distance -= 1000000;

			for (float hl = hlines; hl > 0; hl--)
			{
				float line_y = last_y - line_height;
				last_y = line_y;

				float scale_factor = pow(_height / (line_y - 3), 1.6);

				int z_coord = static_cast<int>(distance - hl * scale_factor);

				current_hill = (static_cast<int>(distance - 3 * hl * scale_factor) / hill_len) % hills.size();

				if (infinite && current_hill == 0) hills.push_back(rand() % 15 - 7);

				hill_d -= (hills[current_hill]) * difficulty / 2400 * scale_factor * scale_factor * scale_factor * scale_factor * scale_factor * scale_factor * scale_factor;
				hill_dd += hill_d / 250;
				hill_ddd += hill_dd / 200;

				int line_num = (z_coord % road_separation);
				bool lighten = line_num <= light_lines;
				int col = lighten ? 215 : 192;

				rs.setSize(sf::Vector2f(_width, line_height));
				rs.setFillColor(sf::Color(0, col, 0, 255));

				float road_width = _width * (hl / hlines + 0.005);

				road.setSize(sf::Vector2f(road_width, line_height));
				col *= 0.5;
				if (z_coord >= road_len && !infinite) {
					col = lighten ? 255 : 0;
				}
				road.setFillColor(sf::Color(col, col, col, 255));

				float road_x = _width / 2 - road_width / 2;
				road_x += car_x * (-hl / hlines);
				int current_curve = (static_cast<int>(distance - 1.2 * hl * scale_factor) / curve_len) % curves.size();

				if (infinite && current_curve == 0) curves.push_back(rand() % 19 - 9);

				road_delta = curves[current_curve] * difficulty;
				road_dd += road_delta / 100 * (scale_factor * scale_factor * scale_factor * scale_factor);
				road_ddd += road_dd / 100;
				road_x += road_ddd;

				line_height = _height / hlines / 2 + hill_ddd;

				if (line_y > min_line_y) {
					continue;
				}

				rs.setPosition(sf::Vector2f(0, line_y));

				road.setPosition(sf::Vector2f(road_x, line_y));

				window.draw(rs);
				window.draw(road);

				min_line_y = line_y;

				road_paint.setSize(sf::Vector2f(road_width / 40, line_height));
				for (int k = 0; k < cars_num; k++) {
					if (abs(line_y - other[k].getPosition().y - 28 * cars_scale[k]) < 1.0)
						other[k].setPosition(sf::Vector2f(road_x + road_width * 0.6 - (0.55 * road_width * (k % 2)), line_y));
				}

				if (lighten) {
					road_paint.setFillColor(sf::Color::White);
					road_paint.setPosition(sf::Vector2f(road_x + road_width * 39 / 80, line_y));
					window.draw(road_paint);

					road_paint.setPosition(sf::Vector2f(road_x, line_y));
					window.draw(road_paint);

					road_paint.setPosition(sf::Vector2f(road_x + road_width * 39 / 40, line_y));
					window.draw(road_paint);

					if (last_dark == lighten) {
						long double tree_scale = 12 * sqrt(hl / hlines);
						long double tree_y = line_y - tree.getTexture()->getSize().y * tree_scale;
						long double tree_x = road_x - tree.getTexture()->getSize().x * tree_scale;
						trees[trees_counter].setPosition(sf::Vector2f(tree_x, tree_y));
						trees[trees_counter].setScale(sf::Vector2f(tree_scale, tree_scale));
						trees_counter += 1;
						trees_counter %= trees_num;

						tree_x = road_x + road_width;
						trees[trees_counter].setPosition(sf::Vector2f(tree_x, tree_y));
						trees[trees_counter].setScale(sf::Vector2f(tree_scale, tree_scale));
						trees_counter += 1;
						trees_counter %= trees_num;
					}
				}
				else
				{
					road_paint.setFillColor(sf::Color::Red);
					road_paint.setPosition(sf::Vector2f(road_x, line_y));
					window.draw(road_paint);

					road_paint.setPosition(sf::Vector2f(road_x + road_width * 39 / 40, line_y));
					window.draw(road_paint);
				}

				last_dark = !lighten;
			}

			float delta_x = road_ddd / 20 * speed / max_speed;
			if (abs(car_x - delta_x) < 555)
				car_x -= delta_x;

			bg_pos -= delta_x;
			if (bg_pos >= _width / 2) bg_pos -= _width;
			if (bg_pos <= -_width / 2) bg_pos += _width;

			if (hills[current_hill] < 0)
				car_rect_y = 302;

			for (int i = trees_counter - 1; i >= 0; i--) {
				window.draw(trees[i]);
			}

			for (int c_car = 0; c_car < cars_num; c_car++) {
				window.draw(other[c_car]);
			}

			window.draw(car);
			window.draw(spd_meter_max);
			window.draw(spd_meter);
			window.draw(dist_meter_max);
			window.draw(dist_meter);
			window.draw(text_dist);
			window.draw(text);
			window.draw(text_pos);

			window.display();


			/*float currentTime = clock.getElapsedTime().asSeconds();
			clock.restart();
			float fps = 1.f / (currentTime);
			if (fps < 110)
				std::cout << fps << '\n';*/
		}
	}
}
