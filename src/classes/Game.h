class Game {
	public:
		Game();
		void start();
		void render();
		void load_textures();
	private:
		texture textures[30];
		Timer fps;
		SDL_Event event;
		Water water;
		User user;
};

