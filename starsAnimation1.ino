/* LedTable
 *
 * Written by: Klaas De Craemer, Ing. David Hrbaty
 * 
 * Basic animation with fading stars
 */

#define NB_NEWSTARS 5
boolean starsRunning;

void initStars()
{
  starsRunning = true;
}

void runStars()
{
  initStars();
  clearTablePixels();
  showPixels();

  unsigned long prevStarsCreationTime = 0;
  unsigned long prevUpdateTime = 0;
  unsigned long curTime = 0;

  while (starsRunning)
  {
    dimLeds(0.97);

    //Create new stars if enough time has passed since last time
    curTime = millis();
    if ((curTime - prevStarsCreationTime) > 1600)
    {
      for (int i = 0; i < NB_NEWSTARS; i++)
      {
        boolean positionOk = false;
        int n = 0;
        //Generate random positions until valid
        while (!positionOk)
        {
          n = random(FIELD_WIDTH * FIELD_HEIGHT);
          if (getPixel(n) == 0)
            positionOk = true;
        }
        //        //Get random color
        if (random(2) == 0)
          setPixel(n, YELLOW);
        else
          setPixel(n, BLUE);
      }
      prevStarsCreationTime = curTime;
    }

    showPixels();

    //Check input keys
    do
    {
      readInput();
      if (curControl == BTN_EXIT)
      {
        starsRunning = false;
        break;
      }
      curTime = millis();
      delay(10);
    } while ((curTime - prevUpdateTime) < 80); //Once enough time  has passed, proceed. The lower this number, the faster the game is
    prevUpdateTime = curTime;
  }
  displayLogo();
}
