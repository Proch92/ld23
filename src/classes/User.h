class User : public Boat {
	public:
		User();
		void place(int, SDL_Event*, texture*, Water*, Bullet**);
		void handle_input();
		void move();
		void show();
	private:
		void action();
		SDL_Event *event;
		bool right_down, left_down, action_down;
		Timer reload;
		int reload_time;
};
