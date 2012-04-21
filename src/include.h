#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

struct texture {
	GLuint id;
	unsigned int w;
	unsigned int h;
};

#include "classes/utils/Timer.h"
#include "classes/Water.h"
#include "classes/Boat.h"
#include "classes/User.h"
#include "classes/Game.h"
#include "classes/utils/Button.h"

void *load_image(texture *tex, /*std::string*/char* filename, int w, int h);
void apply_surface( int x, int y, int w, int h, texture tex);
void drawLine(int x1, int y1, int x2, int y2);

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define SCREEN_BPP 32

#define WATER_WIDTH 900
#define WATER_DEEP 50

#define BOAT_WIDTH 32
#define BOAT_HEIGHT 32

#define USER_INIT_HEALTH 1000
