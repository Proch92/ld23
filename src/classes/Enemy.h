class Enemy : public Boat {
	public:
		Enemy();
		void place(int, texture*, Water*, Bullet**, User*);
		void ai();
		void move();
		void action();
		Enemy* next;
	private:
		Timer reload;
		bool right_down, left_down;
		int reload_time;
		User *user;
		int user_direction; //is the user left or right from here?
};
