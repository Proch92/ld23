#include <string>
#include "../include.h"

void load_menu_tex(texture*);

int menu_start() {
	Timer fps;
	texture start[5];
	SDL_Event event;
	
	load_menu_tex(start);
	
	Button playB((SCREEN_WIDTH - start[1].w) / 2, 60, start[1].w, start[1].h, &start[1]);
	Button exitB((SCREEN_WIDTH - start[2].w) / 2, 130, start[2].w, start[2].h, &start[2]);
	
	fps.start();
	
	do {
		while(SDL_PollEvent(&event)) {
			playB.handle_input(&event);
			exitB.handle_input(&event);
		}
		
		//apply background
		apply_surface(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, start[0]);
		
		//show other buttons
		playB.show();
		exitB.show();
		
		//show banner
		apply_surface(200, 250, 400, 200, start[3]);
		
		SDL_GL_SwapBuffers(); //update screen
		
		if(playB.isClicked()) {
			return 1;
		}
		
		if(fps.get_ticks() < 10 && !exitB.isClicked()) {
			SDL_Delay(10 - fps.get_ticks());
		}
		fps.start();
	} while(!exitB.isClicked());
	
	return 0;
}

void load_menu_tex(texture* tex) {
	load_image(&tex[0], "src/data/menu/start/bg.bmp", 2, 2);
	load_image(&tex[1], "src/data/menu/start/play.bmp", 100, 30);
	load_image(&tex[2], "src/data/menu/start/exit.bmp", 100, 30);
	load_image(&tex[3], "src/data/menu/start/banner.bmp", 400, 200);
}
