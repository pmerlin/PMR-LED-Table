/* LedTable
 *
 */
#include "font2.h"

boolean letter[3][5];

void loadLetter(char l)
{
  uint8_t col;
//  int letterIdx = (l-32)*3;
  
  for (uint8_t x=0; x<4; x++)
  {
    if (x==3) col = 0;
    else col=Wendy3x5[ (l-32)*3 +x ];

    for (uint8_t y=0; y<5; y++)
    {
      if (bitRead(col, y) )
        letter[x][y] = true;
      else
        letter[x][y] = false;
    } 
  }
}

void printText3(char* text, uint8_t xoffset, uint8_t yoffset, unsigned long color[2] ){
//  uint8_t curLetterWidth = 0;
//  uint8_t curX = xoffset;
  
  //Loop over all the letters in the string
  for (uint8_t i=0; i<strlen(text); i++){
    loadLetter(text[i]);
     
    //Loop until width of letter is reached
    for (uint8_t lx=0; lx<3; lx++){
      //Now copy column per column to field (as long as within the field
//      if ( (xoffset+lx)>=LONG_SIDE){//If we are to far to the right, stop loop entirely
  //      break;
 //     } //else if (curX>=0){//Draw pixels as soon as we are "inside" the drawing area
      for (uint8_t ly=0; ly<5; ly++){//Finally copy column
        if (letter[lx][ly])
          setTablePixel(xoffset+lx+3*i,yoffset+ly,color[i%2]);
        else
          setTablePixel(xoffset+lx+3*i,yoffset+ly,BLACK);
      }
//      curX++;
    }
  }
  
  showPixels();
}

void initNbPlayer(){  
  appRunning = true;
  clearTablePixels();
  showPixels();
}

void runNbPlayer(){
  initNbPlayer();
  unsigned long col[2];
  col[0]= YELLOW;
  col[1]= RED;

  
  unsigned long prevStarsCreationTime = 0;
  unsigned long prevUpdateTime = 0;
  unsigned long curTime, click;
  
  while(appRunning){
 
    printText3 ("Playe", 0, 0, col);
//// some stuff
    printNumber (nbPlayer, 4, 5, RED);
    showPixels();
    
    //Check input keys
    curTime=millis();
    do{
      readInput();
      if (curControl == BTN_EXIT){
        appRunning = false;
        break;
      }
      else if (curControl != BTN_NONE && millis()-click > 600)
      {
        click=millis();
        if (curControl == BTN_RIGHT) nbPlayer++;
        else if (curControl == BTN_LEFT) nbPlayer--;

        if (nbPlayer<MINPLAYER) nbPlayer=MINPLAYER;
        else if (nbPlayer>MAXPLAYER) nbPlayer=MAXPLAYER;
        
        clearTablePixels();
      }

    }
    while ((millis()- curTime) <80); ;//Once enough time  has passed, proceed. The lower this number, the faster the game is
  }
  displayLogo();
}
