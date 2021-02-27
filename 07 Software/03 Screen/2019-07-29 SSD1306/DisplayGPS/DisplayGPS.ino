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
#define bufLen 100
SoftwareSerial serialGPS = SoftwareSerial(rxGPS, txGPS);


int a;
int i;
char buffer[bufLen]; 

//------------------------------------------------------------------------------
void setup() {
  Wire.begin();
  Wire.setClock(400000L);

#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0

  oled.setFont(font5x7);

  pinMode(rxGPS, INPUT);
  pinMode(txGPS, OUTPUT);
  
  // GPS Setup
  serialGPS.begin(9600);

  a=0;
  i=0;
  oled.clear();
}


void loop(){
  // Show a counter  
  a++;
  oled.setRow(0);
  oled.setCol(0);
  oled.print(a);

  char c = serialGPS.read();
  if (c=='$')
    i=0;

  if (i<bufLen)
    buffer[i]=c;

  i++;

  oled.setRow(2);
  oled.setCol(0);
  oled.print(buffer);
  

  }


