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

  uint8_t i;

  nbPlayerDie=0;
  
  for (i=0; i<nbPlayer; i++)
  {
    curLength[i] = 3;
    score[i] = 0;
    isDead[i] =0;
  }
  
  xs[0][0]=3; xs[0][1]=2; xs[0][2]=1;
  ys[0][0]=4; ys[0][1]=4; ys[0][2]=4;
  dir[0] = DIR_RIGHT;

  if(nbPlayer >1)
  {
    xs[1][0]=11; xs[1][1]=12; xs[1][2]=13;
    ys[1][0]=6; ys[1][1]=6; ys[1][2]=6;
    dir[1] = DIR_LEFT;
  }
  
  if(nbPlayer > 2)
  {
    ys[0][0]=2; ys[0][1]=2; ys[0][2]=2;
    ys[1][0]=3; ys[1][1]=3; ys[1][2]=3;

    xs[2][0]=3; xs[2][1]=2; xs[2][2]=1;
    ys[2][0]=7; ys[2][1]=7; ys[2][2]=7;
    dir[2] = DIR_RIGHT;
  }

  if(nbPlayer > 3)
  {
    xs[3][0]=11; xs[3][1]=12; xs[3][2]=13;
    ys[3][0]=8; ys[3][1]=8; ys[3][2]=8;
    dir[3] = DIR_LEFT;   
  }
  
  //Generate random apple position
  newApple();
  /*
  
  ax = random(FIELD_WIDTH-1);
  ay = random(FIELD_HEIGHT-1);
  // TODO Check not snake position
*/
  snakeGameOver = false;
}

void newApple()
{
  uint8_t i,j,collision; 
  do
  {
    collision=0;
    ax = random(FIELD_WIDTH-1);
    ay = random(FIELD_HEIGHT-1);
    for (j=0; j<nbPlayer; j++)
      for(i=0; i<curLength[j]; i++)
        if (checkCollision(ax, xs[j][i], ay, ys[j][i])) 
        {
          collision++;
          break;
        }
   }
   while (collision !=0);
}

void runSnake(){
  boolean snakeRunning = true;

  //Check buttons and set snake movement direction while we are waiting to draw the next move
  unsigned long curTime, now;
  unsigned long dirChanged= 0;
  unsigned long dirChanged2=0;
  unsigned long dirChanged3=0;
  unsigned long dirChanged4=0;
  uint8_t i,j,len;

  unsigned long snakecol[]= { PURPLE , BLUE, GREEN, WHITE };
  unsigned long snakecolhead[MAXSNAKEPLAYER];

  snakecolhead[0]=LPURPLE; //CRGB(0xFF0000);
  snakecolhead[1]=LBLUE; //CRGB(0xFFFF00);
  snakecolhead[2]=LGREEN;
  snakecolhead[3]=LWHITE;
  
  if (nbPlayer>MAXSNAKEPLAYER) nbPlayer=MAXSNAKEPLAYER;
  
  snakeInit();
  
  while(snakeRunning){    
 
    //Check collision with snake
    for (j=0; j<nbPlayer; j++)
    {
      if ( isDead[j] ) continue;   
      for (i=0; i<nbPlayer; i++)
      {
        if (i!=j) //not same snake
        {
          for(len=0; len < curLength[i]; len++)
          {
            if (checkCollision(xs[j][0], xs[i][len], ys[j][0], ys[i][len]))
            {
              Serial.println("\ncollision with other\n");
              isDead[j]++;
              die();
              break;
            }
          }
        }
        else // same snake i==j
        {
          len=curLength[j]-1;
          while (len>3) // need to be at least 4 for a self-collision
          {
            if (checkCollision(xs[j][0], xs[j][len], ys[j][0], ys[j][len]))
            {
              Serial.println("\nself collision\n");
              isDead[j]++;
              die();
              break;
            }
            len--;            
          }
        }
      }
/*      
      len=curLength[j]-1;
      while (len>=2){
//        if (collide(xs[j][0], xs[j][len], ys[j][0], ys[j][len], SNAKEWIDTH, SNAKEWIDTH, SNAKEWIDTH, SNAKEWIDTH))
        if (checkCollision(xs[j][0], xs[j][len], ys[j][0], ys[j][len]))
        {
          Serial.println("collision");
          isDead[j]++;
          die();
        }
        len--;
      }
 */     
    }

    if (snakeGameOver){
      snakeRunning = false;
      break;
    }

    //Check collision of snake head with apple
    for (j=0; j<nbPlayer; j++)
    {
      if ( isDead[j] ) continue;
//      if (collide(xs[j][0], ax, ys[j][0], ay, SNAKEWIDTH, SNAKEWIDTH, SNAKEWIDTH, SNAKEWIDTH))
      if (checkCollision(xs[j][0], ax, ys[j][0], ay))
      {

        //Add snake segment with temporary position of new segments
 //       xs[j][curLength[j]] = xs[j][curLength[j]-1]; //TODO usefull ?
 //       ys[j][curLength[j]] = ys[j][curLength[j]-1]; //
 
        //Increase score and snake length;
        score[j]++;
        curLength[j]++;
              
        //Generate new apple position
        newApple();

      }
    }
    
    //Shift snake position array by one   
    for (j=0; j<nbPlayer; j++)
    {
      if ( isDead[j] ) continue;
      i = curLength[j]-1;
      while (i>=1){
        xs[j][i] = xs[j][i-1];
        ys[j][i] = ys[j][i-1];
        i = i-1;
      }
 
      //Determine new position of head of snake
      if (dir[j] == DIR_RIGHT){
        xs[j][0]++;
        if ( xs[j][0] == LONG_SIDE )  xs[j][0]=0;
      } 
      else if (dir[j] == DIR_LEFT){
        if ( xs[j][0] == 0) xs[j][0]=LONG_SIDE;
        xs[j][0]--;
      } 
      else if (dir[j] == DIR_UP){
        if ( ys[j][0] == 0) ys[j][0]=SHORT_SIDE;
        ys[j][0]--;
      } 
      else {//DOWN
        ys[j][0]++;
        if ( ys[j][0] == SHORT_SIDE )  ys[j][0]=0;
      }
    }
    
    //Check if outside playing field
    for (j=0; j<nbPlayer; j++)
    {
      if ( isDead[j] ) continue; 
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

    //Draw snakes
    for (j=0; j<nbPlayer; j++)
    {    
      for (i=0; i<curLength[j]; i++)
      {
        if ( isDead[j] ) setTablePixel(xs[j][i], ys[j][i], RED);
        else 
        {
          if(i==0) setTablePixel(xs[j][i], ys[j][i], snakecolhead[j]);
          else setTablePixel(xs[j][i], ys[j][i], snakecol[j]);
//        setTablePixel(xs[0][i], ys[0][i], WHITE);
        }
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
/*        
        Serial.print(curControl);

 */       
        if (nbPlayer == 2)
        {
          if ( (now-dirChanged )>BUTTIME && ( (curControl&BTN_LEFT) || (curControl&BTN_RIGHT) ) ) //Can only change direction once per loop
          {
//            Serial.print("P1");
            dirChanged=now; 
            setDirectionJ1_2();
          }
        
          if ( (now-dirChanged2)>BUTTIME  && ( (curControl&BTN_LEFT2) || (curControl&BTN_RIGHT2) ) ) //Can only change direction once per loop
          {
//            Serial.print("P2");
            dirChanged2=now;
            setDirectionJ2_2();
          }          
        }
        else if (nbPlayer == 1)
        {
          if (  (now-dirChanged)>BUTTIME &&  (curControl>BTN_START)  )  //Can only change direction once per loop
          {
            Serial.print("\nP1");
            dirChanged=now; 
            setDirection();
          }
        }
       
        else if (nbPlayer >2)
        { 
          Serial.print(curControl);
          if ( (now-dirChanged )>BUTTIME && ( (curControl&BTN_LEFT) || (curControl&BTN_UP) ) ) //Can only change direction once per loop
          {
//            Serial.print("P1");
            dirChanged=now; 
            setDirectionJ1();
          }
        
          if ( (now-dirChanged2)>BUTTIME  && ( (curControl&BTN_LEFT2) || (curControl&BTN_UP2) ) ) //Can only change direction once per loop
          {
//            Serial.print("P2");
            dirChanged2=now;
            setDirectionJ2();
          }

          if ( (nbPlayer >2) && (now-dirChanged3 )>BUTTIME &&( (curControl&BTN_DOWN) || (curControl&BTN_RIGHT) ) )
          {
//            Serial.print("P3");
            dirChanged3=now;
            setDirectionJ3();
          }

          if ( (nbPlayer >3) && (now-dirChanged4 )>BUTTIME && ( (curControl&BTN_DOWN2) || (curControl&BTN_RIGHT2) ) )
          {
//            Serial.print("P4");
            dirChanged4=now;
            setDirectionJ4();
          }
        } 
      }
    } 
    while ( (millis() - curTime ) <SPEED);//Once enough time  has passed, proceed. The lower this number, the faster the game is // 
  }
  
  fadeOut();

  printNumber (score[0], 0, 0, snakecol[0]);
  if (nbPlayer > 1) printNumber (score[1], 8, 0, snakecol[1]);
  if (nbPlayer > 2) printNumber (score[1], 0, 5, snakecol[2]);
  if (nbPlayer > 3) printNumber (score[1], 8, 5, snakecol[3]);
  
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

void setDirection()
{
  if (curControl & BTN_LEFT || curControl & BTN_DOWN || curControl & BTN_RIGHT2 || curControl & BTN_UP2)
  {
     dir[0]--;
     if (dir[0]==0) dir[0]=4;    
  }
  if (curControl & BTN_RIGHT ||  curControl & BTN_UP || curControl & BTN_LEFT2 || curControl & BTN_DOWN2)
  {
     dir[0]++; 
     if (dir[0]==5) dir[0]=1;
  }
}

void setDirectionJ1_2()
{
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

void setDirectionJ2_2()
{
  if (curControl & BTN_RIGHT2)
  {
     dir[1]--;
     if (dir[1]==0) dir[1]=4;    
  }
  if (curControl & BTN_LEFT2)
  {
     dir[1]++;
     if (dir[1]==5) dir[1]=1;
  }
}

void setDirectionJ1()
{
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

void setDirectionJ2()
{
  if (curControl & BTN_UP2)
  {
     dir[1]--;
     if (dir[1]==0) dir[1]=4;    
  }
  if (curControl & BTN_LEFT2)
  {
     dir[1]++;
     if (dir[1]==5) dir[1]=1;
  }
}

void setDirectionJ3()
{
  if (curControl & BTN_DOWN) 
  {
     dir[2]--;
     if (dir[2]==0) dir[2]=4;
  }
  if (curControl & BTN_RIGHT)
  {
     dir[2]++;
     if (dir[2]==5) dir[2]=1;
  }
}

void setDirectionJ4()
{
  if (curControl & BTN_RIGHT2) 
  {
      dir[3]--;
      if (dir[3]==0) dir[3]=4;
  }
  if (curControl & BTN_DOWN2)
  {
      dir[3]++;
      if (dir[3]==5) dir[3]=1;
  }
}

/* Ending, show score */
void die()
{
  nbPlayerDie++;
  if( nbPlayerDie == nbPlayer )
  {
    snakeGameOver = true;
    Serial.println("die");
  }
}

/* Collision detection function */
boolean checkCollision(int x1, int x2, int y1, int y2)
{
  if ( x1==x2 && y1==y2)
    return true;
  return false;
}

/*
boolean collide(int x1, int x2, int y1, int y2, int w1, int w2, int h1, int h2){
  if ((x1+w1>x2) && (x1<x2+w2) && (y1+h1>y2) && (y1<y2+h2)){
    return true;
  } 
  else {
    return false;
  }
}
*/
