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
	
	//---- TEMP ---- spawn enemy
	//enemy.place(500, textures, &water, &bullets, &user);
	
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
			game_over();
		user.floatUp();
		user.move();
		
		//spawn pups
		if(rand()%900 < 2) {
			Pup *tmp;
			tmp = (Pup*) malloc(sizeof(Pup));
			tmp->spawn((rand()%(WATER_WIDTH - PUP_WIDTH)) + ((SCREEN_WIDTH - WATER_WIDTH) / 2), textures, rand()%3);
			struct list *new_elem;
			new_elem = (struct list*) malloc(sizeof(struct list));
			new_elem->data = tmp;
			new_elem->next = pups;
			pups = new_elem;
		}		
		
		//spawn enemies
		if(rand()%1000 < 3) {
			Enemy *tmp;
			tmp = (Enemy*) malloc(sizeof(Enemy));
			tmp->place((rand()%(WATER_WIDTH - BOAT_WIDTH)) + ((SCREEN_WIDTH - WATER_WIDTH) / 2), textures, &water, &bullets, &user);
			struct list *new_elem;
			new_elem = (struct list*) malloc(sizeof(struct list));
			new_elem->data = tmp;
			new_elem->next = enemies;
			enemies = new_elem;
		}
		
		struct list *tmp, *prev;
		
		//move pups
		Pup *pup_ptr;
		tmp = pups;
		prev = NULL;
		while(tmp != NULL) {
			bool to_trash = false;
			pup_ptr = (Pup*)(tmp->data);
			pup_ptr->move();
			if(pup_ptr->y >= (SCREEN_HEIGHT / 2) + 10)
				to_trash = true;
			if(pup_ptr->x < user.x + BOAT_WIDTH && pup_ptr->x + PUP_WIDTH > user.x && pup_ptr->y < user.y + BOAT_HEIGHT && pup_ptr->y + PUP_HEIGHT > user.y) {
				pup_ptr->pickup(&user);
				to_trash = true;
			}
			struct list *list_ptr = enemies;
			Enemy *enemy_ptr;
			while(list_ptr != NULL) {
				enemy_ptr = (Enemy*)(list_ptr->data);
				if(pup_ptr->x < enemy_ptr->x + BOAT_WIDTH && pup_ptr->x + PUP_WIDTH > enemy_ptr->x && pup_ptr->y < enemy_ptr->y + BOAT_HEIGHT && pup_ptr->y + PUP_HEIGHT > enemy_ptr->y) {
					pup_ptr->pickup(enemy_ptr);
					to_trash = true;
				}
				list_ptr = list_ptr->next;
			}
			if(to_trash) {
				free(pup_ptr);
				if(prev != NULL) {
					prev->next = tmp->next;
					free(tmp);
					tmp = prev->next;
				}
				else {
					pups = tmp->next;
					free(tmp);
					tmp = pups;
				}
			}
			else {
				prev = tmp;
				tmp = tmp->next;
			}
		}
		
		//enemies
		Enemy *enemy;
		tmp = enemies;
		prev = NULL;
		while(tmp != NULL) {
			enemy = (Enemy*)(tmp->data);
			enemy->floatUp();
			enemy->ai();
			enemy->move();
			if(enemy->death()) {
				free(enemy);
				if(prev != NULL) {
					prev->next = tmp->next;
					free(tmp);
					tmp = prev->next;
				}
				else {
					enemies = tmp->next;
					free(tmp);
					tmp = enemies;
				}
			}
			else {
				prev = tmp;
				tmp = tmp->next;
			}
		}
		
		//move bullets
		tmp = bullets;
		prev = NULL;
		Bullet *bullet_ptr;
		while(tmp != NULL) {
			bullet_ptr = ((Bullet*)(tmp->data));
			bullet_ptr->move();
			if(bullet_ptr->y > (SCREEN_HEIGHT / 2)) {
				water.giveInput(bullet_ptr->x - ((SCREEN_WIDTH - WATER_WIDTH) / 2), 40);
				
				if(bullet_ptr->x < user.x + BOAT_WIDTH && bullet_ptr->x + BULLET_WIDTH > user.x)
					user.hit();
					
				struct list *list_ptr;
				list_ptr = enemies;
				Enemy *enemy_ptr;
				while(list_ptr != NULL) {
					enemy_ptr = ((Enemy*)(list_ptr->data));
					if(bullet_ptr->x < enemy_ptr->x + BOAT_WIDTH && bullet_ptr->x + BULLET_WIDTH > enemy_ptr->x)
						enemy_ptr->hit();
					list_ptr = list_ptr->next;
				}	
							
				free(bullet_ptr);
				
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
	
	struct list *tmp;
	
	//show pups
	tmp = pups;
	while(tmp != NULL) {
		((Pup*)(tmp->data))->show();
		tmp = tmp->next;
	}
	
	//show enemies
	tmp = enemies;
	while(tmp != NULL) {
		((Enemy*)(tmp->data))->show();
		tmp = tmp->next;
	}
	
	//show bullets
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
	//pups
	load_image(&textures[15], "src/data/pups/healthpack.bmp", PUP_WIDTH, PUP_HEIGHT);
	load_image(&textures[16], "src/data/pups/healthaug.bmp", PUP_WIDTH, PUP_HEIGHT);
	load_image(&textures[17], "src/data/pups/doubleshot.bmp", PUP_WIDTH, PUP_HEIGHT);
	//bullet
	load_image(&textures[20], "src/data/bullet/bullet.bmp", BULLET_WIDTH, BULLET_HEIGHT);
}

void Game::game_over() {
	printf("game over! you lose!\n");
	exit(1);
}
