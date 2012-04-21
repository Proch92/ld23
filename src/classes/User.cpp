#include "../include.h"

User::User() {
}

void User::place(int xpos, SDL_Event *e, texture* t, Water* w) {
	Boat::place(xpos, t, w);
	event = e;
	max_health = USER_INIT_HEALTH;
	health = max_health;
	tex_index = 10;
}

void User::handle_input() {
	
}
