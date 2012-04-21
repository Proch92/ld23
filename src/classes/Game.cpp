#include "../include.h"

Game::Game() {
	glClearColor(1, 1, 1, 1);
	
	//load textures
	load_textures();
	
	//create water
	water.init(((SCREEN_WIDTH - WATER_WIDTH) / 2), 0);
	water.randomInit();
	
	//spawn character
	user.place(300, &event, textures, &water);
	
	//start the main cycle
	printf("starting...\n");
	start();
}

void Game::start() {
	bool quit = false;
	
	fps.start();
	
	do {
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT)
				quit = true;
			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q)
				quit = true;
			//user handle input
		}
		//move objects
		water.compute();
		water.move();
		
		user.floatUp();
		
		render();
		
		if(fps.get_ticks() < 16) { //lock at 60fps
			SDL_Delay(16 - fps.get_ticks());
		}
		fps.start();
	} while(!quit);
}

void Game::render() {
	glClear(GL_COLOR_BUFFER_BIT);
		
	water.show();
	
	user.show();
		
	SDL_GL_SwapBuffers(); //update screen
}

void Game::load_textures() {
	//boats
	load_image(&textures[10], "src/data/boats/user.bmp", BOAT_WIDTH, BOAT_HEIGHT);
}
