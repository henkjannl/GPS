#include <TinyGPS.h>
#include <SoftwareSerial.h>

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

SSD1306AsciiWire oled;

// GPS Setup
#define rxGPS 2
#define txGPS 3
#define bufLen 32
SoftwareSerial serialGPS = SoftwareSerial(rxGPS, txGPS);
TinyGPS gps;

static void smartdelay(unsigned long ms);

int a;
float latitude;
float longitude;
unsigned long age;
int i;
char buffer[bufLen]; 
int bytesRead;

//------------------------------------------------------------------------------
void setup() {
  Wire.begin();
  Wire.setClock(400000L);

#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0

  oled.setFont(Verdana12_bold);

  pinMode(rxGPS, INPUT);
  pinMode(txGPS, OUTPUT);
  
  // GPS Setup
  serialGPS.begin(9600);

  a=0;

  oled.clear();
}


void loop(){
  float flat, flon;
  int sat;
  
  a++;
  oled.setRow(0);
  oled.setCol(0);
  oled.print(a);


  gps.f_get_position(&flat, &flon, &age);
  oled.setRow(2);
  oled.setCol(0);
  oled.print(flat);

  oled.setRow(4);
  oled.setCol(0);
  sat=gps.satellites();
  oled.print(sat);

  smartdelay(1000);
  }


static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (serialGPS.available())
      gps.encode(serialGPS.read());
  } while (millis() - start < ms);
}

