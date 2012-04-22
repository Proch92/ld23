#include "../include.h"

Pup::Pup() {
	
}

void Pup::spawn(int xpos, texture* t, int t_p) {
	x = xpos;
	y = 0;
	tex = t;
	type = t_p;
	tex_index = 15 + type;
}

void Pup::show() {
	apply_surface(x, y, BULLET_WIDTH, BULLET_WIDTH, tex[tex_index]);
}

void Pup::move() {
	y++;
}

void Pup::pickup(void* boat) {
	switch(type) {
		case 0:
			((Boat*)boat)->give_health(200);
			break;
		case 1:
			((Boat*)boat)->max_health += 100;
			break;
		case 2:
			((Boat*)boat)->double_shot();
			break;
		case 4:
			((Boat*)boat)->give_health(-10000);
			break;
		default: break;
	}
}
