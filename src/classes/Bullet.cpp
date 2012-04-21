#include "../include.h"

Bullet::Bullet() {
	
}

void Bullet::spawn(float xpos, float ypos, float xv, float yv, texture* t, int t_i) {
	x = xpos;
	y = ypos;
	xvel = xv;
	yvel = yv;
	tex = t;
	tex_index = t_i;
}

void Bullet::move() {
	yvel += GRAVITY;
	
	x += xvel;
	y += yvel;
}

void Bullet::show() {
	apply_surface((int)x, (int)y, BULLET_WIDTH, BULLET_WIDTH, tex[tex_index]);
}
