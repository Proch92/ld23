#include "../include.h"

Enemy::Enemy() {
}

void Enemy::place(int xpos, texture* t, Water* w, struct list **b) {
	Boat::place(xpos, t, w, b);
	max_health = 1000;
	health = max_health;
	tex_index = 11;
	reload.start();
	reload_time = 1000;
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
			water->giveInput(x - ((SCREEN_WIDTH - WATER_WIDTH) / 2), -2);
		else
			water->giveInput(x + BOAT_WIDTH - ((SCREEN_WIDTH - WATER_WIDTH) / 2), -2);
	}
}

void Enemy::ai() {
	//OMG!!
	//keep the distance and shot when the user is in the right distance
}

void Enemy::action() {
	reload.start();
	
	Bullet *tmp;
	tmp = (Bullet*) malloc(sizeof(Bullet));
	tmp->spawn(x + (BOAT_WIDTH / 2), y + (BOAT_HEIGHT / 2), (vel>0)?1:(-1), -1, tex, 20);
	struct list *new_elem;
	new_elem = (struct list*) malloc(sizeof(struct list));
	new_elem->data = tmp;
	new_elem->next = *bullets;
	*bullets = new_elem;
}