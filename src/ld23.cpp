#include "stdlib.h"
#include <string>
#include "include.h"
#include "menu/menus.h"

SDL_Surface *screen;

bool init();
bool init_GL();
void clean_up();

int main() {
	texture menutex[5];
	Uint8 response;
	
	if(!init()) return 0;
	
	response = menu_start();
	if(response)
		Game game;
	
	clean_up();
	
	return 1;
}

void *load_image(texture *tex, /*std::string*/char* filename, int w, int h) {
	SDL_Surface *surface;
	
	tex->w = w;
	tex->h = h;
	
	surface = SDL_LoadBMP(filename/*.c_str()*/);
	if(surface == NULL) {printf("couldn't load image\n"); return NULL;}
    glGenTextures(1, &tex->id);
    
	glBindTexture( GL_TEXTURE_2D, tex->id);
	
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
	
	free(surface);
}

void apply_surface( int x, int y, int w, int h, texture tex) {
	glBindTexture(GL_TEXTURE_2D, tex.id);        
	
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 0.0); glVertex2f(x, y);
		glTexCoord2f(1.0, 0.0); glVertex2f(x + w, y);
		glTexCoord2f(1.0, 1.0); glVertex2f(x + w, y + h);
		glTexCoord2f(0.0, 1.0); glVertex2f(x, y + h);
	glEnd();
}

void drawLine(int x1, int y1, int x2, int y2) {
	glDisable(GL_TEXTURE_2D);
	glLineWidth(2.0);
	glBegin(GL_LINES);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
	glEnd();
	glColor4f(1.0, 1.0, 1.0, 1.0); //normal color
	glEnable(GL_TEXTURE_2D);
}

bool init_GL() {
	glEnable(GL_TEXTURE_2D);
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1 );
	
	glClearColor(1, 1, 1, 1); //black
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	glDisable(GL_DEPTH_TEST);
	
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	if(glGetError() != GL_NO_ERROR) return false;
	
	return true;
}

bool init() {
	SDL_Init(SDL_INIT_EVERYTHING);

	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL/* | SDL_FULLSCREEN*/);
	
	if(init_GL() == false) return false;
	
	SDL_WM_SetCaption( "ld23", NULL );
	
	SDL_EnableUNICODE(1);

	return true;
}

void clean_up() {
	glDisable(GL_TEXTURE_2D);
    SDL_Quit();
}
