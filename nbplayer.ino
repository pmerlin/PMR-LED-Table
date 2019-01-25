/* LedTable
 *
 */
#define DECAL 4
boolean mappRunning;

void scrollText3(char *text, uint8_t lx, uint8_t ly, unsigned long color[2])
{
  int size = strlen(text) * 3;

  for (int x = 0; x > -(size); x--)
  {
    printText3(text, x + lx, ly, color);
    showPixels();
    delay(500);
  }
}

void scrollText4(char *text, uint8_t lx, uint8_t ly, unsigned long color[2])
{
  int size = strlen(text) * 4;

  for (int x = 0; x > -(size); x--)
  {
    printText4(text, x + lx, ly, color);
    showPixels();
    delay(500);
  }
}

void initNbPlayer()
{
  mappRunning = true;
  clearTablePixels();
  showPixels();
}

void runNbPlayer()
{
  char *text = "Select NB PLAYER ";
  int size = (strlen(text) * 3) + DECAL;
  unsigned long PrintCol[2];
  unsigned long startTime, click = 0, t;
  PrintCol[0] = YELLOW;
  PrintCol[1] = RED;

  initNbPlayer();

  while (mappRunning)
  {

    //    scrollText3 ("ABCDEFGHIJKLMNOPQRSTUVWXYZ ", 7, 0, PrintCol);
    //    scrollText3 ("abcdefghijklmnopqrstuvwxyz ", 0, 0, PrintCol);
    //    scrollText3 ("0123456789 ", 7, 0, PrintCol);
    //    printText3 ("NB PLAYER", -1, 0, PrintCol);

    for (int x = 0; x > -(size); x--)
    {
      printText3(text, x + DECAL, 0, PrintCol);
      //      showPixels();

      //// some stuff
      printNumber(nbPlayer, 4, 5, RED);
      showPixels();

      //Check input keys
      startTime = millis();
      do
      {
        readInput();
        //        t=millis()-click;
        //      Serial.println(t);

        if (curControl == BTN_EXIT || curControl == BTN_START)
        {
          mappRunning = false;
          break;
        }
        else if (curControl != BTN_NONE && millis() - click > 400)
        {
          click = millis();
          if (curControl & BTN_RIGHT)
            nbPlayer++;
          else if (curControl & BTN_LEFT)
            nbPlayer--;

          if (nbPlayer < MINPLAYER)
            nbPlayer = MINPLAYER;
          else if (nbPlayer > MAXPLAYER)
            nbPlayer = MAXPLAYER;
        }
      } while (mappRunning && (millis() - startTime) < 200); //Once enough time  has passed, proceed. The lower this number, the faster the game is
    }
  }
  displayLogo();
}
