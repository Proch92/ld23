class Enemy : public Boat {
	public:
		Enemy();
		void place(int, texture*, Water*, struct list**, User*);
		void ai();
		void move();
		void action();
	private:
		Timer reload;
		bool right_down, left_down;
		int reload_time;
		User *user;
};
