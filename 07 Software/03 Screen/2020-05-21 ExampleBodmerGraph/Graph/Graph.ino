/*
  The TFT_eSPI library incorporates an Adafruit_GFX compatible
  button handling class, this sketch is based on the Arduin-o-phone
  example.

  This example diplays a keypad where numbers can be entered and
  send to the Serial Monitor window.

  The sketch has been tested on the ESP8266 (which supports SPIFFS)

  The minimum screen size is 320 x 240 as that is the keypad size.
*/

// The SPIFFS (FLASH filing system) is used to hold touch screen
// calibration data

#include "FS.h"

#include <SPI.h>
#include <TFT_eSPI.h>      // Hardware-specific library

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

// This is the file name used to store the calibration data
// You can change this to create new calibration files.
// The SPIFFS file name must start with "/".
#define CALIBRATION_FILE "/TouchCalData1"

// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CAL true

// Using two fonts since numbers are nice when bold
#define LABEL1_FONT &FreeSansOblique12pt7b // Key label font 1
#define LABEL2_FONT &FreeSansBold12pt7b    // Key label font 2

// Numeric display box size and location
#define DISP_X 1
#define DISP_Y 10
#define DISP_W 238
#define DISP_H 50
#define DISP_TSIZE 3
#define DISP_TCOLOR TFT_CYAN

#define KEY_TEXTSIZE 1

// Create a polygon to be drawn
typedef struct {
  int x;
  int y;
} type_point;


static const type_point polygon[] = {
  {   67,  64},
  {  104, 224},
  {  252, 153},
  {  150,  25},
  {   47, 153},
  {  195, 224},
  {  223,  64},
  {   67,  64},
  {   -1,  -1} };

// Invoke the TFT_eSPI button class and create all the button objects
TFT_eSPI_Button buttonP;
TFT_eSPI_Button buttonM;

// Zoom factor to draw the map
float zoomFactor = 1.0;

//------------------------------------------------------------------------------------------

void setup() {
  // Use serial port
  Serial.begin(9600);

  // Initialise the TFT screen
  tft.init();

  // Set the rotation before we calibrate
  tft.setRotation(1);

  // Calibrate the touch screen and retrieve the scaling factors
  touch_calibrate();

  // Clear the screen
  tft.fillScreen(TFT_BLACK);

  // Draw keypad
  drawScreen();
}

//------------------------------------------------------------------------------------------

void loop(void) {
  uint16_t t_x = 0, t_y = 0; // To store the touch coordinates

  // Pressed will be set true is there is a valid touch on the screen
  boolean pressed = tft.getTouch(&t_x, &t_y);

  // / Check if any key coordinate boxes contain the touch coordinates  
  if (pressed && buttonP.contains(t_x, t_y)) {
    buttonP.press(true);  // tell the button it is pressed
  } else {
    buttonP.press(false);  // tell the button it is NOT pressed
  }
  
  if (pressed && buttonM.contains(t_x, t_y)) {
    buttonM.press(true);  // tell the button it is pressed
  } else {
    buttonM.press(false);  // tell the button it is NOT pressed
  }

  tft.setFreeFont(LABEL2_FONT);
  if (buttonP.justReleased()) buttonP.drawButton();     // draw normal
  if (buttonM.justReleased()) buttonM.drawButton();     // draw normal

  if (buttonP.justPressed()) {
    buttonP.drawButton(true);  // draw invert
    zoomFactor*=1.1;
    Serial.println(zoomFactor);
  }

  if (buttonM.justPressed()) {
    buttonM.drawButton(true);  // draw invert
    zoomFactor/=1.1;
    Serial.println(zoomFactor);
  }

  delay(10); // UI debouncing
}

//------------------------------------------------------------------------------------------

void drawScreen()
{
  int pointIndex=1;
  while (polygon[pointIndex].x != -1) {
     tft.drawLine(polygon[pointIndex-1].x, polygon[pointIndex-1].y, polygon[pointIndex].x, polygon[pointIndex].y, TFT_WHITE);
     pointIndex++;
  }

   buttonP.initButton(&tft, 280, 18, 55, 25, TFT_WHITE, TFT_RED,  TFT_WHITE, "+", KEY_TEXTSIZE);
   buttonP.drawButton();

   buttonM.initButton(&tft, 280, 47, 55, 25, TFT_WHITE, TFT_BLUE, TFT_WHITE, "-", KEY_TEXTSIZE);
   buttonM.drawButton();
  
}

//------------------------------------------------------------------------------------------

void touch_calibrate()
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check file system exists
  if (!SPIFFS.begin()) {
    Serial.println("Formating file system");
    SPIFFS.format();
    SPIFFS.begin();
  }

  // check if calibration file exists and size is correct
  if (SPIFFS.exists(CALIBRATION_FILE)) {
    if (REPEAT_CAL)
    {
      // Delete if we want to re-calibrate
      SPIFFS.remove(CALIBRATION_FILE);
    }
    else
    {
      File f = SPIFFS.open(CALIBRATION_FILE, "r");
      if (f) {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL) {
    // calibration data valid
    tft.setTouch(calData);
  } else {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}

//------------------------------------------------------------------------------------------

// Print something in the mini status bar
void status(const char *msg) {
  tft.setTextPadding(240);
  //tft.setCursor(STATUS_X, STATUS_Y);
  tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
  tft.setTextFont(0);
  tft.setTextDatum(TC_DATUM);
  tft.setTextSize(1);
  tft.drawString(msg, 10,10);
}

//------------------------------------------------------------------------------------------
