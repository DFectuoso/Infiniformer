#pragma once
#include <allegro.h>
#include "core.h"

class Player;

//TODO: Hardcoded maps should not be part of the class, no need to do it likethat
class LevelMap {
  private:
    static const int map1[25][50];
    static const int map2[25][25];
    static const int map3[25][25];
    static const int map4[25][25];
    static const int map5[25][25];
    static const int map6[25][25];
    static const int map7[25][25];
    static const int map8[25][25];
    static const int map9[25][25];
    static const int map10[25][25];
    static const int map11[25][25];
    static int collideArray[2], collideArraySize;
    int autoScrollDelay;
    int map[MAPH][MAPW];
          
  public:
    BITMAP *tiles;    
    BITMAP *scroll;
    int x, y;
    int tilex, tiley;
    BITMAP *grabframe(BITMAP *source, int width, int height, int startx, int starty, int columns, int frame);
    LevelMap();
    void init();
    void resetCamera();
    void draw();
    void up(int playerY);
    void down(int playerY);
    void left(int playerX);
    void right(int playerX);
    void moveLimit(Player* player);
    bool collide(int playerX, int playerY, int width, int height);
    bool collideRight(int playerX, int playerY, int width, int height);
    bool collideLeft(int playerX, int playerY, int width, int height);
    bool die(int playerX, int playerY, int width, int height);
};