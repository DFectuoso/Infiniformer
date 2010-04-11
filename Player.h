#pragma once
#include "Player.h"
#include <allegro.h>
#include "core.h"

class LevelMap; 

class Player{
  private:
    float speed;
    float acelY,speedY, speedX;
    //TODO: REMOVE, not used
    float weight;
  public:
    int x, y, direction;
    int delay, delayTimer;
    int width, height;
    bool jumping, sliding;
    void move(int dir, LevelMap* map);
    void jump();
    void specialJump(LevelMap* map);
    void reset();
    void setSpeed(int newSpeed);
    void draw(BITMAP *buffer, BITMAP *image, int mapX, int mapY);
    void applyGravity(LevelMap* map);
    float Speed() { return speed; }
    int X();
    int Y();
    int Direction();
    Player();
    ~Player();
};
