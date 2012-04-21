class Boat {
	public:
		Boat();
		void place(int, texture*, Water*);
		void show();
		void floatUp();
	protected:
		int x, y;
		int health;
		int max_health;
		Uint8 tex_index;
		texture* tex;
	private:
		Water *water;
		bool alive;
};
