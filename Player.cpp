#include "Player.h"
#include "core.h"
#include "LevelMap.h"
#include <iostream>

//Generate a new player, we will need to do this when you die so its just a call to rest
Player::Player() { 
  reset();
}

Player::~Player() { }

//Real init, sets the player not jumping, not sliding, height, width, acelY, and position
void Player::reset(){
  speed = STEP; 
  delay = 0;
  delayTimer = 0;
  direction = 2; 
  x = 10; 
  y = HEIGHT/2 - TILEH/2;
  acelY = ACEL;
  weight = 10;
  jumping = false;
  sliding = false;
  width = 64;
  height = 64;  
}

// TODO: Remove this, make this public, no need to use defensive coding in small projects
int Player::X() { 
  return x; 
}

// TODO: Remove this, make this public, no need to use defensive coding in small projects
int Player::Y() { 
  return y; 
}

// TODO: Remove this, make this public, no need to use defensive coding in small projects
int Player::Direction() { 
  return direction; 
}
bool enableJumpSlide = true;

// The player wants to move
// TODO: Standarize the camera and the player, one moves with 4 functions, the other with 1(int:direction), pick ONE
void Player::move(int dir,LevelMap* map) {
  if (dir == DOWN){
//    if (y < MAPH - 64){ 
//      y += speed; 
//    }
//    direction = DOWN;
  } else if (dir == LEFT) {
    if (x > 0 && !map->collide(x - speed, y - 10, width, height)) { 
      x -= speed; 
    }
    direction = LEFT;
    if(jumping && map->collideLeft(x - 32,y,width,height)){
      sliding = true;
      enableJumpSlide = true;
    } else {
      sliding = false;
      enableJumpSlide = false;
    }
  } else if (dir == RIGHT  && !map->collide(x + speed, y - 10, width, height)) {
    if (x < MAPW - 64) { 
      x += speed; 
    }
    if(jumping && map->collideRight(x + 32,y,width,height)){
      sliding = true;
      enableJumpSlide = true;
    } else {
      sliding = false;      
      enableJumpSlide = false;
    }
    direction = RIGHT;
  } else if (dir == UP) {
//    if (y > 10) { 
//      y -= speed; 
//    }
//    direction = UP;
  }
}

// Set new speed
// TODO: REMOVE THIS IS NOT USED
void Player::setSpeed(int newSpeed) {
  speed += newSpeed; 
}

// Allow you to jump while sliding
// TODO: Add AcelX, SpeedX and x so this can make you move nicely(remember, you are looking at DIRECTION)
void Player::specialJump(LevelMap* map){
  if(jumping && sliding && enableJumpSlide){
    enableJumpSlide = false;
    sliding = false;
    std::cout << "ZZZZ jumping ZZZZ \n";
    speedY -= JUMPSPEED;
    if(Direction() == RIGHT){
      move(LEFT, map);
      move(LEFT, map);
      move(LEFT, map);
      move(LEFT, map);
      move(LEFT, map);
      move(LEFT, map);
    } else{
      move(RIGHT, map);
      move(RIGHT, map);
      move(RIGHT, map);
      move(RIGHT, map);
      move(RIGHT, map);
      move(RIGHT, map);
    }
  }
}

//If not jumping, you can jump
void Player::jump(){
  if(!jumping){
    jumping = true;    
    speedY  -= JUMPSPEED;
  } 
}

// Apply gravity, depending if its sliding and/or jumping
// TODO: Refactor to be more readable
void Player::applyGravity(LevelMap* map){
  if(sliding && jumping){
    if(!map->collideRight(x+32,y,width,height) && !map->collideLeft(x-32,y,width,height)){
      sliding = false;
    }
  } 
  if(jumping){
    if(speedY < MAXSPEEDY){
      speedY += acelY;        
    }
    if(speedY > 0) { //its going DOWN
      map->down(y);
      if(!map->collide(x,y, width, height)){
        if(!sliding){
            y += speedY;         
        } else {
          y += speedY/10;
        }
      } else {
        speedY = 0;
        jumping = false;
        sliding = false;
        enableJumpSlide = true;
      }
    } else { //its going up
      map->up(y);   
      if(y + speedY + 64 > 0 && (!map->collide(x,y-height,width,height)))   {
        y += speedY;         
      }
    }
  } else {
    if(!map->collide(x,y, width, height)){
      jumping = true;
    }
  }  
}

//Paint the player.
//TODO: Make a function to know what state to paint, leave 2 lines here, call to the function and paint
void Player::draw(BITMAP *buffer, BITMAP *bmpAlec, int mapX, int mapY) {
  int frame = 0;
  delayTimer++;
  if(delayTimer > ANIMDELAY){
    delay++;
    delayTimer = 0;
  }
  if(delay > 2){
    delay = 0;
  }
  if (Direction() == DOWN || (jumping && speedY > 0 && !sliding))      { 
    frame = 0 + delay;
  } else if (Direction() == LEFT && !jumping) { 
    frame = 14 + delay;
  } else if (Direction() == RIGHT && !jumping) { 
    frame = 4 + delay;
  } else if (Direction() == UP || (jumping && speedY < 0 && !sliding)) { 
    frame = 10 + delay;
  } else if (Direction() == RIGHT && sliding) {
    frame = 20 + delay;
  } else if (Direction() == LEFT && sliding){
    frame = 24 + delay;
  }
  int xFrame = (frame % 10) * 64;
  int yFrame = (frame / 10) * 64;
  masked_blit(bmpAlec,buffer,xFrame,yFrame,x-mapX,y-mapY,64,64);
//  masked_blit(bmpAlec, buffer, delta,0, x - mapX, y - mapY,64,64); 
}