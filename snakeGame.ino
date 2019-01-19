/* LedTable
 *
 * Written by: Klaas De Craemer, Ing. David Hrbaty
 * 
 * Snake game
 */

#define SPEED 500
#define BUTTIME 500

void snakeInit(){
  //Snake start position and direction & initialise variables
  curLength[0] = 3;
  xs[0][0]=3; xs[0][1]=2; xs[0][2]=1;
  ys[0][0]=FIELD_HEIGHT/2; ys[0][1]=FIELD_HEIGHT/2; ys[0][2]=FIELD_HEIGHT/2;
  dir[0] = DIR_RIGHT;

  if(nbPlayer ==2) {
    curLength[1] = 3;
    xs[1][0]=11; xs[0][1]=12; xs[0][2]=13;
    ys[1][0]=FIELD_HEIGHT/2-1; ys[0][1]=FIELD_HEIGHT/2-1; ys[0][2]=FIELD_HEIGHT/2-1;
    dir[1] = DIR_LEFT;
    score[1] = 0;
  }

  
  score[0] = 0;

  
  //Generate random apple position
  ax = random(FIELD_WIDTH-1);
  ay = random(FIELD_HEIGHT-1);
  // Check not snake position

  snakeGameOver = false;
}

void runSnake(){
  boolean snakeRunning = true;

  //Check buttons and set snake movement direction while we are waiting to draw the next move
  unsigned long curTime, now;
  unsigned long  dirChanged= 0;//= false;
  unsigned long  dirChanged2=0;// = false;

  uint8_t i,j;

  unsigned long snakecol[]= { PURPLE , BLUE, GREEN };
  unsigned long snakecolhead[4];

  snakecolhead[0]=WHITE; //CRGB(0xFF0000);
  snakecolhead[1]=LBLUE; //CRGB(0xFFFF00);
  if (nbPlayer>2) nbPlayer=2;
  
  snakeInit();
  
  while(snakeRunning){    
    //Check self-collision with snake
    

    for (j=0; j<nbPlayer; j++)
    {    
      int i=curLength[j]-1;
      while (i>=2){
        if (collide(xs[j][0], xs[j][i], ys[j][0], ys[j][i], SNAKEWIDTH, SNAKEWIDTH, SNAKEWIDTH, SNAKEWIDTH)){
          Serial.println("Selfcollision");
          die();
        }
        i = i-1;
      }
    }
    
    if (snakeGameOver){
      snakeRunning = false;
      break;
    }

    //Check collision of snake head with apple
    for (j=0; j<nbPlayer; j++)
    {
      if (collide(xs[j][0], ax, ys[j][0], ay, SNAKEWIDTH, SNAKEWIDTH, SNAKEWIDTH, SNAKEWIDTH)){
        //Increase score and snake length;
        score[j]++;
        curLength[j]++;
        //Add snake segment with temporary position of new segments
        xs[j][curLength[j]-1] = 150; //TODO usefull ?
        ys[j][curLength[j]-1] = 150; //
      
        //Generate new apple position
        ax = random(FIELD_WIDTH-1);
        ay = random(FIELD_HEIGHT-1);

        //Generate apple color
//        acolor = colorLib[random(1)];

//NOT WORKING for 2 Player
        int i=curLength[j]-1;
        for(int i=0; i<curLength[j]; i++) {
          if (collide(ax, xs[j][i], ay, ys[j][i], SNAKEWIDTH, SNAKEWIDTH, SNAKEWIDTH, SNAKEWIDTH)){
             ax = random(FIELD_WIDTH-1);
             ay = random(FIELD_HEIGHT-1);
             i=0;
          }
        }
      }
    }
    
    //Shift snake position array by one
    
    for (j=0; j<nbPlayer; j++)
    {
      i = curLength[j]-1;
      while (i>=1){
        xs[j][i] = xs[j][i-1];
        ys[j][i] = ys[j][i-1];
        i = i-1;
      }
 
      //Determine new position of head of snake
      if (dir[j] == DIR_RIGHT){
        xs[j][0]++ ;
      } 
      else if (dir[j] == DIR_LEFT){
        xs[j][0]--;
      } 
      else if (dir[j] == DIR_UP){
        ys[j][0]--;
      } 
      else {//DOWN
        ys[j][0]++;
      }
    }
    
    //Check if outside playing field
    for (j=0; j<nbPlayer; j++)
    {    
//      if ((xs[j][0]<0) || (xs[j][0]>=FIELD_WIDTH) || (ys[j][0]<0) || (ys[j][0]>=FIELD_HEIGHT)){
        if (xs[j][0]<0) {xs[j][0] =FIELD_WIDTH -1;}
        else if (xs[j][0]>=FIELD_WIDTH) {xs[j][0] = 0;}      
        else if (ys[j][0]<0) {ys[j][0] =FIELD_HEIGHT -1;}
        else if (ys[j][0]>=FIELD_HEIGHT) {ys[j][0] = 0;}    
//      }
    }
    
    clearTablePixels();
   
    //Draw apple
    setTablePixel(ax,ay,YELLOW);

    //Draw snake
    for (j=0; j<nbPlayer; j++)
    {    
      for (int i=0; i<curLength[j]; i++)
      {
        if(i==0) setTablePixel(xs[j][i], ys[j][i], snakecolhead[j]);//
        else setTablePixel(xs[j][i], ys[j][i], snakecol[j]);
//        setTablePixel(xs[0][i], ys[0][i], WHITE);
      }
    }
    
    showPixels();


    curTime = millis();
    do{
      readInput();
      if (curControl == BTN_EXIT){
        snakeRunning = false;
        break;
      } 
      now=millis();

      if ( curControl != BTN_NONE )
      {
        if (nbPlayer == 1)
        {
          if (  (now-dirChanged)>BUTTIME && ( !(curControl&BTN_LEFT) || !(curControl&BTN_RIGHT) ) )  //Can only change direction once per loop
          {
            dirChanged=now; 
            setDirection();
          }
        }
        else if (nbPlayer == 2)
        { 
          if ( (now-dirChanged )>BUTTIME && ( !(curControl&BTN_LEFT) || !(curControl&BTN_UP) ) ) //Can only change direction once per loop
          {
            dirChanged=now; 
            setDirectionJ1();
          }
        
          if ( (now-dirChanged2)>BUTTIME && ( !(curControl&BTN_DOWN) || !(curControl&BTN_RIGHT) ) ) //Can only change direction once per loop
          {
            dirChanged2=now;
            setDirectionJ2();
          }
        }
      }
 /*     
      if (curControl != BTN_NONE && !dirChanged){//Can only change direction once per loop
        dirChanged = true;
        setDirection();
      }      */

    } 
    while ( (millis() - curTime ) <SPEED);//Once enough time  has passed, proceed. The lower this number, the faster the game is // 

 //   prevUpdateTime = curTime;
  }
  
  fadeOut();

  printNumber (score[0], 0, 0, snakecol[0]);
  printNumber (score[1], 8, 0, snakecol[1]);
  
  showPixels();
  delay (4000);
  fadeOut();
  displayLogo();   
/*
  char buf[4];
  int len = sprintf(buf, "%i", score[0]);
  scrollTextBlocked(buf,len,WHITE);
  */
}

/* Set direction from current button state */

void setDirection(){

  if (curControl & BTN_LEFT)
  {
     dir[0]--;
      if (dir[0]==0) dir[0]=4;    
  }
  if (curControl & BTN_RIGHT)
  {
      dir[0]++;
      if (dir[0]==5) dir[0]=1;
  }
}

void setDirectionJ1(){

  if (curControl & BTN_LEFT)
  {
     dir[0]--;
      if (dir[0]==0) dir[0]=4;    
  }
  if (curControl & BTN_UP)
  {
      dir[0]++;
      if (dir[0]==5) dir[0]=1;
  }
}

void setDirectionJ2(){
  if (curControl & BTN_DOWN) 
  {
      dir[1]--;
      if (dir[1]==0) dir[1]=4;
  }
  if (curControl & BTN_RIGHT)
  {
      dir[1]++;
      if (dir[1]==5) dir[1]=1;
  }
/*
  switch(curControl){
    case BTN_LEFT:
      dir[0]--;
      if (dir[0]==0) dir[0]=4;
      break;
    case BTN_RIGHT:
      dir[0]++;
      if (dir[0]==5) dir[0]=1;
      break;
    case BTN_DOWN:
      dir[1]--;
      if (dir[1]==0) dir[1]=4;
      break;
    case BTN_UP:
      dir[1]++;
      if (dir[1]==5) dir[1]=1;
      break;

    case BTN_LEFT:
      dir = DIR_LEFT;
      break;
    case BTN_RIGHT:
      dir = DIR_RIGHT;
      break;
    case BTN_DOWN:
      dir = DIR_DOWN;
      break;
    case BTN_UP:
      dir = DIR_UP;
      break;
      
    case BTN_START:
      break;
  }
  */
}

/* Ending, show score */
void die(){
  snakeGameOver = true;
  Serial.println("die");
}

/* Collision detection function */
boolean collide(int x1, int x2, int y1, int y2, int w1, int w2, int h1, int h2){
  if ((x1+w1>x2) && (x1<x2+w2) && (y1+h1>y2) && (y1<y2+h2)){
    return true;
  } 
  else {
    return false;
  }
}
