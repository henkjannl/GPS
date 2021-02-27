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
// Code to run a screen calibration, not needed when calibration values set in setup()
void touch_calibrate()
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // Calibrate
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 0);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.println("Touch corners as indicated");

  tft.setTextFont(1);
  tft.println();

  tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

  Serial.println(); Serial.println();
  Serial.println("// Use this calibration code in setup():");
  Serial.print("  uint16_t calData[5] = ");
  Serial.print("{ ");

  for (uint8_t i = 0; i < 5; i++)
  {
    Serial.print(calData[i]);
    if (i < 4) Serial.print(", ");
  }

  Serial.println(" };");
  Serial.print("  tft.setTouch(calData);");
  Serial.println(); Serial.println();

  tft.fillScreen(TFT_BLACK);
  
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Calibration complete!");
  tft.println("Calibration code sent to Serial port.");

  delay(4000);
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

  // Calibrate the touch screen and retrieve the scaling factors
  touch_calibrate();

/*
  // Replace above line with the code sent to Serial Monitor
  // once calibration is complete, e.g.:
  uint16_t calData[5] = { 565, 3242, 367, 3328, 1 };
  tft.setTouch(calData);
*/

  // Clear the screen
  tft.fillScreen(TFT_BLACK);
  tft.drawCentreString("Touch screen to test!",tft.width()/2, tft.height()/2, 2);

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
