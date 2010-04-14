#pragma once
#include "Player.h"
#include <allegro.h>
#include "core.h"

class LevelMap; 

class Player{
  private:
    float speed;
    float acelY,speedY, speedX;
  public:
    BITMAP *bmpPlayer;
    int x, y, direction;
    int delay, delayTimer;
    int width, height;
    bool jumping, sliding;
    void loadImage();
    void unloadImage();
    void move(int dir, LevelMap* map);
    void jump();
    void specialJump(LevelMap* map);
    void reset();
    void setSpeed(int newSpeed);
    void draw(BITMAP *image, int mapX, int mapY);
    void applyGravity(LevelMap* map);
    float Speed() { return speed; }
    int X();
    int Y();
    int Direction();
    Player();
    ~Player();
};
