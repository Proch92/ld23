class Boat {
	public:
		Boat();
		void place(int, texture*, Water*, Bullet**);
		void show();
		void floatUp();
		void hit();
		float x, y;
		float vel;
		bool death();
		void give_health(int);
		int max_health;
		void double_shot();
	protected:
		int health;
		Uint8 tex_index;
		texture* tex;
		Bullet **bullets;
		Water *water;
		bool alive;
		bool double_shot_on;
		Timer pup_timer;
};
