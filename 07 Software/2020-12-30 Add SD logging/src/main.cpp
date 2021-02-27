/**************************************************************************
ToDo:

 **************************************************************************/
#include <string>     

#include "Arduino.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <BMI160Gen.h>
#include <esp.h>
#include "gui_elements.h"
#include "screens.h"

/**************************************************************************
TYPES, CLASSES, ENUMS
**************************************************************************/

/**************************************************************************
 DEFINES AND CONSTANTS
**************************************************************************/
// Constants for GPS module
static const int GPS_RX = 4;
static const int GPS_TX = 3;
static const uint32_t GPS_BAUD = 9600;

/**************************************************************************
 GLOBAL VARIABLES
**************************************************************************/
// Declaration for ILI9341 display connected to SPI using Bodmer library
TFT_eSPI tft = TFT_eSPI();             // Invoke custom library to handle the TFT screen
ScreenGpsValues screenGpsValues(tft);  // GUI for GPS values

// The serial connection to the GPS device
SoftwareSerial gpsConnection(GPS_RX, GPS_TX);

// The TinyGPS++ object
TinyGPSPlus gpsParser;

// Timer for the display routine
volatile bool refreshDisplay;
 
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
/**************************************************************************
FUNCTIONS
**************************************************************************/
// Code to run a screen calibration, not needed when calibration values set in setup()
void touch_calibrate();

// Interrupt timer
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  refreshDisplay=true;
  portEXIT_CRITICAL_ISR(&timerMux); 
}

void displayLocation() {
  static bool drawFirstTime=true;

  if (drawFirstTime) {
    screenGpsValues.drawFirstTime();
    drawFirstTime=false;
  }

  if(gpsParser.satellites.isValid() && (gpsParser.satellites.value()>3)) {
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

  if (gpsParser.location.isValid()) {
    screenGpsValues.Lattitude.updateValue(String(gpsParser.location.lat(), 3));
    screenGpsValues.Longitude.updateValue(String(gpsParser.location.lng(), 3));
  }

  if (gpsParser.date.isValid()) {
    char buffer[20];
    sprintf(buffer, "%02d-%02d-%04d",gpsParser.date.day(), gpsParser.date.month(), gpsParser.date.year());
    screenGpsValues.Date.updateValue(buffer);

    sprintf(buffer, "%02d:%02d:%02d",gpsParser.time.hour(), gpsParser.time.minute(), gpsParser.time.second());
    screenGpsValues.Time.updateValue(buffer);
  }

  if (gpsParser.speed.isValid()) {
    screenGpsValues.Speed.updateValue(String(gpsParser.speed.kmph(),1)+" km/h");
  }

  if (gpsParser.course.isValid()) {
    screenGpsValues.Course.updateValue(String(gpsParser.course.deg(),1));
  }

  if (gpsParser.altitude.isValid()) {
    screenGpsValues.Altitude.updateValue(String(gpsParser.altitude.meters(),1)+" m");
  }

  if (gpsParser.satellites.isValid()) {
    screenGpsValues.Sattelites.updateValue(String(gpsParser.satellites.value()));
  }

  if (gpsParser.hdop.isValid()) {
    char signal[20];
    if (gpsParser.hdop.hdop()<=1)
      strcpy(signal,"Perfect"); 
    else if (gpsParser.hdop.hdop()<=2)
      strcpy(signal,"Excellent"); 
    else if (gpsParser.hdop.hdop()<=5)
      strcpy(signal,"Good"); 
    else if (gpsParser.hdop.hdop()<=10)
      strcpy(signal,"Moderate"); 
    else if (gpsParser.hdop.hdop()<=20)
      strcpy(signal,"Fair"); 
    else 
      strcpy(signal,"Poor");   

    screenGpsValues.Accuracy.updateValue(String(gpsParser.hdop.hdop())+" - "+signal);
  }

  screenGpsValues.drawUpdate();

  if (screenGpsValues.btnOK.isPressed()) drawFirstTime=true;
}


/**************************************************************************
SETUP AND LOOP
**************************************************************************/

void setup() {

  // Setup the serial link
  Serial.begin(115200);

  //spiffsStarted=SPIFFS.begin();

  // Setup the GPS
  gpsConnection.begin(GPS_BAUD);

  delay(100);

  // Setup the LCD
  tft.init();
  tft.setRotation(0);

  tft.fillScreen(TFT_BLACK);

  if (false)
    // Calibrate the touch screen and retrieve the scaling factors
    // ToDo: move this code to some calibration menu
    touch_calibrate();
  else {
    uint16_t calData[5] = { 321, 3442, 583, 3293, 4 };
    tft.setTouch(calData);
  }

  // Setup the timer
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);

}

void loop() {
  // If the timer expires, refresh the display
  if (refreshDisplay) {
    displayLocation();
    refreshDisplay=false;
  }

  // Listen to the GPS
  while (gpsConnection.available() > 0)
    gpsParser.encode(gpsConnection.read());

  // Perhaps put this code in a central diagnosis tool
  if (millis() > 5000 && gpsParser.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }

}



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

