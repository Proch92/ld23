class User : public Boat {
	public:
		User();
		void place(int, SDL_Event*, texture*, Water*);
		void handle_input();
	private:
		SDL_Event *event;
};
