/* LedTable
 *
 * Written by: Klaas De Craemer, Ing. David Hrbaty
 * 
 * 
 * Main file with common methods and defines, such as button reading from Bluetooth controller
 * or setting pixels on the LED area
 */
#include <SoftwareSerial.h>
//#include <Keypad.h>
#define BRIGHTNESS 40
#define TEXTSPEED  140
//#define DEBUG
//#define COLOR_ORDER GRB
//#define FAST_LED_CHIPSET WS2811
#define FAST_LED_CHIPSET NEOPIXEL



//LED field size
//Hori
#define  FIELD_WIDTH       15
#define  FIELD_HEIGHT      10

//Vert
//#define  FIELD_WIDTH       10
//#define  FIELD_HEIGHT      15

#define LONG_SIDE 15
#define SHORT_SIDE 10

#define  ORIENTATION_HORIZONTAL  //Rotation of table, uncomment to rotate field 90 degrees
//#define  ORIENTATION_VERTICAL

#define USE_FAST_LED   // FAST_LED as library to control the LED strips
/*
 * FAST_LED implementation 
 */

#define  NUM_PIXELS    FIELD_WIDTH*FIELD_HEIGHT

#include "FastLED.h"

CRGB leds[NUM_PIXELS];

#define BLUETOOTH_SPEED 38400 
/*
 * Some defines used by the FAST_LED library
 */


#if defined(USE_OCTOWS2811) && defined(USE_FAST_LED)
#error "Only one of USE_OCTOWS2811 and USE_FAST_LED can be defined"
#endif



/* *** LED color table *** */
//BGR
#define  BLACK  0x000000
#define  GREEN  0x00FF00
#define  RED    0xFF0000
#define  BLUE   0x0000FF
#define  YELLOW 0xFFFF00
#define  LBLUE  0x00FFFF
#define  PURPLE 0xFF00FF
#define  WHITE  0xFFFFFF
unsigned int colorLib[3] = {YELLOW, BLUE, WHITE};
/* *** Game commonly used defines ** */
#define  DIR_UP    1
//#define  DIR_DOWN  2
//#define  DIR_LEFT  3
//#define  DIR_RIGHT 4
#define  DIR_DOWN  3
#define  DIR_LEFT  4
#define  DIR_RIGHT 2


/* *** Bluetooth controller button defines and input method *** */
#define  BTN_NONE  0
#define  BTN_UP    1
#define  BTN_DOWN  2
#define  BTN_LEFT  4
#define  BTN_RIGHT  8
#define  BTN_START  16
#define  BTN_EXIT  32
#define  BTN_UP2    64
#define  BTN_DOWN2  128
#define  BTN_LEFT2  256
#define  BTN_RIGHT2  512

//#define WEMOS 
#ifdef WEMOS
#define FAST_LED_DATA_PIN  D6
#define L_pin D2
#define U_pin D3
#define D_pin D4
#define R_pin D5

#define E_pin D8
#define S_pin D9

#define L2_pin D10
#define U2_pin D11
#define D2_pin D12
#define R2_pin D13
#else
#define FAST_LED_DATA_PIN  6
#define L_pin 2
#define U_pin 3
#define D_pin 4
#define R_pin 5

#define E_pin 8
#define S_pin 9

#define L2_pin 10
#define U2_pin 11
#define D2_pin 12
#define R2_pin 13
#endif

/*
#define X_pin  0 // analog pin connected to X output
#define Y_pin  1 // analog pin connected to Y output
int X_init, Y_init;
const byte numRows= 4; //number of rows on the keypad
const byte numCols= 4; //number of columns on the keypad
char keymap[numRows][numCols]=
{
{'E', 'U', '3', 'S'},
{'L', '5', 'R', 'E'},
{'7', 'D', '9', 'C'},
{'*', '0', '#', 'D'}
};

//Code that shows the the keypad connections to the arduino terminals
byte rowPins[numRows] = {45,43,41,39}; //Rows 0 to 3
byte colPins[numCols]= {37,35,33,31}; //Columns 0 to 3
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);
*/

uint16_t curControl = BTN_NONE;
#define MINPLAYER 1
#define MAXPLAYER 8
uint8_t nbPlayer = MINPLAYER ;
boolean appRunning = false;

//unsigned long PrintCol[2];

SoftwareSerial bluetooth(10, 11);


const uint64_t CHIFFRE[] = {
  0x0000000705050507,
  0x0000000702020302,
  0x0000000701070407,
  0x0000000704070407,
  0x0000000404070505,
  0x0000000704070107,
  0x0000000705070107,
  0x0000000404060407,
  0x0000000705070507,
  0x0000000704070507
};

void printDigit (uint8_t num, uint8_t x, uint8_t y, unsigned long col)
{
     for (int i=0; i <5 ; i++)
     {
        byte row= (CHIFFRE[num] >>i *8) & 0xFF;
        for (int j = 0; j<3; j++)
        {
          if (bitRead(row,j))
            setTablePixel (j+x, i+y, col);
          else
            setTablePixel (j+x, i+y, BLACK);
        }
     }
}

void printNumber (uint8_t num, uint8_t x, uint8_t y, unsigned long col)
{
  uint8_t d=num/10;
  uint8_t u=num%10;

  if (d){
    printDigit (d,x,y, col);
    printDigit (u,x+4,y, col);
  }
  else printDigit (u,x+2,y, col);
}


void readInput(){
  curControl = BTN_NONE;

  if (!digitalRead(S_pin) )
    curControl += BTN_START;
  if (!digitalRead(L_pin))
    curControl += BTN_LEFT;
  if (!digitalRead(U_pin))
    curControl += BTN_UP;
  if (!digitalRead(R_pin))
    curControl += BTN_RIGHT;
  if (!digitalRead(D_pin))
    curControl += BTN_DOWN;
  if (!digitalRead(E_pin))
    curControl += BTN_EXIT;
    
  if (!digitalRead(L2_pin))
    curControl += BTN_LEFT2;
  if (!digitalRead(U2_pin))
    curControl += BTN_UP2;
    
  if (!digitalRead(R2_pin))
    curControl += BTN_RIGHT2;

  if (!digitalRead(D2_pin))
    curControl += BTN_DOWN2;


//  Serial.print(curControl);
/*  
  if (bluetooth.available() > 0) {
    // read the incoming byte:
    uint8_t incomingByte = bluetooth.read();
    Serial.println(incomingByte);
      switch(incomingByte){
        case 238:
          curControl = BTN_LEFT;
          break;
        case 239:
          curControl = BTN_RIGHT;
          break;
        case 236:
          curControl = BTN_UP;
          break;
        case 237:
          curControl = BTN_DOWN;
          break;
        case 224:
          curControl = BTN_START;
          break;
        case 225:
          curControl = BTN_EXIT;
          break;
      }
    } 
    */
}


void displayLogo(){
  const CRGB ipalette5[] = {
{ 0, 0, 0}, 
{255, 0, 0},
{0, 255, 0},
{30, 30, 255},
{100, 220, 220},
{160, 190, 200},
{ 100, 220, 250},
{110, 80, 210},
{ 210, 190, 200}
 };

  const uint8_t invader5[10][15] = {
  {1,1,1,0,2,2,2,3,0,0,0,3,4,0,0},
  {1,0,0,1,0,2,0,0,3,0,3,0,4,0,0},
  {1,0,0,1,0,2,0,0,0,3,0,0,4,0,0},
  {1,1,1,0,0,2,0,0,3,0,3,0,4,0,0},
  {1,0,0,0,2,2,2,3,0,0,0,3,4,4,4},
  {5,5,5,0,6,6,0,7,7,7,0,8,0,0,0},
  {0,5,0,6,0,0,6,7,0,0,7,8,0,0,0},
  {0,5,0,6,0,0,6,7,7,7,0,8,0,0,0},
  {0,5,0,6,6,6,6,7,0,0,7,8,0,0,0},
  {0,5,0,6,0,0,6,7,7,7,0,8,8,8,0}
};

  for (uint8_t y = 0; y < SHORT_SIDE; ++y) {
    for (uint8_t x = 0; x < LONG_SIDE; ++x) {
      uint8_t idx = invader5[y][x];
      setTablePixelrgb(x, y, ipalette5[idx]);
    }
  }
  FastLED.show();
  delay(2000);
}

void initPixels(){

//  FastLED.addLeds<FAST_LED_CHIPSET, FAST_LED_DATA_PIN, COLOR_ORDER>(leds, NUM_PIXELS).setCorrection(TypicalSMD5050);
  FastLED.addLeds<FAST_LED_CHIPSET, FAST_LED_DATA_PIN>(leds, NUM_PIXELS).setCorrection(TypicalSMD5050);

  FastLED.setBrightness(BRIGHTNESS);
#ifdef DEBUG
  leds[0] = CRGB(255,0,0); 
  leds[1] = CRGB(0,255,0);
  leds[2] = CRGB(0,255,0);
  leds[3] = CRGB(0,0,255);
  leds[4] = CRGB(0,0,255);
  leds[5] = CRGB(0,0,255);
setTablePixel(0,1,CRGB::Red );
setTablePixel(1,1,CRGB::Green );
setTablePixel(2,1,CRGB::Blue );
setTablePixel(3,1,YELLOW);
setTablePixel(4,1,LBLUE);
setTablePixel(5,1,PURPLE);
setTablePixel(6,1,WHITE);
setTablePixel(0,2,0xFF0000 );
setTablePixel(1,2,0x00FF00 );
setTablePixel(2,2,0x0000FF );
setTablePixel(0,3,CRGB(0xFF0000) );
setTablePixel(1,3,CRGB(0x00FF00) );
setTablePixel(2,3,CRGB(0x0000FF) );
   FastLED.show();
   delay(1000);
for (int i =0xFF; i; i--)
{
  setTablePixel(0,2,i<<16 );
  setTablePixel(1,2,i<<8 );
  setTablePixel(2,2,i );
  unsigned long tmp=i<<16+i<<8+i;
  setTablePixel(3,2,tmp );  
  
  setTablePixelRGB(0,3,i,0,0 );
  setTablePixelRGB(1,3,0,i,0 );
  setTablePixelRGB(2,3,0,0,i );
  setTablePixelRGB(3,3,i,i,i );
  FastLED.show();
  delay(20);
}


#endif
/*
  for (int y = 0; y < SHORT_SIDE ; y++)
  {
    for (int x = 0; x < LONG_SIDE ; x++)
    {
      setTablePixel (x, y, YELLOW );
      FastLED.show();
      delay(10);
      setTablePixel(x, y, 0);
    }
  }   
*/

 displayLogo();
 delay(2000);
 fadeOut();
}


void setPixel(int n, unsigned long color){
  leds[n] = CRGB(color);
}

void setPixelRGB(int n, int r,int g, int b){
  leds[n] = CRGB(r,g,b);
}

void setDelay(int duration) {
  FastLED.delay(duration);
}

unsigned long getPixel(int n){
  return (leds[n].r << 16) + (leds[n].g << 8) + leds[n].b;  
}


void showPixels(){
  FastLED.show();
}

void setTablePixelrgb(int x, int y, CRGB col){
 // #ifdef ORIENTATION_HORIZONTAL
//  setPixel(y%2 ? y*FIELD_WIDTH + x : y*FIELD_WIDTH + ((FIELD_HEIGHT-1)-x),color);
   
   if( y & 0x01) {
      // Odd rows run backwards
        leds[ (y * LONG_SIDE) + (LONG_SIDE - 1) - x ] = col ;
    } else {
      // Even rows run forwards
      leds [ (y * LONG_SIDE) + x] = col ;
      }
}

void setTablePixelrgbv(int x, int y, CRGB col){
  if (x & 0x01)
   leds[ x*LONG_SIDE + y ] = col ; //15
 else
   leds [ (LONG_SIDE-1-y)+ x*LONG_SIDE ] = col ;
}

void setTablePixelDouble(int x, int y, unsigned long col){
   setTablePixel( (x<<1), (y<<1), col);
   setTablePixel( (x<<1)+1, (y<<1), col);
   setTablePixel( (x<<1), (y<<1)+1, col);
   setTablePixel( (x<<1)+1, (y<<1)+1, col);
}

void setTablePixel(int x, int y, unsigned long color){
 // #ifdef ORIENTATION_HORIZONTAL
//  setPixel(y%2 ? y*FIELD_WIDTH + x : y*FIELD_WIDTH + ((FIELD_HEIGHT-1)-x),color);
   
   if( y & 0x01) {
      // Odd rows run backwards
        leds[ (y * LONG_SIDE) + (LONG_SIDE - 1) - x ] = (color) ;
    } else {
      // Even rows run forwards
      leds [ (y * LONG_SIDE) + x] = (color) ;    } 
  
    #ifdef USE_CONSOLE_OUTPUT
      setTablePixelConsole(y,x,color);
    #endif
    /*
  #else
 // setPixel(x%2 ? x*FIELD_WIDTH + ((FIELD_HEIGHT-1)-y) : x*FIELD_WIDTH + y,color);
  if (x & 0x01)
   leds[ x*FIELD_HEIGHT + y ] = CRGB(color) ; //15
 else
   leds [ (FIELD_HEIGHT-1-y)+ x*FIELD_HEIGHT ] = CRGB(color) ;
   
    #ifdef USE_CONSOLE_OUTPUT
      setTablePixelConsole(x,y,color);
    #endif
  #endif*/
  
}

void setTablePixelv(int x, int y, int color){
// setPixel(x%2 ? x*FIELD_HEIGHT + ((FIELD_HEIGHT-1)-y) : x*FIELD_HEIGHT + y,color);
/*
 if (x & 0x01)
   leds[ x*SHORT_SIDE + y ] = CRGB(color) ; //15
 else
   leds [ (SHORT_SIDE-1-y)+ x*SHORT_SIDE ] = CRGB(color) ;
*/
  if (x & 0x01)
   leds[ x*LONG_SIDE + y ] = CRGB(color) ; //15
 else
   leds [ (LONG_SIDE-1-y)+ x*LONG_SIDE ] = CRGB(color) ;
   
}

void initPixelsv(){

//  FastLED.addLeds<FAST_LED_CHIPSET, FAST_LED_DATA_PIN, COLOR_ORDER>(leds, NUM_PIXELS).setCorrection(TypicalSMD5050);
  FastLED.addLeds<FAST_LED_CHIPSET, FAST_LED_DATA_PIN>(leds, NUM_PIXELS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(BRIGHTNESS);
  leds[14] = CRGB(255,0,0);  //0,1
  leds[15] = CRGB(0,255,0);  //0,2
  leds[44] = CRGB(0,255,0);  //0,3
  leds[45] = CRGB(0,0,255);
  leds[74] = CRGB(0,0,255);
  leds[75] = CRGB(0,0,255);
   FastLED.show();
   delay(1000);

  for (int y = 0; y < LONG_SIDE; y++)
  {
    for (int x = 0; x < SHORT_SIDE  ; x++)
    {
      setTablePixelv (x, y, YELLOW );
      FastLED.show();
      delay(5);
      setTablePixelv(x, y, 0);
    }
  }   
  delay(1000);
}


void setTablePixelRGB(int x, int y, int r,int g, int b){
   if( y & 0x01) {
      // Odd rows run backwards
        leds[ (y * FIELD_WIDTH) + (FIELD_WIDTH - 1) - x ] = CRGB(r,g,b) ;
    } else {
      // Even rows run forwards
      leds [ (y * FIELD_WIDTH) + x] = CRGB(r,g,b) ;
    }

//  #ifdef ORIENTATION_HORIZONTAL
//  setPixelRGB(y%2 ? y*FIELD_WIDTH + x : y*FIELD_WIDTH + ((FIELD_HEIGHT-1)-x),r,g,b);
/*
    #ifdef USE_CONSOLE_OUTPUT
      setTablePixelConsole(y,x,color);
    #endif
  #else
  setPixelRGB(x%2 ? x*FIELD_WIDTH + ((FIELD_HEIGHT-1)-y) : x*FIELD_WIDTH + y,r,g,b);
    #ifdef USE_CONSOLE_OUTPUT
      setTablePixelConsole(x,y,color);
    #endif
  #endif
  */
}

void setTablePixelRGBv(int x, int y, int r,int g, int b){
  if (x & 0x01)
   leds[ x*LONG_SIDE + y ] = CRGB(r,g,b) ; 
 else
   leds [ (LONG_SIDE-1-y)+ x*LONG_SIDE ] = CRGB(r,g,b) ;
}
   

void clearTablePixels(){
  for (int n=0; n<FIELD_WIDTH*FIELD_HEIGHT; n++){
    setPixel(n,0);
  }
}


void scrollTextBlocked(char* text, int textLength, int color){
  unsigned long prevUpdateTime = 0;
#ifdef DEBUG
  Serial.println(-textLength);
#endif
  
  for (int x=SHORT_SIDE; x>-(textLength*8); x--){
    printText(text, textLength, x, 2, color);
    //Read buttons
    unsigned long curTime;
    do{
      readInput();
      curTime = millis();
    } while (((curTime - prevUpdateTime) < TEXTSPEED) && (curControl == BTN_NONE));//Once enough time  has passed, proceed
    
    prevUpdateTime = curTime;
  }
}

void scrollTextBlockedv(char* text, int textLength, int color){
  unsigned long prevUpdateTime = 0;
  Serial.println(-textLength);
  
  for (int x=LONG_SIDE; x>-(textLength*8); x--){
    printText(text, textLength, x, 2, color);
    //Read buttons
    unsigned long curTime;
    do{
      readInput();
      curTime = millis();
    } while (((curTime - prevUpdateTime) < TEXTSPEED) && (curControl == BTN_NONE));//Once enough time  has passed, proceed
    
    prevUpdateTime = curTime;
  }
}


/* *** text helper methods *** */
#include "font.h"
uint8_t charBuffer[8][8];

void printText(char* text, unsigned int textLength, int xoffset, int yoffset, int color){
  uint8_t curLetterWidth = 0;
  int curX = xoffset;
  clearTablePixels();
  
  //Loop over all the letters in the string
  for (int i=0; i<textLength; i++){
    //Determine width of current letter and load its pixels in a buffer
    curLetterWidth = loadCharInBuffer(text[i]);
    //Loop until width of letter is reached
    for (int lx=0; lx<curLetterWidth; lx++){
      //Now copy column per column to field (as long as within the field
      if (curX>=LONG_SIDE){//If we are to far to the right, stop loop entirely
        break;
      } else if (curX>=0){//Draw pixels as soon as we are "inside" the drawing area
        for (int ly=0; ly<8; ly++){//Finally copy column
          setTablePixel(curX,yoffset+ly,charBuffer[lx][ly]*color);
        }
      }
      curX++;
    }
  }
  
  showPixels();
}

void printTextv(char* text, unsigned int textLength, int xoffset, int yoffset, int color){
  uint8_t curLetterWidth = 0;
  int curX = xoffset;
  clearTablePixels();
  
  //Loop over all the letters in the string
  for (int i=0; i<textLength; i++){
    //Determine width of current letter and load its pixels in a buffer
    curLetterWidth = loadCharInBuffer(text[i]);
    //Loop until width of letter is reached
    for (int lx=0; lx<curLetterWidth; lx++){
      //Now copy column per column to field (as long as within the field
      if (curX>=SHORT_SIDE){//If we are to far to the right, stop loop entirely
        break;
      } else if (curX>=0){//Draw pixels as soon as we are "inside" the drawing area
        for (int ly=0; ly<8; ly++){//Finally copy column
          setTablePixelv(curX,yoffset+ly,charBuffer[lx][ly]*color);
        }
      }
      curX++;
    }
  }
  
  showPixels();
}
//Load char in buffer and return width in pixels
uint8_t loadCharInBuffer(char letter){
  uint8_t* tmpCharPix;
  uint8_t tmpCharWidth;
  
  int letterIdx = (letter-32)*8;
  
  int x=0; int y=0;
  for (int idx=letterIdx; idx<letterIdx+8; idx++){
    for (int x=0; x<8; x++){
      charBuffer[x][y] = ((font[idx]) & (1<<(7-x)))>0;
    }
    y++;
  }
  
  tmpCharWidth = 8;
  return tmpCharWidth;
}


/* *********************************** */

void fadeOut(){
  //Select random fadeout animation
  int selection = random(3);
  
  switch(selection){
    case 0:
    case 1:
    {
      //Fade out by dimming all pixels
      for (int i=0; i<100; i++){
        dimLeds(0.97);
        showPixels();
        delay(10);
      }
      break;
    }
    case 2:
    {
      //Fade out by swiping from left to right with ruler
      const int ColumnDelay = 10;
      int curColumn = 0;
      for (int i=0; i<FIELD_WIDTH*ColumnDelay; i++){
        dimLeds(0.97);
        if (i%ColumnDelay==0){
          //Draw vertical line
          for (int y=0;y<FIELD_HEIGHT;y++){
            setTablePixel(curColumn, y, GREEN);
          }
          curColumn++;
        }
        showPixels();
        delay(5);
      }
      //Sweep complete, keep dimming leds for short time
      for (int i=0; i<100; i++){
        dimLeds(0.9);
        showPixels();
        delay(5);
      }
      break;
    }
  }
}

void dimLeds(float factor){
  //Reduce brightness of all LEDs, typical factor is 0.97
  for (int n=0; n<(FIELD_WIDTH*FIELD_HEIGHT); n++)
  {
    leds[n].r*=factor;
    leds[n].g*=factor;
    leds[n].b*=factor;
/*
    unsigned long curColor = getPixel(n);
    //Derive the tree colors
    int r = ((curColor & 0xFF0000)>>16);
    int g = ((curColor & 0x00FF00)>>8);
    int b = (curColor & 0x0000FF);
    //Reduce brightness
    r = r*factor;
    g = g*factor;
    b = b*factor;
    //Pack into single variable again
    curColor = (r<<16) + (g<<8) + b;
    //Set led again
    setPixel(n,curColor);
*/
  }
}


#include "font2.h"
/*
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
*/

void printText4(char* text, uint8_t xoffset, uint8_t yoffset, unsigned long color[2] ){
//  uint8_t curLetterWidth = 0;
  uint8_t curX = xoffset, col;
  
  //Loop over all the letters in the string
  for (uint8_t i=0; i<strlen(text); i++){
//    loadLetter(text[i]);
    
     
    //Loop until width of letter is reached
    for (uint8_t lx=0; lx<4; lx++){
      curX= 4*i + xoffset +lx;
      if (lx==3) col = 0;
      else col=Wendy3x5[ (text[i]-32)*3 +lx ];
      
      //Now copy column per column to field (as long as within the field
      if ( curX < LONG_SIDE && curX >= 0)   //If we are to far to the right, stop loop entirely
 //     if ( curX < 0) break;//If we are to far to the right, stop loop entirely      
    
      for (uint8_t ly=0; ly<5; ly++){//Finally copy column
//        if (letter[lx][ly])
        if ( bitRead(col, ly) )
          setTablePixel(curX, yoffset+ly, color[i%2]);
        else
          setTablePixel(curX, yoffset+ly, BLACK);
      }
    }
  }
}

void printText3(char* text, uint8_t xoffset, uint8_t yoffset, unsigned long color[2] ){
//  uint8_t curLetterWidth = 0;
  uint8_t curX = xoffset, col;
  
  //Loop over all the letters in the string
  for (uint8_t i=0; i<strlen(text); i++){
//    loadLetter(text[i]);
    
     
    //Loop until width of letter is reached
    for (uint8_t lx=0; lx<3; lx++){
      curX= 3*i + xoffset +lx;
      col=Wendy3x5[ (text[i]-32)*3 +lx ];
      //Now copy column per column to field (as long as within the field
      if ( curX < LONG_SIDE && curX >= 0)   //If we are to far to the right, stop loop entirely
 //     if ( curX < 0) break;//If we are to far to the right, stop loop entirely      
    
      for (uint8_t ly=0; ly<5; ly++){//Finally copy column
//        if (letter[lx][ly])
        if ( bitRead(col, ly) )
          setTablePixel(curX, yoffset+ly, color[i%2]);
        else
          setTablePixel(curX, yoffset+ly, BLACK);
      }
    }
  }
}



void testMatrix() {
    setTablePixel(0, 0, WHITE);
    showPixels();
    delay(2000);
    setTablePixel(0, 9, WHITE);
    showPixels();
    delay(2000);
    setTablePixel(9, 0, WHITE);
    showPixels();
    delay(2000);
    setTablePixel(9, 9, WHITE);
}
void setup(){
//  PrintCol[0]= YELLOW;
//  PrintCol[1]= RED;

  Serial.begin(115200);
//  Serial.begin(9600);
/*
  X_init = analogRead(X_pin);
  Y_init = analogRead(Y_pin);  
  pinMode(SW_pin,INPUT_PULLUP);
*/
  pinMode(L_pin,INPUT_PULLUP);
  pinMode(R_pin,INPUT_PULLUP);
  pinMode(U_pin,INPUT_PULLUP);
  pinMode(D_pin,INPUT_PULLUP);
  pinMode(S_pin,INPUT_PULLUP);
  pinMode(E_pin,INPUT_PULLUP);
  pinMode(L2_pin,INPUT_PULLUP);
  pinMode(R2_pin,INPUT_PULLUP);
  pinMode(U2_pin,INPUT_PULLUP);
  pinMode(D2_pin,INPUT_PULLUP);

  
  //Wait for serial port to connect
  bluetooth.begin(BLUETOOTH_SPEED);
  //Initialise display
  #ifdef ORIENTATION_HORIZONTAL
    initPixels();
  #else
    initPixelsv();
  #endif
  showPixels();


  //Init random number generator
  randomSeed(millis());

  mainLoop();
}

void loop(){
}
