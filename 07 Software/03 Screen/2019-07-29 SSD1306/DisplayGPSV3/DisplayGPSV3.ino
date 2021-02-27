#include <SoftwareSerial.h>

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

const char* messageHeader[] = {
  "$GLGSV",
  "$GNGGA",
  "$GNVTG"
};

String messages[4]; 

uint8_t nMsg = 3;
  
SSD1306AsciiWire oled;

// GPS Setup
#define rxGPS 2
#define txGPS 3
#define bufLen 100
SoftwareSerial serialGPS = SoftwareSerial(rxGPS, txGPS);



int a;
int row;

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
  row=1;
  oled.clear();
}


void loop(){
  String serialResponse;
  
  // Show a counter  
  a++;
  oled.setRow(0);
  oled.setCol(0);
  oled.print(a);

  if ( serialGPS.available())
  {
    // Read a string from the GPS unit
    String message = serialGPS.readStringUntil('\r\n');

    // Allocate this message to one of the standard messages
    boolean found=false;
    for (uint8_t i = 0; i < nMsg; i++) {
      if (message.startsWith(messageHeader[i]))
        messages[i]=message;
        found=true;
    }

    if (!found)
      messages[3]=message;

    for (uint8_t i = 0; i < 4; i++) {
      oled.setRow(i+2);
      oled.setCol(0);
      oled.print(messages[i]);
    }

  }
  

}
