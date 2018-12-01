#include "Morse.h"

Morse morse;
int photocellPin = 17; // the cell and 10K pulldown are connected to a0
int photocellReading;  // the analog reading from the analog resistor divider
const int sizeArray = 6;
const long timeDelay = 15000; // 15 seconds
const String text[] = {
  "Light up",
  "Dark",
  "Switch off",
  "Lightness",
  "WTF",
  "SOS"
  };

void setup() {
  Serial.begin(9600);
  morse.initialize(9);
}

void loop() {
  photocellReading = analogRead(photocellPin);
  morse.codeString(text[photocellReading % sizeArray]);
  delay(timeDelay);
  Serial.println("EEEEEND");
}
