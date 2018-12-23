/* LedTable
 *
 * Written by: Ing. David Hrbaty
 * 
 * 
 * Main code for Dice game
 */
 
#include "diceCommon.h"
#define MATH_ANIMATION 20
boolean diceRunning;
boolean diceMath;

// https://xantorohara.github.io/led-matrix-editor/

const uint64_t DAFTPUNK[] = {
  0x0000001b1b001b1b,
  0x000000001b1b1b00,
  0x000000000e0e0e00,
  0x0000000e0e000e0e,
  0x0000001b1b001b1b,
  0x0000001500110015,
  0x000000000e0a0e00,
  0x0000001500110015,
  0x00000007051f141c,
  0x0000001c141f0507,
  0x0000001f1515151f,
  0x000000001f111f00,
  0x0000001119151311,
  0x0000001113151911,
  0x00000000181b0300,
  0x00000000031b1800,
  0x0000001111111111,
  0x0000000a0a0a0a0a,
  0x0000000404040404,
  0x0000000c0c0c0c0c,
  0x000000111b1f0e04,
  0x000000040e1f1b11,
  0x0000001f1111111f,
  0x000000001f111f00,
  0x000000110a040a11,
  0x000000111b0e1b11,
  0x000000000a1f0a00,
  0x00000000040e0400,
  0x000000000a040a00,
  0x00000004041f0404,
  0x000000110a000a11
};
const int DAFTPUNK_LEN = sizeof(DAFTPUNK)/8;

               
void initDice() {
  diceRunning = true;
 // diceMath = false;
//  randomSeed(analogRead(0));
  // Clear pixels
  for (int i = 0; i < FIELD_WIDTH * FIELD_HEIGHT; i++)
    setPixel(i, 0);
  showPixels();
}

void displayImageDP(uint64_t image) {
  for (int y = 0; y < 5; y++) 
  {
    byte row = (image >> y * 8) & 0xFF;
    for (int x = 0; x<5; x++) 
    {
	    if( bitRead(row, x) )
      {
          setTablePixelDouble (x+1, y, RED );
/*
          setTablePixel (x, y, RED);
          setTablePixel (x+5, y, YELLOW);
          setTablePixel (x+10, y, RED);
          setTablePixel (x, y+5, YELLOW);
          setTablePixel (x+5, y+5, RED);
          setTablePixel (x+10, y+5, YELLOW);
          */
      }
      else
      {
          setTablePixelDouble (x+1, y, BLACK);
 /*         
         setTablePixel (x, y, BLACK);
          setTablePixel (x+5, y, BLACK);
          setTablePixel (x+10, y, BLACK);
          setTablePixel (x, y+5, BLACK);
          setTablePixel (x+5, y+5, BLACK);
          setTablePixel (x+10, y+5, BLACK);
 */
      }
    }
  }
  showPixels();
  delay (500);
}

void runDice() {
  initDice();
  unsigned long prevUpdateTime = 0;
  unsigned long curTime = 0;
  uint8_t i = 1;
  
  while(diceRunning) 
  {
    displayImageDP(DAFTPUNK[i]);
    if (++i >= DAFTPUNK_LEN ) i = 0;
   
    do
    {
      readInput();
      if (curControl == BTN_EXIT){
        diceRunning = false;
        break;
      }
      curTime = millis();
      delay(550);
    }
    while ((curTime - prevUpdateTime) <20);//Once enough time  has passed, proceed. The lower this number, the faster the game is
    prevUpdateTime = curTime;
   }
}
/*
void printDice(const boolean dice[][10]){
  int x,y;
  for (x=0;x<FIELD_WIDTH;x++){
    for (y=0;y<FIELD_HEIGHT;y++){
      if (pgm_read_byte_near ( &dice[x][y]) == 1){
        setTablePixel(y,x, YELLOW);
      } else {
        setTablePixel(y,x, 0x000000);
      }
    }
  }
  showPixels();
}
*/
