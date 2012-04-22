#include "../include.h"

User::User() {
}

void User::place(int xpos, SDL_Event *e, texture* t, Water* w, Bullet **b) {
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
	
	//stop double shot powerup
	if(pup_timer.get_ticks() > DOUBLE_SHOT_TIME) {
		double_shot_on = false;
		pup_timer.stop();
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
	
	if(!double_shot_on) {
		int direction = -1; //left default
		if(right_down && !left_down)
			direction = 1; //right
		
		tmp = (Bullet*) malloc(sizeof(Bullet));
		tmp->spawn(x + (BOAT_WIDTH / 2), y + (BOAT_HEIGHT / 2) - 5, direction, -1, tex, 20);
		tmp->next = *bullets;
		*bullets = tmp;
	}
	else {
		tmp = (Bullet*) malloc(sizeof(Bullet));
		tmp->spawn(x + (BOAT_WIDTH / 2), y + (BOAT_HEIGHT / 2) - 5, 1, -1, tex, 20);
		tmp->next = *bullets;
		*bullets = tmp;
		
		tmp = (Bullet*) malloc(sizeof(Bullet));
		tmp->spawn(x + (BOAT_WIDTH / 2), y + (BOAT_HEIGHT / 2) - 5, -1, -1, tex, 20);
		tmp->next = *bullets;
		*bullets = tmp;
	}
}


void User::show() {
	Boat::show();
	
	//health bar
	int xb, yb, wb, hb;
	
	xb = x;
	yb = y - 5;
	hb = 3;
	wb = (health * BOAT_WIDTH) / USER_INIT_HEALTH;
	
	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0, 0.0, 0.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex2i(xb, yb);
		glVertex2i(xb + wb, yb);
		glVertex2i(xb + wb, yb + hb);
		glVertex2i(xb, yb + hb);
	glEnd();
	glColor4f(1.0, 1.0, 1.0, 1.0); //normal color
	glEnable(GL_TEXTURE_2D);
}
