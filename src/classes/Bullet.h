class Bullet {
	public:
		Bullet();
		void spawn(float, float, float, float, texture*, int);
		void move();
		void show();
		float x, y;
		Bullet *next;
	private:
		float xvel, yvel;
		texture* tex;
		Uint8 tex_index;
};
