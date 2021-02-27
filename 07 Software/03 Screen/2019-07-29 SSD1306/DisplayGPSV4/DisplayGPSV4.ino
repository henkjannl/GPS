#include <SoftwareSerial.h>

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

// Declare the OLED screen
SSD1306AsciiWire oled;

// Setup the serial link to the GPS
#define RX_PIN  2
#define TX_PIN  3
SoftwareSerial serialGPS = SoftwareSerial(RX_PIN, TX_PIN);

static void parseMsg(String& msg);

// Reserve memory for the words than can be found
String value[20];
String timeUTC;
String lattitude;
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

  pinMode(RX_PIN , INPUT);
  pinMode(TX_PIN , OUTPUT);
  
  // GPS Setup
  serialGPS.begin(9600);

  a=0;
  row=1;
  oled.clear();
}


void loop(){
  String serialResponse;
  String message; 
  String cmd;
  
  if ( serialGPS.available())
  {
    // Read a string from the GPS unit
    message = serialGPS.readStringUntil('\r\n');

    if (message.startsWith("$"))
    {
      cmd=message.substring(3, 6);
  
      if (cmd=="GGA")
        {
          int to = message.indexOf(',', 7);
          timeUTC = message.substring(7, to);
          }
    }
  }

  // Show a counter  
  a++;
  oled.setRow(0);
  oled.setCol(0);
  oled.print(a);

  oled.setRow(1);
  oled.setCol(0);
  oled.print(cmd);

  oled.setRow(2);
  oled.setCol(0);
  oled.print(timeUTC);

}



