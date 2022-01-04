#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <ncurses.h>
#include "monster.h"
using namespace std; //Boo hiss

class Map {
	vector<Monster> m;
	vector<vector<char>> map;
	default_random_engine gen;
	public:
	static const char HERO     = 'H';
	static const char MONSTER  = 'M';
	static const char WALL     = '#';
	static const char WATER    = '~';
	static const char OPEN     = '.';
	static const char TREASURE = '+';
	static const size_t SIZE = 100; //World is a 100x100 map
	static const size_t DISPLAY = 20; //Show a 10x10 area at a time
	//Randomly generate map
	void init_map() {
		uniform_int_distribution<int> d100(1,100);
		map.clear();
		map.resize(SIZE); //100 rows tall
		for (auto &v : map) v.resize(SIZE,'.'); //100 columns wide
		for (size_t i = 0; i < SIZE; i++) {
			for (size_t j = 0; j < SIZE; j++) {
				//Line the map with walls
				if (i == 0 or j == 0 or i == SIZE-1 or j == SIZE-1) 
					map.at(i).at(j) = WALL;
				else if (i == SIZE/2 and j == SIZE/2) 
					map.at(i).at(j) = HERO;
				else {
					//5% chance of monster
					if (d100(gen) <= 5) {
						Monster temp(i,j);
						map.at(i).at(j) = MONSTER;
						m.push_back(temp);
					}
					else if (d100(gen) <= 3) {
						map.at(i).at(j) = TREASURE;
					}
					else if (d100(gen) <= 5) { //5% each spot is wall
						map.at(i).at(j) = WALL;
					}
					else if (d100(gen) <= 1) { //1% each spot is water
						map.at(i).at(j) = WATER;
					}
					else if (d100(gen) <= 50) { //50% chance of water near other water
						if (map.at(i-1).at(j) == WATER or map.at(i+1).at(j) == WATER or map.at(i).at(j-1) == WATER or map.at(i).at(j+1) == WATER)
							map.at(i).at(j) = WATER;
					}
					else if (d100(gen) <= 30) { //30% chance of walls near other walls
						if (map.at(i-1).at(j) == WALL or map.at(i+1).at(j) == WALL or map.at(i).at(j-1) == WALL or map.at(i).at(j+1) == WALL)
							map.at(i).at(j) = WALL;
					}
				}
			}
		}
	}
	//Draw the DISPLAY tiles around coordinate (x,y)
	void draw(int x, int y) {
		int start_x = x - DISPLAY;
		int end_x = x + DISPLAY;
		int start_y = y - DISPLAY/2;
		int end_y = y + DISPLAY/2;

		//Bounds check to handle the edges
		if (start_x < 0) {
			end_x = end_x - start_x;
			start_x = 0;
		}
		if (end_x > SIZE-1) {
			start_x = start_x - (end_x - (SIZE-1));
			end_x = SIZE-1;
		}
		if (start_y < 0) {
			end_y = end_y - start_y;
			start_y = 0;
		}
		if (end_y > SIZE-1) {
			start_y = start_y - (end_y - (SIZE-1));
			end_y = SIZE-1;
		}
		//Now draw the map using NCURSES
		for (size_t i = start_y; i <= end_y; i++) {
			for (size_t j = start_x; j <= end_x; j++) {
				//if (i == cursor_x && j == cursor_y)
				//	attron(A_UNDERLINE | A_BOLD);
				int color = 1;
				if (map.at(i).at(j) == WALL)
					color = 5;
				else if (map.at(i).at(j) == WATER)
					color = 2;
				else if (map.at(i).at(j) == HERO)
					color = 3;
				else if (map.at(i).at(j) == TREASURE)
					color = 4;
				else if (map.at(i).at(j) == MONSTER)
					color = 6;

				attron(COLOR_PAIR(color));
				mvaddch(i-start_y,j-start_x,map.at(i).at(j));
				attroff(COLOR_PAIR(color));
				//attroff(A_UNDERLINE | A_BOLD);
			}
		}
	}	
	void set(int x, int y, char c) {
		for (unsigned int i =0; i < SIZE;i++){
			for (unsigned int j =0; j < SIZE;j++){
				if (map.at(i).at(j)=='H')
					map.at(i).at(j)='.';
			}
		}

		map.at(y).at(x) = c;
	}
	char get(int x, int y) {
		return map.at(y).at(x);
	}
	int monster_count() { return m.size();}
	void monster_move() {
		for (size_t i = 0; i < m.size(); i++){
			int x = rand() % 4;
			int first = m.at(i).get_x();
			int second = m.at(i).get_y();
			if (x == 0) {
				if (m.at(i).get_y() < SIZE - 2){
					if (map.at(first).at(second+1) == OPEN) {
						map.at(first).at(second) = OPEN;
						map.at(first).at(second+1)= MONSTER;
						m.at(i).set_y(second+1);
					}	
				}
			}
			else if (x == 1) {
				if (m.at(i).get_x() < SIZE - 2){
					if (map.at(first+1).at(second) == OPEN) {
						map.at(first).at(second)= OPEN;
						map.at(first+1).at(second)= MONSTER;
						m.at(i).set_x(first+1);
					}	
				}
			}
			else if (x == 2) {
				if (m.at(i).get_y() > 1){
					if (map.at(first).at(second-1) == OPEN) {
						map.at(first).at(second) = OPEN;
						map.at(first).at(second-1)= MONSTER;
						m.at(i).set_y(second-1);
					}	
				}
			}
			else if (x == 3) {
				if (m.at(i).get_x() > 1) {
					if (map.at(first-1).at(second) == OPEN) {
						map.at(first).at(second)= OPEN;
						map.at(first-1).at(second)= MONSTER;
						m.at(i).set_x(first-1);
					}		
				}
			}
		}
	}
	Map() {
		init_map();
	}
};
