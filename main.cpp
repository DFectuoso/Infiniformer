#include "core.h"
#include "LevelMap.h"
#include "Player.h"
#include <iostream>

LevelMap levelMap;
Player player;

bool done = false;    
bool fullscreen = false;
enum { STATE_MENU=0, STATE_ABOUT=1, STATE_GAME=2 };
int screenState, gameState;
BITMAP *bmpPlayer;
int textColor = makecol(256,100,256);

//Function to make the screen back
//TODO: Remove after doing the menu and about, should not be used after that
void makeScreenBlack() {
  //clear the screen to black
  //without this next line, the game "smears"
  //comment it out to try it!
  clear_to_color(screenBuffer, makecol(0,0,0));
}

//Call the init, set the menu, load the bitmap of the player
//TODO: MAKE THE images be owned by the player and them
//TODO: MAKE sure to dispose of the images
void gameSetup(){
  screenState = STATE_MENU;   
  levelMap.init();  
  bmpPlayer = load_bitmap("robo.bmp",NULL);
}

void drawMenu(){
  textprintf_ex(screenBuffer, font, 10, 10, makecol(255, 100, 200), -1, "You are in the Menu, press 1 to go to the game, 2 to see the about and 3 to exit");
}

void drawAbout(){
  textprintf_ex(screenBuffer, font, 10, 10, makecol(255, 100, 200), -1, "This game was made by someone, press esc to go back to the menu");
}

void drawGame(){
  levelMap.draw();
  player.draw(screenBuffer, bmpPlayer, levelMap.x, levelMap.y);
  bool asd = levelMap.die(player.X(),player.Y(), player.width, player.height);  
  textprintf_ex(screenBuffer, font, 10, 10, textColor, -1, "press esc to go to the menu, ESTAS Jumping: %s y estas Sliding: %s", (player.jumping)?"JUMP":"NO",(player.sliding)?"SI":"NO");   
//  textprintf_ex(screenBuffer, font, 10, 10, textColor, -1, "press esc to go to the menu, ESTAS DEAD");   
}

//This function selects what draw to call
void drawingThings(){
  makeScreenBlack();
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
void receiveMenuInput(){
  if (key[KEY_1]) { 
    screenState = STATE_GAME;
  } else if (key[KEY_2]) {  
    screenState = STATE_ABOUT;
  } else if (key[KEY_3]) { done = true;}                     //Quit
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
  return 0;
} END_OF_MAIN()
