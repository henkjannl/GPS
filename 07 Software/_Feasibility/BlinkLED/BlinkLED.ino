
void setup() {                
  pinMode(LED_BUILTIN, OUTPUT);   // LED pin as output.    
}

void loop() {

  int SOS_ON[9]  = {100, 100, 100, 400, 400, 400, 100, 100, 100};
  int SOS_OFF[9] = {100, 100, 500, 200, 200, 500, 100, 100, 1000};

  for(int i=0; i<9; i++)  {
    digitalWrite(LED_BUILTIN, HIGH);         // turn the LED on.
    delay(SOS_ON[i]);                   // take a SLEEP...
    digitalWrite(LED_BUILTIN, LOW);          // turn the LED off.
    delay(SOS_OFF[i]);                   // take a SLEEP...
  } 

}
