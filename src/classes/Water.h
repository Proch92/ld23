//water proprierties
#define VISCOSITY 2 //more viscosity ---> lower value
#define DENSITY 3 //more density ---> lower value

//water random initialization
const int W_AMPLITUDE = 200;

#define RAIN_SPAWN_RATE 95
#define RAIN_TIMELAPSE 20

struct particle {
	float height;
	float vel;
};

class Water {
	public:
		Water();
		void compute();
		void randomInit();
		void move();
		void show();
		void giveInput(int, float);
		void init(int, int);
		int operator [] (int);
	private:
		int LENGHT;
		struct particle *particles;
		int x, y;
		texture watertex;
		Timer delta;
		GLubyte ***waterlayer;
		GLubyte *wl;
};
