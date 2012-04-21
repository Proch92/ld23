class Boat {
	public:
		Boat();
		void place(int, texture*, Water*, struct list**);
		void show();
		void floatUp();
		void hit();
	protected:
		float x, y;
		int health;
		int max_health;
		Uint8 tex_index;
		texture* tex;
		float vel;
		struct list **bullets;
		Water *water;
		bool alive;
};
