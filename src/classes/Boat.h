class Boat {
	public:
		Boat();
		void place(int, texture*, Water*, struct list**);
		void show();
		void floatUp();
		void hit();
		float x, y;
		float vel;
		bool death();
	protected:
		int health;
		int max_health;
		Uint8 tex_index;
		texture* tex;
		struct list **bullets;
		Water *water;
		bool alive;
};
