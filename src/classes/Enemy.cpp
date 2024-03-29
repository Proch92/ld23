#include "../include.h"

Enemy::Enemy() {
}

void Enemy::place(int xpos, texture* t, Water* w, Bullet **b, User* u) {
	Boat::place(xpos, t, w, b);
	max_health = 300;
	health = max_health;
	tex_index = 11;
	reload.start();
	reload_time = 1000;
	user = u;
	user_direction = -1; //left default
}

void Enemy::move() {
	
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
}

void Enemy::ai() {
	//keep the distance and shot when the user is in the right distance
	int distance;
	distance = (int)x - (int)user->x;
	
	if(distance < 0)
		user_direction = 1;
	else
		user_direction = -1;
	
	if(AI_DISTANCE - abs(distance) < AI_RANGE_OK)
		if(reload.get_ticks() > reload_time)
			action();
	
	int correction;
	correction = (abs(distance) - AI_DISTANCE) * ((distance > 0)?1:(-1));
	
	if(correction > 0) {
		right_down = false;
		left_down = true;
	}
	else if(correction < 0) {
		right_down = true;
		left_down = false;
	}
	else {
		right_down = false;
		left_down = false;
	}
}

void Enemy::action() {
	reload.start();
	
	Bullet *tmp;
	
	if(!double_shot_on) {
		tmp = (Bullet*) malloc(sizeof(Bullet));
		tmp->spawn(x + (BOAT_WIDTH / 2), y + (BOAT_HEIGHT / 2) - 5, user_direction, -1, tex, 20);
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
