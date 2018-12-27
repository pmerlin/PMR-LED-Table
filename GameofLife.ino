/*
  Part of this code comes from Copyright (c) 2014 Jason Coon. Updated and modified by Mark Quinn 2017 to run without a smart matrix and be part of the LED table overall code.
  This code runs a simulation of Conway's game of life on the LED matrix
  The code will run for 256 generations unless there are no births or deaths detected for that generation and it will begin again.

*/

#define  gameoflifedelay 300
boolean GameofLiferunning = false;
long density = 50;
int generation = 0;

int births = 0;
int deaths = 0;
// blur between each frame of the simulation
boolean blur = false;

// adjust the amount of blur
float blurAmount = 0.01;

boolean randomBlur = false;

// alternate between blurred and not blurred each time a new world is filled
boolean alternateBlur = false;

// switch to a random palette each time a new world is filled
boolean switchPalette = true;

CRGBPalette16 currentpalette_gof = PartyColors_p;


class Cell {
  public:
    byte alive =  1;
    byte prev =  1;
    byte hue = 6;
    byte brightness;
};
Cell world[FIELD_WIDTH][FIELD_HEIGHT];

//Game of life Functions

void randomFillWorld() {
  for (int i = 0; i < FIELD_WIDTH; i++) {
    for (int j = 0; j < FIELD_HEIGHT; j++) {
      if (random(100) < (unsigned long)density) {
        world[i][j].alive = 1;
        world[i][j].brightness = 255;
      }
      else {
        world[i][j].alive = 0;
        world[i][j].brightness = 0;
      }
      world[i][j].prev = world[i][j].alive;
      world[i][j].hue = 0;
    }
  }
}

void chooseNewPalette() {
  switch (random(0, 8)) {
    case 0:
      currentpalette_gof = CloudColors_p;
      break;

    case 1:
      currentpalette_gof = ForestColors_p;
      break;

    case 2:
      currentpalette_gof = HeatColors_p;
      break;

    case 3:
      currentpalette_gof = LavaColors_p;
      break;

    case 4:
      currentpalette_gof = OceanColors_p;
      break;

    case 5:
      currentpalette_gof = PartyColors_p;
      break;

    case 6:
      currentpalette_gof = RainbowColors_p;
      break;

    case 7:
    default:
      currentpalette_gof = RainbowStripeColors_p;
      break;
  }
}

uint16_t XY( uint8_t x, uint8_t y) {
  return (y * FIELD_WIDTH) + x;
}
/*
int neighbours(int x, int y) {
  return (world[(x + 1)][y].prev) +
         (world[(x - 1)][y].prev) +
         (world[x][(y - 1)].prev) + (world[x][(y + 1)].prev) +
         (world[(x + 1)][(y + 1)].prev) +
         (world[(x - 1)][(y + 1)].prev) +
         (world[(x - 1)][(y - 1) ].prev) +
         (world[(x + 1)][(y - 1)].prev);
}
*/
int neighbours(int x, int y) {
  return (world[(x + 1) % FIELD_WIDTH][y].prev) +
         (world[x][(y + 1) % FIELD_HEIGHT].prev) +
         (world[(x + FIELD_WIDTH - 1) % FIELD_WIDTH][y].prev) +
         (world[x][(y + FIELD_HEIGHT - 1) % FIELD_HEIGHT].prev) +
         (world[(x + 1) % FIELD_WIDTH][(y + 1) % FIELD_HEIGHT].prev) +
         (world[(x + FIELD_WIDTH - 1) % FIELD_WIDTH][(y + 1) % FIELD_HEIGHT].prev) +
         (world[(x + FIELD_WIDTH - 1) % FIELD_WIDTH][(y + FIELD_HEIGHT - 1) % FIELD_HEIGHT].prev) +
         (world[(x + 1) % FIELD_WIDTH][(y + FIELD_HEIGHT - 1) % FIELD_HEIGHT].prev);
}

void initGameofLife() {
  GameofLiferunning = true;

  clearTablePixels();
//  showPixels();
}

void runGameofLife() {
  initGameofLife();
  runGameofLifecode();
  displayLogo();
}

void runGameofLifecode() {
  while (GameofLiferunning) {
    if (generation == 0) {
      //fadeOut();

      randomFillWorld();

      if (alternateBlur)
        blur = !blur;

      if (switchPalette)
        chooseNewPalette();

      if (randomBlur)
        blurAmount = ((float)random(50, 90)) / 100.0;
    }

  // Display current generation
  for (int i = 0; i < FIELD_WIDTH; i++) {
    for (int j = 0; j < FIELD_HEIGHT; j++) {
      if(blur) {
        leds[XY(i, j)] = ColorFromPalette(currentpalette_gof, world[i][j].hue * 4, world[i][j].brightness);
      }
      else if (world[i][j].alive == 1) {
        leds[XY(i, j)] = ColorFromPalette(currentpalette_gof, world[i][j].hue * 4, world[i][j].brightness);
      }
      else {
        leds[XY(i, j)] = (CRGB)CRGB::Black;
      }
    }
  }

// Reorder pixels based on Even rows
for (int currentrow = 0; currentrow < FIELD_HEIGHT  ;  currentrow++)
      {
        //even
        int rowstart = currentrow * FIELD_WIDTH; //calculate the row start position in the snake
        if ( (currentrow % 2) == 1) { //only work on even rows
          for (int i = 0, j = FIELD_WIDTH - 1; i < FIELD_WIDTH / 2; i++, j--)
          {
            CRGB temp = leds[i + rowstart]; //take temporary value from start of row
            leds[i + rowstart] = leds[j + rowstart]; // put value from end of row at start
            leds[j + rowstart] = temp; // put temporary value at the end of the row
          }
        }
      }


      
FastLED.show();
delay(gameoflifedelay);

  // Birth and death cycle
  births=0;
  deaths=0;
  for (int x = 0; x < FIELD_WIDTH; x++) {
    for (int y = 0; y < FIELD_HEIGHT; y++) {
      // Default is for cell to stay the same
      if (world[x][y].brightness > 0 && world[x][y].prev == 0)
        world[x][y].brightness *= blurAmount;
      int count = neighbours(x, y);
      if (count == 3 && world[x][y].prev == 0) {
        // A new cell is born
        world[x][y].alive = 1;
        world[x][y].hue += 2;
        world[x][y].brightness = 255;
        births++;
      } 
      else if ((count < 2 || count > 3) && world[x][y].prev == 1) {
        // Cell dies
        deaths++;
        world[x][y].alive = 0;
      }
    }
  }

    // Copy next generation into place
    for (int x = 0; x < FIELD_WIDTH; x++) {
      for (int y = 0; y < FIELD_HEIGHT; y++) {
        world[x][y].prev = world[x][y].alive;
      }
    }

    generation++;
    if (generation >= 256) {
      delay(1000);
      generation = 0;
      //Fade out by swiping from left to right with ruler
      const int ColumnDelay = 10;
      int curColumn = 0;
      for (int i = 0; i < FIELD_WIDTH * ColumnDelay; i++) {
        dimLeds(0.97);
        if (i % ColumnDelay == 0) {
          //Draw vertical line
          for (int y = 0; y < FIELD_HEIGHT; y++) {
            setTablePixel(curColumn, y, GREEN);
          }
          curColumn++;
        }
        showPixels();
        delay(5);
      }
      //Sweep complete, keep dimming leds for short time
      for (int i = 0; i < 100; i++) {
        dimLeds(0.9);
        showPixels();
        delay(5);
      }
    }
    else if (births == 0 & deaths == 0) {
      generation = 0;
      delay(1000);
      //Fade out by swiping from left to right with ruler
      const int ColumnDelay = 10;
      int curColumn = 0;
      for (int i = 0; i < FIELD_WIDTH * ColumnDelay; i++) {
        dimLeds(0.97);
        if (i % ColumnDelay == 0) {
          //Draw vertical line
          for (int y = 0; y < FIELD_HEIGHT; y++) {
            setTablePixel(curColumn, y, GREEN);
          }
          curColumn++;
        }
        showPixels();
        delay(5);
      }
      //Sweep complete, keep dimming leds for short time
      for (int i = 0; i < 100; i++) {
        dimLeds(0.9);
        showPixels();
        delay(5);
      }
    }




    readInput();
    if (curControl == BTN_EXIT) {
      Serial.println("EXIT");
      GameofLiferunning = false;
      break;
    }
  }
}
