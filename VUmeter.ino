/* LedTable
 *
 * Written by: Ing. David Hrbaty
 * 
 * 
 * VU Meter animation based on FFT
 */
#include <fix_fft.h>
#include <HSBColor.h>


#define BAR_HUE_DIFF 8
boolean vuRunning;
int curHue = 0;

// FFT data storage
char im[LONG_SIDE * 2], data[SHORT_SIDE * 2];
int prev[SHORT_SIDE];

// HSB/RGB data buffer
int rColor[3];

// Converts a 2d visualizer point to it's location on the strip
int getStripLocation(int col, int row)
{
  // The strips are in alternating directions. This adjusts the result for that.
  if (col % 2 == 0)
    row = SHORT_SIDE - row - 1;
    
  return col * SHORT_SIDE - row;
}
void initVUmeter() {
  vuRunning = true;
  analogReference(DEFAULT);
}

void runVUmeter()
{
  initVUmeter();
  int i, j, k;
  uint32_t color;
  int shift;
  int fft_start;
  int fft_count;
  int curData;
  int brightness;
  unsigned long prevUpdateTime = 0;
  unsigned long curTime = 0;
  while(vuRunning) {

  // Read analog input
  for (i = 0; i < LONG_SIDE * 2; i++)
  {
    int val = (analogRead(3) + analogRead(2)) / 2;
    data[i] = val * 2;
    im[i] = 0;
    
    delay(2);
  }
  
  // Perform FFT on data
  // HACK 4 represents 2^4, or 16 (LONG_SIDE * 2). Change this to adjust with LONG_SIDE!
  shift = fix_fft(data, im, 2, 0);

  // Clear pixels
  for (i = 0; i < LONG_SIDE * SHORT_SIDE; i++)
    setPixel(i, 0);
  // Set the proper pixels in each bar
  //for (i = 0; i < LONG_SIDE; i++)
  for (i = 9; i >=0; i--)
  {
    // Each LED bar has 2 FFT frequencies that are summed together
    fft_start = i * 2;
    fft_count = 1;
    
    // Get a positive data point from the FFT
    curData = 0;
    for (k = 0; k < fft_count; k++)
      curData += sqrt(data[fft_start + k] * data[fft_start + k] + im[fft_start + k] * im[fft_start + k]);
      
    // Account for the ShiftyVU's filtering
    if (i == 0 || i == 9)
      curData /= 2;
    
    // Smoothly drop from peaks by only allowing data points to be one LED lower than the previous iteration.
    // This prevents seizure-inducing flashes which might be caused by the ShiftyVU's filtering (?)
    if (prev[i] > SHORT_SIDE && curData < prev[i] - SHORT_SIDE)
      curData = prev[i] - SHORT_SIDE;
    
    // Base color for each bar
    H2R_HSBtoRGB((curHue + i * 8) % 360, 99, 99, rColor);
    color = ((rColor[0]/2)<<16) + ((rColor[1]/2)<<8) + (rColor[2]/2);
    // If only the first LED is lit, but not fully. This is outside the for loop because the subtraction of
    // BAR_LENGTH causes the value to wrap around to a very high number.
    if (curData < SHORT_SIDE)
    {
      brightness = curData * 99 / SHORT_SIDE;
      H2R_HSBtoRGB((curHue + i * BAR_HUE_DIFF) % 360, 99, brightness, rColor);
      color = ((rColor[0]/2)<<16) + ((rColor[1]/2)<<8) + (rColor[2]/2);
      //setPixel(getStripLocation(i, 0),color);
      //setTablePixel(0, i, color);
     // setTablePixel(i, 9, color);
     setTablePixelRGBv(i, 9, rColor[0]/2,rColor[1]/2,rColor[2]/2);
    }
    else
    {
     for (j = 0; j < SHORT_SIDE; j++)
     //for (j = 9; j >= 0; j--)
      {
        // Light up each fully lit LED the same way.
        if (curData - SHORT_SIDE > (SHORT_SIDE-j) * SHORT_SIDE)
          //setPixel(getStripLocation(i, j),color);
          //setTablePixel(i, j, color);
           setTablePixelRGB(i, j, rColor[0]/2,rColor[1]/2,rColor[2]/2);
        else if (curData > (SHORT_SIDE-j) * SHORT_SIDE)
        {
          // Dims the last LED in the bar based on how close the data point is to the next LED.
          brightness = (j * SHORT_SIDE - curData) * 99 / SHORT_SIDE;
          H2R_HSBtoRGB((curHue + i * BAR_HUE_DIFF) % 360, 99, 255, rColor);
          color = ((rColor[2]/2)<<16) + ((rColor[0]/2)<<8) + (rColor[1]/2);
          //setPixel(getStripLocation(i, j),color);
          setTablePixelRGB(i, j, rColor[2]/2,rColor[0]/2,rColor[1]/2);
          //setTablePixel(i, j, color);
        }
      }
    }
    
    // Store all of the data points for filtering of the next iteration.
    prev[i] = curData;
  }
  
  // Cycle through all the colors.
  if (curHue == 359)
    curHue = 0;
  else
    curHue++;
  
  // Display the strip.
  showPixels();

  do{
      readInput();
      if (curControl == BTN_EXIT){
        vuRunning = false;
        break;
      } 
      curTime = millis();
      delay(5);
    }
    while ((curTime - prevUpdateTime) <20);//Once enough time  has passed, proceed. The lower this number, the faster the game is
    prevUpdateTime = curTime;
  }
}
