#pragma once
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>

class Monster {
		int x;
		int y;
		int SPEED;
		int HEALTH;
	public:
		Monster (int new_x, int new_y, int new_SPEED =  rand() % (21 - 10) + 10, int new_HEALTH = 10) : x(new_x), y(new_y), SPEED(new_SPEED), HEALTH(new_HEALTH){}
		int get_x() { return x; }
		int get_y() { return y; }
		int get_speed() { return SPEED; }
		int get_health() { return HEALTH; }
		void set_speed (int new_speed) { SPEED = new_speed; }
		void set_health (int new_health) { HEALTH = new_health; }
		void set_x (int new_x) { x = new_x; }
		void set_y (int new_y) { y = new_y; }
};
