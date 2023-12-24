#pragma once
#include "constants.h"
#include "flagManager.h"
#include <math.h>

CAT cats[CAT_COUNT];
DOG dogs[DOG_COUNT];
MOUSE mouses[MOUSE_COUNT];
short int map[BOARD_SIZE][BOARD_SIZE];

void __initColors() {
	BLUE = al_map_rgb_f(0, 0, 1);
	GREEN = al_map_rgb_f(0, 1, 0);
	RED = al_map_rgb_f(1, 0, 0);
	MAGENTA = al_map_rgb_f(1, 0, 1);
	DOG_COLOR = al_map_rgb(93,38,137);
	MOUSE_COLOR = al_map_rgb(0, 128, 129);
	COLOR1 = al_map_rgb(212, 0, 0);
	COLOR2 = al_map_rgb(225, 44, 44);
	COLOR3 = al_map_rgb(225, 165, 0);
	COLOR4 = al_map_rgb(255, 223, 0);
	WHITE = al_map_rgb(255, 255, 255);
	BLACK = al_map_rgb(0, 0, 0);
}

void __generateRandomMap() {
	// ---- start -- reset board map and modifing two iterators
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++) for (j = 0; j < BOARD_SIZE; j++) map[i][j] = 0;
	// ---- prereserve -- the location of all cats
	map[BOARD_SIZE / 2][BOARD_SIZE / 2] = FLAG_CAT;
	// ---- init -- change the random seed
	srand(time(NULL));
	// ---- init -- dogs
	for (i = 0; i < DOG_COUNT; i++) {
		dogs[i].speed = 1;
		do {
			dogs[i].x = rand() % (BOARD_SIZE / 2) + (i % 2 ? BOARD_SIZE / 2 : 0);
			dogs[i].y = rand() % (BOARD_SIZE / 2) + (i > 1 ? BOARD_SIZE / 2 : 0);
			if (!map[dogs[i].y][dogs[i].x]) {
				map[dogs[i].y][dogs[i].x] = FLAG_DOG;
				break;
			}
		} while (1);
	}
	// ---- init -- cats and place all of them in a one place
	{
		CAT t;
		t.attackPoint = 2;
		t.defencePoint = 5;
		t.mousePoint = 0;
		t.state = 'a';
		t.x = BOARD_SIZE / 2;
		t.y = BOARD_SIZE / 2;
		for (i = 0; i < CAT_COUNT; i++) t.index = i + 1, cats[i] = t;
		cats[0].color = BLUE;
		cats[1].color = GREEN;
		cats[2].color = RED;
		cats[3].color = MAGENTA;
	}
	// ---- init -- mouses
	int k = 0; // first i split the board to 15 peaces and place mouses randomfully
	for (i = 0; i < BOARD_SIZE; i+=3) for (j = 0; j < BOARD_SIZE; j+=5,k++) 
		do {
			mouses[k].x = i + rand() % 3;
			mouses[k].y = j + rand() % 5;
			if (!map[mouses[k].y][mouses[k].x]) {
				map[mouses[k].y][mouses[k].x] = FLAG_MOUSE;
				break;
			}
		} while (1);
	// place another unplaced mouses
	for (; k < MOUSE_COUNT;k++) 
		do {
			mouses[k].x = rand() % BOARD_SIZE;
			mouses[k].y = rand() % BOARD_SIZE;
			if (!map[mouses[k].y][mouses[k].x]) {
				map[mouses[k].y][mouses[k].x] = FLAG_MOUSE;
				break;
			}
		} while (1);
}

char __noWall(int x,int y) {
	if (hasFlag(map[y][x], FLAG_RWALL) ||
		hasFlag(map[y][x], FLAG_LWALL) ||
		hasFlag(map[y][x], FLAG_UWALL) ||
		hasFlag(map[y][x], FLAG_DWALL))
		return 0;
	return 1;
}

void __generateRandomWalls() {
	srand(time(NULL));
	for (int i = 0; i < WALL_COUNT; i++) {
		int x = 1 + rand() % (BOARD_SIZE-2);
		int y = 1 + rand() % (BOARD_SIZE-2);
		if (__noWall(x, y)) {
			switch (rand() % 4) {
			case 0:
				addFlag(&map[y][x], FLAG_RWALL);
				addFlag(&map[y][x + 1], FLAG_LWALL);
				break;
			case 1:
				addFlag(&map[y][x], FLAG_LWALL);
				addFlag(&map[y][x - 1], FLAG_RWALL);
				break;
			case 2:
				addFlag(&map[y][x], FLAG_DWALL);
				addFlag(&map[y + 1][x], FLAG_UWALL);
				break;
			case 3:
				addFlag(&map[y][x], FLAG_UWALL);
				addFlag(&map[y - 1][x], FLAG_DWALL);
				break;
			}
		}
		else i--;
	}
}

void setMap() {
	float variance;
	__initColors();
	__generateRandomMap();
	__generateRandomWalls();
}