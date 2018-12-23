/* LedTable
 *
 * Written by: Klaas De Craemer, Ing. David Hrbaty
 * 
 * Basic animation with fading stars
 */

#define DELAY 60

const uint64_t IMAGES[] = {
  0xf09090f00f09090f,
  0x00f0909ff9090f00,
  0x0000ff9999ff0000,
  0x000f09f99f90f000,
  0x0f09090ff09090f0,
  0x1e12121e78484878,
  0x3c24243c3c24243c,
  0x784848781e12121e
};
const int IMAGES_LEN = sizeof(IMAGES)/8;

const uint64_t IMAGES2[] = {
  0x0000000000000000,
  0x0000001818000000,
  0x00003c3c3c3c0000,
  0x007e7e7e7e7e7e00,
  0xffffffffffffffff,
  0xffffffe7e7ffffff,
  0xffffc3c3c3c3ffff,
  0xff818181818181ff
};

const uint64_t IMAGES3[] = {
  0x2020f824241f0404,
  0x10107c18183e0808,
  0x08083e18187c1010,
  0x04041f2424f82020,
  0x0004243ffc242000,
  0x002024fc3f240400
};

const uint64_t IMAGES5[] = {
  0xe0a0e00000070507,
  0x70507000000e0a0e,
  0x38283800001c141c,
  0x1c141c0000382838,
  0x0e0a0e0000705070,
  0x0705070000e0a0e0
};

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

const byte bytex[]={3,4,5, 6,7,8, 8,8,8, 8,7,6, 5,4,3, 2,1,0, 0,0,0, 0,1,2 }; //24
//const byte bytey[]={0,0,0, 0,1,2, 3,4,5, 6,7,8, 8,8,8, 8,7,6, 5,4,3, 2,1,0  };

void countDown(byte nb)
{
  for (int r=nb; r >=0 ; r--)
  {
     for (int i=0; i <5 ; i++)
     {
        byte row= (CHIFFRE[r] >>i *8) & 0xFF;
        for (int j = 0; j<3; j++)
        {
          if (bitRead(row,j))
            setTablePixel (j+3, i+2, RED);
          else
            setTablePixel (j+3, i+2, BLACK);
        }
         
      }
      for (int i=0; i <24 ; i++)
      {
      
///      ledtable.fill(bytec[r],bytec[ (r+17)%24] , color_red);
        for (int j = 0; j<8; j++)
        {
          setTablePixel(bytex[(i-j+24)%24],bytex[(i-j+18)%24] , WHITE);
          setTablePixel(bytex[(i-j+16)%24],bytex[(i-j+10)%24] , BLACK); 
        }      
        showPixels();
        delay(1000/24);
      }

     clearTablePixels();
  }
}

void displayDoubleImage(const uint64_t image)
{
  for (int i=0; i <8 ; i++)
  {
    byte row= (image >>i *8) & 0xFF;
    for (int j = 0; j<8; j++)
    {
      if (bitRead(row,j)){
        setTablePixel(j, i, PURPLE);
        setTablePixel(j+7, i+2, RED);}
      else{
        setTablePixel(j, i, BLACK);
        setTablePixel(j+7, i+2, BLACK);}
    }
    showPixels();
  }
}

boolean AppRunning;

void DelayAndTestExit(int time){
  static unsigned long prevUpdateTime = 0;
  static unsigned long curTime = 0;

    //Check input keys
    do{
      readInput();
      if (curControl == BTN_EXIT){
        AppRunning = false;
        break;
      }
      curTime = millis();
      delay(10);
    } 
    while ((curTime - prevUpdateTime) <80);//Once enough time  has passed, proceed. The lower this number, the faster the game is
    prevUpdateTime = curTime;  
}




void initApp(){
  AppRunning = true;
  countDown(3);
}

void runTest(){
  clearTablePixels();
  showPixels();
  initApp();  


  
  while(AppRunning)
  {

  if (AppRunning)  for (int d=0; d<5; d++)
  {
  for (int c=1; c<6; c++)
  {
    displayDoubleImage(IMAGES5[c]);
    DelayAndTestExit(DELAY);
  }
 
  if (AppRunning) for (int c=6-2; c>=0; c--)
  {
    displayDoubleImage(IMAGES5[c]);
    DelayAndTestExit(DELAY);
  }
  }

  if (AppRunning) for (int d=0; d<5; d++)
  {
  for (int c=0; c<IMAGES_LEN; c++)
  {
    displayDoubleImage(IMAGES2[c]);
    DelayAndTestExit(DELAY);
  }
 
  if (AppRunning) for (int c=IMAGES_LEN-1; c>=0; c--)
  {
    displayDoubleImage(IMAGES2[c]);
    DelayAndTestExit(DELAY);
  }
  }


  if (AppRunning) for (int d=0; d<10; d++)
  for (int c=0; c<6; c++)
  {
    displayDoubleImage(IMAGES3[c]);
    DelayAndTestExit(DELAY);
  }

  if (AppRunning) for (int d=0; d<10; d++)
  for (int c=0; c<IMAGES_LEN; c++)
  {
    displayDoubleImage(IMAGES[c]);
    DelayAndTestExit(DELAY);
  }

/*    
    dimLeds(0.97);
    
    //Create new stars if enough time has passed since last time
    curTime = millis();
    if ((curTime - prevStarsCreationTime) > 1600){
      for (int i=0; i<NB_NEWSTARS; i++){
        boolean positionOk = false;
        int n = 0;
        //Generate random positions until valid
        while (!positionOk){
          n = random(FIELD_WIDTH*FIELD_HEIGHT);
          if (getPixel(n) == 0)
            positionOk = true;
        }
//        //Get random color
          if (random(2)==0)
          setPixel(n,YELLOW);
          else
          setPixel(n,BLUE);
      }
      prevStarsCreationTime = curTime;
    }
*/ 
/* 
    showPixels();
    
    //Check input keys
    do{
      readInput();
      if (curControl == BTN_EXIT){
        AppRunning = false;
        break;
      }
      curTime = millis();
      delay(10);
    } 
    while ((curTime - prevUpdateTime) <80);//Once enough time  has passed, proceed. The lower this number, the faster the game is
    prevUpdateTime = curTime;
  */
  }
}
