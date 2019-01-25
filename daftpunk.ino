/* LedTable
 *
 * Written by: Patrick MERLIN
 * Main code for DaftPunk Table
 */

//boolean dpRunning;

uint8_t printmode = 0;

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
    0x000000110a000a11};
const int DAFTPUNK_LEN = sizeof(DAFTPUNK) / 8;

void initDP()
{
  appRunning = true;

  //  randomSeed(analogRead(0));
  clearTablePixels();
  showPixels();
}

void displayImageDP(uint64_t image)
{
  for (int y = 0; y < 5; y++)
  {
    byte row = (image >> y * 8) & 0xFF;
    for (int x = 0; x < 5; x++)
    {
      if (bitRead(row, x))
      {
        if (!printmode)
        {
          setTablePixelDouble(x + 1, y, RED);
        }
        else
        {
          setTablePixel(x, y, RED);
          setTablePixel(x + 5, y, YELLOW);
          setTablePixel(x + 10, y, RED);
          setTablePixel(x, y + 5, YELLOW);
          setTablePixel(x + 5, y + 5, RED);
          setTablePixel(x + 10, y + 5, YELLOW);
        }
      }
      else
      {
        if (!printmode)
        {
          setTablePixelDouble(x + 1, y, BLACK);
        }
        else
        {
          setTablePixel(x, y, BLACK);
          setTablePixel(x + 5, y, BLACK);
          setTablePixel(x + 10, y, BLACK);
          setTablePixel(x, y + 5, BLACK);
          setTablePixel(x + 5, y + 5, BLACK);
          setTablePixel(x + 10, y + 5, BLACK);
        }
      }
    }
  }
  showPixels();
  //  delay (500);
}

void runDP()
{
  initDP();
  unsigned long curTime, click = 0;
  uint8_t i = 1;

  while (appRunning)
  {
    displayImageDP(DAFTPUNK[i]);
    if (++i >= DAFTPUNK_LEN)
      i = 0;

    curTime = millis();
    do
    {
      readInput();
      if (curControl == BTN_EXIT)
      {
        appRunning = false;
        break;
      }
      else if (curControl != BTN_NONE && millis() - click > 600)
      {
        printmode = 1 - printmode;
        click = millis();
        clearTablePixels();
      }

    } while ((millis() - curTime) < 1000); //Once enough time  has passed, proceed. The lower this number, the faster the game is //20
  }
  displayLogo();
}
