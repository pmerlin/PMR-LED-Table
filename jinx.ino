/* LedTable
 *
 */

void initJinx(){
  appRunning = true;
}

int serialJinx () {
//  while (!Serial.available()) {}
  return Serial.read();
}

void runJinx(){
  initJinx();
  clearTablePixels();
  showPixels();
  
  unsigned long prevStarsCreationTime = 0;
  unsigned long prevUpdateTime = 0;
  unsigned long curTime, click;
  
  while(appRunning){
 

//// some stuff
    while (serialJinx () != 1) {}
    for (int i=0; i < NUM_PIXELS; i++) {
      leds[i].r = serialJinx ();
      leds[i].g = serialJinx ();
      leds[i].b = serialJinx ();
    }
    showPixels();
    
    //Check input keys
    curTime=millis();
    do{
      readInput();
      if (curControl == BTN_EXIT){
        appRunning = false;
        break;
      }
    }
    while ((millis()- curTime) <80); ;//Once enough time  has passed, proceed. The lower this number, the faster the game is
  }
  displayLogo();
}
