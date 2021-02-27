#include <SoftwareSerial.h>

// Preparations for the OLED module
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

// Declare the OLED screen
SSD1306AsciiWire oled;

// Preparations for the GPS module
#define RX_PIN  2
#define TX_PIN  3
SoftwareSerial gpsPort = SoftwareSerial(RX_PIN, TX_PIN);

//#include <GPSport.h>
#include <NMEAGPS.h>

// Declare the GPS module
static NMEAGPS  gps;
static gps_fix  fix;

int counter;

static void doSomeWork()
{
  // Show a counter  
  counter++;

  oled.setFont(lcd5x7);
  oled.setRow(0);
  oled.setCol(0);
  oled.print(String(counter));

  if (fix.valid.location) 
    oled.println(" Valid location       ");
  else
    oled.println(" Location not fixed   ");

  oled.setRow(2);
  oled.setCol(0);
  oled.setFont(Arial_bold_14);
  //oled.println("Lat: " + String(1000*fix.latitude())+"   ");
  oled.println("Lat: " + String(fix.latitudeL())+"   ");  
  oled.println("Lon: " + String(fix.longitudeL())+"   ");



} 

static void GPSloop()
{
  while (gps.available( gpsPort )) {
    fix = gps.read();
    doSomeWork();
  }
} // GPSloop

//------------------------------------------------------------------------------
void setup() {

  // Setup I2C
  Wire.begin();
  Wire.setClock(400000L);

  // Setup OLED via I2C

  #if RST_PIN >= 0
    oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
  #else // RST_PIN >= 0
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
  #endif // RST_PIN >= 0

  //oled.setFont(font5x7);
  oled.setFont(Arial_bold_14);
    
  // Configure RX and TX pins for the GPS
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);
  
  // GPS Setup
  gpsPort.begin(9600);

  // Reset the counter
  counter=0;

  // Clear the screen
  oled.clear();
}


void loop(){
  // Call the GPS loop
  GPSloop();
}

