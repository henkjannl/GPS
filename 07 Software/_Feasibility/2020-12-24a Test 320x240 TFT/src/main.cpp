/**************************************************************************
ToDo:

 **************************************************************************/
#include "Arduino.h"
#include <SPI.h>
#include <TFT_eSPI.h>

/**************************************************************************
TYPES, CLASSES, ENUMS
**************************************************************************/


/**************************************************************************
 DEFINES AND CONSTANTS
**************************************************************************/
#define SCREEN_WIDTH  320 // TFT display width, in pixels
#define SCREEN_HEIGHT 240 // TFT display height, in pixels

/**************************************************************************
 GLOBAL VARIABLES
**************************************************************************/
// Declaration for ILI9341 display connected to SPI using Bodmer
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

/**************************************************************************
FUNCTIONS
**************************************************************************/
unsigned int rainbow(int value)
{
  // Value is expected to be in range 0-127
  // The value is converted to a spectrum colour from 0 = blue through to red = blue

  byte red = 0; // Red is the top 5 bits of a 16 bit colour value
  byte green = 0;// Green is the middle 6 bits
  byte blue = 0; // Blue is the bottom 5 bits

  byte quadrant = value / 32;

  if (quadrant == 0) {
    blue = 31;
    green = 2 * (value % 32);
    red = 0;
  }
  if (quadrant == 1) {
    blue = 31 - (value % 32);
    green = 63;
    red = 0;
  }
  if (quadrant == 2) {
    blue = 0;
    green = 63;
    red = value % 32;
  }
  if (quadrant == 3) {
    blue = 0;
    green = 63 - 2 * (value % 32);
    red = 31;
  }
  return (red << 11) + (green << 5) + blue;
}


/**************************************************************************
SETUP AND LOOP
**************************************************************************/

void setup() {
  Serial.begin(115200);

  delay(1000);

  // Setup the LCD
  tft.init();
  tft.setRotation(3);

  tft.fillScreen(TFT_BLACK);
/*
  for (int px = 1; px < 320; px++)
  {
    for (int py = 0; py < 240; py++)
    {
      float x0 = (map(px, 0, 320, -250000/2, -242500/2)) / 100000.0; //scaled x coordinate of pixel (scaled to lie in the Mandelbrot X scale (-2.5, 1))
      float yy0 = (map(py, 0, 240, -75000/4, -61000/4)) / 100000.0; //scaled y coordinate of pixel (scaled to lie in the Mandelbrot Y scale (-1, 1))
      float xx = 0.0;
      float yy = 0.0;
      int iteration = 0;
      int max_iteration = 128;
      while ( ((xx * xx + yy * yy) < 4)  &&  (iteration < max_iteration) )
      {
        float xtemp = xx * xx - yy * yy + x0;
        yy = 2 * xx * yy + yy0;
        xx = xtemp;
        iteration++;
      }
      int color = rainbow((3*iteration+64)%128);
      yield();tft.drawPixel(px, py, color);
    }
  }
*/
  while (true)
  {
    tft.startWrite();
    tft.fillScreen(TFT_BLUE);
    tft.endWrite();
    delay(500);
    tft.startWrite();
    tft.fillScreen(TFT_RED);
    tft.endWrite();
    delay(500);
    tft.startWrite();
    tft.fillScreen(TFT_GREEN);
    tft.endWrite();
    delay(500);
  }
  

}

void loop() {

}
