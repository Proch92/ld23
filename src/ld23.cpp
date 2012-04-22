#include "stdlib.h"
#include <string>
#include "include.h"
#include "menu/menus.h"

SDL_Surface *screen;

bool init();
bool init_GL();
void clean_up();
void intro();
bool game_over_screen();

int main() {
	Uint8 response;
	
	if(!init()) return 0;
	
	response = menu_start();
	while(response) {
		response = false;
		intro();
		Game game;
		response = game_over_screen();
	}
	
	clean_up();
	
	return 1;
}

void *load_image(texture *tex, /*std::string*/const char* filename, int w, int h) {
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
	
	return NULL;
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

void intro() {
	texture pages[6];
	
	load_image(&pages[0], "src/data/story/1.bmp", SCREEN_WIDTH, SCREEN_HEIGHT);
	load_image(&pages[1], "src/data/story/2.bmp", SCREEN_WIDTH, SCREEN_HEIGHT);
	load_image(&pages[2], "src/data/story/3.bmp", SCREEN_WIDTH, SCREEN_HEIGHT);
	load_image(&pages[3], "src/data/story/4.bmp", SCREEN_WIDTH, SCREEN_HEIGHT);
	load_image(&pages[4], "src/data/story/5.bmp", SCREEN_WIDTH, SCREEN_HEIGHT);
	load_image(&pages[5], "src/data/story/6.bmp", SCREEN_WIDTH, SCREEN_HEIGHT);
	
	bool quit = false;
	SDL_Event event;
	int page_number = 0;
	Timer fps;
	
	fps.start();
	
	while(!quit) {
		bool clicked = false;
		
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_MOUSEBUTTONDOWN)
				clicked = true;
		}
		
		apply_surface(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, pages[page_number]);
		
		SDL_GL_SwapBuffers(); //update screen
		
		if(fps.get_ticks() < 16)
			SDL_Delay(16 - fps.get_ticks());
		
		if(clicked)
			page_number++;
		
		if(page_number == 6)
			quit = true;
		
		fps.start();
	}
}

bool game_over_screen() {
	texture gameover;
	
	load_image(&gameover, "src/data/gameover/gameover.bmp", SCREEN_WIDTH, SCREEN_HEIGHT);
	
	bool quit = false;
	SDL_Event event;
	Timer fps;
	
	fps.start();
	
	while(!quit) {		
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_MOUSEBUTTONDOWN)
				return false;
			if(event.type == SDL_KEYDOWN)
				if(event.key.keysym.sym == SDLK_r)
					return true;
		}
		
		apply_surface(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, gameover);
		
		SDL_GL_SwapBuffers(); //update screen
		
		if(fps.get_ticks() < 25)
			SDL_Delay(25 - fps.get_ticks());

		fps.start();
	}
	return false;
}

