/* LedTable
 *
 */

void initApp(){
  appRunning = true;
}

void runApp(){
  initApp();
  clearTablePixels();
  showPixels();
  
  unsigned long prevStarsCreationTime = 0;
  unsigned long prevUpdateTime = 0;
  unsigned long curTime, click;
  
  while(appRunning){
 

//// some stuff
    
    showPixels();
    
    //Check input keys
    curTime=millis();
    do{
      readInput();
      if (curControl == BTN_EXIT){
        appRunning = false;
        break;
      }
      else if (curControl != BTN_NONE && millis()-click > 600)
      {
        click=millis();
//        clearTablePixels();
      }

    }
    while ((millis()- curTime) <80); ;//Once enough time  has passed, proceed. The lower this number, the faster the game is
  }
  displayLogo();
}
