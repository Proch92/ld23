class Game {
	public:
		Game();
		void start();
		void render();
		void load_textures();
	private:
		void game_over(bool*);
		texture textures[30];
		Timer fps;
		SDL_Event event;
		Water water;
		User user;
		Bullet *bullets;
		Enemy *enemies;
		Pup *pups;
};
