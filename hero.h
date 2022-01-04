#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
class Hero {
		string name;
		int SPEED;
		int HEALTH;
		int MAX_HEALTH;
		int ATTACK;
		int EXP;
		int LVL;
		int LVL_UP;
	public:
		Hero () {
			name = "";
			SPEED = 15;
			HEALTH = 25;
			MAX_HEALTH = 25;
			ATTACK = rand() % (11 - 5) + 5;
			EXP = 0;
			LVL = 1;
			LVL_UP = 20;
		}
		Hero (string new_name, int new_SPEED = 15, int new_HEALTH = 25, int new_max = 25, int new_attack = rand() % (11 - 5) + 5): name(new_name), SPEED(new_SPEED), HEALTH(new_HEALTH), MAX_HEALTH(new_max), ATTACK(new_attack){}
		int get_speed() { return SPEED; }
		int get_health() { return HEALTH; }
		int get_max() { return MAX_HEALTH; }
		int get_attack() { return ATTACK; }
		int get_exp() { return EXP; }
		int get_lvl() { return LVL; }
		int get_lvlup() { return LVL_UP; }
		void set_speed (int new_speed) { SPEED = new_speed; }
		void set_health (int new_health) { HEALTH = new_health; }
		void set_max (int new_max) { MAX_HEALTH = new_max; }
		void set_exp (int new_exp) { EXP = new_exp; }
		void set_lvl (int new_lvl) { LVL = new_lvl; }
		void set_lvlup (int new_lvlup) { LVL_UP = new_lvlup; }
};
