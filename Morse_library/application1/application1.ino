#include "Morse.h"

Morse morse(9);
long randNumber;
const int sizeArray = 5;
const long timeDelay = 300000; // 5 minutes
const String text[sizeArray] = {
  "Light up",
  "Dark",
  "Switch off",
  "Lightness",
  "WTF"
  };

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  randomSeed(analogRead(0)); // for every start set up different seek
}

void loop() {
  randNumber = random(sizeArray);
  Serial.println(randNumber);
  morse.codeString(text[randNumber]);
  delay(timeDelay);
}
