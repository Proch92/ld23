#include "../include.h"

Pup::Pup() {
	
}

void Pup::spawn(int xpos, texture* t, int t_i) {
	x = xpos;
	y = 0;
	tex = t;
	type = t_i;
	tex_index = 15 + type;
}

void Pup::show() {
	apply_surface(x, y, BULLET_WIDTH, BULLET_WIDTH, tex[tex_index]);
}

void Pup::move() {
	y++;
}

void Pup::pickup(void*) {
	
}
