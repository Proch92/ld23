#include "../include.h"

Game::Game() {
	glClearColor(1, 1, 1, 1);
	
	//load textures
	load_textures();
	
	//create water
	water.init(((SCREEN_WIDTH - WATER_WIDTH) / 2), 0);
	water.randomInit();
	
	//spawn character
	user.place(300, &event, textures, &water, &bullets);
	
	//---- TEMP ---- spawn enemy
	enemy.place(500, textures, &water, &bullets); 
	
	//misc
	bullets = NULL;
	
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
			
			user.handle_input();
		}
		//move objects
		water.compute();
		water.move();
		
		user.floatUp();
		user.move();
		
		//TEMP
		enemy.floatUp();
		
		//move bullets
		struct list *tmp, *prev;
		tmp = bullets;
		prev = NULL;
		while(tmp != NULL) {
			((Bullet*)(tmp->data))->move();
			if(((Bullet*)(tmp->data))->y > (SCREEN_HEIGHT / 2)) {
				water.giveInput(((Bullet*)(tmp->data))->x - ((SCREEN_WIDTH - WATER_WIDTH) / 2), 40);
				free(tmp->data);
				if(prev != NULL) {
					prev->next = tmp->next;
					free(tmp);
					tmp = prev->next;
				}
				else {
					bullets = tmp->next;
					free(tmp);
					tmp = bullets;
				}
			}
			else {
				prev = tmp;
				tmp = tmp->next;
			}
		}
		
		render();
		
		if(fps.get_ticks() < 16) { //lock at 60fps
			SDL_Delay(16 - fps.get_ticks());
		}
		fps.start();
	} while(!quit);
}

void Game::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	user.show();
	
	//TEMP
	enemy.show();
	
	//show bullets
	struct list *tmp;
	tmp = bullets;
	while(tmp != NULL) {
		((Bullet*)(tmp->data))->show();
		tmp = tmp->next;
	}
	
	water.show();
	
	SDL_GL_SwapBuffers(); //update screen
}

void Game::load_textures() {
	//boats
	load_image(&textures[10], "src/data/boats/user.bmp", BOAT_WIDTH, BOAT_HEIGHT);
	load_image(&textures[11], "src/data/boats/enemy.bmp", BOAT_WIDTH, BOAT_HEIGHT);
	//bullet
	load_image(&textures[20], "src/data/bullet/bullet.bmp", BULLET_WIDTH, BULLET_HEIGHT);
}
