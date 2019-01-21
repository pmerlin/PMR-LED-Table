/* LedTable
 *
 * Written by: Ing. David Hrbaty
 * 
 * 
 * Main code for Bricks game
 */
 
float ballX = 10;
float ballY = 6;
float xincrement = 1;
float yincrement = 1;

int rad = 1;
int scorePlayer = 0;
int blockWidth = 1;
int blockHeight = 1;
int maxAttempt = 1;

int positionPlayer = 6;

int numBlocks = 30;
#define MAX_SCORE numBlocks
#define MAX_ATTEMPT 5
#define PADDLE_SIZE 3
boolean continueGame = true;

char bricks[30][3] = {
  // in play, xloc, yloc 
   {1,0,0}  ,
   {1,1,0}  ,
   {1,2,0}  ,
   {1,3,0}  ,
   {1,4,0}  ,
   {1,5,0}  ,
   {1,6,0}  ,
   {1,7,0}  ,
   {1,8,0}  ,
   {1,9,0}  ,
   {1,0,1}  ,
   {1,1,1}  ,
   {1,2,1}  ,
   {1,3,1}  ,
   {1,4,1}  ,
   {1,5,1}  ,
   {1,6,1}  ,
   {1,7,1}  ,
   {1,8,1}  ,
   {1,9,1}  , 
   {1,0,2}  ,
   {1,1,2}  ,
   {1,2,2}  ,
   {1,3,2}  ,
   {1,4,2}  ,
   {1,5,2}  ,
   {1,6,2}  ,
   {1,7,2}  ,
   {1,8,2}  ,   
   {1,9,2}  
};
/* Block shape */
//static uint8_t brick[] PROGMEM = {
static uint8_t brick[]  = {
  10,
  8,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

void bricksInit(){
  scorePlayer = 0;
  maxAttempt = 0;
  ballY = 6;
  ballX = 10;
  for (int i=0; i<numBlocks; i++){
    bricks[i][0] = 1;
  }
}

void runBricks(){
  bricksInit();
  unsigned long prevUpdateTime = 0;
  boolean bricksRunning = true;

  while(bricksRunning){    
    
    if (scorePlayer == MAX_SCORE || maxAttempt == MAX_ATTEMPT){
      bricksRunning = false;
      break;
    }
    
    checkBallHitByPaddle();
    checkBlockCollision();
    
    ballX += xincrement;
    ballY += yincrement;
    
    checkBallOutOfBoundsTable();
    clearTablePixels();
    

    // Draw ball
    setTablePixelv(ballX,ballY,WHITE);
    
    // Draw player paddle
    for (int x=positionPlayer-PADDLE_SIZE/2; x<=positionPlayer+PADDLE_SIZE/2; ++x){
      setTablePixelv(x, LONG_SIDE-1, BLUE);
//      setTablePixelv(x, SHORT_SIDE-1, BLUE);
    }
    // Draw bricks
    for (int i=0; i<numBlocks; i++){
      if(bricks[i][0] == 1) {
        setTablePixelv(bricks[i][1],bricks[i][2], GREEN);
      }
    }
    showPixels();

    unsigned long curTime;
    boolean dirChanged = false;
    do{
      readInput();
      if (curControl == BTN_EXIT){
        bricksRunning = false;
        break;
      }
      if (curControl != BTN_NONE && !dirChanged){//Can only change direction once per loop
        dirChanged = true;
        setPositionPaddle();
      }
      curTime = millis();
    } 
    while ((curTime - prevUpdateTime) <250);//Once enough time  has passed, proceed. The lower this number, the faster the game is
    prevUpdateTime = curTime;
  }
  
  fadeOut();
  char buf[4];
  int len = sprintf(buf, "%i", scorePlayer);
  scrollTextBlockedv(buf,len,WHITE);
  displayLogo();
}

void setPositionPaddle(){
  switch(curControl){
    case BTN_RIGHT:
      if(positionPlayer + (PADDLE_SIZE-1) / 2 < SHORT_SIDE-1){
        ++positionPlayer;
      }
      break;
    case BTN_LEFT:
      if(positionPlayer - PADDLE_SIZE / 2 > 0) {
        --positionPlayer;
      }
      break;
    case BTN_START:
      break;
    case BTN_UP:
      break;
    case BTN_DOWN:
      break;
  }
}

void checkBallHitByPaddle() {
//  if(ballY == SHORT_SIDE-2)
  if(ballY == LONG_SIDE-2) // line above paddle
  {
    if(ballX == positionPlayer) // paddle center
    {
      yincrement = -1;
 //     ballY = SHORT_SIDE-2;
 //     ballY = LONG_SIDE-2;
    } 
    else if(ballX < positionPlayer && ballX >= positionPlayer - PADDLE_SIZE / 2) // left side of paddle
    {
      yincrement = -1;
      xincrement = max(-1,(int)xincrement-1); 
//      ballY = FIELD_WIDTH-2;
//     ballY = LONG_SIDE-2;
      ballX = positionPlayer - PADDLE_SIZE / 2-1;
    }    
    else if(ballX > positionPlayer && ballX <= positionPlayer + (PADDLE_SIZE-1) / 2) // right side of the paddle
    {
      yincrement = -1;
      xincrement = min(1,(int)xincrement+1); //right
 //     ballY = SHORT_SIDE-2;
      ballX = positionPlayer + (PADDLE_SIZE-1) / 2+1;
    }    
  } 
}

void checkBallOutOfBoundsTable() {
  if(ballY < 0) 
  {
    yincrement = - yincrement;
    ballY = 1;
  } 
  else if(ballY > LONG_SIDE-1) 
  {
    yincrement = - yincrement;
    xincrement = 0;
    ballY = LONG_SIDE/2;
    ballX = SHORT_SIDE/2;
    maxAttempt++;   
  } 
  if(ballX < 0) 
  {
    xincrement = - xincrement;
    ballX = 1;
  } 
  else if(ballX > SHORT_SIDE-1) 
  {
    xincrement = - xincrement;
    ballX = SHORT_SIDE-2;
  } 
}

boolean checkBlockCollision(){
    int ballTop = ballY-rad;                                            // Values for easy reference
    int ballBottom = ballY+rad;
    int ballLeft = ballX-rad;
    int ballRight = ballX+rad;
    
    for(int i=0;i<numBlocks;i++){                                       // Loop through the blocks
        if(bricks[i][0] == 1){                                          // If the block hasn't been eliminated
         int blockX = bricks[i][1];                                     // Grab x and y location
         int blockY = bricks[i][2];
         if(ballBottom >= blockY && ballTop <= blockY+blockHeight){     // If hitting BLOCK
           if(ballRight >= blockX && ballLeft <= blockX+blockWidth){       
             removeBlock(i);                                            // Mark the block as out of play
             return true;
           }
         }
      }
    }
  return false;                                                         // No collision detected
}
/* Removes a block from game play */
void removeBlock(int index){
      bricks[index][0] = 0;                                             // Mark it as out of play
      scorePlayer++;                                                          // Increment score
      yincrement = -yincrement;                                         // Flip the y increment
}
