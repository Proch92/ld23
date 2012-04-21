#include "../include.h"

Boat::Boat() {
}

void Boat::place(int xpos, texture* t, Water* w) {
	x = xpos;
	alive = true;
	tex_index = 10;
	tex = t;
	water = w;
}

void Boat::show() {
	apply_surface(x, y, BOAT_WIDTH, BOAT_HEIGHT, tex[tex_index]);
}

void Boat::floatUp() {
	int sum = 0;
	int i;
	
	for (i=0; i!=BOAT_WIDTH; i++)
		sum += (*water)[i + x - ((SCREEN_WIDTH - WATER_WIDTH) / 2)];
	
	int avg = sum / BOAT_WIDTH;
	
	y = (SCREEN_HEIGHT / 2) - (BOAT_HEIGHT / 2) - avg;
}