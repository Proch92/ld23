#include "../include.h"

User::User() {
}

void User::place(int xpos, SDL_Event *e, texture* t, Water* w, struct list **b) {
	Boat::place(xpos, t, w, b);
	event = e;
	max_health = USER_INIT_HEALTH;
	health = max_health;
	tex_index = 10;
	right_down = false;
	left_down = false;
	action_down = false;
	reload.start();
	reload_time = 1000;
}

void User::move() {
	
	if(right_down)
		vel += BOAT_ACCELL;
	if(left_down)
		vel -= BOAT_ACCELL;
	
	if(vel != 0) {
		if(vel < 0 && x - vel < (SCREEN_WIDTH - WATER_WIDTH) / 2)
			vel = 0;
		if(vel > 0 && x + vel > WATER_WIDTH + ((SCREEN_WIDTH - WATER_WIDTH) / 2) - BOAT_WIDTH)
			vel = 0;
	}
	
	if(!right_down && !left_down) {
		if(vel > 0)
			vel -= WATER_FRICTION;
		if(vel < 0)
			vel += WATER_FRICTION;
	}
	x += vel;
	
	if(right_down xor left_down) {
		if(right_down)
			water->giveInput(x - ((SCREEN_WIDTH - WATER_WIDTH) / 2), -1);
		else
			water->giveInput(x + BOAT_WIDTH - ((SCREEN_WIDTH - WATER_WIDTH) / 2), -1);
	}
	
	//action
	if(action_down && reload.get_ticks() > reload_time) {
		action();
		reload.start();
	}
}

void User::handle_input() {
	if(event->type == SDL_KEYDOWN)
		switch(event->key.keysym.sym) {
			case SDLK_d: right_down = true; break;
			case SDLK_a: left_down = true; break;
			case SDLK_j: action_down = true; break;
			default: break;
		}
	if(event->type == SDL_KEYUP)
		switch(event->key.keysym.sym) {
			case SDLK_d: right_down = false; break;
			case SDLK_a: left_down = false; break;
			case SDLK_j: action_down = false; break;
			default: break;
		}
}

void User::action() {
	Bullet *tmp;
	tmp = (Bullet*) malloc(sizeof(Bullet));
	tmp->spawn(x + (BOAT_WIDTH / 2), y + (BOAT_HEIGHT / 2), (vel>0)?1:(-1), -1, tex, 20);
	struct list *new_elem;
	new_elem = (struct list*) malloc(sizeof(struct list));
	new_elem->data = tmp;
	new_elem->next = *bullets;
	*bullets = new_elem;
}
