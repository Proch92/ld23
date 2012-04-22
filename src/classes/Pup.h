class Pup {
	public:
		Pup();
		void spawn(int, texture*, int);
		void move();
		void show();
		void pickup(void*);
		int x, y;
	private:
		int type;
		Uint8 tex_index;
		texture *tex;
};

/*types
0: health pack
1: augment max health
2: double shot
3: ...
*/
