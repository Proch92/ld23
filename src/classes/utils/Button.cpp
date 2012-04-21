#include <string>
#include "../../include.h"

Button::Button(int xx, int yy, int ww, int hh, texture *Texture) {
	x = xx;
	y = yy;
	w = ww;
	h = hh;
	tex = Texture;
	reset();
}

void Button::reset() {
	focused = false;
	clicked = false;
}

bool Button::check_inside(int xx, int yy) {
	return (xx > x && xx < x + w && yy > y && yy < y + h);
}

bool Button::handle_input(SDL_Event* event) {
	bool actived = false;
	if(event->type == SDL_MOUSEMOTION) {
		if(check_inside(event->motion.x, event->motion.y)) {
			setFocus(true);
		}
		else {
			setFocus(false);
		}
	}
	if(event->type == SDL_MOUSEBUTTONDOWN) {
		if(event->button.button == SDL_BUTTON_LEFT)
			if(check_inside(event->button.x, event->button.y)) {
				onClick();
				actived = true;
			}
	}
	if(event->type == SDL_KEYDOWN)
		if(event->key.keysym.sym == SDLK_RETURN)
			if(focused) {
				onClick();
				actived = true;
			}
	return actived;
}

void Button::show() {
	apply_surface(x, y, w, h, *tex);
	if(focused) {
		glDisable(GL_TEXTURE_2D);
		glColor4f(0.9, 0.8, 0, 0.2);
		glBegin(GL_POLYGON);
			glVertex2f(x, y);
			glVertex2f(x + w, y);
			glVertex2f(x + w, y + h);
			glVertex2f(x, y + h);
		glEnd();
		glColor4f(1,1,1,1);
		glEnable(GL_TEXTURE_2D);
	}
}

void Button::onClick() {
	clicked = true;
}

bool Button::isClicked() {
	return clicked;
}

void Button::setFocus(bool value) {
	focused = value;
}

int Button::getX() {
	return x;
}

int Button::getY() {
	return y;
}
