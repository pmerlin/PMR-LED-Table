/* LedTable
 *
 * Written by: Klaas De Craemer, Ing. David Hrbaty
 * 
 * Menu system for the LED table
 */

#define MINSELECTION 1
#define MAXSELECTION 12

//PMR update
unsigned int curSelection = MINSELECTION;
//unsigned int curSelection = 2;

#define TEXTSPEED 140

void mainLoop(void)
{
  char *curSelectionText;
  int curSelectionTextLength;
  unsigned long prevUpdateTime = 0;
  int8_t oldNbPlayer;

  char *SelectionText[] = {"0 Menu,", "1 Rainbow", "2 Animation", "3 Stars", "4 Vu Meter", "5 DaftPunk", "6 Tetris", "7 Snake", "8 Pong", "9 Bricks", "10 Test", "11 GameOfLife", "12 Nb Player"};

  //runNbPlayer();
  //runTest();
  runSnake();
  //runDP();
  //runColorPalette();

  while (true)
  {
    //Show menu system and wait for input
    clearTablePixels();
    curSelectionText = SelectionText[curSelection];
    /*
    switch (curSelection){
      case 1:
        curSelectionText = "1 Rainbow";
        break;
      case 2:
        curSelectionText = "2 Animation";
        break;    
      case 3:
        curSelectionText = "3 Stars";  
      break;  
      case 4:
        curSelectionText = "4 Vu Meter";
        break;    
      case 5:
        curSelectionText = "5 DaftPunk"; 
        break;                
      case 6:
        curSelectionText = "6 Tetris";
        break;
      case 7:
        curSelectionText = "7 Snake";
        break;
      case 8:
        curSelectionText = "8 Pong";
        break;       
      case 9:
        curSelectionText = "9 Bricks";
        break;       
      case 10:
        curSelectionText = "10 Test";
        break;
      case 11:
        curSelectionText = "11 GameOfLife";
        break;      
      
    }
    */
    curSelectionTextLength = strlen(curSelectionText);

    boolean selectionChanged = false;
    boolean runSelection = false;
    /*
  unsigned long col[2];
  col[0]= YELLOW;
  col[1]= RED;
  int size=strlen(curSelectionText)*3;
*/
    //Scroll current selection text from right to left;
    for (int x = LONG_SIDE; x > -(curSelectionTextLength * 8); x--)
    {
      //    for (int x=0; x>-(size); x--){

      printText(curSelectionText, curSelectionTextLength, x, (SHORT_SIDE - 8) / 2, YELLOW);
      //      printText3(curSelectionText, x, 3, col );

      //Read buttons
      unsigned long curTime;
      do
      {
        readInput();
        if (curControl != BTN_NONE)
        {
          if (curControl == BTN_LEFT)
          {
            curSelection--;
            selectionChanged = true;
            delay(400);
          }
          else if (curControl & BTN_EXIT)
          {
            Serial.print(curControl);
            oldNbPlayer = nbPlayer;
            if (curControl & BTN_LEFT)
            {
              nbPlayer = 1;
              Serial.print("NB");
            }
            else if (curControl & BTN_UP)
              nbPlayer = 2;
            else if (curControl & BTN_DOWN)
              nbPlayer = 3;
            else if (curControl & BTN_RIGHT)
              nbPlayer = 4;
            if (oldNbPlayer != nbPlayer)
            {
              Serial.print("PRINT");
              clearTablePixels();
              //             printText3 ("NbPla", 0, 0, PrintCol);
              printNumber(nbPlayer, 4, 5, RED);
              showPixels();
              delay(1000);
            }
          }
          else if (curControl == BTN_RIGHT)
          {
            curSelection++;
            selectionChanged = true;
            delay(400);
          }
          else if (curControl == BTN_START)
          {
            runSelection = true;
          }
          checkSelectionRange();
        }
        curTime = millis();
      } while (((curTime - prevUpdateTime) < TEXTSPEED) && (curControl == BTN_NONE)); //Once enough time  has passed, proceed
      prevUpdateTime = curTime;

      if (selectionChanged || runSelection)
        break;
    }

    //If we are here, it means a selection was changed or a game started, or user did nothing
    if (selectionChanged)
    {
      //For now, do nothing
    }
    else if (runSelection)
    { //Start selected game
      switch (curSelection)
      {
      case 1:
        runRainbowPalette();
        break;
      case 2:
        runColorPalette();
        break;
      case 3:
        runStars();
        break;
      case 4:
        runVUmeter();
        break;
      case 5:
        runDP();
        break;
      case 6:
        runTetris();
        break;
      case 7:
        runSnake();
        break;
      case 8:
        runPong();
        break;
      case 9:
        runBricks();
        break;
      case 10:
        runTest();
        break;
      case 11:
        runGameofLife();
        break;
      case 12:
        runNbPlayer();
        break;
      }
    }
    else
    {
      //If we are here, no action was taken by the user, so we will move to the next selection automatically
      curSelection++;
      checkSelectionRange();
    }
  }
}

void checkSelectionRange()
{
  if (curSelection > MAXSELECTION)
  {
    curSelection = MINSELECTION;
  }
  else if (curSelection < MINSELECTION)
  {
    curSelection = MAXSELECTION;
  }
}
