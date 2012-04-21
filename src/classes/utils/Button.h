class Button {
	public:
		Button(int, int, int, int, texture*);
		bool handle_input(SDL_Event*);
		void show();
		bool isClicked();
		void setFocus(bool);
		void reset();
		int getX();
		int getY();
	private:
		int x, y, w, h;
		texture *tex;
		bool focused, clicked;
		void onClick();
		bool check_inside(int, int);
};
