/* LedTable
 *
 * Written by: Klaas De Craemer, Ing. David Hrbaty
 * 
 * Basic animation with fading stars
 */

//#define UPDATES_PER_SECOND 100
//#define UPDATES_PER_SECOND 50
#define UPDATES_PER_SECOND 12
CRGBPalette16 currentPalette;
TBlendType currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

boolean colorPaletteRunning;

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for (int i = 0; i < 16; i++)
    {
        currentPalette[i] = CHSV(random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid(currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV(HUE_PURPLE, 255, 255);
    CRGB green = CHSV(HUE_GREEN, 255, 255);
    CRGB black = CRGB::Black;

    currentPalette = CRGBPalette16(
        green, green, black, black,
        purple, purple, black, black,
        green, green, black, black,
        purple, purple, black, black);
}

// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
    {
        CRGB::Red,
        CRGB::Gray, // 'white' is too bright compared to red and blue
        CRGB::Blue,
        CRGB::Black,

        CRGB::Red,
        CRGB::Gray,
        CRGB::Blue,
        CRGB::Black,

        CRGB::Red,
        CRGB::Red,
        CRGB::Gray,
        CRGB::Gray,
        CRGB::Blue,
        CRGB::Blue,
        CRGB::Black,
        CRGB::Black};

void FillLEDsFromPaletteColors(uint8_t colorIndex)
{
    uint8_t brightness = 255;

    for (int i = 0; i < FIELD_WIDTH * FIELD_HEIGHT; i++)
    {
        leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void changePalette()
{
    static uint8_t pal = 1;

    switch (pal)
    {
    case 0:
        currentPalette = RainbowColors_p;
        currentBlending = LINEARBLEND;
        break;
    case 1:
        currentPalette = RainbowStripeColors_p;
        currentBlending = NOBLEND;
    case 2:
        currentPalette = RainbowStripeColors_p;
        currentBlending = LINEARBLEND;
        break;
    case 3:
        SetupPurpleAndGreenPalette();
        currentBlending = LINEARBLEND;
        break;
    case 4:
        SetupTotallyRandomPalette();
        currentBlending = LINEARBLEND;
        break;
    case 5:
        SetupBlackAndWhiteStripedPalette();
        currentBlending = NOBLEND;
        break;
    case 6:
        SetupBlackAndWhiteStripedPalette();
        currentBlending = LINEARBLEND;
        break;
    case 7:
        currentPalette = CloudColors_p;
        currentBlending = LINEARBLEND;
        ;
        break;
    case 8:
        currentPalette = PartyColors_p;
        currentBlending = LINEARBLEND;
        ;
        break;
    case 9:
        currentPalette = myRedWhiteBluePalette_p;
        currentBlending = NOBLEND;
        break;
    case 10:
        currentPalette = myRedWhiteBluePalette_p;
        currentBlending = LINEARBLEND;
        ;
        break;
    }
    if (++pal > 10)
        pal = 0;
}

void runColorPalette()
{
    static uint8_t startIndex = 0;
    unsigned long click = 0;
    unsigned long curTime;
    colorPaletteRunning = true;
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
    clearTablePixels();
    showPixels();

    while (colorPaletteRunning)
    {
        uint8_t secondHand = (millis() / 1000) % 60;
        static uint8_t lastSecond = 99;
        /*   
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
    }
*/

        FillLEDsFromPaletteColors(startIndex);
        showPixels();
        setDelay(1000 / UPDATES_PER_SECOND);
        startIndex = startIndex + 1;
        //Check input keys

        curTime = millis();
        do
        {
            //      setDelay(1000/UPDATES_PER_SECOND);
            readInput();
            if (curControl == BTN_EXIT)
            {
                colorPaletteRunning = false;
                break;
            }
            else if (curControl != BTN_NONE && millis() - click > 600)
            {
                changePalette();
                click = millis();
                //      clearTablePixels();
            }

        } while ((millis() - curTime) < 20); //Once enough time  has passed, proceed. The lower this number, the faster the game is //20

        //    while ((curTime - prevUpdateTime) <20);//Once enough time  has passed, proceed. The lower this number, the faster the game is
        //    prevUpdateTime = curTime;
    }
    displayLogo();
}

void runRainbowPalette()
{
    static uint8_t startIndex = 0;
    unsigned long click = 0;
    unsigned long curTime;
    colorPaletteRunning = true;
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
    clearTablePixels();
    showPixels();

    while (colorPaletteRunning)
    {
        currentPalette = RainbowColors_p;
        currentBlending = LINEARBLEND;

        FillLEDsFromPaletteColors(startIndex);
        showPixels();
        setDelay(1000 / UPDATES_PER_SECOND);
        startIndex = startIndex + 1;

        //Check input keys
        curTime = millis();
        do
        {
            readInput();
            if (curControl == BTN_EXIT)
            {
                colorPaletteRunning = false;
                break;
            }
            /*             
      else if (curControl != BTN_NONE && millis()-click > 600)
      {
        changePalette();
        click=millis();
  //      clearTablePixels();
      }
*/
        } while ((millis() - curTime) < 20); //Once enough time  has passed, proceed. The lower this number, the faster the game is //20

        //    while ((curTime - prevUpdateTime) <20);//Once enough time  has passed, proceed. The lower this number, the faster the game is
        //    prevUpdateTime = curTime;
    }
    displayLogo();
}
