//Shami Heckathorne
#include "map.h"
#include "hero.h"
#include "monster.h"
#include <unistd.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

const unsigned int TIMEOUT = 10; //Milliseconds to wait for a getch to finish
const int UP = 65; //Key code for up arrow
const int DOWN = 66;
const int LEFT = 68;
const int RIGHT = 67;
int monster_count = 20;
Hero h;
vector<Hero> party(1);

void go_red();
void combat();
void win();
void loss();
void load();
void save();

void turn_on_ncurses() {
	initscr();//Start curses mode
	start_color(); //Enable Colors if possible
	//Formatting for colors: Char color, background color
	init_pair(1, COLOR_GREEN, COLOR_GREEN); //Set up some color pairs
	init_pair(2, COLOR_CYAN, COLOR_BLUE);
	init_pair(3, COLOR_BLACK, COLOR_GREEN);
	init_pair(4, COLOR_BLACK, COLOR_YELLOW);
	init_pair(5, COLOR_BLACK, COLOR_RED);
	init_pair(6, COLOR_MAGENTA, COLOR_GREEN);
	clear();
	noecho();
	cbreak();
	timeout(TIMEOUT); //Set a max delay for key entry
}

int main() {
	int choice;
	cout << "Press 1 to begin anew. Press 2 to load in.\n";
	cin >> choice;
	if (choice == 2) {
		load();
	}
	turn_on_ncurses();
	Map map;
	int monster_move = 0;
	int x = Map::SIZE / 2, y = Map::SIZE / 2; //Start in middle of the world
	while (true) {
		monster_move++;
		int ch = getch(); // Wait for user input, with TIMEOUT delay
		if (h.get_health() < 1) {
			go_red();
			refresh();
			sleep(5);
			break;
		}
		if (monster_count < 1) break;
		if (ch == 'q' || ch == 'Q') break;
		else if (ch == RIGHT) {
			x++;
			if (map.get(x, y) == Map::TREASURE) {
				int temp = h.get_health() + 10;
				if (temp > h.get_max()) temp = h.get_max();
				h.set_health(temp);
			}
			if (map.get(x, y) == Map::MONSTER)combat();
			if (map.get(x, y) == Map::WALL or map.get(x, y) == Map::WATER) x--;
			if (y >= Map::SIZE) y = Map::SIZE - 1; //Clamp value
		} else if (ch == LEFT) {
			x--;
			if (map.get(x, y) == Map::TREASURE) {
				int temp = h.get_health() + 10;
				if (temp > h.get_max()) temp = h.get_max();
				h.set_health(temp);
			}
			if (map.get(x, y) == Map::MONSTER) combat();
			if (map.get(x, y) == Map::WALL or map.get(x, y) == Map::WATER) x++;
			if (y < 0) y = 0;
		} else if (ch == UP) {
			y--;
			if (map.get(x, y) == Map::TREASURE) {
				int temp = h.get_health() + 10;
				if (temp > h.get_max()) temp = h.get_max();
				h.set_health(temp);
			}
			if (map.get(x, y) == Map::MONSTER) combat();
			if (map.get(x, y) == Map::WALL or map.get(x, y) == Map::WATER) y++;
			if (x < 0) x = 0;
		} else if (ch == DOWN) {
			y++;
			if (map.get(x, y) == Map::TREASURE) {
				int temp = h.get_health() + 10;
				if (temp > h.get_max()) temp = h.get_max();
				h.set_health(temp);
			}
			if (map.get(x, y) == Map::MONSTER)combat();
			if (map.get(x, y) == Map::WALL or map.get(x, y) == Map::WATER) y--;
			if (x >= Map::SIZE) x = Map::SIZE - 1; //Clamp value
		} else if (ch == ERR) { //No keystroke
			; //Do nothing
		} else if (ch == 'p') {
//			clear();
//			map.draw(x,y);
			while (true) {
				ch = getch();
				mvprintw(10, 15, "GAME PAUSED");
				mvprintw(21, 0, "Click S to save and quit");
				mvprintw(22, 0, "Click Q to quit\t\t\t\t\t\t\t\t\t\t\t");
				mvprintw(23, 0, "\t\t\t\t\t");
				if (ch == ERR) {
					;
				} else if (ch == 'p') break;
				else if (ch == 's') {
					save();
					clear();
					endwin();
					system("clear");
					exit(0);
				} else if (ch == 'q') {
					monster_count = 0;
					h.set_speed(0);
					save();
					clear();
					endwin();
					system("clear");
					exit(0);
				}
			}
		}
//		clear();
		map.set(x, y, 'H');
		map.draw(x, y);
		mvprintw(21, 0, "X: %i Y: %i\n", x, y);
//		for (int i = 0; i < party.size(); i++) {
//			mvprintw(22 + i, 0, "%s Health: %i %s Speed: %i %s LVL: %i %s EXP: %i/%i\n", party.at(i).get_name, party.at(i).get_health(), party.at(i).get_name, party.at(i).get_speed(), party.at(i).get_name, party.at(i).get_lvl(), party.at(i).get_name, party.at(i).get_exp(), party.at(i).get_name, party.at(i).get_lvlup());
//		}
		mvprintw(22, 0, "Player Health: %i Player Speed: %i Player LVL: %i Player EXP: %i/%i\n", h.get_health(), h.get_speed(), h.get_lvl(), h.get_exp(), h.get_lvlup());
		mvprintw(23, 0, "Monster kills needed to win: %i\n", monster_count);
		mvprintw(0, 41, "Click P to pause");
		if (monster_move % 50 == 0) map.monster_move();
//		refresh();
//		usleep(5000);
	}
	clear();
	endwin(); // End curses mode
	system("clear");
	if (h.get_health() < 1) loss();
	if (monster_count < 1) win();
}

void go_red() {
	init_pair(1, COLOR_RED, COLOR_RED);
	init_pair(2, COLOR_RED, COLOR_RED);
	init_pair(3, COLOR_RED, COLOR_RED);
	init_pair(4, COLOR_RED, COLOR_RED);
	init_pair(5, COLOR_RED, COLOR_RED);
	init_pair(6, COLOR_RED, COLOR_RED);
	mvprintw(10, 15, "GAME OVER");
}

void load() {
	ifstream load;
	int temp = 0;
	load.open("save.txt");
	if (load.is_open()) {
		load >> monster_count;
		load >> temp;
		if (monster_count == 0 and temp == 0) {
			cout << "No save data found!" << endl;
			exit(0);
		}
		h.set_speed(temp);
		load >> temp;
		h.set_health(temp);
		load >> temp;
		h.set_max(temp);
		load >> temp;
		h.set_lvl(temp);
		load >> temp;
		h.set_exp(temp);
		load >> temp;
		h.set_lvlup(temp);
	} else cout << "File not found!" << endl;
	load.close();
}

void save() {
	ofstream save;
	save.open("save.txt");
	if (save.is_open()) {
		save << monster_count << endl;
		save << h.get_speed() << endl;
		save << h.get_health() << endl;
		save << h.get_max() << endl;
		save << h.get_lvl() << endl;
		save << h.get_exp() << endl;
		save << h.get_lvlup() << endl;
	} else {
		mvprintw(25, 0, "File save failed");
	}
	save.close();
}

void level_up() {
	int max = h.get_max() * 1.2;
	int newExp = h.get_lvlup() * 1.3;
	int newSpeed = h.get_speed() * 1.3;
	h.set_max(max);
	h.set_health(max);
	h.set_lvlup(newExp);
	h.set_speed(newSpeed);
	h.set_exp(0);
	h.set_lvl(h.get_lvl() + 1);
}

void combat() {
	clear();
	mvprintw(24, 0, "You encountered an enemy\n");
	srand(time(0));
	int monster_speed = rand() % (21 - 10) + 10;
	int monster_attack = rand() % (12 - 5) + 5;
	int hero_health;
	int monster_health = 15;
	int coin = 0;
	if (h.get_speed() == monster_speed) coin = rand() % 2;
	while (true) {
		if (h.get_health() < 1 or monster_health < 1) break;
		if (h.get_speed() > monster_speed or coin == 1) {
			mvprintw(25, 0, "The hero is faster and attacks first\n");
			monster_health = monster_health - h.get_attack();
			if (monster_health < 1) {
				mvprintw(26, 0, "The monster has fainted\n");
				int xp = h.get_exp() + (rand() % 10) + 10;
				h.set_exp(xp);
				if (h.get_exp() >= h.get_lvlup()) level_up();
				monster_count--;
				break;
			}
			if (monster_health > 1) {
				mvprintw(26, 0, "The monster has %i health left\n", monster_health);
				mvprintw(27, 0, "Now the monster attacks\n");
			}
			hero_health = h.get_health();
			hero_health = hero_health - monster_attack;
			h.set_health(hero_health);
			if (h.get_health() < 1) {
				hero_health = 0;
				h.set_health(hero_health);
				mvprintw(29, 0, "THE HERO HAS FAINTED, CLOSING IN 5 SECONDS\n");
				break;
			}
			if (h.get_health() > 1)mvprintw(28, 0, "The hero has %i health left\n", h.get_health());
		}
		if (monster_speed > h.get_speed() or coin == 2) {
			mvprintw(25, 0, "The monster is faster and attacks first\n");
			hero_health = h.get_health();
			hero_health = hero_health - monster_attack;
			h.set_health(hero_health);
			if (h.get_health() < 1) {
				hero_health = 0;
				h.set_health(hero_health);
				mvprintw(29, 0, "THE HERO HAS FAINTED, CLOSING IN 5 SECONDS\n");
				break;
			}
			if (h.get_health() > 1) {
				mvprintw(26, 0, "The hero has %i health left\n", h.get_health());
				mvprintw(27, 0, "Now the hero attacks\n");
			}
			monster_health = monster_health - h.get_attack();
			if (monster_health < 1) {
				mvprintw(28, 0, "The monster has fainted\n");
				int xp = h.get_exp() + (rand() % 10) + 10;
				h.set_exp(xp);
				if (h.get_exp() >= h.get_lvlup()) level_up();
				monster_count--;
				break;
			}
			if (monster_health > 0) mvprintw(28, 0, "The monster has %i health left\n", monster_health);
		}
	}
}
void loss() {
	system("figlet bruh.mp4 | lolcat");

}

void win() {
	system("figlet WINNER | lolcat");
}
