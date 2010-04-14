#include "core.h"
#include "LevelMap.h"
#include "Player.h"
#include <iostream>

LevelMap levelMap;
Player player;

bool done = false;    
bool fullscreen = false;
enum { STATE_MENU=0, STATE_ABOUT=1, STATE_GAME=2 };
int screenState, gameState, menuPointer;
BITMAP *bmpMenu, *bmpAbout;
int textColor = makecol(256,100,256);

//Call the init, set the menu, load the bitmap of the player
//TODO: MAKE the user image be owned by the player 
void gameSetup(){
  screenState = STATE_MENU;   
  levelMap.init();  
  player.loadImage();
  bmpMenu = load_bitmap("infiniformer.bmp",NULL);
  bmpAbout= load_bitmap("about.bmp",NULL);
}

void drawMenu(){
  masked_blit(bmpMenu, screenBuffer,0,0,0,0,800,600);
  textprintf_ex(screenBuffer, font, 310, 400,  makecol(0, 0, 0), -1, "Pick an option:");
  textprintf_ex(screenBuffer, font, 330, 415,  makecol(0, 0, 0), -1, "Start game");
  textprintf_ex(screenBuffer, font, 330, 430,  makecol(0, 0, 0), -1, "About");
  textprintf_ex(screenBuffer, font, 330, 445,  makecol(0, 0, 0), -1, "Exit");
  textprintf_ex(screenBuffer, font, 315, 415 + (menuPointer * 15), makecol(0,0,0), -1, ">");
}

void drawAbout(){
  masked_blit(bmpAbout, screenBuffer,0,0,0,0,800,600);
}

void drawGame(){
  levelMap.draw();
  player.draw(screenBuffer, levelMap.x, levelMap.y);
  bool asd = levelMap.die(player.X(),player.Y(), player.width, player.height);  
  textprintf_ex(screenBuffer, font, 10, 10, textColor, -1, "press esc to go to the menu, ESTAS Jumping: %s y estas Sliding: %s", (player.jumping)?"JUMP":"NO",(player.sliding)?"SI":"NO");   
//  textprintf_ex(screenBuffer, font, 10, 10, textColor, -1, "press esc to go to the menu, ESTAS DEAD");   
}

//This function selects what draw to call
void drawingThings(){
  if(screenState == STATE_MENU){
    drawMenu(); 
  }  
  if(screenState == STATE_ABOUT){
    drawAbout(); 
  }  
  if(screenState == STATE_GAME){
    drawGame();  
  }    
}

//Make this a nice Select list
bool removeMenuBounce = true;
void receiveMenuInput(){

  if(key[KEY_UP] && removeMenuBounce){
    removeMenuBounce = false;
    menuPointer = menuPointer - 1;
  } else if(key[KEY_DOWN] && removeMenuBounce) {
    removeMenuBounce = false;
    menuPointer = menuPointer + 1;
  }
  if(menuPointer < 0) {
    menuPointer = 2;
  }
  if(menuPointer > 2){
    menuPointer = 0;
  }
  if(!key[KEY_UP] && !key[KEY_DOWN]){
    removeMenuBounce = true;
  }

  if(key[KEY_ENTER]){
    if(menuPointer == 0){
      screenState = STATE_GAME;
    } else if(menuPointer == 1){
      screenState = STATE_ABOUT;
    } else {
      done = true;
    }
  }
}

void receiveAboutInput(){
  if(key[KEY_ESC]){
    screenState = STATE_MENU;
  }
}

//Aplies the gravity
// TODO: The plaer should call the camare and say it moved, not the main
void gravity(){
    player.applyGravity(&levelMap);
    levelMap.down(player.Y());
}

//Check if we are dead and reset
void die(bool forceKill){
  bool die = levelMap.die(player.X(),player.Y(), player.width, player.height);  
  if (die || forceKill){
    player.reset();
    levelMap.resetCamera();
  }
}

bool specialJumpEnabled = true;
//TODO: Remove gravity and DIE from here, it is NOT the place
void receiveGameInputWalking(){
  gravity();
  die(false);
  if(key[KEY_S]){
    die(true);
  }

  if(!key[KEY_LEFT] && !key[KEY_RIGHT]){
    player.sliding = false;
  }
  if(key[KEY_UP]){
    player.jump();
  } 
  if(key[KEY_UP] && specialJumpEnabled){
    player.specialJump(&levelMap);
    specialJumpEnabled = false;
  } 
  if(!key[KEY_UP]){
    specialJumpEnabled = true;
  }
  if(key[KEY_DOWN]){
//    player.move(DOWN);
//    levelMap.down(player.Y());
  }
  if(key[KEY_LEFT]){
    player.move(LEFT, &levelMap);
    levelMap.left(player.X());
  }
  if(key[KEY_RIGHT]){
    player.move(RIGHT, &levelMap);
    levelMap.right(player.X());
  }    
  levelMap.moveLimit(&player);
}

void receiveGameInput(){
  if(key[KEY_ESC]){
    screenState = STATE_MENU;
  } else {
    receiveGameInputWalking();
  }
}

void receiveInput(){
  //F4 == quit
  if (key[KEY_F4] && key[KEY_LCONTROL]) { done = true; }
  //F5 == toggle Fullscreen
  if (key[KEY_F5]){
    if ( fullscreen ) { set_gfx_mode(GFX_AUTODETECT_WINDOWED, WIDTH , HEIGHT, 0, 0); fullscreen = false; }
    else { set_gfx_mode(GFX_AUTODETECT, WIDTH , HEIGHT, 0, 0); fullscreen = true; }
  }
  if(screenState == STATE_MENU){
    receiveMenuInput(); 
  }  
  if(screenState == STATE_ABOUT){
    receiveAboutInput(); 
  }  
  if(screenState == STATE_GAME){
    receiveGameInput();  
  }    
}

int main(){
  techInit();
  gameSetup();  
    
  // this next while loop
  while(!done){
    while(speed_counter > 0){
      receiveInput();      
      speed_counter--;
    }
    drawingThings();
    updateScreen();
  }
  destroy_bitmap(bmpMenu);
  destroy_bitmap(bmpAbout);
  player.unloadImage();
  return 0;
} END_OF_MAIN()
