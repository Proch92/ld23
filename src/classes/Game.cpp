#include "../include.h"
#include "time.h"

Game::Game() {
	glClearColor(1, 1, 1, 1);
	
	//load textures
	load_textures();
	
	//create water
	water.init(((SCREEN_WIDTH - WATER_WIDTH) / 2), 0);
	water.randomInit();
	
	//spawn character
	user.place(300, &event, textures, &water, &bullets);
	
	//misc
	bullets = NULL;
	enemies = NULL;
	pups = NULL;
	
	srand(time(NULL));
	
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
		
		if(user.death())
			game_over(&quit);
		user.floatUp();
		user.move();
		
		//spawn pups
		if(rand()%1000 < 2) {
			Pup *tmp;
			tmp = (Pup*) malloc(sizeof(Pup));
			tmp->spawn((rand()%(WATER_WIDTH - PUP_WIDTH)) + ((SCREEN_WIDTH - WATER_WIDTH) / 2), textures, rand()%3);
			tmp->next = pups;
			pups = tmp;
		}		
		
		//spawn enemies
		if(rand()%1000 < 3) {
			Enemy *tmp;
			tmp = (Enemy*) malloc(sizeof(Enemy));
			tmp->place((rand()%(WATER_WIDTH - BOAT_WIDTH)) + ((SCREEN_WIDTH - WATER_WIDTH) / 2), textures, &water, &bullets, &user);
			tmp->next = enemies;
			enemies = tmp;
		}
		
		//move pups
		Pup *pup_ptr;
		Pup *prev_pup;
		pup_ptr = pups;
		prev_pup = NULL;
		while(pup_ptr != NULL) {
			bool to_trash = false;
			pup_ptr->move();
			if(pup_ptr->y >= (SCREEN_HEIGHT / 2) + 10)
				to_trash = true;
			if(pup_ptr->x < user.x + BOAT_WIDTH && pup_ptr->x + PUP_WIDTH > user.x && pup_ptr->y < user.y + BOAT_HEIGHT && pup_ptr->y + PUP_HEIGHT > user.y) {
				pup_ptr->pickup(&user);
				to_trash = true;
			}
			Enemy *enemy_ptr;
			enemy_ptr = enemies;
			while(enemy_ptr != NULL) {
				if(pup_ptr->x < enemy_ptr->x + BOAT_WIDTH && pup_ptr->x + PUP_WIDTH > enemy_ptr->x && pup_ptr->y < enemy_ptr->y + BOAT_HEIGHT && pup_ptr->y + PUP_HEIGHT > enemy_ptr->y) {
					pup_ptr->pickup(enemy_ptr);
					to_trash = true;
				}
				enemy_ptr = enemy_ptr->next;
			}
			if(to_trash) {
				if(prev_pup != NULL) {
					prev_pup->next = pup_ptr->next;
					free(pup_ptr);
					pup_ptr = prev_pup->next;
				}
				else {
					pups = pup_ptr->next;
					free(pup_ptr);
					pup_ptr = pups;
				}
			}
			else {
				prev_pup = pup_ptr;
				pup_ptr = pup_ptr->next;
			}
		}
		
		//enemies
		Enemy *enemy_ptr, *prev_enemy;
		enemy_ptr = enemies;
		prev_enemy = NULL;
		while(enemy_ptr != NULL) {
			enemy_ptr->floatUp();
			enemy_ptr->ai();
			enemy_ptr->move();
			if(enemy_ptr->death()) {
				if(prev_enemy != NULL) {
					prev_enemy->next = enemy_ptr->next;
					free(enemy_ptr);
					enemy_ptr = prev_enemy->next;
				}
				else {
					enemies = enemy_ptr->next;
					free(enemy_ptr);
					enemy_ptr = enemies;
				}
			}
			else {
				prev_enemy = enemy_ptr;
				enemy_ptr = enemy_ptr->next;
			}
		}
		
		//move bullets
		Bullet *bullet_ptr, *prev_bullet;
		bullet_ptr = bullets;
		prev_bullet = NULL;
		while(bullet_ptr != NULL) {
			bullet_ptr->move();
			if(bullet_ptr->y > (SCREEN_HEIGHT / 2)) {
				water.giveInput(bullet_ptr->x - ((SCREEN_WIDTH - WATER_WIDTH) / 2), 40);
				
				if(bullet_ptr->x < user.x + BOAT_WIDTH && bullet_ptr->x + BULLET_WIDTH > user.x)
					user.hit();
				
				Enemy *enemy_ptr = enemies;
				while(enemy_ptr != NULL) {
					if(bullet_ptr->x < enemy_ptr->x + BOAT_WIDTH && bullet_ptr->x + BULLET_WIDTH > enemy_ptr->x)
						enemy_ptr->hit();
					enemy_ptr = enemy_ptr->next;
				}
				
				if(prev_bullet != NULL) {
					prev_bullet->next = bullet_ptr->next;
					free(bullet_ptr);
					bullet_ptr = prev_bullet->next;
				}
				else {
					bullets = bullet_ptr->next;
					free(bullet_ptr);
					bullet_ptr = bullets;
				}
			}
			else {
				prev_bullet = bullet_ptr;
				bullet_ptr = bullet_ptr->next;
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
	
	apply_surface(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, textures[0]);
	
	user.show();
	
	//show pups
	Pup *pup_ptr;
	pup_ptr = pups;
	while(pup_ptr != NULL) {
		pup_ptr->show();
		pup_ptr = pup_ptr->next;
	}
	
	//show enemies
	Enemy *enemy_ptr;
	enemy_ptr = enemies;
	while(enemy_ptr != NULL) {
		enemy_ptr->show();
		enemy_ptr = enemy_ptr->next;
	}
	
	//show bullets
	Bullet *bullet_ptr;
	bullet_ptr = bullets;
	while(bullet_ptr != NULL) {
		bullet_ptr->show();
		bullet_ptr = bullet_ptr->next;
	}
	
	water.show();
	
	apply_surface(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, textures[1]);
	
	SDL_GL_SwapBuffers(); //update screen
}

void Game::load_textures() {
	//ambient
	load_image(&textures[0], "src/data/ambient/back.bmp", SCREEN_WIDTH, SCREEN_HEIGHT);
	load_image(&textures[1], "src/data/ambient/front.bmp", SCREEN_WIDTH, SCREEN_HEIGHT);
	//boats
	load_image(&textures[10], "src/data/boats/user.bmp", BOAT_WIDTH, BOAT_HEIGHT);
	load_image(&textures[11], "src/data/boats/enemy.bmp", BOAT_WIDTH, BOAT_HEIGHT);
	//pups
	load_image(&textures[15], "src/data/pups/healthpack.bmp", PUP_WIDTH, PUP_HEIGHT);
	load_image(&textures[16], "src/data/pups/healthaug.bmp", PUP_WIDTH, PUP_HEIGHT);
	load_image(&textures[17], "src/data/pups/doubleshot.bmp", PUP_WIDTH, PUP_HEIGHT);
	//bullet
	load_image(&textures[20], "src/data/bullet/bullet.bmp", BULLET_WIDTH, BULLET_HEIGHT);
}

void Game::game_over(bool* quit) {
	printf("game over! you lose!\n");
	*quit = true;
}
