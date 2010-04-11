#include <allegro.h>

// Screen size
#define WIDTH 800
#define HEIGHT 600

//map properties
#define TILEW 32
#define TILEH 32
#define TILES 41
#define COLS 10

//map size
#define MAP_ACROSS 50
#define MAP_DOWN 25
#define MAPW MAP_ACROSS * TILEW
#define MAPH MAP_DOWN * TILEH

//animation delay
#define ANIMDELAY 50

//autoscroll
#define AUTOSCROLLDELAY 20
#define AUTOSCROLL false

//Falling phisics:
#define MAXSPEEDY 10
#define ACEL .4
#define JUMPSPEED 12
#define STEP 3

//better language constants
#define UP 8
#define DOWN 2
#define LEFT 4
#define RIGHT 6


extern BITMAP *screenBuffer;
extern volatile long speed_counter;

void techInit();
void updateScreen();
void increment_speed_counter();
