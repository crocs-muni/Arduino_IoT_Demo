#include "Morse.h"

Morse morse;
long randNumber;
const int sizeArray = 5;
const long timeDelay = 15000; // 15 seconds
const String text[sizeArray] = {
  "Light up",
  "Dark",
  "Switch off",
  "Lightness",
  "WTF"
  };

void setup() {
  Serial.begin(9600);
  morse.initialize(9);
  //pinMode(9, OUTPUT);
  randomSeed(analogRead(0)); // for every start set up different seek
}

void loop() {
  randNumber = random(sizeArray);
  Serial.println(text[randNumber]);
  morse.codeString(text[randNumber]);
  delay(timeDelay);
}
