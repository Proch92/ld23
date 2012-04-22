class Game {
	public:
		Game();
		void start();
		void render();
		void load_textures();
	private:
		void game_over();
		texture textures[30];
		Timer fps;
		SDL_Event event;
		Water water;
		User user;
		struct list *bullets;
		struct list *enemies;
		struct list *pups;
		
		//temp
		Enemy enemy;
};
