/**************************************************************************
ToDo:

 **************************************************************************/
#include <string>     

#include "Arduino.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
//#include "SPIFFS.h"

#include <esp.h>
#include "gui_elements.h"
#include "screens.h"

/**************************************************************************
TYPES, CLASSES, ENUMS
**************************************************************************/

/**************************************************************************
 DEFINES AND CONSTANTS
**************************************************************************/
#define SCREEN_WIDTH  320 // TFT display width, in pixels
#define SCREEN_HEIGHT 240 // TFT display height, in pixels

/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 4;
static const int TXPin = 3;
static const uint32_t GPSBaud = 9600;

/**************************************************************************
 GLOBAL VARIABLES
**************************************************************************/
// Declaration for ILI9341 display connected to SPI using Bodmer
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library to handle the TFT screen
Theme theme;               // Default colors for the GUI, can be overridden
//ScreenGpsValues screenGpsValues(tft, theme);  // GUI for GPS values

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

/**************************************************************************
FUNCTIONS
**************************************************************************/
// Code to run a screen calibration, not needed when calibration values set in setup()
void touch_calibrate();

/*
void displayLocation() {
  static bool drawFirstTime=true;

  if (drawFirstTime) {
    screenGpsValues.drawFirstTime();
    drawFirstTime=false;
  }

  if(gps.satellites.isValid() && (gps.satellites.value()>3)) {
    screenGpsValues.Status.updateValue("Locked");
  } 
  else {
    static int blips=0;
    blips++;
    if(blips>3) blips=0;
    String s=String("Searching");
    for(int i=0; i<blips; i++) s+='.';
    screenGpsValues.Status.updateValue(s);
  }

  screenGpsValues.Status.updateValue(String("")+ESP.getFreeHeap());

  if (gps.location.isValid()) {
    screenGpsValues.Lattitude.updateValue(String(gps.location.lat(), 3));
    screenGpsValues.Longitude.updateValue(String(gps.location.lng(), 3));
  }

  if (gps.date.isValid()) {
    char buffer[20];
    sprintf(buffer, "%02d-%02d-%04d",gps.date.day(), gps.date.month(), gps.date.year());
    screenGpsValues.Date.updateValue(buffer);

    sprintf(buffer, "%02d:%02d:%02d",gps.time.hour(), gps.time.minute(), gps.time.second());
    screenGpsValues.Time.updateValue(buffer);
  }

  if (gps.speed.isValid()) {
    screenGpsValues.Speed.updateValue(String(gps.speed.kmph(),1)+" km/h");
  }

  if (gps.course.isValid()) {
    screenGpsValues.Course.updateValue(String(gps.course.deg(),1));
  }

  if (gps.altitude.isValid()) {
    screenGpsValues.Altitude.updateValue(String(gps.altitude.meters(),1)+" m");
  }

  if (gps.satellites.isValid()) {
    screenGpsValues.Sattelites.updateValue(String(gps.satellites.value()));
  }

  if (gps.hdop.isValid()) {
    char signal[20];
    if (gps.hdop.hdop()<=1)
      strcpy(signal,"Perfect"); 
    else if (gps.hdop.hdop()<=2)
      strcpy(signal,"Excellent"); 
    else if (gps.hdop.hdop()<=5)
      strcpy(signal,"Good"); 
    else if (gps.hdop.hdop()<=10)
      strcpy(signal,"Moderate"); 
    else if (gps.hdop.hdop()<=20)
      strcpy(signal,"Fair"); 
    else 
      strcpy(signal,"Poor");   

    screenGpsValues.Accuracy.updateValue(String(gps.hdop.hdop())+" - "+signal);
  }

  screenGpsValues.drawUpdate();

  if (screenGpsValues.btnOK.isPressed()) drawFirstTime=true;
}
*/

/**************************************************************************
SETUP AND LOOP
**************************************************************************/

void setup() {
  Serial.begin(115200);

  pinMode(26, OUTPUT);
  digitalWrite(26, HIGH);
  Serial.println("high");
  
  ss.begin(GPSBaud);

  delay(1000);

  // Setup the LCD
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);


  // Calibrate the touch screen and retrieve the scaling factors
  if (false)
    touch_calibrate();
  else {
    uint16_t calData[5] = { 321, 3442, 583, 3293, 4 };
    tft.setTouch(calData);
  }

}

void loop() {

  //displayLocation();

  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    gps.encode(ss.read());

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }

  tft.setTextFont(2);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Hello.", 200, 20);

}



void touch_calibrate()
{
  uint16_t calData[5];
  //uint8_t calDataOK = 0;

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

