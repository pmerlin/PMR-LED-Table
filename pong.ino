/* LedTable
 *
 * Written by: Ing. David Hrbaty
 * 
 * 
 * Main code for Pong game
 */
 
#include "pongCommon.h"
unsigned long curTime;
unsigned long prevUpdateTime = 0;
void pongInit(){
  scorePlayerLeft  = 0;
  scorePlayerRight = 0;
  FIELD_WIDTH * FIELD_HEIGHT;
  positionPlayerLeft  = FIELD_HEIGHT/2;
  positionPlayerRight = FIELD_HEIGHT/2;
  ballx = FIELD_WIDTH/2;
  bally = FIELD_WIDTH/2;
  velocityx = 1; 
  velocityy = 0;
  ballBounces = 0;
  gameSpeed = 180;
  lastAutoPlayerMoveTime = 0;
  rumbleUntil = 0;
  waitUntil = 0;
}

void runPong(){
  pongInit();
  
  boolean pongRunning = true;
  while(pongRunning){    
    
    if (scorePlayerLeft == MAX_SCORE || scorePlayerRight == MAX_SCORE){
      pongRunning = false;
      break;
    }
    
    checkBallHitByPlayer();
    
    if((curTime - lastAutoPlayerMoveTime) > AUTO_PLAYER_SPEED) {
      if(moveAutoPlayer()) {
        lastAutoPlayerMoveTime = curTime;
      }
    }
    
    ballx += velocityx;
    bally += velocityy;
    
    checkBallOutOfBounds();
    clearTablePixels();
    

    // Draw ball
    setTablePixel(ballx,bally,WHITE);
    // Draw player left
    for (int y=positionPlayerLeft-PLAYER_HEIGHT/2; y<=positionPlayerLeft+PLAYER_HEIGHT/2; ++y){
      setTablePixel(0, y, BLUE);
    }
    // Draw player right
    for (int y=positionPlayerRight-PLAYER_HEIGHT/2; y<=positionPlayerRight+PLAYER_HEIGHT/2; ++y){
      setTablePixel(FIELD_WIDTH-1, y, YELLOW);
    }
    
    showPixels();
    boolean dirChanged = false;
    do{
      readInput();
      if (curControl == BTN_EXIT){
        pongRunning = false;
        break;
      }
      if (curControl != BTN_NONE && !dirChanged){//Can only change direction once per loop
        dirChanged = true;
        setPosition();
      }
      curTime = millis();
    } 
    while ((curTime - prevUpdateTime) <250);//Once enough time  has passed, proceed. The lower this number, the faster the game is
    prevUpdateTime = curTime;
  }
  
  fadeOut();
  displayLogo();
}

void setPosition(){
  switch(curControl){
    case BTN_DOWN:
    case BTN_RIGHT:
      if(positionPlayerLeft + (PLAYER_HEIGHT-1) / 2 < FIELD_HEIGHT-1){
        ++positionPlayerLeft;
      }
      break;
    case BTN_UP:     
    case BTN_LEFT: 
      if(positionPlayerLeft - PLAYER_HEIGHT / 2 > 0) {
        --positionPlayerLeft;
      }
      break;
  }
}

void checkBallHitByPlayer() {
  if(ballx == 1) 
  {
    if(bally == positionPlayerLeft)
    {
      velocityx = 1;
      ballx = 1;
      ++ballBounces;
      rumbleUntil = curTime + 200;
    } 
    else if(bally < positionPlayerLeft && bally >= positionPlayerLeft - PLAYER_HEIGHT / 2) 
    {
      velocityx = 1;
      velocityy = max(-1,velocityy-1); 
      ballx = 1;
      bally = positionPlayerLeft - PLAYER_HEIGHT / 2-1;
      ++ballBounces;
      rumbleUntil = curTime + 200;
    }    
    else if(bally > positionPlayerLeft && bally <= positionPlayerLeft + (PLAYER_HEIGHT-1) / 2) 
    {
      velocityx = 1;
      velocityy = min(1,velocityy+1); 
      ballx = 1;
      bally = positionPlayerLeft + (PLAYER_HEIGHT-1) / 2+1;
      ++ballBounces;
      rumbleUntil = curTime + 200;
    }    
  } 
  else if(ballx == FIELD_WIDTH-2)
  {
    if(bally == positionPlayerRight)
    {
      velocityx = -1;
      ballx = FIELD_WIDTH-2;
      ++ballBounces;
    } 
    else if(bally < positionPlayerRight && bally >= positionPlayerRight - PLAYER_HEIGHT / 2) 
    {
      velocityx = -1;
      velocityy = max(-1,velocityy-1); 
      ballx = FIELD_WIDTH-2;
      bally = positionPlayerRight - PLAYER_HEIGHT / 2-1;
      ++ballBounces;
    }    
    else if(bally > positionPlayerRight && bally <= positionPlayerRight + (PLAYER_HEIGHT-1) / 2) 
    {
      velocityx = -1;
      velocityy = min(1,velocityy+1); 
      ballx = FIELD_WIDTH-2;
      bally = positionPlayerRight + (PLAYER_HEIGHT-1) / 2+1;
      ++ballBounces;
    }    
  } 
}

void checkBallOutOfBounds() {
  if(bally < 0) 
  {
    velocityy = - velocityy;
    //bally = 0;
    bally = 1;
  } else if(bally > FIELD_HEIGHT-1) 
  {
    velocityy = - velocityy;
    bally = FIELD_HEIGHT-2;
    //bally = FIELD_HEIGHT-1;
  } 
  if(ballx < 0) 
  {
    velocityx = - velocityx;
    velocityy = 0;
    ballx = FIELD_WIDTH/2;
    bally = FIELD_HEIGHT/2;
    ++scorePlayerRight;
    ballBounces = 0;
    waitUntil = curTime + 2000;
  } 
  else if(ballx > FIELD_WIDTH-1) 
  {
    velocityx = - velocityx;
    velocityy = 0;
    ballx = FIELD_WIDTH/2;
    bally = FIELD_HEIGHT/2;
    ++scorePlayerLeft;
    ballBounces = 0;
    waitUntil = curTime + 2000;
  } 
}

boolean moveAutoPlayer()
{
  if(bally < positionPlayerRight)
  {
    if(positionPlayerRight - PLAYER_HEIGHT / 2>0) 
    {
      --positionPlayerRight;
      return true;
    }
  } 
  else if(bally > positionPlayerRight) 
  {
    if(positionPlayerRight + (PLAYER_HEIGHT-1) / 2 < FIELD_HEIGHT -1)
    {
      ++positionPlayerRight;
      return true;
    }      
  } 
  return false;
}
